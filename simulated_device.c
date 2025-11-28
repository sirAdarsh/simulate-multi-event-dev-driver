// simulated_device_adv.c
// Simulated multi-source interrupt device with per-CPU buffers, rate-limiting and char device control
// Build: use kernel headers; tested conceptually on modern kernels
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/atomic.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Advanced Simulated Multi-Interrupt Device (per-CPU, rate limit, char dev)");
MODULE_VERSION("0.2");

#define NUM_SOURCES 3
#define EVENT_BUFFER_SIZE 64       // per-CPU buffer size
#define PROC_FILENAME "fake_events_adv"
#define DEFAULT_RATE 10            // tokens/sec default
#define DEFAULT_BURST 20           // burst tokens
#define INTERVAL_NS (100 * 1000 * 1000LL) // 100ms

/* ioctl definitions */
#define SIMIO_MAGIC 's'
struct rate_cmd { int src; unsigned int rate; unsigned int burst; };
struct stats_user { unsigned int processed[NUM_SOURCES]; unsigned int dropped[NUM_SOURCES]; };

#define SIMIO_SET_RATE _IOW(SIMIO_MAGIC, 1, struct rate_cmd)
#define SIMIO_GET_STATS _IOR(SIMIO_MAGIC, 2, struct stats_user)
#define SIMIO_ENABLE_STORM _IOW(SIMIO_MAGIC, 3, int)  // 0/1
#define SIMIO_CLEAR_BUFF _IO(SIMIO_MAGIC, 4)

typedef enum {
    EVENT_NETWORK = 0,
    EVENT_KEYBOARD,
    EVENT_SENSOR
} event_type_t;

struct event {
    ktime_t ts;
    event_type_t type;
    char msg[48];
};

/* Per-CPU buffer and metadata */
struct per_cpu_buf {
    struct event buf[EVENT_BUFFER_SIZE];
    unsigned int head;
    spinlock_t lock;
};

static DEFINE_PER_CPU(struct per_cpu_buf, pc_buf);

/* Global counters and rate control per source */
struct src_ctrl {
    unsigned int rate;      // tokens added per second
    unsigned int burst;     // maximum tokens
    u64 last_ns;            // last refill time (ns)
    unsigned int tokens;    // current tokens
    atomic_t processed;
    atomic_t dropped;
};

static struct src_ctrl srcs[NUM_SOURCES];
static bool storm_mode = false;

/* Workqueue and event work */
struct event_work {
    struct work_struct work;
    event_type_t type;
    int target_cpu;
};
static struct workqueue_struct *event_wq;

/* Timers */
static struct hrtimer timers[NUM_SOURCES];
static ktime_t interval;

/* Character device */
static int major = 0;
static struct class *sim_class;
static struct device *sim_device;
static struct cdev sim_cdev;

/* Proc */
static const struct proc_ops proc_fops; // forward

/* Helper: refill tokens for a src */
static void refill_tokens(struct src_ctrl *s)
{
    u64 now_ns = ktime_to_ns(ktime_get());
    if (!s->last_ns) {
        s->last_ns = now_ns;
        return;
    }
    u64 delta_ns = now_ns - s->last_ns;
    if (delta_ns == 0) return;
    // tokens to add = rate * seconds
    u64 add = (s->rate * delta_ns) / 1000000000ULL;
    if (add) {
        unsigned int new_tokens = s->tokens + (unsigned int)add;
        if (new_tokens > s->burst) new_tokens = s->burst;
        s->tokens = new_tokens;
        s->last_ns = now_ns;
    }
}

/* push event into per-cpu buffer (called from workqueue context) */
static void push_event_cpu(event_type_t type, int cpu)
{
    struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);
    unsigned long flags;
    spin_lock_irqsave(&p->lock, flags);
    p->buf[p->head].ts = ktime_get();
    p->buf[p->head].type = type;
    scnprintf(p->buf[p->head].msg, sizeof(p->buf[p->head].msg), "Src %d on CPU %d", type, cpu);
    p->head = (p->head + 1) % EVENT_BUFFER_SIZE;
    spin_unlock_irqrestore(&p->lock, flags);
}

/* Work handler (bottom half) */
static void process_event(struct work_struct *work)
{
    struct event_work *ew = container_of(work, struct event_work, work);
    int cpu = ew->target_cpu;
    push_event_cpu(ew->type, cpu);
    atomic_inc(&srcs[ew->type].processed);
    pr_info("simdev: processed src=%d on cpu=%d\n", ew->type, cpu);
    kfree(ew);
}

