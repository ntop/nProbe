# CHANGELOG

#### nProbe 9.0 (March 2020)

## New Features

*  Added flowRelay tool to collect flows through a firewall.
*  Data serialisation based on nDPI: data can now be serialised in either JSON or binary format.
*  Implemented %SEQ_PLEN, %SEQ_TDIFF, %ENTROPY_CLIENT_BYTES and %ENTROPY_SERVER_BYTES.
*  Implemented --dump-bad-packets in collector mode for dumping invalid collected packets
*  Improved GTPv2 plugin with dissection of many new fields.
*  Implemented GTP v1/v2 per IMSI and APN aggregation via --imsi-apn-aggregation.
*  Added HPERM and TZSP encapsulation support.
*  Added %SSL_CIPHER %SSL_UNSAFE_CIPHER %SSL_VERSION.
*  Implemented JA3 support via %JA3C_HASH and %JA3S_HASH.
*  Implemented %SRC_HOST_NAME and %DST_HOST_NAME for host symbolic name export (if known).
*  Implemented IP-in-IP support.
*  Various Diameter plugin improvements.
*  Implemented --collector-passthrough.
*  Added support for IXIA packet trailer.
*  Extended statistics reported via ZMQ so ntopng can better monitor the nProbe status.
*  Implemented ZMQ encryption.
*  Various Kafka export improvements (export plugin).
*  Added support for the latest ElasticSearch format (export plugin).
*  Implemented flow collection drop counters on UDP socket (Linux only).


## Fixes

*  Fixed NetFlow Lite processing.
*  Implemented various checks for discarding corrupted packets that caused nProbe to crash.
*  Fixed flow upscale calculation (e.g. with sFlow traffic).
*  Application latency is now computed properly for some specific TCP flows with retransmissions.
*  Fixed DNS dissection over TCP.
*  Improvements in Out-of-Order and retransmission accounting.
*  Fixes for %EXPORTER_IPV4_ADDRESS with sFlow.
*  Support for the new GeoIP database format.
*  Extended the @NTOPNG@ template macro with new fields supported by ntopng.
*  Fixed bug in RTP plugin that was leading in some cases nprobe to crash.
*  Various minor fixes in the nProbe engine.

-------------------------------------------------------------------------------

#### nProbe 8.6 (October 2018)


## Main New Features

* Added second-by-second client-to-server and server-to-client flow bytes
  * https://www.ntop.org/nprobe/introducing-per-second-measurements-in-nprobe-flow-exports/
* Implemented an embedded web server that can be optionally enabled to
  * Force a flush of all the active flows
  * Receive a response monitored traffic traffic statistics
  * Query the nProbe version
* Seamless support of ElasticSearch 5 and 6 and automatic template push
  * ElasticSearch version is automatically determined upon nProbe startup
  * A proper template is pushed to ElasticSearch on the basis of its version
* Implemented modbus plugin


## Extensions

* Added support for the collection of NetFlow/IPFIX vendor-proprietary information elements through simple configuration files
  * Supported vendors include Sonicwall, Cisco, IXIA, an others
  * Configuration files published at https://github.com/ntop/nProbe/tree/master/custom_fields
* The default NetFlow version is now V9
* Plugins are disabled in collector mode
* Improved support for Ubuntu18
* Implements SIP user agents dissections (Client and Server)
* Implements TCP/UDP/Other min flow size to discard flows below a certain minimum size
* nProbe runs with user 'nprobe' by default, falling back to nobody if user 'nprobe' does not exist
* New NetFlow information elements `%NAT_ORIGINATING_ADDRESS_REALM` and `%NAT_EVENT`
* `L7_PROTO` exports now the protocol in format `<master>.<application>` protocol
* Added fields `%SRC_TO_DST_SECOND_BYTES` and `%DST_TO_SRC_SECOND_BYTES` to export second-by-second flow bytes
* Migrates geolocation support to GeoLite2 and libmaxminddb
* Migration of nProbe manual to HTML
  * Manual available at https://www.ntop.org/guides/nProbe/  

## New Options

* `--http-server` to enable the embedded web server
* `--help-neflow` to dump a long help including plugin and template information


## Fixes

* Checks for hardening comparisons with partial strings
* Further Checks to avoid crossing certain memory bundaries
* Checks to avoid loops with malformed sctp packets
* Fixes for flow start/end times and timestamp calculation in proxy mode
* Fixes issues with SIP call id in RTP flows
* Fixes length calculation in IPFIX variable-length fields
* Fixed ZMQ buffer termination when flushing ZMQ buffers
* Fixed wrong `%EXPORTER_IPV4_ADDRESS` exported over ZMQ export in case on Netflow != v5
* Fixed a race condition that was preventing all flows to be dumped on file
* Fix to avoid dumped files to be owerwritten when `-P` is used with `-F < 60`
* Adds missing librdkafka support on Centos7

-------------------------------------------------------------------------------

#### nProbe 8.4 (April 2018)


## Main New Features

