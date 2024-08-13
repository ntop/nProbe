# CHANGELOG

-------------------------------------------------------------------------------

#### nProbe 10.6 (Aug 2024)

## Command Line Options
 
 - Add --blacklists <path> for configuring blacklists
 - Add --ndpi-categories-dir <dir>
 - Add --dns-dump-blacklisted for dumping only blacklisted client queries
 - Add --gtpv1-track-imsi and --gtpv2-track-imsi
 - Add --gtpv1-teid-cache-duration and --gtpv2-teid-cache-duration
 - Add --gtp-use-host-in-tunnels
 - Add --estimate-tcp-latency for making TCP network latency optionally estimated in case a flow start was not observed
 - Add -gtp-use-host-in-tunnels for GTPv1
 - Extend --map-ifnames option to accept a file in addition to the CLI

## Improvements

 - New %JA4C_HASH IE
 - New %GTPV1_GSN_ADDRESS_IPV4_A and %GTPV1_GSN_ADDRESS_IPV4_B containing the first and secondGSN IPv4 address
 - New %FLOW_ENCRYPTED IE
 - New %L7_DOMAIN_INFO IE similar to %L7_INFO but returns only the domain name of the host
 - Add support for datalink 10 (raw packets)
 - Add support for flow source for detecting how flows are generated (packets, collection of sflow/netflow, collection of sflow)
 - Improve ZMQ events messages
 - Improve HTTP dump to file (flush after write)
 - Hash size is now automatically incresed when -M is used
 - Various GTP-C/GTP-U improvements
   - Add support for DeleteBearerRequest in GTPv2
   - Enhanced %UPSTREAM_TUNNEL_ID %DOWNSTREAM_TUNNEL_ID with GTP gateways
 - Improve RTP handling with Zoom and Teams
 - Add Zoom p2p support
 - Improve Zoom Media Encapsulation decoding
 - Improve utility to export flows to Google Pub/Sub
   - Add native batching support
   - Add options to control import/export settings
 - Add instance UUID for detecting the individual nprobe instances
   - Add NPROBE_UUID NPROBE_IP Information elements
 - Add support fo HTTPS ports in the HTTP plugin
 - Implement TCP flow swap in case SYN|ACK is observed before SYN
 - Add @timestamp to the ELK plugin

## Fixes

 - Fix Clickhouse database schema types
 - Various RTP fixes
 - Various GTP fixes
   - Restore GTP accounting with --imsi-apn-aggregation
   - Fix GTPv2 delete session handling
   - Add fixes for discarding negative GTP-C responses
   - Fix GTPv2 Bearer Context decoding that was previously unable to handle IPv4/IPv6 F-TEID
   - Fix crash with GTP flow aggregation
   - Fix GTP-C dump
 - Fix DNS additional records decoding
 - Fix --map-postnat cli option
 - Fix plugin handling with null-ethernet (e.g. loopback) encapsulation
 - Fix DHCP export
 - Fix DHCP_CLIENT_NAME dissection
 - Fix TOS handling
 - Fix --in-iface-idx/--out-iface-idx values populated in case of packet capture from network device
 - Fix endianess with --map-postnat
 - Fix memory leaks
 - Fix -p aggregation parsing
 - Fix IMSI correlation
 - Fix double Kafka flow export
 - Fix IPS policing
 - Fix nflite initialization

## Misc

 - Add support for the ntop Cloud
 - ZMQ events are now emitted every 5 sec (used to be every second)
 - Disabled JA3+ support in favour of JA4
 - Diabled --collector-passthrough when ZMQ is in use as this can cause inconsistencies in ntopng side due to template format
 - Add nprobe user to the ntop group
 - Package for Ubuntu 24

-------------------------------------------------------------------------------

#### nProbe 10.4 (Oct 2023)

## New Features
 - Add cloud mode support for integrating with ntopng in cloud mode
 - Rework Kafka export support to make it robust and scalable
 - Merge agent mode code in nProbe so that it is automatically enabled as the templace defined host
 - RTP handling for Zoom/Ms Teams streams
 - Add support for FreeBSD 14, Debian 12, ARM 64 bit

## Command Line Options
 - Add support for encryption to --zmq-publish-events
 - Add support for multi-line json export (--zmq-format m)
 - Add --cloud option to ease integration with ntopng in cloud mode
 - Add --gtpv1-dont-export-flows-immediately and --gtpv2-dont-export-flows-immediately flags for disabling immediate flow export as soon as the first response packet is received
 - Add --map-postnat
 - Add --template-ids to specify the list of templates to accept

