# Chapter 2: What is discrete-event simulation?

You want to simulate a real physical system. The first idea would be to calculate what
the system does at a fixed time step, however at small time step this would take
too long. As such, in discrete-event simulation you only do something if there is
something meaningful: an event occurs. The simulation hops from event to event,
and time progresses as such.

The following terminology is important to understand:

* **Physical time**: time in the physical system (definition from [1])
* **Simulation time**: abstraction used by the simulation to model physical time (definition from [1])
* **Wallclock time**: time during the execution of the simulation program (definition from [1])
* **Event**: something occurs at a certain point in time.
  In the simulation, an event is represented by a data structure with two fields:
  1. Simulation timestamp (integer): at what time moment (in simulation time) it needs to be executed.
  2. Function to execute: the action which is performed, which can modify state and/or
     insert future events into the event queue.
* **Event queue**: a priority queue of events, ordered by the simulation time.
* **Simulation state**: abstraction used by the simulation to model the physical state (definition based on [1])

The mechanism of discrete-event simulation can be condensed to a small piece of pseudo-code:

```
initial state S
event_queue Q  // Contains events ordered by time, each event has (time T, function to execute (with parameters))
insert_first_events(Q)
current_simulation_time = 0
while (Q.not_empty()) {
    next_event = Q.pop()  // Front = the event with the lowest time T
    current_simulation_time = next_event.T
    next_event.execute()  // Function can insert future events into Q and modify state
}
```

## Example

Consider the following example we have a graph of nodes, and want to answer
the question "How long does it take for a packet from node 8 to reach node 113?".
Secondly, we want to afterwards know how many packets were forwarded by each node.
We want to model transmission delay and propagation delay.

The simulation setup and execution would be as follows:
  
  1. Initial state are the nodes. Each node `i` has a counter of how many
     packets it has forwarded, initially set to zero for all (`nodes[i].forward_counter=0`).
  
   2. Before the simulation start you insert into the event queue
      that a packet P (size: 1500 byte) arrives:
      `Event(T=100ms, nodes[8].Receive(P))`
  
   3. At t=100ms, nodes[8].Receive(P) is executed:
   
      - Node 8 wants to send P to node 3, to which it is connected over a 10 Mbit/s link
      - It calculates transmission delay: `1500 byte / 10 Mbit/s = 1.2ms`
      - Node 3 is modeled to be nearby with a propagation delay of 5ms
      - It increments how many packets it has forwarded: `nodes[8].forward_counter++`
      - It inserts into the event queue: `Event(T=100+1.2+5ms, nodes[3].Receive(P))`
     
   4. At t=106.2ms, nodes[3].Receive(P) is executed: 
   
      - Node 3 wants to send P to node 113, to which it is connected over a 10 Mbit/s link
      - It calculates transmission delay: `1500 byte / 10 Mbit/s = 1.2ms`
      - Node 113 is modeled to be located on Mars. The propagation delay is: 
        `66.873 million km / c = 223064.3ms`
      - It increments how many packets it has forwarded: `nodes[3].forward_counter++`
      - It inserts into the event queue: `Event(T=106.2+1.2+223064.3ms, nodes[113].Receive(P))`
     
   5. At t=223171.7ms, nodes[113].Receive(P) is executed:
   
      - Node 113 is happy to receive packet P
      - It saves that the time it takes to receive packet P is 223171.7ms
      
      
The final state is `nodes[8].forward_counter=1` and `nodes[3].forward_counter=1`.
For all other nodes the counter is still zero. The time it takes to
send a packet from node 8 to 113 is 223171.7ms.
   

## Modeling statements

- **You need initial events**

   Initially of course you have to insert first event(s) into the event queue Q
   (else the while loop exits immediately).

- **New events can only be schedule in the future**

   The current simulation time increases (weakly) ascending, as it jumps from
   one event's simulation-timestamp to the next's using the while loop. As such as
   a hard constraint, it is impossible to insert events with a simulation-timestamp
   less than the current simulation time.
   
- **Time is discrete: the current simulation time jumps to the next event time**

   In the above example, there was only something executed at t=100ms, t=106.2ms, and t=223171.7ms.
   For instance, at t=50ms nothing was executed, because there was no event scheduled at that time.
   
- **Events manipulate state**

  Events are there to manipulate state. The insertion of new events is to space out
  when these state manipulations happen (in other words: the modeling of physical
  time in simulation time). "State" effectively means "variables which
  exist during the simulation". Examples for state are: a counter on a node which
  for how many packets a node has forwarded, or a queue of packets of which each
  is waiting for the event which dequeues it.
  
- **If you want to model something which would take time in the physical system, you determine
  how much simulation time you want it to be, and then insert an event in the future**
  
- **Wallclock time can be much longer than simulation time, or the other way around**

   The above example simulated 223171.7ms simulation time. However, in wall-clock time
   executing this will literally have taken less than a 1ms. If the event execution's
   computations would have been very difficult (let's say matrix factorization of a
   billion x billion matrix) it could have also been the other way around.
   
- **You decide what part of the physical system to model in the simulation**

   The duration of executing an event in wall-clock time can be arbitrarily long,
   yet no simulation time will pass. For example, in (iii) nodes[3].Receive(P), we could also
   perform a very difficult calculation involving relativity and whatnot, which might
   take 200 hours (= 7.2e+8ms) to compute the more accurately modelled propagation delay.
   If this calculation would also be present in the physical system and thus also
   take up physical time, we can _decide_ to model this in simulation time.
   For instance, let's say we model it to take 50ms in simulation time.
   After calculating the propagation delay we insert an `Event(T=106.2ms + 50ms,
   ComputePropagationFinished())` which in its execution inserts  
   `Event(T=156.2+1.2+223 064.3ms, nodes[113].Receive(P))`.
   
- **Wallclock time &#8800; physical time**

  The wallclock time is the time it takes to simulate on the computer you are running
  the simulation on. Generally, this is NOT the physical system you are modeling, and even if it
  is, the timings can be significantly off as it is only a model (and also, the computer
  can have other applications running which influence it). 
   
- **Using wallclock time to determine simulation time is bad modeling and results
  in unreproducible experiments and platform dependency**


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
    of garbage collection in Java (process in batches).
    
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
    
* **Pre-process/pre-calculate as much as possible**

  If you have values which you can calculate beforehand which remain the same for each
  experimental run, it might be worthwhile to pre-calculate these, and then read in the
  pre-calculated values from file at the start of each ns-3 run. Depending on the task, the
  pre-calculation you might want to do on many machines or in a more convenient 
  programming language other than C++ (with e.g., better libraries available).
  
  For example: if you have a 20000-node network, pre-calculating and saving routing
  state in file might save you a lot of time each run. 


## References

[1] Definitions from the book: Parallel and Distributed Simulation Systems. Richard M. Fujimoto.

## Navigation

**Continue to the next chapter:**

[&#187; Chapter 3: What are we going to simulate?](3_what_to_simulate.md)

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