* DPDK support
* Implements Kafka batching, options parsing, and variable number of producers
* Adds Kafka messages transmission statistics


## New Options

* `--plugin-dir` to load plugins from the specified directory
* `--adj-from-as-path` to get previous/next adjacent ASNs from BGP AS-path
* `--disable-sflow-upscale` to disable sFlow upscaling
* `--terminate-in` for debugging purposes


## Extensions

* Implemented ICMP network latency
* Added ICMP type/code on flow keys to differenciate ICMP flows from the same peers
* sFlow upscale now takes into account sample drops
* Improves throughput calculations with NetFlow


## Fixes

* Fixed crash with ICMP traffic and not extended template Rework for whatsapp support
* Fixes framentation issues that could lead to crashes
* Prevents leaks with multiple BGP updates
* Fixes a crash when exporting option templates to Kafka
* Fixes missing fields (e.g, `FIREWALL_EVENT`) in MySQL db columns
* Preserve endianness of string_dump_as_hex NetFlow custom fields
* Fixes overwrite of untunnelled addresses for tunnels after the first
* Updates centos7 mysql dependency to work either with mysql and mariadb
* Fixed invalid FTP detection
* Fix for computing `%DIRECTION` even with reduced temolate IEs
* Fixes wrong sFlow average scale estimation
* Fix for wrapping ZMQ rates > 4Gbps
* Fixed loop bug in plugin handling when multiple plugins are enabled

-------------------------------------------------------------------------------

#### nProbe 8.2 (December 2017)


## Main New Features

* Support for multiple `--zmq` endpoints to load-banace exported flows in a round-robin fashion
* Full support for NetFlow exported by ASA, including firewall events and cumulative counters
* MySQL database interoperability with ntopng using template `-T "@NTOPNG@"`


## New Options

*  Added `--plugin-dir <dir>` for loading plugins from the specified directory


## Extensions

* bgpNextHop support
* sFlow
  * Improved sFlow upscale algorithm and added heuristic to prevent sFlow exporters bugs
  * Fixed throughput calculation and upsampling of sFlow traffic
* Full systemd support for Debian, Ubuntu, Centos, and Raspbian
* Fixes wrong flow first/last calculations when collecting IPFIX
* Added support for flowDurationMillis Fixed bug for properly handling flowStart/flowEndMillis

-------------------------------------------------------------------------------

#### nProbe 8 (June 2017)

## Main New Features

* Reworked packet fragmentation support that was not properly rebuilding packet fragments
* Many tiny bugs fixed that increase stability and metrics reliability
* Implemented BPF filtering with PF_PACKET directional sockets
* Added VXLAN support
* Created multiple kafka publishers to enhance performance
* Implemented options template export via Kafka
* Added support for collection of IXIA URI and Host
* Added @SIP@ and @RTP@ plugin shortcuts for VoIP analysis
* Improved SSL dissection
* Added support for GTPv2 PCO
* Added support for IPFIX flowEndMilliSeconds when observationTimeMilliSeconds (often in Cisco ASA)
* Added ability to export sFlow interface counters via ZMQ
* Added drops (export/elk/too many flows) drops
* Added kflow export (kentik.com)

## New Options