## Improvements
 - Implement --map-ifnames <list> for mapping interfaceNames to interfaceIds
 - Implement GTPV1_C2S_TEID/GTPV1_S2C_TEID and GTPV2_C2S_TEID/GTPV2_S2C_TEID
 - Implement ZMQ stress tester
 - Implement packet flow template
 - Implement supoort for proprietary Nokia IPFIX exports
 - Improve VPC log scanning
 - Improve nDPI protocol guess
 - Improve packet decoding checkes
 - Improve parameter check
 - Improve tracing
 - Add support for ndpi serializer in sflow counters export
 - Add %FLOW_CONTENT_TYPE in order to export nDPI-dissected flow datatype (audio, video...)
 - Add %FLOW_EXPORT_TIME that contains the epoch of a flow export
 - Add -6 to sendPcap (Delivery of packets over IPv6)
 - Add BPF filter in cloud mode
 - Add BPF filter when started in --agent-mode
 - Add Ent XL bundle support
 - Add GTPV2_C2S_TEID and GTPV2_S2C_TEID
 - Add Ms Teams support in RTP plugin
 - Add Zoom RTP tracing
 - Add check for avoiding crash when offload is on and packet length exceeds the snaplen size
 - Add code for handling RTP stats when using dynamic payload (e.g. Teams)
 - Add compressed AWS VPC log file support
 - Add debian 12 support
 - Add diameter handling of mesageId 300, 301, 303
 - Add extra check
 - Add support for %EXPORTER_IPV6_ADDRESS
 - Add support of %INTERFACE_NAME %ACCOUNT_ID in VPC log collection
 - Add test pcaps
 - Add trace in case of being unable to delete a file
 - Add tracing of log deletion
 - Add FreeBSD 14 support

## Fixes
 - Fix invalid print with large number of files
 - Fix for Win32 cross-compatibility
 - Fix for collecting bidirectional flows
 - Fix for various memory leaks
 - Fix netflow packets reforging during replay (it was adding 2 extra IP/UDP headers)
 - Fix DNS dissection
 - Fix ELK export with username/pwd Add support for ELK 8
 - Fix RTP detection with RTP flows starting with a STUN packet
 - Fix clickhouse export bug (missing username)
 - Fix flow direction due to a value overflow https://github.com/ntop/ntopng/issues/6267
 - Fix invalid bytes/octets decoding during flow collection
 - Fix buffer overflow issues
 - Fix cache statistics
 - Fix for exporting process information in cloud mode
 - Fix https://github.com/ntop/nProbe/issues/584
 - Fix sampling support Extended -p (aggregation) flag format

## Misc
 - Agent mode changes
 - Changes due to nDPI API changes
 - Changes for adding cloud support in Windows
 - Code cleanup adding a new member to plugin dissector
 - Do not enable the service on upgrade if disabled by the user
 - Enable sflow counters export in TLV using numerial keys
 - Enlarged Diameter port range 3868-3900
 - Extended -p support with discard of OUT (NFv9)/Post (IPFIX) counters
 - Fix Kafka export format
 - Minor fix in custom template handling
 - Missing check to avoid using DPDK (when installed) if --with-dpdk is not used
 - Now -3 allows to recursively scan a directory looking for AVC files
 - Optimization to avoid handling binary data with HTTP plugin
 - Print when running in Cloud mode. Disable demo mode when in Cloud mode.
 - Removed --ucloud Add --instance-name <name> that is exported in %NPROBE_INSTANCE_NAME
 - Rework embedded check (at runtime now)
 - Rework kafka options handling
 - Set edr_mode when the cloud license should unlock the endpoint mode only
 - Workaround for non-standard DLT_NULL encapsulations
 - aarch64 changes

-------------------------------------------------------------------------------

#### nProbe 10.2 (Feb 2023)

## New Features

 - New nTap support (--ntap) for capturing traffic with the new ntop Virtual/Remote TAP (Enterprise M/L/XL)
 - Rework and improve Kafka support (Kafka can be used as an alternative to ZMQ for delivering flow data to ntopng)
 - Introduce support for exporting data to Google Pub/Sub
 - Introduce support for Catchpoint
 - Introduce a new nProbe XL model

