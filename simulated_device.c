#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simulated Multi-Interrupt Event Logger");
MODULE_VERSION("0.1");

#define EVENT_BUFFER_SIZE 128
#define PROC_FILENAME "fake_events"
#define NUM_SOURCES 3

typedef enum {
    EVENT_NETWORK = 0,
    EVENT_KEYBOARD,
    EVENT_SENSOR
} event_type_t;

struct event {
    ktime_t timestamp;
    event_type_t type;
    char message[64];
};

static struct event event_buffer[EVENT_BUFFER_SIZE];
static int buffer_head = 0;
static spinlock_t buffer_lock;

static struct workqueue_struct *event_wq;

struct event_work {
    struct work_struct work;
    event_type_t type;
};

static struct hrtimer timers[NUM_SOURCES];
static ktime_t interval = 1e8; // 100ms

/* Workqueue handler: process event */
static void process_event(struct work_struct *work) {
    struct event_work *ew = container_of(work, struct event_work, work);
    unsigned long flags;

    spin_lock_irqsave(&buffer_lock, flags);

    event_buffer[buffer_head].timestamp = ktime_get();
    event_buffer[buffer_head].type = ew->type;
    snprintf(event_buffer[buffer_head].message, sizeof(event_buffer[buffer_head].message),
             "Event from source %d", ew->type);

    buffer_head = (buffer_head + 1) % EVENT_BUFFER_SIZE;

    spin_unlock_irqrestore(&buffer_lock, flags);

    printk(KERN_INFO "SimulatedDevice: Event %d processed\n", ew->type);

    kfree(ew);
}

/* Timer callback simulating an interrupt */
static enum hrtimer_restart timer_callback(struct hrtimer *timer) {
    struct event_work *ew;

    ew = kmalloc(sizeof(*ew), GFP_ATOMIC);
    if (!ew) return HRTIMER_NORESTART;

    ew->type = (timer == &timers[0]) ? EVENT_NETWORK :
               (timer == &timers[1]) ? EVENT_KEYBOARD : EVENT_SENSOR;

    INIT_WORK(&ew->work, process_event);
    queue_work(event_wq, &ew->work);

    hrtimer_forward_now(timer, interval);
    return HRTIMER_RESTART;
}

/* Procfs interface */
static int proc_show(struct seq_file *m, void *v) {
    int i;
    unsigned long flags;

    spin_lock_irqsave(&buffer_lock, flags);
    seq_printf(m, "Recent events:\n");
    for (i = 0; i < EVENT_BUFFER_SIZE; i++) {
        if (event_buffer[i].timestamp) {
            seq_printf(m, "[%lld] Type: %d Msg: %s\n",
                       ktime_to_ns(event_buffer[i].timestamp),
                       event_buffer[i].type,
                       event_buffer[i].message);
        }
    }
    spin_unlock_irqrestore(&buffer_lock, flags);

    return 0;
}

static int proc_open(struct inode *inode, struct file *file) {
    return single_open(file, proc_show, NULL);
}

static const struct proc_ops proc_fops = {
    .proc_open = proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

/* Module init */
static int __init simulated_device_init(void) {
    int i;
    pr_info("SimulatedDevice: initializing\n");

    spin_lock_init(&buffer_lock);
    event_wq = create_workqueue("event_wq");

    if (!proc_create(PROC_FILENAME, 0444, NULL, &proc_fops)) {
        pr_err("SimulatedDevice: failed to create /proc/%s\n", PROC_FILENAME);
        destroy_workqueue(event_wq);
        return -ENOMEM;
    }

    for (i = 0; i < NUM_SOURCES; i++) {
        hrtimer_init(&timers[i], CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        timers[i].function = timer_callback;
        hrtimer_start(&timers[i], interval, HRTIMER_MODE_REL);
    }

    pr_info("SimulatedDevice: initialized\n");
    return 0;
}

/* Module exit */
static void __exit simulated_device_exit(void) {
    int i;
    flush_workqueue(event_wq);
    destroy_workqueue(event_wq);

    for (i = 0; i < NUM_SOURCES; i++)
        hrtimer_cancel(&timers[i]);

    remove_proc_entry(PROC_FILENAME, NULL);

    pr_info("SimulatedDevice: unloaded\n");
}

module_init(simulated_device_init);
module_exit(simulated_device_exit);
