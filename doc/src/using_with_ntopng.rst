.. _UsingNprobeWithNtopng:

Using nProbe with ntopng
########################

Traffic data generated or collected by nProbe can be exported to ntopng that takes
care of data visualization. This is convenient in several scenarios, including:

- *The visualization of NetFlow/sFlow data originated by routers, switches, and network devices in general.* In this scenario, nProbe collects and parse NetFlow/sFlow traffic from the devices, and send the resulting flows to ntopng for the visualization.
- *The monitoring of physical network interfaces that are attached to remote systems.*  In this scenario, ntopng cannot directly monitor network interfaces nor it can see their packets. One or multiple nProbe can be used to capture remote network interfaces traffic and send the resulting flows towards a central ntopng for the analysis and visualization.

The following picture summarizes the two scenarios highlighted above and demonstrates that they can also be combined together.

.. figure:: ./img/using_nprobe_with_ntopng.png
  :align: center
  :alt: Using nProbe with ntopng

  Using nProbe with ntopng

Instructions for configuring nProbe and ntopng are available in the 
`Using ntopng with nProbe section of the ntopng User's Guide 
<http://www.ntop.org/guides/ntopng/using_with_nprobe.html>`_.