## Command Line Options

 - Add --kafka-ntopng <brokers> option to deliver flow data to ntopng
 - Add --snmp-mapping option for mapping SNMP interfaces and export mapping information to ntopng
 - Add --tcp-dont-send-flow-lenght for flow collectors over TCP that do not expect the flow lenght
 - Add --ntopng zmq://<socket>:<port> option (--zmq tcp://<socket> is now deprecated)
 - Add support for encrytion keys in hex format with --zmq-encryption
 - Add -J to ignore Netflow sender port
 - Add the ability to specity an alternative topic in --kafka using "," as topic delimiter
 - Add --accurate-hash flag
 - Change --collector-port|-3 option for ZMQ accepting zmq:// to avoid mixing it with TCP collection
 - Change --use-obs-domain-id-port which is IPFIX only now
 - Rename --use-obs-domain-id-port to --use-obs-domain-id
 - When not specified -n=2055 is now automatically when required (e.g. if - no -P - no --ntopng)
 
## Improvements
 
 - Add support for Linux cooked sockets v2 capture
 - Preserve L7 protocol across flow updates
 - Improve Zoom handling and add Zoom detection in RTP streams
 - Improve RTP call quality calculation
 - Add caching of application ID/Name mapping exported by Cisco NBAR
 - Add custom formatting of Nokia ULI
 - Improve processing of nasty corner cases (e.g. flows with the same 5-tuple)
 
## Tools

 - New build_snmp_mappings.sh tool to build SNMP interface mapping file (to be used with --snmp-mapping)
 - Improve zmqReflector (ZMQ proxy)
 - Improve sendPcap
   - Add -f <filter> option
   - Add the ability to handle multiple senders in the same PCAP file
 
## Fixes

 - Fix HTTP_SITE handling
 - Fix crash in IMAP dissection
 - Fix decoding loop with invalid Diameter packets
 - Fix for supporting reassembly of Diameter flows on non-standard ports
 - Fix bug with --collector-nf-reforge
 - Fix SCTP dissection
 - Fix first/last switched with collector passthrough (--collector-passthrough) when collecting IPFIX data
 - Fix collector passthrough representation of bytes/packets
 - Fix interface aggregation with ZC ice interfaces
 - Fix for reading packets from pcap dumps
 
## Misc

 - Ignoring observationDomainId (i.e. sourceId) for both IPFIX and NetFlow
 - Support for Rocky Linux 9
 - Update support for (latest) OPNsense
 - Windows improvements
 - Update homebrew support

-------------------------------------------------------------------------------

#### nProbe 10.0 (July 2022)

## New Features

 - Agent mode for process monitoring on Linux (eBPF) and Windows
 - Implement timeseries support for nProbe self-monitoring and sFlow-based counter timeseries
 - Conversion of Amazon AWS VPC files into flows
 - Improve collection of proprietary flows, including Nokia and Calix
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

* Add --collector-passthrough for exporting flows to remote collectors without passing through the flow cache and without any flow format change. This will result is a great speed improvements (+ 5x) with respect to going through the flow cache.
* Add --collector-nf-reforge for filtering and reforging collected netflow flows before exporting to remote collectors

## New Command Line Options

* Add Kafka/ZMQ/Syslog export passthtough support when --collector-passthrough is used
* Add --collector-nf-reforge for filtering and reforging collected netflow flows before exporting to remote collectors
* Changed -p format to <Outer VLAN Id>.<Inner VLAN Id/<proto>/<IP>/<port>/<TOS>/<SCTP StreamId>/<exporter IP>
* Add the ability to specify a binding IPv4 address in collector mode (e.g. -3 127.0.0.1:1234)
* Flow cache is now disabled by default in collection mode: replaced --disable-cache with --enable-collection-cache
* Add --gtpv1-track-non-gtp-u-traffic and --gtpv2-track-non-gtp-u-traffic for non GTP-encapsulated user export in IE %FLOW_USER_NAME

## Extensions

