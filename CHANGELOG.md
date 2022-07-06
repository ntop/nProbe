# CHANGELOG

-------------------------------------------------------------------------------

#### nProbe 10.0 (July 2022)

## New Features

 - Agent mode for process monitoring on Linux (eBPF) and Windows
 - Implemented timeseries support for nProbe self-monitoring and sFlow-based counter timeseries
 - Conversion of Amazon AWS VPC files into flows
 - Improved collection of proprietary flows, including Nokia and Calix
 - Support for collecting flows from syslog

## New Command Line Options

 - Add --agent-mode option to enable eBPF support
 - Add --timestamp-format 3/4 option
 - Add --tzsp-port option for specifying custom TZSP port
 - Add --tls-insecure option to skip TLS certificate validation
 - Add --tls-priv-key and --tls-cert options
 - Add --nat-subscribe option
 - Add --ndpi-protocols to limit export only to the selected L7 protocols (e.g. -O "DNS,HTTP")
 - Add --memory-footprint to print memory usage statistics
 - Add --short-proc-stats for printing stats in a parseable format
 - Add --process-learning-duration to control process duration check
 - Add --syslog-flow-format option
 - Add --cache-geoip-asn to cache GeoIP ASN
 - Add --collect-check-bidirectional-flows
 - Add --influxdb-dump-dir and --influxdb-exec-cmd 
 - Add -D j option for dumping flows in JSON format
 - Add c suffic in collector mode to enable flow cache, disabled by default (e.g. -3 2055c)
 - Add tcp:// and udp:// support to -3 option
 - Change --ignore-obs-domain-id-port to --use-obs-domain-id-port (ignore by default)
 
## Extensions

 - Add %FLOW_VERDICT to export the flow verdict in IPS mode
 - Add more DPI IEs
   - %L7_CONFIDENCE
   - %L7_INFO
   - %L7_RISK_INFO to export information about the flow risk
   - %L7_ERROR_CODE
 - Add set of process Information Elements
   - {SRC,DST}_PROC_PID
   - {SRC,DST}_PROC_NAME
   - {SRC,DST}_PROC_UID
   - {SRC,DST}_PROC_USER_NAME
   - {SRC,DST}_PROC_PKG_NAME
   - {SRC,DST}_PROC_CMDLINE
   - {SRC,DST}_PROC_CONTAINER_ID
   - {SRC,DST}_FATHER_PROC_NAME
   - {SRC,DST}_FATHER_PROC_UID
   - {SRC,DST}_FATHER_PROC_PID
   - {SRC,DST}_FATHER_PROC_USER_NAME
   - {SRC,DST}_FATHER_PROC_PKG_NAME
 - Add %APPLICATION_ID and %APPLICATION_NAME
 - Add NAT IEs
   - %POST_NAT_SRC_IPV4_ADDR
   - %POST_NAT_DST_IPV4_ADDR
   - %POST_NAPT_SRC_TRANSPORT_PORT
   - %POST_NAPT_DST_TRANSPORT_PORT
 - Add %AAA_NAT_KEY template key 
 - Add %PORT_RANGE_START and %PORT_RANGE_END
 - Add %INGRESS_VRFID and %EGRESS_VRFID

## Improvements

 - Add support for 16 bit TCP_FLAGS
 - Add support for 64 bit IN/OUT BYTES/PKTS (required by IPFIX)
 - Add support for BSD Tunnel encapsulation
 - Add nProbe stats to influxdb
 - Add sFlow timeseries dump
 - Add check for unidirectional traffic
 - Add support of L2TP with dynamic ports
 - Add UDP data collection via syslog
 - Add NFLOG support
 - Add Nokia NAT/AAA mobile mapping
 - Restore MySQL plugin
 - Improve syslog collection
 - Enlarge the Netfilter queue in IPS mode with nfnl_rcvbufsiz (to avoid ENOBUFS failures)
 - Implement TLS certificate validation 
 - Improve NAT mapping and caching of AAA/NAT
 - Improve @NTOPNG@ ZMQ template with more IEs
 - Improve handling of ZMQ events for IPS mode
 - Improve TZSP support
 - Improve flow collection performance

