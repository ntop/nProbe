Models Comparison Table
-----------------------

The comparison table below shows feature and capacity of nProbe by version. 
nProbe is extensible and includes several plugins which are also unlocked based on the license version. 
For instance nProbe Enterprise S includes HTTP, DNS and a few more. Please refer to the comparison table 
below to check the compatibility, or check the nProbe Plugins section to learn more about all plugins and
what they provide.

.. list-table:: Features by Model
  :widths: 40 15 15 15 15
  :header-rows: 1

  * - Feature
    - Pro
    - Enterprise S
    - Enterprise M
    - Enterprise L
  * - nDPI Traffic Inspection
    - ✓
    - ✓
    - ✓
    - ✓
  * - Flow Collection
    - ✓
    - ✓
    - ✓
    - ✓
  * - PF_RING Acceleration
    - ✓
    - ✓
    - ✓
    - ✓
  * - HTTP Plugin
    -
    - ✓
    - ✓
    - ✓
  * - DNS Plugin
    -
    - ✓
    - ✓
    - ✓
  * - DHCP Plugin
    -
    -
    - ✓
    - ✓
  * - Diameter Plugin
    -
    -
    - ✓
    - ✓
  * - Elastic/JSON/Kafka Plugin
    -
    -
    - ✓
    - ✓
  * - FTP Plugin
    -
    -
    - ✓
    - ✓
  * - IMAP/SMTP/POP Plugin
    -
    -
    - ✓
    - ✓
  * - NetFlow-Lite Plugin
    -
    - ✓
    - ✓
    - ✓
  * - SIP/RTP Plugin
    -
    -
    - ✓
    - ✓
  * - GTP V0/V1/V2 Plugins
    -
    -
    -
    - ✓
  * - Modbus Plugin
    - ✓
    - ✓
    - ✓
    - ✓
  * - Radius Plugin
    -
    -
    - ✓
    - ✓
  * - IPv4 Packet Deduplication
    - ✓
    - ✓
    - ✓
    - ✓
  * - Agent Mode
    -
    -
    - ✓
    - ✓
  * - Native nTap Support
    -
    -
    - ✓
    - ✓
  * - Flow Collection Deduplication
    -
    -
    - ✓
    - ✓
  * - Max Number of Monitored Hosts
    - Unlimited*
    - Unlimited*
    - Unlimited*
    - Unlimited*
  * - Max Number of Instances per Host/License
    - Unlimited*
    - Unlimited*
    - Unlimited*
    - Unlimited*
  * - Max Flow Collection Devices††
    - 4
    - 8
    - 16
    - 128
  * - Max number of ZMQ exporters (–zmq)
    - 4
    - 8
    - 16
    - 32
  * - Max number of Rules/Pools in IPS Mode (–ips-mode)
    - 4
    - 8
    - 32
    - 256

\* limited by available CPU/memory

††This is the number of flow devices (e.g. a NetFlow router or sFlow switch) from which a single nProbe instance can collect flows from. Note that you can run multiple nProbe instances simultaneously on the same host with a single license.

†Enterprise L is offered with customised features and personalized maximums for flow collection devices and ZMQ exporters. If requested,  it is also available Enterprise XL with capabilities limited by the available computing resources.