* `--upscale-traffic` to scale sampled sFlow traffic
* `--kafka-enable-batch` and `--kafka-batch-len` to batch flow export to kafka
* `--load-custom-fields` to support custom fields shipped with NetFlow (see http://www.ntop.org/nprobe/collecting-proprietary-flows-with-nprobe/)
* `--max-num-untunnels` to decapsulate up to 16 tunnelling levels.
* `--vlanid-as-iface-idx` to use the VLAN tag as the interface index
* `--zmq-disable-compression` to disable ZMQ data compression

## Extensions

* Implemented min/avg/max throughput with %SRC_TO_DST_MIN_THROUGHPUT %SRC_TO_DST_AVG_THROUGHPUT %SRC_TO_DST_MAX_THROUGHPUT %DST_TO_SRC_MIN_THROUGHPUT %DST_TO_SRC_AVG_THROUGHPUT %DST_TO_SRC_MAX_THROUGHPUT
* Added support in collection of %IN_SRC_MAC %OUT_DST_MAC %FRAGMENTS %CLIENT_NW_LATENCY_MS %SERVER_NW_LATENCY_MS %APPL_LATENCY_MS %RETRANSMITTED_IN_PKTS %RETRANSMITTED_OUT_PKTS %OOORDER_IN_PKTS %OOORDER_OUT_PKTS
* Split %FRAGMENTS IN %SRC_FRAGMENTS and %DST_FRAGMENTS
* Added %NPROBE_IPV4_ADDRESS to export the IP address of the nProbe sensor, whereas %EXPORTER_IPV4_ADDRESS contains the IP address of the flow exporter (e.g. the router that generated the exported flow)
* Implemented %ICMP_IPV4_TYPE, %ICMP_IPV4_CODE, %FLOW_DURATION_MILLISECONDS, %FLOW_DURATION_MICROSECONDS, %FLOW_START_MICROSECONDS, %FLOW_END_MICROSECONDS
* VXLAN VNI exported in %UPSTREAM_TUNNEL_ID and %DOWNSTREAM_TUNNEL_ID

-------------------------------------------------------------------------------

#### nProbe 7.4 (June 22nd, 2016)

## Main New Features

* Lua scriptability and support for plugins: DHCP, DNS, IMAP, RADIUS, SIP, SMTP, GTPv1, POP, FTP, HTTP
* Ability to drop HTTP flows via Lua
* Ability to push flow information into Lua (e.g., flow.protocol, flow.l7_proto)
* ZMQ *data encryption* to safely exchange information with ntopng
* ZMQ *data compression* to reduce the bandwith consumed when interacting with ntopng
* ZMQ *probe mode* to seamlessly work behind firewalls / NATs
* HTTP full requests/responses content dump
* Ability to specify traffic capture direction (tx, rx, both)
* Flows dump to syslog in JSON format
* Flows export to Apache Kafka via the export plugin
* Implemented SSDP and NETBIOS plugins
* Implemented CAPWAP protocol support
* MIPSEL builds

## New Options
* `--add-engineid-to-logfile` to append the NetFlow engineId to dump files
* `--bind-export-interface` to bind export socket to the specified interface
* `--capture-direction` to specify packet capture direction
* `--cli-http-port` command line
* `--disable-startup-checks` to prevent nProbe public ip detection
* `--host` to capture packets from pcap interfaces rather than from a mirror/tap
* `--json-to-syslog` to export flows to syslog in JSON format
* `--notify-plugin` to notify users of a pluging activity immediately and not when the flow has matched
* `--online-license-check` to check nProbe license online
* `--with-minimal-nprobe` to build nProbe with a minimum set of dependencies
* `--zmq-encrypt-pwd` to encrypt ZMQ data
* `--zmq-probe-mode` to implement ZMQ *probe mode*
* `--http-dump-dir` to dump HTTP logs
* `--http-content-dump-dir` to dump full HTTP requests content
* `--http-content-dump-response` to dump both HTTP requests and responses content with `--http-content-dump-dir`
* `--http-content-dump-dir-layout` to specify layout to be used with `--http-content-dump-dir`
* `--http-exec-cmd` to execute a command whenever an HTTP dump directory has been written
* `--minute-expire` to force active flows to expire when a minute is past

## Plugin Extensions
* Extended the export template with %BITTORENT_HASH, %PACKET_HASH, %SSL_SERVER_NAM, %UPSTREAM_SESSION_ID, %DOWNSTREAM_SESSION_ID, %SRC_AS_MAP and %DST_AS_MAP
* Extended the export template to include longitude and latitude (%SRC_IP_LONG, %SRC_IP_LAT, %DST_IP_LONG and %DST_IP_LAT)
* Implemented SIP RTP support, handling of early export and support of OPTIONS messages
* Extended GTPV1 plugin support to field GTPV1_RAT_TYPE
* Extended GTPV2 plugin support to fields GTPV2_C2S_S5_S8_GTPC_IP and GTPV2_S2C_S5_S8_GTPC_IP, GTPV2_PDN_IP, GTPV2_END_USER_IMEI, GTPV2_C2S_S5_S8_GTPU_TEID, GTPV2_C2S_S5_S8_GTPU_IP, GTPV2_S2C_S5_S8_GTPU_TEID, GTPV2_S2C_S5_S8_GTPU_IP, GTPV2_C2S_S5_S8_SGW_GTPU_TEID, GTPV2_S2C_S5_S8_SGW_GTPU_TEID, GTPV2_C2S_S5_S8_SGW_GTPU_IP and GTPV2_S2C_S5_S8_SGW_GTPU_IP
* GTPV2 plugin check to the export bucket when response isn't the right type for a request and when a response has been done from the same peer as the request
* Implemented GTPV2 0x4A message type management
* Implemented Diameter support of fields DIAMETER_CLR_CANCEL_TYPE and DIAMETER_CLR_FLAGS and 3GPP type messages (317, 319, 320, 321, 322 and 323)
* Extended the Diameter plugin in order to export ""Diameter Hop-by-Hop Identifier" information
* Added DOT1Q_SRC_VLAN/DOT1Q_DST_VLAN for supporting Q-in-Q VLANs
* HTTP plugin export of HTTP_X_FORWARDED_FOR and HTTP_VIA fields
* Extended DNS plugin with multicast DNS


## ZMQ
* ZMQ event handling to send interface statistics to ntopng
* ZMQ statistics in flow collector mode
* Ability to use ZMQ zlib compression
* Enhanced ZMQ statistics with bps/pps rates

## Miscellaneous
* Added black list support for netflow data when nprobe is in proxy mode (ipv4 - V5,V9,IPFIX)
* Twitter heuristics to guess user activities
* Implemented support for TCP fast retransmissions
