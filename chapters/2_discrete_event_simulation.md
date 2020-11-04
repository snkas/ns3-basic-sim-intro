# Chapter 2: What is discrete-event simulation?

Ns-3 is a **discrete-event** network **simulator**.
This concept of discrete-event simulation can be condensed to a small piece of pseudo-code:

```
event_queue Q  // Contains events ordered by time, each event has (time T, function to execute (with parameters))
insert_first_events(Q)
current_time = 0
while (Q.not_empty()) {
    next_event = Q.pop()  // Front = the event with the lowest time T
    current_time = next_event.T
    next_event.execute()  // Function can insert future events into Q and modify state
}
```


## Notes to get a general intuition

1. Initially of course you have to insert first event(s) into the event queue Q
   (else the while loop exits immediately)

2. An event's `execute()` can only insert future events into Q
   which have a T >= `current_time`

3. As you can observe in the code, the `current_time` increases as 
   the while loop goes from event to event.

4. An event's `execute()` can take any time in wall-clock time, however 
   from a `current_time` perspective no time will have passed.
  
5. Thus, the `execute()` can only simulate time by breaking its operations into events which
   schedule each other. Consider the following example in which you have a set of nodes
   which are simulated to pass packets to each other:
  
   1. Before the simulation start you insert into Q event (T=100ms, nodes[8].Receive(P))
  
   2. At t=100ms, nodes[8].Receive(P): Node 8 wants to send it to node 3.
      It calculates how long it would take to transmit packet P: `1500 byte / 10 Mbit/s = 1.2ms`.
      Node 3 is nearby with a propagation delay of 5ms.
      It inserts into Q event (T=100+1.2+5ms, nodes[3].Receive(P))
     
   3. At t=106.2ms, nodes[3].Receive(P): Node 3 wants to send it to node 113.
      It calculates how long it would take: `1500 byte / 10 Mbit/s = 1.2ms`.
      However, node 113 is located on Mars in the simulation. The propagation delay is 
       66.873 million km / c = 223 064.3ms`. It inserts into Q event
      (T=106.2+1.2+223 064.3ms, nodes[113].Receive(P)).
     
   4. At t=223171.7ms, nodes[113].Receive(P): Node 113 is happy to receive packet P
  
6. The above example simulated 223171.7ms simulation time. However, in wall-clock time (real time)
   executing this will literally have taken less than a 1ms.
   
7. The simulation clock time *jumps* from one event time to the next event time. In the above example,
   there was only something executed at t=100ms, t=106.2ms, and t=223171.7ms. For instance,
   at t=50ms nothing was executed, because there was no event scheduled at that time.
  
8. The duration of executing an event in wall-clock time can be arbitrarily long,
   yet no simulation time will pass. For example, in (iii) nodes[3].Receive(P), we could also
   perform a very difficult calculation involving relativity and whatnot, which might
   take 200 hours (= 7.2e+8ms) to compute the more accurately modelled propagation delay.
   If we want this difficultly simulated to take time, we can do this.
   For instance, let's say 50ms in our simulation we want it take.
   After calculating the propagation delay we insert an event (T=106.2ms + 50ms,
   ComputePropagationFinished()) which in its execution inserts into Q event 
   (T=156.2+1.2+223 064.3ms, nodes[113].Receive(P)).

9. Besides events, of course there can also be state changed in an event's execution,
   which is used and changed by events which are executed over time. 
   "State" effectively means "variables which exist during the simulation".
   For example: a counter on a node which counts how many
   packets it has forwarded, or a queue of packets of which each is waiting for the
   event which dequeues it.


## Practical tips

* **Event's execution is generally the bottleneck: keep it O(1) or O(log n)**

  Generally, insertion of new events into the event queue is not the bottleneck, rather
  the time it takes to execute the events is the dominant time factor.
  In an event execution, you can of course do any computation you want.
  This almost always consists of events modifying state, which consists of data
  structures whose functions have time complexity (e.g., memory allocation,
  queue insertion, passing of events).

  An event type with a slow runtime which is often inserted/executed can slow
  down your simulation significantly. E.g., if you have the arrival of packets
  into a queue of 100000 elements, make sure the queue insertion is O(1) or
  O(log n) at worse. Discrete-event simulation is not magic, it is only the
  simulation of time.
 
* **However: also do not fill up the event queue unnecessarily**

  Of course, insert events only if it is absolutely necessary. If you blow up your
  event queue to 1'000'000+ events, the O(log n) insertion for each new event is
  still going to slow you down. 
  
  The following guidelines should be followed:
  
  * **Do not pre-plan all events of a process which is regenerative**
  
    Example: let's say every 10ms you want to send a ping from A to B. Don't insert
    for the entire duration of the simulation (e.g., 100s) all events (e.g., 10000) but
    instead insert one, which then at the end of its execution inserts the next one 10ms
    from then.
 
  * **Be wary of inserting a far-in-the-future event for a thing which occurs often**
  
    For example, TCP has a concept of CLOSE_WAIT in which it waits before it actually closes
    in case its FIN got list. If you have 100'000 short TCP flows per second, and the CLOSE_WAIT
    timeout is 2s, this will result in there always being 200'000 events in the queue.
    Probably better to set the CLOSE_WAIT timeout to something lower.
 
  * **Group events if the model accuracy loss is acceptable**
  
    It might be that many events actually happen at slightly different times, but that
    with some loss of model accuracy, you can group them. You do this by maintaining a list of
    the things that happened in that time interval, and then having a regenerative event
    in a time interval (e.g., 10ms) which goes over the list. It's like the efficiency
    of garbage collection in Java. 
    
  * **If you can, leave it to post-processing**
  
    You can always save things in state and then later on process them. This is especially
    true for collection of logs or statistics: don't for each thing you log something for
    insert an event at the end of the simulation which processes the logs, but instead
    keep it in state and save a list of the things. Then in post-processing after the simulation
    go over the list.
    
  * **Be careful of events generating events does not spiral out of control**

    Events creating other events is the key concept of discrete event simulation. However,
    be aware that this does not spiral out of control, don't insert insert new events
    continuously at a small future time scale. This can event accidentally: zero is a valid
    future time, indicating it needs to be execute in the same time step as the event
    being executed right now. In strongly typed languages, e.g., C++, an integer
    division of `int a = 10 / 100;` rounds to `a=0`. Discrete event simulation also happens
    at fixed time granularity, so even if you set an event to 0.3 nanoseconds in the future,
    if the granularity is 1 nanosecond, the event will either be 0 or 1ns in the future.


## Next chapter

[&#187; Continue to chapter 3: Ns-3 basics](3_ns3_basics.md)