*  Add the ability to sniff from stdin by using -i -
*  Add %L7_PROTO_RISK %L7_PROTO_RISK_NAME
*  Add %TCP_WIN_MAX_IN %TCP_WIN_MAX_OUT IEs to @NTOPNG@
*  Add DNS/HTTP IEs to @NTOPNG@ in probe mode
*  Add collected flow lifetime export via ZMQ
*  Add IP-in-IP (IPv4 encapsulated in IPv6) support
*  Improve DNS plugin with additional records and NAPTR query type
*  Exporting %SEQ_PLEN as 8 bit element
*  Add TOS export via ZMQ
*  GTP traffic analysis improvements
*  Improve IMSI/APN traffic accounting and aggregation when using --imsi-apn-aggregation
*  Support for SIP over TCP (VoLTE)
*  Add IPv6 support in GTPv1
*  Add IPv4+IPv6 GTP-C v2 dissection
*  Improvement on GTP-C v1 dissection
*  Add support for %BGP_PREV_ADJACENT_ASN %BGP_NEXT_ADJACENT_ASN when collecting sFlow and Netflow
*  Add IPv6 PAA export
*  Support for overwriting element names with aliases provided by the user (case sensitive)

## Fixes

*  Fix detection of multiple connections on the same port (RST) exporting multiple flows
*  Fix EXPORTER_IPV6_ADDRESS
*  Fix UNTUNNELED_IPV6_SRC_ADDR / UNTUNNELED_IPV6_DST_ADDR
*  Fix dump of IPv6 flows to MySQL
*  Fix shutdown crashes
*  Fix kafka stats number overflow
*  Fix multiple --collection-filter options
*  Fix accounting of bidirectional flows in stats
*  Fix export of empty data
*  Fix invalid flow idle computation
*  Fix CSV export (always print all columns)
*  Fix AS lookup/calculation via MaxMind
*  Fix bug that caused FLOW_USER_NAME to be empty
*  Fix custom template elements support
*  Fix SIP decoding with malformed packets
*  Fix IPv6 dissection when encapsulated in GTP
*  Fix application protocol detection with GTP
*  Fix GTPv1 GTPV1_END_USER_IP field
*  Fix drop count

## Miscellaneous

*  Moved all binaries and libraries from /usr/local/ to /usr/
*  Plugins are now loaded from ./plugins, /usr/lib/nprobe/plugins, /usr/local/lib/nprobe/plugins 
*  Add Ubuntu 20.04 support
*  Improve OpenWRT support
*  Windows fixes
*  Improve plugins SDK

-------------------------------------------------------------------------------

#### nProbe 9.0 (March 2020)

## New Features

*  Add flowRelay tool to collect flows through a firewall.
*  Data serialisation based on nDPI: data can now be serialised in either JSON or binary format.
*  Implement %SEQ_PLEN, %SEQ_TDIFF, %ENTROPY_CLIENT_BYTES and %ENTROPY_SERVER_BYTES.
*  Implement --dump-bad-packets in collector mode for dumping invalid collected packets
*  Improve GTPv2 plugin with dissection of many new fields.
*  Implement GTP v1/v2 per IMSI and APN aggregation via --imsi-apn-aggregation.
*  Add HPERM and TZSP encapsulation support.
*  Add %SSL_CIPHER %SSL_UNSAFE_CIPHER %SSL_VERSION.
*  Implement JA3 support via %JA3C_HASH and %JA3S_HASH.
*  Implement %SRC_HOST_NAME and %DST_HOST_NAME for host symbolic name export (if known).
*  Implement IP-in-IP support.
*  Various Diameter plugin improvements.
*  Implement --collector-passthrough.
*  Add support for IXIA packet trailer.
*  Extended statistics reported via ZMQ so ntopng can better monitor the nProbe status.
*  Implement ZMQ encryption.
*  Various Kafka export improvements (export plugin).
*  Add support for the latest ElasticSearch format (export plugin).
*  Implement flow collection drop counters on UDP socket (Linux only).


## Fixes

*  Fix NetFlow Lite processing.
*  Implement various checks for discarding corrupted packets that caused nProbe to crash.
*  Fix flow upscale calculation (e.g. with sFlow traffic).
*  Application latency is now computed properly for some specific TCP flows with retransmissions.
*  Fix DNS dissection over TCP.
*  Improvements in Out-of-Order and retransmission accounting.
*  Fix for %EXPORTER_IPV4_ADDRESS with sFlow.
*  Support for the new GeoIP database format.
*  Extended the @NTOPNG@ template macro with new fields supported by ntopng.
*  Fix bug in RTP plugin that was leading in some cases nprobe to crash.
*  Various minor fixes in the nProbe engine.

-------------------------------------------------------------------------------

#### nProbe 8.6 (October 2018)

## Main New Features

