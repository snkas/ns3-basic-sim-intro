# Chapter 4: Ns-3 basics

ns-3 is a discrete-event **network** simulator. In this chapter we discuss
the absolute basics of ns-3.

To emphasize again: ns-3 is essentially the 9 lines of pseudo-code of chapter 2,
written in C++, with a whole lot of helpers around it to aid abstraction. 

- **The state** consists of network concepts, such as queues, TCP state, counters,
  or forwarding state.
  
- **The events** are about those network concepts. Most of these events are
  about packets arriving or being sent, (e.g., transmission of a
  packet causing dequeueing, sending or receiving of packets by TCP, TCP timeouts),
  however it can be anything related to the network that you want to model:
  changes to the links (e.g., going down, changing bandwidth), forwarding
  state updates, etc..


## `Node` is the core state concept

In ns-3, the network is a collection of `Node`s. The `Node` is the core concept of ns-3.
Everything is attached to a `Node`, or attached to things attached to a `Node`.
Think of it as a node in a graph, except there are no edges explicitly defined.
The `Node`s and what is attached to it is the state.

A `Node` has the following state:

- A list of `NetDevice`s

  A `NetDevice` you can think of as physical network port on a computer. This can be
  a thing in which you plug a cable, or which uses radio frequencies to communicate.
  It encompasses the driver queue and the NIC in the Linux networking stack.
  A node communicates to other nodes exclusively through its `NetDevice`s by handing
  `Packet`s to it or receiving `Packet`s from it.

- A list of `Application`s
  
  An `Application` you can equate to "something that can produce packets out of nowhere
  and receive packets taking them out of the network".
  
- A list of protocol handlers

  When a Node gets a `Packet` handed to it from either (a) a `NetDevice` or (b) an `Application`
  it basically asks itself "What do I do with this?". When it gets handed a packet, luckily,
  the `NetDevice` or `Application` tell also what is the outer header. For all intents and 
  purposes, generally the outer header is "IP". Then it looks up in its list of protocol handlers
  "IP", and returns a protocol implementation.
  
  The protocol implementation then gets the `Packet`, and decides to either (a) give it to a local
  Application, (b) pass it along to a `NetDevice` because it should be forwarded to another Node,
  or (c) whatever else the protocol demands: it can drop it (effectively, this means
  just do nothing with it and return), do an `Application`-like thing and generate new `Packet`s
  (for example, ICMP reply, or if it should be broadcast).
  
  A `Node` has multiple protocols attached to it when it has the Internet stack on it:
  * `Ipv4L3Protocol`
  * `ArpL3Protocol`
  * `TcpL4Protocol`
  * `UdpL4Protocol`
  * `Icmpv4L4Protocol`
  * ...
  
  Some of which call each other (e.g., `Ipv4L3Protocol` will read the IP header and then
  forward to `TcpL4Protocol` if the next header is TCP). Although `Application`s are able
  to directly interact with the `Node` generally they interact with a `Protocol`.


## Traffic-control

The Internet stack maintains an interface for each `NetDevice`.
The Internet stack Protocols do not directly pass outgoing packets to its respective `NetDevice`.
Instead they give it to the `TrafficControlLayer`, which maintains
a queueing discipline (a more general version of a queue) for each interface.
After passing through the queue (discipline) in the `TrafficControlLayer`
it is actually passed to the `NetDevice`. A `NetDevice` itself
also has an internal queue.
 
In ns-3 is simulation and consists only of C++ code, which can be edited easily.
As such, from a simulation standpoint, it seems like the distinction between
queueing discipline and net-device internal queue are rather arbitrary.
In the previous version, ns-2, these were actually combined together!
However, separating them is more accurate with the real-world which
we are modeling (see previous chapter). Secondly, it is a good software engineering
practice (i.e., decouple functionality) to have more maintainable code.
It is thus important to keep this distinction!

You might want to install different traffic-control on different nodes. E.g., if a Node
is meant to be a Linux endpoint machine in an Internet setting, FqCoDel might be good.
For things that represent switches, it might be that the type of switch you want to
simulate does not have FqCoDel (which is most of them) supported and instead you want to
do first-in-first-out (FIFO) or priority queuing (PFifoFast). You can even disable
the traffic-control layer is not present on the switch you want to model.

**Key takeaways:**

1. There are TWO queues before a Packet is sent out by the `NetDevice`: (1) the queue (discipline)
   of the `TrafficControlLayer` for that interface/`NetDevice`, and (2) the `NetDevice`'s own internal
   queue implementation.

2. The `TrafficControlLayer` queueing discipline asks the `NetDevice` queue gets notified if there
   is space to accept a Packet. It does not pass a Packet if there is no space. The only drops
   occur at the `TrafficControlLayer` queueing discipline for that interface. If there is a traffic-control queueing discipline for the `NetDevice` (interface),
   the `NetDevice`'s internal queue will never drop a `Packet`!


## Medium between `Node`s: `Channel`

`NetDevice`s are connected to each other via `Channel`(s). 
The simplest form of a `NetDevice` is point-to-point, connecting only two `NetDevice`s
(you can think of it as a cable between them). However, the `Channel` can be any medium,
such as e.g., representing various frequency spectra for WiFi.


## What is passed around: `Packet`

A `Packet` consists of a payload and headers. Payloads are allocated in memory in a lazy
fashion -- only if they are defined or accessed are they actually allocated.
As such, as long as you don't actually touch the payload, the payload size is just a
number to use for calculations (e.g., `(headers_size + payload_size) / data_rate` 
to determine how long it takes to transmit the packet).


## Overview diagram

![Node with Internet stack and point-to-point net-devices](images/node-internet-stack-p2p-net-devices.drawio.png)


Notes:
* Although the `Application`s can interact with any layer, typically we use UDP / TCP, so we
  just interact with them by creating their respective sockets via the API offered by `TcpL4Protocol`
  and `UdpL4Protocol`
* The `Ipv4L3Protocol` takes care of the routing decisions (it contains a `Ipv4RoutingProtocol`); it
  inherits from `Ipv4`
* The IP(v4) stack is installed using `InternetStackHelper`
* There is only an egress queueing discipline in the `TrafficControlLayer`
* There is only an egress queue in the `PointToPointNetDevice`


## Navigation

**Continue to the next chapter:**

[&#187; Chapter 5: Building ns-3](5_ns3_building.md)

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
