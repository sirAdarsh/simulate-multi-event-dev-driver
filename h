[1mdiff --git a/.simulated_device.o.cmd b/.simulated_device.o.cmd[m
[1mindex 071b8ac..c63c99e 100644[m
[1m--- a/.simulated_device.o.cmd[m
[1m+++ b/.simulated_device.o.cmd[m
[36m@@ -1064,6 +1064,51 @@[m [mdeps_simulated_device.o := \[m
   /usr/src/linux-headers-6.14.0-36-generic/include/linux/string_choices.h \[m
   /usr/src/linux-headers-6.14.0-36-generic/include/linux/proc_fs.h \[m
     $(wildcard include/config/PROC_PID_ARCH_STATUS) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/device.h \[m
[32m+[m[32m    $(wildcard include/config/HAS_IOMEM) \[m
[32m+[m[32m    $(wildcard include/config/GENERIC_MSI_IRQ) \[m
[32m+[m[32m    $(wildcard include/config/ENERGY_MODEL) \[m
[32m+[m[32m    $(wildcard include/config/PINCTRL) \[m
[32m+[m[32m    $(wildcard include/config/ARCH_HAS_DMA_OPS) \[m
[32m+[m[32m    $(wildcard include/config/DMA_DECLARE_COHERENT) \[m
[32m+[m[32m    $(wildcard include/config/DMA_CMA) \[m
[32m+[m[32m    $(wildcard include/config/SWIOTLB) \[m
[32m+[m[32m    $(wildcard include/config/SWIOTLB_DYNAMIC) \[m
[32m+[m[32m    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_DEVICE) \[m
[32m+[m[32m    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_CPU) \[m
[32m+[m[32m    $(wildcard include/config/ARCH_HAS_SYNC_DMA_FOR_CPU_ALL) \[m
[32m+[m[32m    $(wildcard include/config/DMA_OPS_BYPASS) \[m
[32m+[m[32m    $(wildcard include/config/DMA_NEED_SYNC) \[m
[32m+[m[32m    $(wildcard include/config/IOMMU_DMA) \[m
[32m+[m[32m    $(wildcard include/config/PM_SLEEP) \[m
[32m+[m[32m    $(wildcard include/config/OF) \[m
[32m+[m[32m    $(wildcard include/config/DEVTMPFS) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/dev_printk.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/energy_model.h \[m
[32m+[m[32m    $(wildcard include/config/SCHED_DEBUG) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/sched/cpufreq.h \[m
[32m+[m[32m    $(wildcard include/config/CPU_FREQ) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/sched/topology.h \[m
[32m+[m[32m    $(wildcard include/config/SCHED_CLUSTER) \[m
[32m+[m[32m    $(wildcard include/config/SCHED_MC) \[m
[32m+[m[32m    $(wildcard include/config/CPU_FREQ_GOV_SCHEDUTIL) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/sched/idle.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/sched/sd_flags.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/ioport.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/klist.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/pm.h \[m
[32m+[m[32m    $(wildcard include/config/VT_CONSOLE_SLEEP) \[m
[32m+[m[32m    $(wildcard include/config/CXL_SUSPEND) \[m
[32m+[m[32m    $(wildcard include/config/PM) \[m
[32m+[m[32m    $(wildcard include/config/PM_CLK) \[m
[32m+[m[32m    $(wildcard include/config/PM_GENERIC_DOMAINS) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/device/bus.h \[m
[32m+[m[32m    $(wildcard include/config/ACPI) \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/device/class.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/device/driver.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/arch/x86/include/asm/device.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/pm_wakeup.h \[m
[32m+[m[32m  /usr/src/linux-headers-6.14.0-36-generic/include/linux/cdev.h \[m
 [m
 simulated_device.o: $(deps_simulated_device.o)[m
 [m
[1mdiff --git a/simulated_device.c b/simulated_device.c[m
[1mindex c686b99..1e970e9 100644[m
[1m--- a/simulated_device.c[m
[1m+++ b/simulated_device.c[m
[36m@@ -1,3 +1,6 @@[m
[32m+[m[32m// simulated_device_adv.c[m
[32m+[m[32m// Simulated multi-source interrupt device with per-CPU buffers, rate-limiting and char device control[m
[32m+[m[32m// Build: use kernel headers; tested conceptually on modern kernels[m
 #include <linux/module.h>[m
 #include <linux/kernel.h>[m
 #include <linux/init.h>[m
[36m@@ -7,15 +10,36 @@[m
 #include <linux/spinlock.h>[m
 #include <linux/seq_file.h>[m
 #include <linux/proc_fs.h>[m
[32m+[m[32m#include <linux/slab.h>[m
[32m+[m[32m#include <linux/percpu.h>[m
[32m+[m[32m#include <linux/cpumask.h>[m
[32m+[m[32m#include <linux/uaccess.h>[m
[32m+[m[32m#include <linux/fs.h>[m
[32m+[m[32m#include <linux/device.h>[m
[32m+[m[32m#include <linux/cdev.h>[m
[32m+[m[32m#include <linux/atomic.h>[m
 [m
 MODULE_LICENSE("GPL");[m
[31m-MODULE_AUTHOR("Your Name");[m
[31m-MODULE_DESCRIPTION("Simulated Multi-Interrupt Event Logger");[m
[31m-MODULE_VERSION("0.1");[m
[32m+[m[32mMODULE_AUTHOR("You");[m
[32m+[m[32mMODULE_DESCRIPTION("Advanced Simulated Multi-Interrupt Device (per-CPU, rate limit, char dev)");[m
[32m+[m[32mMODULE_VERSION("0.2");[m
 [m
[31m-#define EVENT_BUFFER_SIZE 128[m
[31m-#define PROC_FILENAME "fake_events"[m
 #define NUM_SOURCES 3[m
[32m+[m[32m#define EVENT_BUFFER_SIZE 64       // per-CPU buffer size[m
[32m+[m[32m#define PROC_FILENAME "fake_events_adv"[m
[32m+[m[32m#define DEFAULT_RATE 10            // tokens/sec default[m
[32m+[m[32m#define DEFAULT_BURST 20           // burst tokens[m
[32m+[m[32m#define INTERVAL_NS (100 * 1000 * 1000LL) // 100ms[m
[32m+[m
[32m+[m[32m/* ioctl definitions */[m
[32m+[m[32m#define SIMIO_MAGIC 's'[m
[32m+[m[32mstruct rate_cmd { int src; unsigned int rate; unsigned int burst; };[m
[32m+[m[32mstruct stats_user { unsigned int processed[NUM_SOURCES]; unsigned int dropped[NUM_SOURCES]; };[m
[32m+[m
[32m+[m[32m#define SIMIO_SET_RATE _IOW(SIMIO_MAGIC, 1, struct rate_cmd)[m
[32m+[m[32m#define SIMIO_GET_STATS _IOR(SIMIO_MAGIC, 2, struct stats_user)[m
[32m+[m[32m#define SIMIO_ENABLE_STORM _IOW(SIMIO_MAGIC, 3, int)  // 0/1[m
[32m+[m[32m#define SIMIO_CLEAR_BUFF _IO(SIMIO_MAGIC, 4)[m
 [m
 typedef enum {[m
     EVENT_NETWORK = 0,[m
[36m@@ -24,131 +48,377 @@[m [mtypedef enum {[m
 } event_type_t;[m
 [m
 struct event {[m
[31m-    ktime_t timestamp;[m
[32m+[m[32m    ktime_t ts;[m
     event_type_t type;[m
[31m-    char message[64];[m
[32m+[m[32m    char msg[48];[m
 };[m
 [m
[31m-static struct event event_buffer[EVENT_BUFFER_SIZE];[m
[31m-static int buffer_head = 0;[m
[31m-static spinlock_t buffer_lock;[m
[32m+[m[32m/* Per-CPU buffer and metadata */[m
[32m+[m[32mstruct per_cpu_buf {[m
[32m+[m[32m    struct event buf[EVENT_BUFFER_SIZE];[m
[32m+[m[32m    unsigned int head;[m
[32m+[m[32m    spinlock_t lock;[m
[32m+[m[32m};[m
 [m
[31m-static struct workqueue_struct *event_wq;[m
[32m+[m[32mstatic DEFINE_PER_CPU(struct per_cpu_buf, pc_buf);[m
[32m+[m
[32m+[m[32m/* Global counters and rate control per source */[m
[32m+[m[32mstruct src_ctrl {[m
[32m+[m[32m    unsigned int rate;      // tokens added per second[m
[32m+[m[32m    unsigned int burst;     // maximum tokens[m
[32m+[m[32m    u64 last_ns;            // last refill time (ns)[m
[32m+[m[32m    unsigned int tokens;    // current tokens[m
[32m+[m[32m    atomic_t processed;[m
[32m+[m[32m    atomic_t dropped;[m
[32m+[m[32m};[m
[32m+[m
[32m+[m[32mstatic struct src_ctrl srcs[NUM_SOURCES];[m
[32m+[m[32mstatic bool storm_mode = false;[m
 [m
[32m+[m[32m/* Workqueue and event work */[m
 struct event_work {[m
     struct work_struct work;[m
     event_type_t type;[m
[32m+[m[32m    int target_cpu;[m
 };[m
[32m+[m[32mstatic struct workqueue_struct *event_wq;[m
 [m
[32m+[m[32m/* Timers */[m
 static struct hrtimer timers[NUM_SOURCES];[m
[31m-static ktime_t interval = 1e8; // 100ms[m
[32m+[m[32mstatic ktime_t interval;[m
[32m+[m
[32m+[m[32m/* Character device */[m
[32m+[m[32mstatic int major = 0;[m
[32m+[m[32mstatic struct class *sim_class;[m
[32m+[m[32mstatic struct device *sim_device;[m
[32m+[m[32mstatic struct cdev sim_cdev;[m
[32m+[m
[32m+[m[32m/* Proc */[m
[32m+[m[32mstatic const struct proc_ops proc_fops; // forward[m
[32m+[m
[32m+[m[32m/* Helper: refill tokens for a src */[m
[32m+[m[32mstatic void refill_tokens(struct src_ctrl *s)[m
[32m+[m[32m{[m
[32m+[m[32m    u64 now_ns = ktime_to_ns(ktime_get());[m
[32m+[m[32m    if (!s->last_ns) {[m
[32m+[m[32m        s->last_ns = now_ns;[m
[32m+[m[32m        return;[m
[32m+[m[32m    }[m
[32m+[m[32m    u64 delta_ns = now_ns - s->last_ns;[m
[32m+[m[32m    if (delta_ns == 0) return;[m
[32m+[m[32m    // tokens to add = rate * seconds[m
[32m+[m[32m    u64 add = (s->rate * delta_ns) / 1000000000ULL;[m
[32m+[m[32m    if (add) {[m
[32m+[m[32m        unsigned int new_tokens = s->tokens + (unsigned int)add;[m
[32m+[m[32m        if (new_tokens > s->burst) new_tokens = s->burst;[m
[32m+[m[32m        s->tokens = new_tokens;[m
[32m+[m[32m        s->last_ns = now_ns;[m
[32m+[m[32m    }[m
[32m+[m[32m}[m
 [m
[31m-/* Workqueue handler: process event */[m
[31m-static void process_event(struct work_struct *work) {[m
[31m-    struct event_work *ew = container_of(work, struct event_work, work);[m
[32m+[m[32m/* push event into per-cpu buffer (called from workqueue context) */[m
[32m+[m[32mstatic void push_event_cpu(event_type_t type, int cpu)[m
[32m+[m[32m{[m
[32m+[m[32m    struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);[m
     unsigned long flags;[m
[32m+[m[32m    spin_lock_irqsave(&p->lock, flags);[m
[32m+[m[32m    p->buf[p->head].ts = ktime_get();[m
[32m+[m[32m    p->buf[p->head].type = type;[m
[32m+[m[32m    scnprintf(p->buf[p->head].msg, sizeof(p->buf[p->head].msg), "Src %d on CPU %d", type, cpu);[m
[32m+[m[32m    p->head = (p->head + 1) % EVENT_BUFFER_SIZE;[m
[32m+[m[32m    spin_unlock_irqrestore(&p->lock, flags);[m
[32m+[m[32m}[m
 [m
[31m-    spin_lock_irqsave(&buffer_lock, flags);[m
[31m-[m
[31m-    event_buffer[buffer_head].timestamp = ktime_get();[m
[31m-    event_buffer[buffer_head].type = ew->type;[m
[31m-    snprintf(event_buffer[buffer_head].message, sizeof(event_buffer[buffer_head].message),[m
[31m-             "Event from source %d", ew->type);[m
[31m-[m
[31m-    buffer_head = (buffer_head + 1) % EVENT_BUFFER_SIZE;[m
[31m-[m
[31m-    spin_unlock_irqrestore(&buffer_lock, flags);[m
[31m-[m
[31m-    printk(KERN_INFO "SimulatedDevice: Event %d processed\n", ew->type);[m
[31m-[m
[32m+[m[32m/* Work handler (bottom half) */[m
[32m+[m[32mstatic void process_event(struct work_struct *work)[m
[32m+[m[32m{[m
[32m+[m[32m    struct event_work *ew = container_of(work, struct event_work, work);[m
[32m+[m[32m    int cpu = ew->target_cpu;[m
[32m+[m[32m    push_event_cpu(ew->type, cpu);[m
[32m+[m[32m    atomic_inc(&srcs[ew->type].processed);[m
[32m+[m[32m    pr_info("simdev: processed src=%d on cpu=%d\n", ew->type, cpu);[m
     kfree(ew);[m
 }[m
 [m
[31m-/* Timer callback simulating an interrupt */[m
[31m-static enum hrtimer_restart timer_callback(struct hrtimer *timer) {[m
[32m+[m[32m/* Timer callback simulating IRQ */[m
[32m+[m[32mstatic enum hrtimer_restart timer_cb(struct hrtimer *t)[m
[32m+[m[32m{[m
[32m+[m[32m    int src = -1;[m
[32m+[m[32m    int cpu;[m
     struct event_work *ew;[m
[32m+[m[32m    struct src_ctrl *sc;[m
[32m+[m
[32m+[m[32m    if (t == &timers[0]) src = EVENT_NETWORK;[m
[32m+[m[32m    else if (t == &timers[1]) src = EVENT_KEYBOARD;[m
[32m+[m[32m    else src = EVENT_SENSOR;[m
[32m+[m
[32m+[m[32m    sc = &srcs[src];[m
[32m+[m[32m    /* simple token refill + rate-limit logic */[m
[32m+[m[32m    refill_tokens(sc);[m
[32m+[m
[32m+[m[32m    /* in storm_mode, we intentionally bypass rate limiting to test overload */[m
[32m+[m[32m    if (!storm_mode) {[m
[32m+[m[32m        if (sc->tokens == 0) {[m
[32m+[m[32m            atomic_inc(&sc->dropped);[m
[32m+[m[32m            /* drop this event */[m
[32m+[m[32m            hrtimer_forward_now(t, interval);[m
[32m+[m[32m            return HRTIMER_RESTART;[m
[32m+[m[32m        }[m
[32m+[m[32m        sc->tokens--;[m
[32m+[m[32m    } else {[m
[32m+[m[32m        /* increment dropped occasionally to indicate storm measurement (optional) */[m
[32m+[m[32m    }[m
 [m
[31m-    ew = kmalloc(sizeof(*ew), GFP_ATOMIC);[m
[31m-    if (!ew) return HRTIMER_NORESTART;[m
[31m-[m
[31m-    ew->type = (timer == &timers[0]) ? EVENT_NETWORK :[m
[31m-               (timer == &timers[1]) ? EVENT_KEYBOARD : EVENT_SENSOR;[m
[32m+[m[32m    /* Choose a target CPU to process this event: round-robin across online cpus */[m
[32m+[m[32m    cpu = cpumask_first(cpu_online_mask) + (src % num_online_cpus()); // crude mapping[m
[32m+[m[32m    if (cpu >= nr_cpu_ids) cpu = smp_processor_id();[m
 [m
[32m+[m[32m    ew = kmalloc(sizeof(*ew), GFP_ATOMIC);[m
[32m+[m[32m    if (!ew) {[m
[32m+[m[32m        atomic_inc(&sc->dropped);[m
[32m+[m[32m        hrtimer_forward_now(t, interval);[m
[32m+[m[32m        return HRTIMER_RESTART;[m
[32m+[m[32m    }[m
[32m+[m[32m    ew->type = src;[m
[32m+[m[32m    ew->target_cpu = cpu;[m
     INIT_WORK(&ew->work, process_event);[m
     queue_work(event_wq, &ew->work);[m
 [m
[31m-    hrtimer_forward_now(timer, interval);[m
[32m+[m[32m    hrtimer_forward_now(t, interval);[m
     return HRTIMER_RESTART;[m
 }[m
 [m
[31m-/* Procfs interface */[m
[31m-static int proc_show(struct seq_file *m, void *v) {[m
[31m-    int i;[m
[31m-    unsigned long flags;[m
[31m-[m
[31m-    spin_lock_irqsave(&buffer_lock, flags);[m
[31m-    seq_printf(m, "Recent events:\n");[m
[31m-    for (i = 0; i < EVENT_BUFFER_SIZE; i++) {[m
[31m-        if (event_buffer[i].timestamp) {[m
[31m-            seq_printf(m, "[%lld] Type: %d Msg: %s\n",[m
[31m-                       ktime_to_ns(event_buffer[i].timestamp),[m
[31m-                       event_buffer[i].type,[m
[31m-                       event_buffer[i].message);[m
[32m+[m[32m/* ---- procfs to dump per-cpu buffers ---- */[m
[32m+[m[32mstatic int proc_show(struct seq_file *m, void *v)[m
[32m+[m[32m{[m
[32m+[m[32m    int cpu;[m
[32m+[m[32m    for_each_online_cpu(cpu) {[m
[32m+[m[32m        struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);[m
[32m+[m[32m        unsigned long flags;[m
[32m+[m[32m        int i;[m
[32m+[m[32m        spin_lock_irqsave(&p->lock, flags);[m
[32m+[m[32m        seq_printf(m, "CPU %d head=%u\n", cpu, p->head);[m
[32m+[m[32m        for (i = 0; i < EVENT_BUFFER_SIZE; i++) {[m
[32m+[m[32m            struct event *e = &p->buf[i];[m
[32m+[m[32m            if (e->ts) {[m
[32m+[m[32m                seq_printf(m, "  [%llu] src=%d msg=%s\n",[m
[32m+[m[32m                           (unsigned long long)ktime_to_ns(e->ts),[m
[32m+[m[32m                           e->type, e->msg);[m
[32m+[m[32m            }[m
         }[m
[32m+[m[32m        spin_unlock_irqrestore(&p->lock, flags);[m
     }[m
[31m-    spin_unlock_irqrestore(&buffer_lock, flags);[m
[31m-[m
     return 0;[m
 }[m
[31m-[m
[31m-static int proc_open(struct inode *inode, struct file *file) {[m
[32m+[m[32mstatic int proc_open(struct inode *inode, struct file *file)[m
[32m+[m[32m{[m
     return single_open(file, proc_show, NULL);[m
 }[m
[31m-[m
[31m-static const struct proc_ops proc_fops = {[m
[32m+[m[32mstatic const struct proc_ops proc_ops = {[m
     .proc_open = proc_open,[m
     .proc_read = seq_read,[m
     .proc_lseek = seq_lseek,[m
     .proc_release = single_release,[m
 };[m
 [m
[31m-/* Module init */[m
[31m-static int __init simulated_device_init(void) {[m
[32m+[m[32m/* ---- char device ioctl handlers ---- */[m
[32m+[m[32mstatic long simio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)[m
[32m+[m[32m{[m
[32m+[m[32m    int enabled;[m
[32m+[m[32m    struct rate_cmd rc;[m
[32m+[m[32m    struct stats_user st;[m
     int i;[m
[31m-    pr_info("SimulatedDevice: initializing\n");[m
[32m+[m[32m    void __user *uarg = (void __user *)arg;[m
[32m+[m
[32m+[m[32m    switch (cmd) {[m
[32m+[m[32m    case SIMIO_SET_RATE:[m
[32m+[m[32m        if (copy_from_user(&rc, uarg, sizeof(rc))) return -EFAULT;[m
[32m+[m[32m        if (rc.src < 0 || rc.src >= NUM_SOURCES) return -EINVAL;[m
[32m+[m[32m        if (rc.rate == 0) return -EINVAL;[m
[32m+[m[32m        srcs[rc.src].rate = rc.rate;[m
[32m+[m[32m        srcs[rc.src].burst = rc.burst ? rc.burst : rc.rate;[m
[32m+[m[32m        pr_info("simdev: set rate src=%d rate=%u burst=%u\n", rc.src, rc.rate, srcs[rc.src].burst);[m
[32m+[m[32m        return 0;[m
[32m+[m
[32m+[m[32m    case SIMIO_GET_STATS:[m
[32m+[m[32m        for (i = 0; i < NUM_SOURCES; i++) {[m
[32m+[m[32m            st.processed[i] = (unsigned int)atomic_read(&srcs[i].processed);[m
[32m+[m[32m            st.dropped[i] = (unsigned int)atomic_read(&srcs[i].dropped);[m
[32m+[m[32m        }[m
[32m+[m[32m        if (copy_to_user(uarg, &st, sizeof(st))) return -EFAULT;[m
[32m+[m[32m        return 0;[m
[32m+[m
[32m+[m[32m    case SIMIO_ENABLE_STORM:[m
[32m+[m[32m        if (copy_from_user(&enabled, uarg, sizeof(int))) return -EFAULT;[m
[32m+[m[32m        storm_mode = (enabled != 0);[m
[32m+[m[32m        pr_info("simdev: storm_mode=%d\n", storm_mode);[m
[32m+[m[32m        return 0;[m
[32m+[m
[32m+[m[32m    case SIMIO_CLEAR_BUFF:[m
[32m+[m[32m        /* clear per-cpu buffers */[m
[32m+[m[32m        for_each_online_cpu(i) {[m
[32m+[m[32m            struct per_cpu_buf *p = &per_cpu(pc_buf, i);[m
[32m+[m[32m            unsigned long flags;[m
[32m+[m[32m            spin_lock_irqsave(&p->lock, flags);[m
[32m+[m[32m            memset(p->buf, 0, sizeof(p->buf));[m
[32m+[m[32m            p->head = 0;[m
[32m+[m[32m            spin_unlock_irqrestore(&p->lock, flags);[m
[32m+[m[32m        }[m
[32m+[m[32m        pr_info("simdev: cleared buffers\n");[m
[32m+[m[32m        return 0;[m
 [m
[31m-    spin_lock_init(&buffer_lock);[m
[31m-    event_wq = create_workqueue("event_wq");[m
[32m+[m[32m    default:[m
[32m+[m[32m        return -ENOTTY;[m
[32m+[m[32m    }[m
[32m+[m[32m}[m
 [m
[31m-    if (!proc_create(PROC_FILENAME, 0444, NULL, &proc_fops)) {[m
[31m-        pr_err("SimulatedDevice: failed to create /proc/%s\n", PROC_FILENAME);[m
[31m-        destroy_workqueue(event_wq);[m
[32m+[m[32mstatic ssize_t simio_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)[m
[32m+[m[32m{[m
[32m+[m[32m    struct event e;[m
[32m+[m[32m    int cpu = smp_processor_id();[m
[32m+[m[32m    struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);[m
[32m+[m[32m    unsigned long flags;[m
[32m+[m
[32m+[m[32m    spin_lock_irqsave(&p->lock, flags);[m
[32m+[m[32m    if (p->head == 0) { // no events[m
[32m+[m[32m        spin_unlock_irqrestore(&p->lock, flags);[m
[32m+[m[32m        return 0;[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    // pop the oldest event[m
[32m+[m[32m    e = p->buf[(p->head - 1 + EVENT_BUFFER_SIZE) % EVENT_BUFFER_SIZE];[m
[32m+[m[32m    p->head = (p->head - 1 + EVENT_BUFFER_SIZE) % EVENT_BUFFER_SIZE;[m
[32m+[m[32m    spin_unlock_irqrestore(&p->lock, flags);[m
[32m+[m
[32m+[m[32m    // copy to user[m
[32m+[m[32m    if (count < sizeof(e))[m
[32m+[m[32m        return -EINVAL;[m
[32m+[m
[32m+[m[32m    if (copy_to_user(buf, &e, sizeof(e)))[m
[32m+[m[32m        return -EFAULT;[m
[32m+[m
[32m+[m[32m    return sizeof(e);[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mstatic int simio_open(struct inode *inode, struct file *file) { return 0; }[m
[32m+[m[32mstatic int simio_release(struct inode *inode, struct file *file) { return 0; }[m
[32m+[m
[32m+[m[32mstatic const struct file_operations simio_fops = {[m
[32m+[m[32m    .owner = THIS_MODULE,[m
[32m+[m[32m    .unlocked_ioctl = simio_ioctl,[m
[32m+[m[32m    .open = simio_open,[m
[32m+[m[32m    .release = simio_release,[m
[32m+[m[32m    .read = simio_read,[m
[32m+[m[32m};[m
[32m+[m
[32m+[m[32m/* ---- module init / exit ---- */[m
[32m+[m[32mstatic int __init simdev_init(void)[m
[32m+[m[32m{[m
[32m+[m[32m    int i, cpu;[m
[32m+[m[32m    dev_t dev;[m
[32m+[m[32m    int ret;[m
[32m+[m
[32m+[m[32m    pr_info("simdev: init\n");[m
[32m+[m
[32m+[m[32m    /* init per-cpu buffers */[m
[32m+[m[32m    for_each_possible_cpu(cpu) {[m
[32m+[m[32m        struct per_cpu_buf *p = &per_cpu(pc_buf, cpu);[m
[32m+[m[32m        memset(p, 0, sizeof(*p));[m
[32m+[m[32m        spin_lock_init(&p->lock);[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    /* init src controls */[m
[32m+[m[32m    for (i = 0; i < NUM_SOURCES; i++) {[m
[32m+[m[32m        srcs[i].rate = DEFAULT_RATE;[m
[32m+[m[32m        srcs[i].burst = DEFAULT_BURST;[m
[32m+[m[32m        srcs[i].tokens = srcs[i].burst;[m
[32m+[m[32m        srcs[i].last_ns = ktime_to_ns(ktime_get());[m
[32m+[m[32m        atomic_set(&srcs[i].processed, 0);[m
[32m+[m[32m        atomic_set(&srcs[i].dropped, 0);[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    /* create workqueue */[m
[32m+[m[32m    event_wq = create_workqueue("sim_event_wq");[m
[32m+[m[32m    if (!event_wq) {[m
[32m+[m[32m        pr_err("simdev: create_workqueue failed\n");[m
         return -ENOMEM;[m
     }[m
 [m
[32m+[m[32m    /* create char device */[m
[32m+[m[32m    ret = alloc_chrdev_region(&dev, 0, 1, "simirq");[m
[32m+[m[32m    if (ret) {[m
[32m+[m[32m        pr_err("simdev: alloc_chrdev_region failed\n");[m
[32m+[m[32m        destroy_workqueue(event_wq);[m
[32m+[m[32m        return ret;[m
[32m+[m[32m    }[m
[32m+[m[32m    major = MAJOR(dev);[m
[32m+[m[32m    cdev_init(&sim_cdev, &simio_fops);[m
[32m+[m[32m    sim_cdev.owner = THIS_MODULE;[m
[32m+[m[32m    ret = cdev_add(&sim_cdev, dev, 1);[m
[32m+[m[32m    if (ret) {[m
[32m+[m[32m        pr_err("simdev: cdev_add failed\n");[m
[32m+[m[32m        unregister_chrdev_region(dev, 1);[m
[32m+[m[32m        destroy_workqueue(event_wq);[m
[32m+[m[32m        return ret;[m
[32m+[m[32m    }[m
[32m+[m[32m    sim_class = class_create("simdev");[m
[32m+[m[32m    if (IS_ERR(sim_class)) {[m
[32m+[m[32m        pr_err("simdev: class_create failed\n");[m
[32m+[m[32m        cdev_del(&sim_cdev);[m
[32m+[m[32m        unregister_chrdev_region(dev, 1);[m
[32m+[m[32m        destroy_workqueue(event_wq);[m
[32m+[m[32m        return PTR_ERR(sim_class);[m
[32m+[m[32m    }[m
[32m+[m[32m    sim_device = device_create(sim_class, NULL, dev, NULL, "simirq");[m
[32m+[m[32m    if (IS_ERR(sim_device)) {[m
[32m+[m[32m        pr_err("simdev: device_create failed\n");[m
[32m+[m[32m        class_destroy(sim_class);[m
[32m+[m[32m        cdev_del(&sim_cdev);[m
[32m+[m[32m        unregister_chrdev_region(dev, 1);[m
[32m+[m[32m        destroy_workqueue(event_wq);[m
[32m+[m[32m        return PTR_ERR(sim_device);[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    /* proc entry */[m
[32m+[m[32m    if (!proc_create(PROC_FILENAME, 0444, NULL, &proc_ops)) {[m
[32m+[m[32m        pr_warn("simdev: failed to create /proc/%s\n", PROC_FILENAME);[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    /* start timers */[m
[32m+[m[32m    interval = ktime_set(0, INTERVAL_NS);[m
     for (i = 0; i < NUM_SOURCES; i++) {[m
         hrtimer_init(&timers[i], CLOCK_MONOTONIC, HRTIMER_MODE_REL);[m
[31m-        timers[i].function = timer_callback;[m
[32m+[m[32m        timers[i].function = timer_cb;[m
         hrtimer_start(&timers[i], interval, HRTIMER_MODE_REL);[m
     }[m
 [m
[31m-    pr_info("SimulatedDevice: initialized\n");[m
[32m+[m[32m    pr_info("simdev: started (major=%d /dev/simirq)\n", major);[m
     return 0;[m
 }[m
 [m
[31m-/* Module exit */[m
[31m-static void __exit simulated_device_exit(void) {[m
[32m+[m[32mstatic void __exit simdev_exit(void)[m
[32m+[m[32m{[m
     int i;[m
[32m+[m[32m    dev_t dev = MKDEV(major, 0);[m
[32m+[m
[32m+[m[32m    pr_info("simdev: exit\n");[m
[32m+[m[32m    for (i = 0; i < NUM_SOURCES; i++) hrtimer_cancel(&timers[i]);[m
[32m+[m
[32m+[m[32m    /* flush and destroy workqueue */[m
     flush_workqueue(event_wq);[m
     destroy_workqueue(event_wq);[m
 [m
[31m-    for (i = 0; i < NUM_SOURCES; i++)[m
[31m-        hrtimer_cancel(&timers[i]);[m
[31m-[m
[32m+[m[32m    /* remove proc */[m
     remove_proc_entry(PROC_FILENAME, NULL);[m
 [m
[31m-    pr_info("SimulatedDevice: unloaded\n");[m
[32m+[m[32m    /* destroy char device */[m
[32m+[m[32m    device_destroy(sim_class, dev);[m
[32m+[m[32m    class_destroy(sim_class);[m
[32m+[m[32m    cdev_del(&sim_cdev);[m
[32m+[m[32m    unregister_chrdev_region(dev, 1);[m
[32m+[m
[32m+[m[32m    pr_info("simdev: stopped\n");[m
 }[m
 [m
[31m-module_init(simulated_device_init);[m
[31m-module_exit(simulated_device_exit);[m
[32m+[m[32mmodule_init(simdev_init);[m
[32m+[m[32mmodule_exit(simdev_exit);[m
[1mdiff --git a/simulated_device.ko b/simulated_device.ko[m
[1mindex 5454377..fa2be48 100644[m
Binary files a/simulated_device.ko and b/simulated_device.ko differ
[1mdiff --git a/simulated_device.mod.c b/simulated_device.mod.c[m
[1mindex 9163310..7c5ed0e 100644[m
[1m--- a/simulated_device.mod.c[m
[1m+++ b/simulated_device.mod.c[m
[36m@@ -18,103 +18,166 @@[m [m__section(".gnu.linkonce.this_module") = {[m
 [m
 static const struct modversion_info ____versions[][m
 __used __section("__versions") = {[m
[31m-	{ 0xcb8b6ec6, "kfree" },[m
[31m-	{ 0xd272d446, "__x86_return_thunk" },[m
[31m-	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },[m
 	{ 0x003b23f9, "single_open" },[m
[32m+[m	[32m{ 0x4ff89cd9, "pcpu_hot" },[m
[32m+[m	[32m{ 0x092a35a2, "_copy_to_user" },[m
[32m+[m	[32m{ 0xd272d446, "__stack_chk_fail" },[m
[32m+[m	[32m{ 0xf296206e, "nr_cpu_ids" },[m
[32m+[m	[32m{ 0xb5c51982, "__cpu_online_mask" },[m
[32m+[m	[32m{ 0x86632fd6, "_find_next_bit" },[m
 	{ 0xf2c4f3f1, "seq_printf" },[m
[32m+[m	[32m{ 0xb5c51982, "__cpu_possible_mask" },[m
 	{ 0xdf4bee3d, "alloc_workqueue" },[m
[32m+[m	[32m{ 0x9f222e1e, "alloc_chrdev_region" },[m
[32m+[m	[32m{ 0xefd5d5d8, "cdev_init" },[m
[32m+[m	[32m{ 0x4c1dbbd9, "cdev_add" },[m
[32m+[m	[32m{ 0xea5ac1d9, "class_create" },[m
[32m+[m	[32m{ 0xf98f93a7, "device_create" },[m
 	{ 0x82c6f73b, "proc_create" },[m
 	{ 0x535f4f5f, "hrtimer_init" },[m
 	{ 0x5fa07cc0, "hrtimer_start_range_ns" },[m
[32m+[m	[32m{ 0x0bc5fb0d, "unregister_chrdev_region" },[m
 	{ 0xbeb1d261, "destroy_workqueue" },[m
[31m-	{ 0xbeb1d261, "__flush_workqueue" },[m
[32m+[m	[32m{ 0x0c72f9ad, "cdev_del" },[m
[32m+[m	[32m{ 0x14fcde53, "class_destroy" },[m
 	{ 0x36a36ab1, "hrtimer_cancel" },[m
[32m+[m	[32m{ 0xbeb1d261, "__flush_workqueue" },[m
 	{ 0x33c78c8a, "remove_proc_entry" },[m
[32m+[m	[32m{ 0x6fdeeff0, "device_destroy" },[m
[32m+[m	[32m{ 0x092a35a2, "_copy_from_user" },[m
[32m+[m	[32m{ 0x494c552b, "_find_first_bit" },[m
[32m+[m	[32m{ 0x2182515b, "__num_online_cpus" },[m
 	{ 0xbd03ed67, "random_kmalloc_seed" },[m
 	{ 0xfed1e3bc, "kmalloc_caches" },[m
 	{ 0x70db3fe4, "__kmalloc_cache_noprof" },[m
 	{ 0x49733ad6, "queue_work_on" },[m
 	{ 0x5a844b26, "__x86_indirect_thunk_rax" },[m
 	{ 0x49fc4616, "hrtimer_forward" },[m
[32m+[m	[32m{ 0xe4de56b4, "__ubsan_handle_load_invalid_value" },[m
 	{ 0xbd4e501f, "seq_read" },[m
 	{ 0xfc8fa4ce, "seq_lseek" },[m
 	{ 0xcb077514, "single_release" },[m
 	{ 0xd272d446, "__fentry__" },[m
[32m+[m	[32m{ 0xd272d446, "__x86_return_thunk" },[m
[32m+[m	[32m{ 0x5ae9ee26, "__per_cpu_offset" },[m
 	{ 0xe1e1f979, "_raw_spin_lock_irqsave" },[m
 	{ 0x97acb853, "ktime_get" },[m
[31m-	{ 0x40a621c5, "snprintf" },[m
[32m+[m	[32m{ 0x40a621c5, "scnprintf" },[m
 	{ 0x81a1a811, "_raw_spin_unlock_irqrestore" },[m
 	{ 0xe8213e80, "_printk" },[m
[32m+[m	[32m{ 0xcb8b6ec6, "kfree" },[m
[32m+[m	[32m{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },[m
 	{ 0xba157484, "module_layout" },[m
 };[m
 [m
 static const u32 ____version_ext_crcs[][m
 __used __section("__version_ext_crcs") = {[m
[31m-	0xcb8b6ec6,[m
[31m-	0xd272d446,[m
[31m-	0x90a48d82,[m
 	0x003b23f9,[m
[32m+[m	[32m0x4ff89cd9,[m
[32m+[m	[32m0x092a35a2,[m
[32m+[m	[32m0xd272d446,[m
[32m+[m	[32m0xf296206e,[m
[32m+[m	[32m0xb5c51982,[m
[32m+[m	[32m0x86632fd6,[m
 	0xf2c4f3f1,[m
[32m+[m	[32m0xb5c51982,[m
 	0xdf4bee3d,[m
[32m+[m	[32m0x9f222e1e,[m
[32m+[m	[32m0xefd5d5d8,[m
[32m+[m	[32m0x4c1dbbd9,[m
[32m+[m	[32m0xea5ac1d9,[m
[32m+[m	[32m0xf98f93a7,[m
 	0x82c6f73b,[m
 	0x535f4f5f,[m
 	0x5fa07cc0,[m
[32m+[m	[32m0x0bc5fb0d,[m
 	0xbeb1d261,[m
[31m-	0xbeb1d261,[m
[32m+[m	[32m0x0c72f9ad,[m
[32m+[m	[32m0x14fcde53,[m
 	0x36a36ab1,[m
[32m+[m	[32m0xbeb1d261,[m
 	0x33c78c8a,[m
[32m+[m	[32m0x6fdeeff0,[m
[32m+[m	[32m0x092a35a2,[m
[32m+[m	[32m0x494c552b,[m
[32m+[m	[32m0x2182515b,[m
 	0xbd03ed67,[m
 	0xfed1e3bc,[m
 	0x70db3fe4,[m
 	0x49733ad6,[m
 	0x5a844b26,[m
 	0x49fc4616,[m
[32m+[m	[32m0xe4de56b4,[m
 	0xbd4e501f,[m
 	0xfc8fa4ce,[m
 	0xcb077514,[m
 	0xd272d446,[m
[32m+[m	[32m0xd272d446,[m
[32m+[m	[32m0x5ae9ee26,[m
 	0xe1e1f979,[m
 	0x97acb853,[m
 	0x40a621c5,[m
 	0x81a1a811,[m
 	0xe8213e80,[m
[32m+[m	[32m0xcb8b6ec6,[m
[32m+[m	[32m0x90a48d82,[m
 	0xba157484,[m
 };[m
 static const char ____version_ext_names[][m
 __used __section("__version_ext_names") =[m
[31m-	"kfree\0"[m
[31m-	"__x86_return_thunk\0"[m
[31m-	"__ubsan_handle_out_of_bounds\0"[m
 	"single_open\0"[m
[32m+[m	[32m"pcpu_hot\0"[m
[32m+[m	[32m"_copy_to_user\0"[m
[32m+[m	[32m"__stack_chk_fail\0"[m
[32m+[m	[32m"nr_cpu_ids\0"[m
[32m+[m	[32m"__cpu_online_mask\0"[m
[32m+[m	[32m"_find_next_bit\0"[m
 	"seq_printf\0"[m
[32m+[m	[32m"__cpu_possible_mask\0"[m
 	"alloc_workqueue\0"[m
[32m+[m	[32m"alloc_chrdev_region\0"[m
[32m+[m	[32m"cdev_init\0"[m
[32m+[m	[32m"cdev_add\0"[m
[32m+[m	[32m"class_create\0"[m
[32m+[m	[32m"device_create\0"[m
 	"proc_create\0"[m
 	"hrtimer_init\0"[m
 	"hrtimer_start_range_ns\0"[m
[32m+[m	[32m"unregister_chrdev_region\0"[m
 	"destroy_workqueue\0"[m
[31m-	"__flush_workqueue\0"[m
[32m+[m	[32m"cdev_del\0"[m
[32m+[m	[32m"class_destroy\0"[m
 	"hrtimer_cancel\0"[m
[32m+[m	[32m"__flush_workqueue\0"[m
 	"remove_proc_entry\0"[m
[32m+[m	[32m"device_destroy\0"[m
[32m+[m	[32m"_copy_from_user\0"[m
[32m+[m	[32m"_find_first_bit\0"[m
[32m+[m	[32m"__num_online_cpus\0"[m
 	"random_kmalloc_seed\0"[m
 	"kmalloc_caches\0"[m
 	"__kmalloc_cache_noprof\0"[m
 	"queue_work_on\0"[m
 	"__x86_indirect_thunk_rax\0"[m
 	"hrtimer_forward\0"[m
[32m+[m	[32m"__ubsan_handle_load_invalid_value\0"[m
 	"seq_read\0"[m
 	"seq_lseek\0"[m
 	"single_release\0"[m
 	"__fentry__\0"[m
[32m+[m	[32m"__x86_return_thunk\0"[m
[32m+[m	[32m"__per_cpu_offset\0"[m
 	"_raw_spin_lock_irqsave\0"[m
 	"ktime_get\0"[m
[31m-	"snprintf\0"[m
[32m+[m	[32m"scnprintf\0"[m
 	"_raw_spin_unlock_irqrestore\0"[m
 	"_printk\0"[m
[32m+[m	[32m"kfree\0"[m
[32m+[m	[32m"__ubsan_handle_out_of_bounds\0"[m
 	"module_layout\0"[m
 ;[m
 [m
 MODULE_INFO(depends, "");[m
 [m
 [m
[31m-MODULE_INFO(srcversion, "F812E7BF477706E39D8D5DB");[m
[32m+[m[32mMODULE_INFO(srcversion, "CF5451DE16C8D62135C5CCF");[m
[1mdiff --git a/simulated_device.mod.o b/simulated_device.mod.o[m
[1mindex 9e83706..0d81f4f 100644[m
Binary files a/simulated_device.mod.o and b/simulated_device.mod.o differ
[1mdiff --git a/simulated_device.o b/simulated_device.o[m
[1mindex 84f3a3f..7382ac1 100644[m
Binary files a/simulated_device.o and b/simulated_device.o differ