* Add second-by-second client-to-server and server-to-client flow bytes
  * https://www.ntop.org/nprobe/introducing-per-second-measurements-in-nprobe-flow-exports/
* Implement an embedded web server that can be optionally enabled to
  * Force a flush of all the active flows
  * Receive a response monitored traffic traffic statistics
  * Query the nProbe version
* Seamless support of ElasticSearch 5 and 6 and automatic template push
  * ElasticSearch version is automatically determined upon nProbe startup
  * A proper template is pushed to ElasticSearch on the basis of its version
* Implement modbus plugin

## Extensions

* Add support for the collection of NetFlow/IPFIX vendor-proprietary information elements through simple configuration files
  * Supported vendors include Sonicwall, Cisco, IXIA, an others
  * Configuration files published at https://github.com/ntop/nProbe/tree/master/custom_fields
* The default NetFlow version is now V9
* Plugins are disabled in collector mode
* Improve support for Ubuntu18
* Implements SIP user agents dissections (Client and Server)
* Implements TCP/UDP/Other min flow size to discard flows below a certain minimum size
* nProbe runs with user 'nprobe' by default, falling back to nobody if user 'nprobe' does not exist
* New NetFlow information elements `%NAT_ORIGINATING_ADDRESS_REALM` and `%NAT_EVENT`
* `L7_PROTO` exports now the protocol in format `<master>.<application>` protocol
* Add fields `%SRC_TO_DST_SECOND_BYTES` and `%DST_TO_SRC_SECOND_BYTES` to export second-by-second flow bytes
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
* Fix for flow start/end times and timestamp calculation in proxy mode
* Fix issues with SIP call id in RTP flows
* Fix length calculation in IPFIX variable-length fields
* Fix ZMQ buffer termination when flushing ZMQ buffers
* Fix wrong `%EXPORTER_IPV4_ADDRESS` exported over ZMQ export in case on Netflow != v5
* Fix a race condition that was preventing all flows to be dumped on file
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

* Implement ICMP network latency
* Add ICMP type/code on flow keys to differenciate ICMP flows from the same peers
* sFlow upscale now takes into account sample drops
* Improves throughput calculations with NetFlow

## Fixes

* Fix crash with ICMP traffic and not extended template Rework for whatsapp support
* Fix framentation issues that could lead to crashes
* Prevents leaks with multiple BGP updates
* Fix a crash when exporting option templates to Kafka
* Fix missing fields (e.g, `FIREWALL_EVENT`) in MySQL db columns
* Preserve endianness of string_dump_as_hex NetFlow custom fields
* Fix overwrite of untunnelled addresses for tunnels after the first
* Updates centos7 mysql dependency to work either with mysql and mariadb
* Fix invalid FTP detection
* Fix for computing `%DIRECTION` even with reduced temolate IEs
* Fix wrong sFlow average scale estimation
* Fix for wrapping ZMQ rates > 4Gbps
* Fix loop bug in plugin handling when multiple plugins are enabled

-------------------------------------------------------------------------------

#### nProbe 8.2 (December 2017)

## Main New Features

* Support for multiple `--zmq` endpoints to load-banace exported flows in a round-robin fashion
* Full support for NetFlow exported by ASA, including firewall events and cumulative counters
* MySQL database interoperability with ntopng using template `-T "@NTOPNG@"`

## New Options

*  Add `--plugin-dir <dir>` for loading plugins from the specified directory

## Extensions

* bgpNextHop support
* sFlow
  * Improve sFlow upscale algorithm and added heuristic to prevent sFlow exporters bugs
  * Fix throughput calculation and upsampling of sFlow traffic
* Full systemd support for Debian, Ubuntu, Centos, and Raspbian
* Fix wrong flow first/last calculations when collecting IPFIX
* Add support for flowDurationMillis Fix bug for properly handling flowStart/flowEndMillis

-------------------------------------------------------------------------------

#### nProbe 8 (June 2017)

## Main New Features

* Rework packet fragmentation support that was not properly rebuilding packet fragments
* Many tiny bugs fixed that increase stability and metrics reliability
* Implement BPF filtering with PF_PACKET directional sockets
* Add VXLAN support
* Created multiple kafka publishers to enhance performance
* Implement options template export via Kafka
* Add support for collection of IXIA URI and Host
* Add @SIP@ and @RTP@ plugin shortcuts for VoIP analysis
* Improve SSL dissection
* Add support for GTPv2 PCO
* Add support for IPFIX flowEndMilliSeconds when observationTimeMilliSeconds (often in Cisco ASA)
* Add ability to export sFlow interface counters via ZMQ
* Add drops (export/elk/too many flows) drops
* Add kflow export (kentik.com)

