# Chapter 12: Test your knowledge

Below are a set of common struggles, and a set of questions which you can use to test your knowledge
of the ns-3 framework and the `basic-sim` module.

## Common struggles

The following things you should be aware of:

- The default traffic-control layer qdisc in ns-3 is FqCoDel, which creates a queue for each
  flow going over the interface. FqCoDel is tuned for the Internet! The default values
  are VERY SLOW to react and will permit many packets to get queued before dropping anything.
  (in `basic-sim` you can set qdiscs in the topology file easily).

- The default TCP settings are very limited, for example: initial congestion window 
  size is 1 packet, clock granularity is 1ms, maximum send/receive buffer size is 128 KiB, 
  segment size is 500 byte, and minimum RTO is 1s. (in `basic-sim` there is a class
  called `TcpOptimizer` which tunes these)
  
- There are TWO queues for each interface: the traffic-control qdisc and the net-device internal queue.
  If there is a traffic-control qdisc, the `NetDevice` internal queue will never drop a packet -- the qdisc
  gets notified by the `NetDevice` if there is a free spot. The qdisc on the other hand of course
  CAN decide to drop based on its own queueing policy. (in `basic-sim` you can set net-device
  queues and traffic-control qdiscs in the topology file easily)

## Questions

(Answers are at the bottom)

1. How long does the execution of an event take in simulation time?

2. In default setting, how many queues does a packet encounter from 
   when it gets forwarded by `Ipv4` to an interface till it is forwarded by the next
   node's `Ipv4`?

3. What is the default traffic-control layer queueing discipline in ns-3?

4. What is the default queue implementation in a point-to-point net-device?

5. How many queues can be in a queueing discipline?

6. How much memory is allocated for a 1500 byte packet?

7. Why does distributed mode only work for point-to-point links?


## Answers

1. Zero. Simulation time does not elapse during event execution.

2. Two. One in the outgoing traffic-control layer, and one in the outgoing net-device.
   There are no 

3. Ns-3 aims to follow both RFCs and the current design decisions of Linux.
   Like Linux, the default queueing discipline for Internet stack is set to FqCoDel, 
   with its default parameters tuned to the Internet (50ms+ RTTs). Especially in the 
   data center context this can lead to excessive initial queueing for a flow.
   
4. First-in-first-out (drop-tail).

5. As many as desired. A `FifoQueueDisc` has one queue. A `FqCoDelQueueDisc` creates
   a queue for each flow that goes over it. A `PfifoFastQueueDisc` maintains three queues.
   
6. By default, there is no explicit data allocated for a packet's payload. Only when it is
   accessed does it get instantiated (with zeroes). If you want to set packet payload,
   you can however.
   
7. Because if you have a complex channel (e.g., WiFi) it requires arbitration.
   `PointToPointNetDevice`s can send at their line rate independently of the other
   network devices. It is possible of course to design an MPI message passing scheme
   which incorporates the arbitration, but this is a big investment and any change
   to the channel model will require significant rework.
   
   See also: https://www.nsnam.org/docs/models/html/distributed.html


## Navigation

**Full menu:**

* [Chapter 1: The toolbox](1_toolbox.md)
* [Chapter 2: What is discrete-event simulation?](2_discrete_event_simulation.md)
* [Chapter 3: What are we going to simulate?](3_what_to_simulate.md)
* [Chapter 4: Ns-3 basics](4_ns3_basics.md)
* [Chapter 5: Building ns-3](5_ns3_building.md)
* [Chapter 6: First ns-3 tutorial](6_ns3_tutorial.md)
* [Chapter 7: Adding your own ns-3 module](7_ns3_adding_your_own_module.md)
* [Chapter 8: Writing a manual script](8_ns3_script_manually.md)
* [Chapter 9: Introducing basic-sim](9_ns3_introducing_basic_sim.md)
* [Chapter 10: Writing a script with basic-sim](10_ns3_script_with_basic_sim.md)
* [Chapter 11: Further readings](11_further_readings.md)
* [Chapter 12: Test your knowledge](12_test_your_knowledge.md)
