Introduction
############

Traffic measurements are necessary to operate all types of IP networks. Networks admin need a detailed view of network traffic for security, accounting and management reasons. The compositions of the traffic have to be analyzed accurately when estimating traffic metrics or when finding network problems. All of these measurements have to be made by analyzing all the packets flowing to the central points in the network (such as router and/or switches). The analysis could be done on the fly or by logging all the packets and than post-processing them. But with the increasing network capacities and traffic volumes this kind of approach is not very efficient. Instead similar packets (packets with a set of common properties) can be grouped together composing flows. As an example, a flow can be composed of all flowing packets that share the same source and destination address so a flow can be derived using only some fields of a network packet. This way, similar types of traffic can be stored in a more compact format without loosing the information we are interested in. This information can be aggregated in a flow datagram and exported to a collector able to report network metrics in a user-friendly format.   
When collected this information provides a detailed view of the network traffic. 

Precise network metric measurements is a challenging task so a lot of work has been done in this filed. In commercial environments, NetFlow is probably the de-facto standard for network traffic accounting and billing. NetFlow is a technology originally created by Cisco in 1996 and is now standardized as Internet Protocol Flow Information eXport (IPFIX -- RFC 3917). NetFlow is based on the probe/collector paradigm. The probe, usually part of network appliance such as a router or a switch, is deployed on the measured network segment, it sends traffic information in NetFlow format towards a central collector. 

nProbe is a software NetFlow v5/v9/IPFIX probe able to collect, analyze and export network traffic reports using the standard Cisco NetFlow v5/v9/IPFIX format. It is available for most of the OSs on the market (Windows, BSD, Linux, MacOSX). When installed on a PC, nProbe turn it into a Network-aware monitoring appliance.

This manual aims at describing how to use nProbe, deploy it in networks, and how to develop plugins for extending it functionalities.

Installation
============
Installation instructions can be found at
http://packages.ntop.org/. Nightly and stable builds are
available. Stable builds are intended for production environments whereas
nightly builds are intended for testing.


Windows Notes
-------------
By default, the nProbe installer does not install capture drivers. It is
recommended to install `npcap drivers <https://nmap.org/npcap/windows-10.html>`_
before installing nProbe.

.. note::

   If Wireshark is already installed on Windows, then
   `npcap drivers <https://nmap.org/npcap/windows-10.html>`_ are already installed.

.. _licensing:

Licensing
---------
Binary nProbe instances require a per-server license that is released according to the EULA (End User License Agreement). Each license is perpetual (i.e. it does not expire) and it allows to install updates for one year since purchase/license issue. This means that a license generated on 1/1/2018 will be able to activate new versions of the software until 31/12/2018. If you want to install new versions of the software release after that date, you need to purchase a new license or avoid further updating the software. For source-based nProbes you still have to obey to the nProbe license listed in appendix.

nProbe licenses are generated using the orderId and email you provided when the license has been purchased on the ntop shop. The license page is accessible at https://shop.ntop.org/mklicense/. On such page you need to specify the nprobe systemId and version. You can obtain such information with :code:`nprobe --version` (Linux) or :code:`nprobe /c --version` (Windows). On Linux the nProbe binary is installed under :code:`/usr/bin/` whereas on Windows on :code:`C:/ProgramFiles/nProbe`.

Plugins
~~~~~~~

nProbe plugins are licensed independently from the main nProbe license. Plugin licenses are add-ons which can be added on top of a valid nProbe license and can be purchased at https://shop.ntop.org/.


.. warning::

   Plugin licenses will only work if a valid nProbe license is found. Do not purchase an nProbe plugin license without having a licensed nProbe.


nProbe Professional license comes bundled with HTTP and DNS plugin licenses.

Applying the License
--------------------

Once the nProbe license has been generated, it can be applied in order for
nProbe to become fully-functional.

Linux
~~~~~
On Linux, the license must
be placed in a one-line file :code:`/etc/nprobe.license`. On Windows,
the license must be placed in a one-line file in the nProbe
installation directory which is typically :code:`C:/Program
Files/nProbe`.

Windows
~~~~~~~
To create this one-line file on Windows, open the
Notepad as an administrator and paste the generated license key. Then
select "File->Save as", specify :code:`nprobe.license` as file name and
save it in the nProbe installation directory. Make sure the file name
has no extension (e.g., :code:`.txt`) by selecting "Save as Type: All Files".


Plugins licenses are applied in a way which is identical to the way the main nProbe license is applied as described above. The only thing that changes is the file name of the plugin license file. The plugin license generation procedure will tell you the exact file name to use, which you can then place in the directory above.


.. note::

   A nProbe restart is recommended once the license has been applied
   to make sure all the new functionalities will be unlocked.

