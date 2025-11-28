                hrtimer expires
                       │
                       ▼
      ========================================
                HARDIRQ (Top Half)
      ========================================
                 Timer Callback
             (cannot sleep, fast)
                       │
                       ▼
     queue_work(event_wq, &work)
                       │
                       ▼
      ========================================
            WORKQUEUE (Bottom Half)
      ========================================
       runs in kworker → sleeps allowed
    writes event to buffer, prints log





--------------------------------------------------------




 ┌──────────────────────────────────────────────────────────────┐
 │                        Kernel Module                         │
 └──────────────────────────────────────────────────────────────┘

         ┌────────────────────────────────────────────────┐
         │               HRTIMER SOURCES                  │
         │ (Simulated Interrupt Generators — 100ms each)  │
         └────────────────────────────────────────────────┘
                   │               │                │
                   ▼               ▼                ▼

        ┌────────────────┐ ┌────────────────┐ ┌────────────────┐
        │ NETWORK Timer  │ │ KEYBOARD Timer │ │  SENSOR Timer  │
        └────────────────┘ └────────────────┘ └────────────────┘
                   │               │                │
                   │ Timer fires   │ Timer fires    │ Timer fires
                   ▼               ▼                ▼

=======================================================================
                 (Simulated INTERRUPT CONTEXT — FAST)
=======================================================================

                      TIMER CALLBACK (ISR)
 ------------------------------------------------------------------------------
  • Identify which timer fired  
  • Allocate event_work  
  • INIT_WORK()  
  • queue_work(event_wq, &work)  
  • hrtimer_forward_now()  
 ------------------------------------------------------------------------------

                   │
                   ▼

=======================================================================
                 (Deferred work — BOTTOM HALF)
=======================================================================

              WORKQUEUE: event_wq (kworker threads)
              --------------------------------------
              Many events may run in parallel here.
              --------------------------------------

                   ▼

        ┌───────────────────────────────────────────────┐
        │            process_event(work_struct)         │
        └───────────────────────────────────────────────┘
        • Acquire spinlock  
        • Write into circular event_buffer  
        • buffer_head = (buffer_head + 1) % N  
        • Release spinlock  
        • printk("Event processed")  

                   ▼

┌───────────────────────────────────────────────────────────────┐
│                  Circular Event Buffer (Protected)             │
│───────────────────────────────────────────────────────────────│
│ [0] timestamp, type, "Event from source X"                    │
│ [1] timestamp, type, "Event from source X"                    │
│ .... up to EVENT_BUFFER_SIZE                                  │
└───────────────────────────────────────────────────────────────┘

                   ▼

=======================================================================
                   USER-SPACE INTERFACE (Procfs)
=======================================================================

                     /proc/fake_events

 When user runs:
       $ cat /proc/fake_events

 Kernel executes:
     proc_show()
       • Acquire spinlock
       • Print all valid events in seq_file format
       • Release spinlock
       • Return output to user

=======================================================================
