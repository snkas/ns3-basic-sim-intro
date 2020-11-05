# Chapter 6: First ns-3 tutorial

Ns-3 provides a tutorial as well in which it explains in more detail
the concepts described before. Please go over the tutorial there:

https://www.nsnam.org/docs/tutorial/html/conceptual-overview.html

**The official tutorial is a not very comfortably formatted document, yet it is important
to read the text on this page as it will save you A LOT of struggle
later down the line.**

Below is a short summary of the steps:

1. The `scratch/` folder inside your `ns-3` folder is where you can write
   small C++ scripts which use ns-3. In this tutorial you are going to make
   a very simple one.

2. Copy over the source file `first.cc` from the examples folder to your scratch folder:

   ```
   cp examples/tutorial/first.cc scratch/myfirst.cc
   ```
   
3. Study and understand `scratch/myfirst.cc`. The way a ns-3 program is structured
   is effectively the following steps:
   
   1. Create nodes
   2. Install net-devices on the nodes to give them a way to reach each other
   3. Install the Internet stack on the nodes
   4. Set the IP addresses for the IP interfaces of the net-devices
     (because the Internet stack wants this, net-devices themselves
     don't need an IP address to function)
   5. Install applications on the nodes
   6. Run the simulator
   7. Collect results
   8. Exit
   
4. Build ns-3 with `waf`:

   ```
   ./waf
   ```
   
5. Run the scratch main with `waf`:

   ```
   ./waf --run scratch/myfirst
   ```


## Navigation

**Continue to the next chapter:**

[&#187; Chapter 7: Adding your own ns-3 module](7_ns3_adding_your_own_module.md)

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