## Fixes

 - Fix timeseries export
 - Fix FreeBSD support
 - Fix crash with fragmented tunnelled traffic used in plugins
 - Fix potential security issues
 - Fix sscanf potential buffer overwrite vulnerability
 - Fix HTTP dump when using --http-content-dump-dir
 - Fix byte order in ZMQ message size
 - Fix -i none on Windows
 - Fix --black-list
 - Fix invalid dissection of DNS responses with too many NSs entries
 - Fix for memory boundary check
 - Fix buffer access on Fiberblaze
 - Fix -U overwriting the -V value
 - Fix various bugs in IPS mode
 - Fix stats reported in /proc
 - Fix race condition in ZMQ export
 - Fix -HUP file reload
 - Fix handling of L2TP encapsulation
 - Fix user change
 - Fix loopback support on Windows
 - Fix windows interface listing with the new pcap API
 - Fix GTP handling
 - Fix @NTOPNG@ in collector mode (NAT info)
 - Fix invalid engineType/Id parsing
 - Fix 64 bit counter issue in NetFlow v5
 - Fix BPF

## Misc

 - Add support for Ubuntu 22.04, Debian 11, RPi Bullseye, RockyLinux
 - Add nfFanout tool
 - Add tool for collecting from ZMQ and publishing to Google PubSub
 - Update DEB, RPM, FreeBSD, RPi packages dependencies
 - Modified idle timeout to 60 sec (used to be 30 sec) as TCP keep-alive is 45 sec so we can avoid cutting flows (e.g. SSH)
 - Change label HTTP_UA to HTTP_USER_AGENT
 - Add eBPF dependency on supported OSs (Ubuntu 22)
 - Update DPDK support
 - Removed deprecated multithread support (-O)
 - Remove deprecated --online-license-check option

-------------------------------------------------------------------------------

#### nProbe 9.6 (July 2021)

## New Features

 - New support for FreeBSD/OPNsense/pfsense
 - New UI plugin for configuring nProbe in OPNsense
 - New IPS mode, supported both on Linux (based on Netfilter) and FreeBSD/OPNsense/pfsense (based on Netmap)
 - New support for ClickHouse and Maria DB (in addition to MySQL and other export formats)
 - New AWS VPC Flow Logs collection (via dump files)

## New Command Line Options

 - Extend -E to support 16-bit observationDomainId (IPFIX) 
 - Add --ips-mode to enable IPS mode
 - Add --zmq-publish-events to enable collection of events from ntopng, including IPS policies
 - Add --ignore-obs-domain-id-port to ignore probe port and observation domain id
 - Add --ja3plus to enable JA3+
 - Add --version-json for exporting the version and license information in JSON format
 - Add --host-labels to load host labels from file 
 - Add -D 'T' dump format (compressed text)
 - Add --collector-reforge-timestamps for reforging collected timestamps

## Extensions

 - Add %FLOW_VERDICT to report the verdict associated with the flow in IPS mode
 - Add %SRC_TO_DST_MAX_EST_THROUGHPUT %DST_TO_SRC_MAX_EST_THROUGHPUT to export per direction throughput
 - Add %SRC_HOST_LABEL %DST_HOST_LABEL to export host labels configured with --host-labels
 - Add %L7_RISK_SCORE for associating flow risk score with a flow
 - Add %SIP_REGISTER_MAX_RRD %SIP_REGISTER_NUM_OK %SIP_REGISTER_NUM_OTHER SIP IEs
 - Add %SRC_TO_DST_IAT_MIN %SRC_TO_DST_IAT_MAX %SRC_TO_DST_IAT_AVG %SRC_TO_DST_IAT_STDDEV %DST_TO_SRC_IAT_MIN %DST_TO_SRC_IAT_MAX %DST_TO_SRC_IAT_AVG %DST_TO_SRC_IAT_STDDEV min/max/avg/stddev packet IAT
 - Add %OBSERVATION_POINT_TYPE %OBSERVATION_POINT_ID for exporting Observation Point information
 - Add %L7_INFO with L7 flow information (used by ntopng)
 - Add collection of %IPV4_NEXT_HOP %IPV4_BGP_NEXT_HOP %FORWARDING_STATUS IEs

## Improvements

 - Add support for decoding fragmented tunnelled packets
 - Improve Throughput calculation
 - Extend max template size to 256
 - Add handlign of ingress VLAN on sFlow extended switch data
 - Enhance MPLS-tagged packet decoding
 - Improve dump to Influx DB

## Fixes

 - Fix crash when using --pcap-file-list with --zmq
 - Fix Win CLI option handling
 - Fix L2TP dissection of tunnels with optional lenght set
 - Fix -i DIR option (pcaps are read continuously until shutdown)
 - Fix handling of %EXPORTER_IPV4_ADDRESS in template when using @NTOPNG@
 - Fix support of large packets (> MTU) due to GTO/TSO/LRO
 - Fix RTP invalid memory allocation
 - Fix @NTOPNG@ template that caused TCP flags to be sent only on one direction, generating invalid security alerts
 - Fix/rework flow direction and %DIRECTION information element
 - Fix crash with too many templates defined