/* Timer callback simulating IRQ */
static enum hrtimer_restart timer_cb(struct hrtimer *t)
{
    int src = -1;
    int cpu;
    struct event_work *ew;
    struct src_ctrl *sc;

    if (t == &timers[0]) src = EVENT_NETWORK;
    else if (t == &timers[1]) src = EVENT_KEYBOARD;
    else src = EVENT_SENSOR;

    sc = &srcs[src];
    /* simple token refill + rate-limit logic */
    refill_tokens(sc);

    /* in storm_mode, we intentionally bypass rate limiting to test overload */
    if (!storm_mode) {
        if (sc->tokens == 0) {
            atomic_inc(&sc->dropped);
            /* drop this event */
            hrtimer_forward_now(t, interval);
            return HRTIMER_RESTART;
        }
        sc->tokens--;
    } else {
        /* increment dropped occasionally to indicate storm measurement (optional) */
    }

    /* Choose a target CPU to process this event: round-robin across online cpus */
    cpu = cpumask_first(cpu_online_mask) + (src % num_online_cpus()); // crude mapping
    if (cpu >= nr_cpu_ids) cpu = smp_processor_id();

    ew = kmalloc(sizeof(*ew), GFP_ATOMIC);
    if (!ew) {
        atomic_inc(&sc->dropped);
        hrtimer_forward_now(t, interval);
        return HRTIMER_RESTART;
    }
    ew->type = src;
    ew->target_cpu = cpu;
    INIT_WORK(&ew->work, process_event);
    queue_work(event_wq, &ew->work);

    hrtimer_forward_now(t, interval);
    return HRTIMER_RESTART;
}

/* ---- procfs to dump per-cpu buffers ---- */
static int proc_show(struct seq_file *m, void *v)
{
    int cpu;
    for_each_online_cpu(cpu) {
        struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);
        unsigned long flags;
        int i;
        spin_lock_irqsave(&p->lock, flags);
        seq_printf(m, "CPU %d head=%u\n", cpu, p->head);
        for (i = 0; i < EVENT_BUFFER_SIZE; i++) {
            struct event *e = &p->buf[i];
            if (e->ts) {
                seq_printf(m, "  [%llu] src=%d msg=%s\n",
                           (unsigned long long)ktime_to_ns(e->ts),
                           e->type, e->msg);
            }
        }
        spin_unlock_irqrestore(&p->lock, flags);
    }
    return 0;
}
static int proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, proc_show, NULL);
}
static const struct proc_ops proc_ops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

/* ---- char device ioctl handlers ---- */
static long simio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int enabled;
    struct rate_cmd rc;
    struct stats_user st;
    int i;
    void __user *uarg = (void __user *)arg;

    switch (cmd) {
    case SIMIO_SET_RATE:
        if (copy_from_user(&rc, uarg, sizeof(rc))) return -EFAULT;
        if (rc.src < 0 || rc.src >= NUM_SOURCES) return -EINVAL;
        if (rc.rate == 0) return -EINVAL;
        srcs[rc.src].rate = rc.rate;
        srcs[rc.src].burst = rc.burst ? rc.burst : rc.rate;
        pr_info("simdev: set rate src=%d rate=%u burst=%u\n", rc.src, rc.rate, srcs[rc.src].burst);
        return 0;

    case SIMIO_GET_STATS:
        for (i = 0; i < NUM_SOURCES; i++) {
            st.processed[i] = (unsigned int)atomic_read(&srcs[i].processed);
            st.dropped[i] = (unsigned int)atomic_read(&srcs[i].dropped);
        }
        if (copy_to_user(uarg, &st, sizeof(st))) return -EFAULT;
        return 0;

    case SIMIO_ENABLE_STORM:
        if (copy_from_user(&enabled, uarg, sizeof(int))) return -EFAULT;
        storm_mode = (enabled != 0);
        pr_info("simdev: storm_mode=%d\n", storm_mode);
        return 0;

    case SIMIO_CLEAR_BUFF:
        /* clear per-cpu buffers */
        for_each_online_cpu(i) {
            struct per_cpu_buf *p = &per_cpu(pc_buf, i);
            unsigned long flags;
            spin_lock_irqsave(&p->lock, flags);
            memset(p->buf, 0, sizeof(p->buf));
            p->head = 0;
            spin_unlock_irqrestore(&p->lock, flags);
        }
        pr_info("simdev: cleared buffers\n");
        return 0;

    default:
        return -ENOTTY;
    }
}

