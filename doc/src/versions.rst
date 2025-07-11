Models Comparison Table
-----------------------

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

Plugins
-------

nProbe is extensible and includes several plugins which are unlocked based on the license version. For instance nProbe Enterprise S includes HTTP, DNS and a few more. Please refer to the comparison table above to check the compatibility. Below you can find the list of currently available plugins.

HTTP
~~~~
Decode HTTP traffic and HTTPS certificates. It can generate a comprehensive log of HTTP traffic, including page download and network/server delay.

DHCP
~~~~
Decode DHCP traffic and export DHCP information in flows or file dump.

Export
~~~~~~
Export to ElasticSearchPlugin/Kafka that can natively export flow information into ElasticSearch without third party converters such as Logstash.

DNS
~~~
Decodes DNS traffic, and produce a log of main domain name resolution activities. Microcloud friendly. Available only in binary format.

flow-to-MySQL
~~~~~~~~~~~~~
Dumps exported flows into a MySQL database. This plugin is part of nProbe Pro and it does not require a license.

MySQL
~~~~~
Decodes (unencrypted) MySQL traffic, and produce a log of SQL requests/responses along with performance indicators.

Email: IMAP, POP3, SMTP
~~~~~~~~~~~~~~~~~~~~~~~
Email plugins for decoding (unencrypted) email traffic and generate flows and logs of email activities.

Voice: SIP, RTP
~~~~~~~~~~~~~~~
Plugins for decoding VoIP (Voice over IP) traffic and producing call log, and voice information (jitter and packet loss, pseudo-MOS/R-Factor).

Radius
~~~~~~
Plugin decoding Radius traffic including 3GPP extensions for mobile networks.

Diameter
~~~~~~~~
Plugin decoding Diameter traffic for both wired and mobile networks.

GTPv0
~~~~~
Same as GTPv1 plugin, just for v0 protocol version.

GTPv1
~~~~~
Plugin for decoding GTPv1-C (2G and 3G networks) signalling and producing comprehensive mobile user and traffic tracking.

GTPv2
~~~~~
Same as GTPv1 plugin, just for v2 protocol version used in LTE (Long Term Evolution) mobile networks.

SSDP
~~~~
Plugin decoding SSDP (Simple Service Discovery Protocol) traffic used on networks to discover network devices and services.

NetFlow-Lite
~~~~~~~~~~~~
Plugin for collecting NetFlow-Lite traffic sent by some Cisco switches.

NetBIOS
~~~~~~~
Plugin decoding NetBIOS traffic used in Windows networks.
