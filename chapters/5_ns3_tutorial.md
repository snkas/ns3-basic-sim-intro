# Chapter 5: First ns-3 tutorial

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


## Next chapter

[&#187; Continue to chapter 6: Adding your own ns-3 module](6_ns3_adding_your_own_module.md)