static ssize_t simio_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct event e;
    int cpu = smp_processor_id();
    struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);
    unsigned long flags;

    spin_lock_irqsave(&p->lock, flags);
    if (p->head == 0) { // no events
        spin_unlock_irqrestore(&p->lock, flags);
        return 0;
    }

    // pop the oldest event
    e = p->buf[(p->head - 1 + EVENT_BUFFER_SIZE) % EVENT_BUFFER_SIZE];
    p->head = (p->head - 1 + EVENT_BUFFER_SIZE) % EVENT_BUFFER_SIZE;
    spin_unlock_irqrestore(&p->lock, flags);

    // copy to user
    if (count < sizeof(e))
        return -EINVAL;

    if (copy_to_user(buf, &e, sizeof(e)))
        return -EFAULT;

    return sizeof(e);
}

static int simio_open(struct inode *inode, struct file *file) { return 0; }
static int simio_release(struct inode *inode, struct file *file) { return 0; }

static const struct file_operations simio_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = simio_ioctl,
    .open = simio_open,
    .release = simio_release,
    .read = simio_read,
};

/* ---- module init / exit ---- */
static int __init simdev_init(void)
{
    int i, cpu;
    dev_t dev;
    int ret;

    pr_info("simdev: init\n");

    /* init per-cpu buffers */
    for_each_possible_cpu(cpu) {
        struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);
        memset(p, 0, sizeof(*p));
        spin_lock_init(&p->lock);
    }

    /* init src controls */
    for (i = 0; i < NUM_SOURCES; i++) {
        srcs[i].rate = DEFAULT_RATE;
        srcs[i].burst = DEFAULT_BURST;
        srcs[i].tokens = srcs[i].burst;
        srcs[i].last_ns = ktime_to_ns(ktime_get());
        atomic_set(&srcs[i].processed, 0);
        atomic_set(&srcs[i].dropped, 0);
    }

    /* create workqueue */
    event_wq = create_workqueue("sim_event_wq");
    if (!event_wq) {
        pr_err("simdev: create_workqueue failed\n");
        return -ENOMEM;
    }

    /* create char device */
    ret = alloc_chrdev_region(&dev, 0, 1, "simirq");
    if (ret) {
        pr_err("simdev: alloc_chrdev_region failed\n");
        destroy_workqueue(event_wq);
        return ret;
    }
    major = MAJOR(dev);
    cdev_init(&sim_cdev, &simio_fops);
    sim_cdev.owner = THIS_MODULE;
    ret = cdev_add(&sim_cdev, dev, 1);
    if (ret) {
        pr_err("simdev: cdev_add failed\n");
        unregister_chrdev_region(dev, 1);
        destroy_workqueue(event_wq);
        return ret;
    }
    sim_class = class_create("simdev");
    if (IS_ERR(sim_class)) {
        pr_err("simdev: class_create failed\n");
        cdev_del(&sim_cdev);
        unregister_chrdev_region(dev, 1);
        destroy_workqueue(event_wq);
        return PTR_ERR(sim_class);
    }
    sim_device = device_create(sim_class, NULL, dev, NULL, "simirq");
    if (IS_ERR(sim_device)) {
        pr_err("simdev: device_create failed\n");
        class_destroy(sim_class);
        cdev_del(&sim_cdev);
        unregister_chrdev_region(dev, 1);
        destroy_workqueue(event_wq);
        return PTR_ERR(sim_device);
    }

    /* proc entry */
    if (!proc_create(PROC_FILENAME, 0444, NULL, &proc_ops)) {
        pr_warn("simdev: failed to create /proc/%s\n", PROC_FILENAME);
    }

    /* start timers */
    interval = ktime_set(0, INTERVAL_NS);
    for (i = 0; i < NUM_SOURCES; i++) {
        hrtimer_init(&timers[i], CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        timers[i].function = timer_cb;
        hrtimer_start(&timers[i], interval, HRTIMER_MODE_REL);
    }

    pr_info("simdev: started (major=%d /dev/simirq)\n", major);
    return 0;
}

static void __exit simdev_exit(void)
{
    int i;
    dev_t dev = MKDEV(major, 0);

    pr_info("simdev: exit\n");
    for (i = 0; i < NUM_SOURCES; i++) hrtimer_cancel(&timers[i]);

    /* flush and destroy workqueue */
    flush_workqueue(event_wq);
    destroy_workqueue(event_wq);

    /* remove proc */
    remove_proc_entry(PROC_FILENAME, NULL);

    /* destroy char device */
    device_destroy(sim_class, dev);
    class_destroy(sim_class);
    cdev_del(&sim_cdev);
    unregister_chrdev_region(dev, 1);

    pr_info("simdev: stopped\n");
}

module_init(simdev_init);
module_exit(simdev_exit);
