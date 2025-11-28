adarsh@adarsh:~/simulate-multi-event-dev-driver$ sudo dmesg | tail -n 20
[19450.343736] SimulatedDevice: Event 1 processed
[19450.343736] SimulatedDevice: Event 2 processed
[19450.443897] SimulatedDevice: Event 0 processed
[19450.443903] SimulatedDevice: Event 1 processed
[19450.443904] SimulatedDevice: Event 2 processed
[19450.545345] SimulatedDevice: Event 0 processed
[19450.545350] SimulatedDevice: Event 1 processed
[19450.545351] SimulatedDevice: Event 2 processed
[19450.645025] SimulatedDevice: Event 0 processed
[19450.645031] SimulatedDevice: Event 1 processed
[19450.645032] SimulatedDevice: Event 2 processed
[19450.743784] SimulatedDevice: Event 0 processed
[19450.743792] SimulatedDevice: Event 1 processed
[19450.743793] SimulatedDevice: Event 2 processed
[19450.843644] SimulatedDevice: Event 0 processed
[19450.843649] SimulatedDevice: Event 1 processed
[19450.843650] SimulatedDevice: Event 2 processed
[19450.943692] SimulatedDevice: Event 0 processed
[19450.943698] SimulatedDevice: Event 1 processed




adarsh@adarsh:~/simulate-multi-event-dev-driver$ cat /proc/fake_events 
Recent events:
[19431169409210] Type: 2 Msg: Event from source 2
[19431269108077] Type: 0 Msg: Event from source 0
[19431269114574] Type: 1 Msg: Event from source 1
[19431269115532] Type: 2 Msg: Event from source 2
[19431369431666] Type: 0 Msg: Event from source 0
[19431369517228] Type: 1 Msg: Event from source 1
[19431369520710] Type: 2 Msg: Event from source 2
[19431546271494] Type: 0 Msg: Event from source 0
[19431546277866] Type: 1 Msg: Event from source 1
[19431546278773] Type: 2 Msg: Event from source 2
[19431570468573] Type: 0 Msg: Event from source 0
[19431570474441] Type: 1 Msg: Event from source 1
[19431570475374] Type: 2 Msg: Event from source 2
[19431679181939] Type: 0 Msg: Event from source 0
[19431679189471] Type: 1 Msg: Event from source 1
[19431679190429] Type: 2 Msg: Event from source 2
[19431794764225] Type: 0 Msg: Event from source 0
[19431794779067] Type: 1 Msg: Event from source 1
[19431794781856] Type: 2 Msg: Event from source 2
[19431903734671] Type: 0 Msg: Event from source 0
[19431903741045] Type: 1 Msg: Event from source 1
[19431903741919] Type: 2 Msg: Event from source 2
[19431985942748] Type: 0 Msg: Event from source 0
[19431985950113] Type: 1 Msg: Event from source 1
[19431985951000] Type: 2 Msg: Event from source 2
[19432078294876] Type: 0 Msg: Event from source 0
[19432078301463] Type: 1 Msg: Event from source 1
[19432078302404] Type: 2 Msg: Event from source 2
[19432232269794] Type: 0 Msg: Event from source 0
[19432232308123] Type: 1 Msg: Event from source 1