## New Options

* `--upscale-traffic` to scale sampled sFlow traffic
* `--kafka-enable-batch` and `--kafka-batch-len` to batch flow export to kafka
* `--load-custom-fields` to support custom fields shipped with NetFlow (see http://www.ntop.org/nprobe/collecting-proprietary-flows-with-nprobe/)
* `--max-num-untunnels` to decapsulate up to 16 tunnelling levels.
* `--vlanid-as-iface-idx` to use the VLAN tag as the interface index
* `--zmq-disable-compression` to disable ZMQ data compression

## Extensions

* Implement min/avg/max throughput with %SRC_TO_DST_MIN_THROUGHPUT %SRC_TO_DST_AVG_THROUGHPUT %SRC_TO_DST_MAX_THROUGHPUT %DST_TO_SRC_MIN_THROUGHPUT %DST_TO_SRC_AVG_THROUGHPUT %DST_TO_SRC_MAX_THROUGHPUT
* Add support in collection of %IN_SRC_MAC %OUT_DST_MAC %FRAGMENTS %CLIENT_NW_LATENCY_MS %SERVER_NW_LATENCY_MS %APPL_LATENCY_MS %RETRANSMITTED_IN_PKTS %RETRANSMITTED_OUT_PKTS %OOORDER_IN_PKTS %OOORDER_OUT_PKTS
* Split %FRAGMENTS IN %SRC_FRAGMENTS and %DST_FRAGMENTS
* Add %NPROBE_IPV4_ADDRESS to export the IP address of the nProbe sensor, whereas %EXPORTER_IPV4_ADDRESS contains the IP address of the flow exporter (e.g. the router that generated the exported flow)
* Implement %ICMP_IPV4_TYPE, %ICMP_IPV4_CODE, %FLOW_DURATION_MILLISECONDS, %FLOW_DURATION_MICROSECONDS, %FLOW_START_MICROSECONDS, %FLOW_END_MICROSECONDS
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
* Implement SSDP and NETBIOS plugins
* Implement CAPWAP protocol support
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
* Implement SIP RTP support, handling of early export and support of OPTIONS messages
* Extended GTPV1 plugin support to field GTPV1_RAT_TYPE
* Extended GTPV2 plugin support to fields GTPV2_C2S_S5_S8_GTPC_IP and GTPV2_S2C_S5_S8_GTPC_IP, GTPV2_PDN_IP, GTPV2_END_USER_IMEI, GTPV2_C2S_S5_S8_GTPU_TEID, GTPV2_C2S_S5_S8_GTPU_IP, GTPV2_S2C_S5_S8_GTPU_TEID, GTPV2_S2C_S5_S8_GTPU_IP, GTPV2_C2S_S5_S8_SGW_GTPU_TEID, GTPV2_S2C_S5_S8_SGW_GTPU_TEID, GTPV2_C2S_S5_S8_SGW_GTPU_IP and GTPV2_S2C_S5_S8_SGW_GTPU_IP
* GTPV2 plugin check to the export bucket when response isn't the right type for a request and when a response has been done from the same peer as the request
* Implement GTPV2 0x4A message type management
* Implement Diameter support of fields DIAMETER_CLR_CANCEL_TYPE and DIAMETER_CLR_FLAGS and 3GPP type messages (317, 319, 320, 321, 322 and 323)
* Extended the Diameter plugin in order to export ""Diameter Hop-by-Hop Identifier" information
* Add DOT1Q_SRC_VLAN/DOT1Q_DST_VLAN for supporting Q-in-Q VLANs
* HTTP plugin export of HTTP_X_FORWARDED_FOR and HTTP_VIA fields
* Extended DNS plugin with multicast DNS

## ZMQ

* ZMQ event handling to send interface statistics to ntopng
* ZMQ statistics in flow collector mode
* Ability to use ZMQ zlib compression
* Enhanced ZMQ statistics with bps/pps rates

## Miscellaneous

* Add black list support for netflow data when nprobe is in proxy mode (ipv4 - V5,V9,IPFIX)
* Twitter heuristics to guess user activities
* Implement support for TCP fast retransmissions