## Misc

 - Add configuration Wizard (nprobe-config) for configuring nProbe
 - Windows now uses a virtual NT SERVICE\nprobe account
 - Add support for reading the configuration from both the configuration file and CLI parameters (at the same time)
 - Add scripts for configuring Netfilter with nProbe in IPS mode (installed under /usr/share/nprobe/netfilter/scripts)
 - Add/improve support for embedded systems, including:
   - OpenWRT
   - Ubiquiti (e.g. EdgeRouter X)
   - Raspberry (Raspbian)
 - Removed obsolete --ndpi-proto and --ndpi-proto-ports 

-------------------------------------------------------------------------------

#### nProbe 9.2 (November 2020)

## New Features

* Added --collector-passthrough for exporting flows to remote collectors without passing through the flow cache and without any flow format change. This will result is a great speed improvements (+ 5x) with respect to going through the flow cache.
* Added --collector-nf-reforge for filtering and reforging collected netflow flows before exporting to remote collectors

## New Command Line Options

* Added Kafka/ZMQ/Syslog export passthtough support when --collector-passthrough is used
* Added --collector-nf-reforge for filtering and reforging collected netflow flows before exporting to remote collectors
* Changed -p format to <Outer VLAN Id>.<Inner VLAN Id/<proto>/<IP>/<port>/<TOS>/<SCTP StreamId>/<exporter IP>
* Added the ability to specify a binding IPv4 address in collector mode (e.g. -3 127.0.0.1:1234)
* Flow cache is now disabled by default in collection mode: replaced --disable-cache with --enable-collection-cache
* Added --gtpv1-track-non-gtp-u-traffic and --gtpv2-track-non-gtp-u-traffic for non GTP-encapsulated user export in IE %FLOW_USER_NAME

## Extensions

*  Added the ability to sniff from stdin by using -i -
*  Added %L7_PROTO_RISK %L7_PROTO_RISK_NAME
*  Added %TCP_WIN_MAX_IN %TCP_WIN_MAX_OUT IEs to @NTOPNG@
*  Added DNS/HTTP IEs to @NTOPNG@ in probe mode
*  Added collected flow lifetime export via ZMQ
*  Added IP-in-IP (IPv4 encapsulated in IPv6) support
*  Improved DNS plugin with additional records and NAPTR query type
*  Exporting %SEQ_PLEN as 8 bit element
*  Added TOS export via ZMQ
*  GTP traffic analysis improvements
*  Improved IMSI/APN traffic accounting and aggregation when using --imsi-apn-aggregation
*  Support for SIP over TCP (VoLTE)
*  Added IPv6 support in GTPv1
*  Added IPv4+IPv6 GTP-C v2 dissection
*  Improvement on GTP-C v1 dissection
*  Added support for %BGP_PREV_ADJACENT_ASN %BGP_NEXT_ADJACENT_ASN when collecting sFlow and Netflow
*  Added IPv6 PAA export
*  Support for overwriting element names with aliases provided by the user (case sensitive)

## Fixes

*  Fixed detection of multiple connections on the same port (RST) exporting multiple flows
*  Fixed EXPORTER_IPV6_ADDRESS
*  Fixed UNTUNNELED_IPV6_SRC_ADDR / UNTUNNELED_IPV6_DST_ADDR
*  Fixed dump of IPv6 flows to MySQL
*  Fixed shutdown crashes
*  Fixed kafka stats number overflow
*  Fixed multiple --collection-filter options
*  Fixed accounting of bidirectional flows in stats
*  Fixed export of empty data
*  Fixed invalid flow idle computation
*  Fixed CSV export (always print all columns)
*  Fixed AS lookup/calculation via MaxMind
*  Fixed bug that caused FLOW_USER_NAME to be empty
*  Fixed custom template elements support
*  Fixed SIP decoding with malformed packets
*  Fixed IPv6 dissection when encapsulated in GTP
*  Fixed application protocol detection with GTP
*  Fixed GTPv1 GTPV1_END_USER_IP field
*  Fixed drop count

## Miscellaneous

*  Moved all binaries and libraries from /usr/local/ to /usr/
*  Plugins are now loaded from ./plugins, /usr/lib/nprobe/plugins, /usr/local/lib/nprobe/plugins 
*  Added Ubuntu 20.04 support
*  Improved OpenWRT support
*  Windows fixes
*  Improved plugins SDK

-------------------------------------------------------------------------------

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
