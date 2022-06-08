.. _CliOptions:

Command Line Options
--------------------

nProbe supports a large number of command line parameters. 
To see what they are, simply enter the command :code:`nprobe -h` 
and the help information should be printed. The most important 
parameters are briefly discussed here.

:code:`-n: collector addresses`

      This specifies the NetFlow collectors addresses to which nProbe will send the flows. If more than one is specified, they need to be separated with a comma or the --n flag can be repeated several times (e.g. -n 172.22.3.4:33,172.22.3.4:34 and -n 172.22.3.4:33 --n 172.22.3.4:34 are equivalent). When multiple collectors are defined, you can control the way flows are exported using the --a option (see below); if on a collector address the destination port is omitted, flows are sent to 2055 port and whereas if all the option is not specified, by default, flows are sent to the loop back interface (127.0.0.1) on port 2055. If this parameter is used, nProbe exports flows towards collector running at 127.0.0.1:2055. By default the UDP protocol is used but also TCP and SCTP (Linux only when nProbe is compiled with SCTP support and the kernel supports it). In this case you can specify the collector address as udp://<host>:<port>, tcp://<host>:<port>, and sctp://<host>:<port>,

:code:`-i: interface name`

      It specifies the interface from which packets are captured. If -i is not used, nProbe will use the default interface (if any). In case a user needs to activate nProbe on two different interfaces, then he/she needs to activate multiple nProbe instances once per interface. For debugging purposes it is possible to pass nProbe a .pcap file from which packets will be read. If nProbe is compiled and activated with PF_RING support, you can specify multiple interfaces from which packets are captured. For example, “-i eth0,eth1”, will merge packets received on eth0 and eth1 into a single traffic stream. This configuration is particularly useful when merging the two directions (TX and RX) of a network TAP.

:code:`-t: maximum flow lifetime`

      Regardless of the flow duration, a flow that has been active for more that the specified maximum lifetime is considered expired and it will be emitted. Further packets belonging to the same flow will be accounted on a new flow. 

:code:`-d: maximum flow idle lifetime`

      A flow is over when the last packet received is older that the maximum flow idle lifetime. This means that whenever applicable, (e.g. SNMP walk) UDP flows will not be accounted on 1 packet/1 flow basis, but on one global flow that accounts all the traffic. This has a benefit on the total number of generated flows and on the overall collector performance. 

:code:`-l: maximum queue timeout`

      It specifies the maximum amount of time that a flow can be queued waiting to be exported. Use this option in order to try to pack several flows into fewer packets, but at the same time have an upper bound timeout for queuing flows into the probe.

:code:`-s:  snaplen`

      This flag specifies the portion of the packet (also called snaplen) that will be captured by nProbe. By default nprobe sets the snaplen automatically according to its configuration, but you can override its value using thia flag.

:code:`-p: <VLAN Id>/<proto>/<IP>/<port>/<TOS>/<SCTP StreamId>/<exporter IP>`

      Flows can be aggregated both at collector and probe side. However probe allocation is much more effective as it reduces significantly the number of emitted flows hence the work that the collector has to carry on. nProbe supports various aggregation levels that can be selected specifying with the -p flag. The aggregation format is <VLAN Id>/<proto>/<IP>/<port>/<TOS>/<SCTP StreamId>/<exporter IP> where each option can be set to 0 (ignore) or 1 (take care). Ignored fields are set to a null value when doing the aggregation as well as when doing the export. For example setting the <exporter IP> to 0 (ignore) will consider all the incoming flows as if they were coming from the same null exporter that will be output in %EXPORTER_IPV4_ADDRESS as 0.0.0.0. By default no aggregation is performed. For the sake of example, the value 0/0/1/0/0/0/0 can be used to create a map of who's talking to who (network conversation matrix).

:code:`-f: packet capture filter`

      This BPF filter (see the appendix for further information about BPF filters) allows nProbe to take into account only those packets that match the filter (if specified). 

:code:`-a: select flow export policy`

      When multiple collectors are defined (see --n option), nProbe sends them flows in round robin. However it is possible to send the same flow to all collectors as a flow redirector does if the --a option is used.

:code:`-b: enable verbose logging`

      Using this flag, nProbe generates verbose output that can be used to tune its performance (see chapter 2.4). Zero is the lowest level (little information is printed), 1 displays traffic statistics, 2 is really verbose. Example of traffic statistics:

.. code:: bash

	30/Oct/2020 16:10:00 Average traffic: [1.7 pkt/sec][1 Kb/sec]
	30/Oct/2020 16:10:00 Current traffic: [1.9 pkt/sec][1 Kb/sec]
	30/Oct/2020 16:10:00 Current flow export rate: [0.9 flows/sec]
	30/Oct/2020 16:10:00 [Buckets: [active=13][allocated=21][free=8][toBeExported=0][frags=0]
	30/Oct/2020 16:10:00 Fragment queue: [len=0]
	30/Oct/2020 16:10:00 Num Packets: 111 (max bucket search: 0)
	30/Oct/2020 16:10:00 115 pkts rcvd/0 pkts dropped

:code:`-G: start nprobe as a daemon.`

      Useful when starting nprobe as daemon.

:code:`--ndpi-protocols|-O: export only flows whose L7 protocol is in the specified list.`

      This option can be used to filter exported flows based on the application protocol. Example: -O "DNS,HTTP,BitTorrent".

:code:`-P: dump flows`

      This path specifies the directory where flows will be dumped. The dump format is text and it depends on the nProbe template specified with -T.

:code:`-F`

      It specifies the frequency at which files are dumped on disk

:code:`-D: dump flows format`

	Flows stored on disks can be stored in multiple formats: text (default), binary (as they are exported), JSON, or gzip-compressed text flows. Note that this flag has no effect unless -P is used.

:code:`-u: input device index`

      The NetFlow specification contains a numeric index in order to identify flows coming from different interfaces of the same probe. As multiple nProbe instances can be started on the same host but on different devices, the collector can use this flag to divide flows according to the interface number. If --u is not used, then nprobe will use 0 as interface index. Alternatively, if -1 is used then the last two bytes of the mac address of the flow sender are used as index.

:code:`-Q: output device index`

   Similar to --u but for the output interface.

:code:`--vlanid-as-iface-idx <mode: inner outer single double>`

      nProbe can use the VLAN tag as interface identifier. Using this flag you enable this feature. As VLAN tags can be stacked you need to specify if the inner or outer tag will be used for the interface identifier.

:code:`--discard-unknown-flows <mode:0 1 2>`

      nProbe includes nDPI support for analyzing packet contents in order to detect application protocol. The mode value can be used to:

      - 0: Export all know (i.e. those whose application protocol has been detected) and unknown (i.e. the application protocol is unknown)
      - 1: Export only know flows, discarding unknown flows.
      - 2: Export only unknown flows, discarding known flows.

:code:`-v: print version`

      This flag is used to print the nProbe version number and date.

:code:`-C: flow export lock`

   This is a simple way to implement high-availability. Start two probes capturing the same data. The master probe emit flows, the slave probe is started with --C <path>. As long as <path> exists, the slave works but no flow is emitted. If the <path> file is deleted (e.g. using an external program for controlling the master/slave such as heartbeat) the slave starts emitting flows. If the file is restored, the slave is silent again.

:code:`-h: print help`

	 Prints the nProbe help.

:code:`--dont-nest-dump-dirs`

      nProbe dumps data on disk (e.g. with -P) using a nested directory. In essence the base directory will be partitioned in sub-directories with <year>/<month>/<day>/<hour>/<min> structure. use this option is you want nProbe to dump all data in the base directory without creating this nested directory tree.

:code:`-I: log to syslog <probe name>`

      nProbe logs on stdout unless the --g flag (see above) is used. If the syslog needs to be used instead of a file, this flag instruments nProbe to log on it using the specified name (this is useful when multiple nProbe instances are active on the same host). Please note that --g is ignored if --I is used, and this option is not available on nProbe for Win32.

:code:`-w: size of the hash that stores the flows`

      The default size is 131072 and it should be enough for most of networks. In case flows are not emitted often and with strong traffic conditions it would be necessary to increase the hash. See later in this manual for knowing more about nProbe tuning.

:code:`-W: Discard IPv6 traffic`

      Use this flag if you want nProbe not to account IPv6 traffic.

:code:`-e: flow export delay`

      Some collectors cannot keep up with nProbe export speed. This flag allows flows to be slow down by adding a short delay (specified in ms) between two consecutive exports. The maximum allowed delay is 1000 ms.

:code:`-B: packet count delay`

      It specified how many flow packets need to be sent before --e is applied,

:code:`-z: <TCP[:UDP[:O]]>`

       Peer-to-peer applications, attacks or misconfigured applications often generate a lot of tiny flows that can cause significant load on the collector side. As most collector setups often discarded those flows, it is possible to instrument nProbe via the --z flag not to emit such flows. 

:code:`-M: maximum number of active flows`

      It is used to limit the maximum number of concurrent flows that the probe can sustain. This is useful for preventing the probe from creating as many flows as needed and hence to take over all the available resources.

:code:`-E: netflow engine`

      Specify the netflow engineType:engineId into the generated flows.

:code:`-m: minimum number of flows per packet`

      In order to minimize the number of emitted packets containing flows, it is possible to specify the minimum number of flows that necessarily need to be contained in a packet. This means that the packet is not emitted until the specified number of flows is reached.

:code:`-q: <host>:[<port>] flow sender address and port`

      This option is used to specify the address and, optionally, the port that will be used by nProbe to emit the flows towards the destination indicated with -n. In practice, nProbe will create a socket and bind it to :code:`<host>:[port]`, thus allowing the user to choose the interface taken by the emitted flows when leaving the host.

:code:`-S <pkt rate>:<flow collection rate>:<flow export rate>`

      Three different rates can be specified with this option:

      - Packet capture sampling rate <pkt rate>. This rate is effective for interfaces specified with -i and allows to control the sampling rate of incoming packets. For example, a sampling rate of 100 will instruct nprobe to actually process one packet out of 100, discarding all the others. All the statistics, including total bytes and packets, will be automatically up-scaled by nprobe to reflect the sample rate. In the previous example, the size of the sampled packet will be multiplied by 100. <pkt rate> can be prepended with a '@'  to instruct nprobe to only use the sampling rate for the up-scaling, without performing any actual sampling. This is particularly useful when incoming packets are already sampled on the capture device connected to nprobe but it is still meaningful to have up-scaled statistics.
      - Flow collection sampling rate <flow collection rate>. This rate works when nprobe is in collector mode, that is, when option --collector-port is used and specifies the flow rate at which flows being collected have been sampled. In this case, no actual sampling is performed on the incoming flows. The specified rate is only used to perform the upscaling. For example, a flow with 250 IN_BYTES will be up-scaled by a factor equal to the sampling rate. If the sampling rate is 100, a total of 2500 IN_BYTES will be accounted for that flow.
      - Flow export rate <flow export rate>. This rate is effective when nprobe exports NetFlow towards a downstream collector, that is, when option -n is used. It controls the output sampling. For example, a <flow export rate> of 100 will cause nprobe to only export 1 flow out of 100 towards the downstream collector.

:code:`-A: AS file`

      Network probes are usually installed on systems where the routing information is available (e.g. via BGP) in order to specify the AS (Autonomous System) id of the flow peer. As nProbe has no access to BGP information unless you enable the BGP plugin, users need to provide this information by means of a static file whose format is <AS>:<network>. The file can be stored in both plain text and gzip format.

:code:`--city-list: City List`

      With this option you can enable geolocation of IP addresses at city/country detail level. Here you need to specify the GeoIP city database (e.g. GeoLiteCity.dat)

:code:`-g`

      It specifies the path where nProbe will save the process PID.

:code:`-T: flow template definition`

      Contrary to NetFlow v5 where the flow format is fixed, NetFlow V9 and IPFIX flows have a custom format that can be specified at runtime using this option as specified in appendix.

:code:`-U: flow template id`

      NetFlow v9 and IPFIX flows format is specified in a template whose definition is sent by nProbe before to start sending flows. The flow format is defined by --T, where --U is used to set the template identifier. This option should not be used unless the default template value (257) needs to be changed. As based on -T nProbe can define several templates, this value is the one used for the first defined template.

:code:`-V: flow export version`

      It is used to specify the flow version for exported flows. Supported versions are 5 (v5), 9 (v9) and 10 (IPFIX).

:code:`-o: intra templates packet export.`

      It specifies the number of flow packets that are exported between two templates export.

:code:`--aggregate-gtp-tunnels`

      Aggregates traffic flowing in each GTP tunnel based in tunnel id.

:code:`-L: local networks`

      Use this flag to specify (format network/mask, e.g. 192.168.0.10/24) the list of networks that are considered local (see --c).

:code:`-c: track local hosts only`

      It allows nProbe to set to 0.0.0.0 all those hosts that are considered non-local (see --L). This is useful when it is necessary to restrict the traffic analysis only to local hosts.

:code:`-r: set traffic direction`

      When this option is used (-L must be specified before --r), all the traffic that goes towards the local networks is considered incoming, all the rest is outgoing. This has effect on the --u/-Q that are then forced with --r.

:code:`--if-networks`

      Flags -u and -Q are used to specify the SNMP interface identifiers for emitted flows. In mirrored environments, it is possible to simulate a switched environment by playing with MAC addresses. This option allows users to bind a MAC or IP address to a specified interfaceId.. The syntax of --if-networks is <MAC|IP/mask>@<interfaceId> where multiple entries can be separated by a comma (,). Example: --if-networks "AA:BB:CC:DD:EE:FF@3,192.168.0.0/24@2" or --if-networks @<filename> where <filename> is a file path containing the networks specified using the above format.

:code:`--count: debug only`

      Let the probe capture only up to the specified number of packets.

:code:`--collector-port: specifies the NetFlow collector port`

      Use nProbe to collect NetFlow/jFlow/IPFIX/sFlow packets. Use option :code:`--collector-port` to specify on which on which ports such packets should be collected. nProbe is able to ingest and convert flows from various versions. For instance :code:`nprobe --collector-port 2055 --i 192.168.0.1:2056 --V 10` converts each flow received on port 2055 to IPFIX and sends them to 192.168.0.1:2056. By default nProbe binds the collection port to all available interfaces. If you want you can bind the port only to one interface. This can accomplshed specifying an optional local (to the host where nprobe is running) IP address. Exampple -3 192.168.1.23:2055.

      Option :code:`--collector-port` can also be used to receive NetFlow/jFlow/IPFIX/sFlow packets through a ZMQ relay. In this case one should specify a ZMQ endpoint. An implementation of a ZMQ relay is available in executable :code:`flowRelay`. Run :code:`flowRelay -h` to see how to use it.

:code:`--collector-passthrough`

      Export flows to the configured ZMQ endpoints as-is, ignoring the :code:`-T`. Using :code:`--collector-passthrough` gives the highest collection throughput. ZMQ/Syslog/Kafka exports are supported. See :ref:`UnderstandingFlowCollection` for a detailed discussion. Note that -T is ignore when passthrough is used. This is a nProbe Pro-only feature.

:code:`--collector-nf-reforge <file>`

      Flow collection-only feature. It allows users to configure NetFlow collection filtering and reforge by specifying a configuration file and passing it as argument. The file format is the one shown in this example (columns are tab separated).
      In this example, flows sent by NeFflow probe active at IP address 192.168.1.1 are collected by nProbe and exported (e.g. via ZMQ to ntopng or to a remote collector via -n) as if they were sent by host 192.168.1.1: only flows from Netflow interfaceId 1,2,3,4 are handled, all other interfaces are discarded. For collecting all interfaces and just reforginf the probe IP address use * in the interface list. Probes not listed in the file are handled as-is without any reforging or template filtering.

.. code:: bash

 # CollectorIP   ReforgedIP      ListOfAllowedInterfaces
 # Example:
 127.0.0.1       10.0.24.25      12
 192.168.1.1     192.168.1.1     1,2,3,4

      
:code:`--tunnel`

      Let the probe decode tunneled traffic (e.g. GTP or GRE traffic) and thus extract traffic information from such traffic rather than from the external envelope.

:code:`--no-promisc`

      With this option nProbe does not use promiscuous mode to capture packets.

:code:`--smart-udp-frags:`

      Ignore UDP fragmented packets with fragment offset greater than zero, and compute the fragmented packet length on the initial fragment header. This flag might lead to inaccuracy in measurement but it speeds us operations with fragmented traffic.

:code:`--ipsec-auth-data-len`

      Length of the authentication data of IPSec in tunnel mode. If not set, IPSec will not be decoded but just accounted.

:code:`--dump-stats:  dump some flow statistics on file`

      Periodically dump NetFlow statistics on the specified file. Note that when using nProbe over PF_RING, nProbe dumps statistics on /proc/net/pf_ring/stats/<nprobe stats file>.

:code:`--black-list`

      With this option you can specify a list of networks or hosts from which all the incoming packets will be discarded by the probe. The accepted notation can be CIDR format or the classical network/netmask format.

:code:`--pcap-file-list <file>`

      The specified file path contains a list of pcap files to be read in sequence by nProbe. Use this option when you want nProbe to read a list of pcap files (e.g. when generated using tcpdump).

:code:`--biflows-export-policy <policy>`

      Bi-directional flows are such when there is traffic in both direction of the flow (i.e. source->dest and dest->source). As mono-directional flows might indicate suspicious activities, this flag is used to determine the export policy:

      - 0: Export all know (i.e. mono and bi-directional flows)
      - 1: Export only bi-directional flows, discarding mono-directional flows.
      - 2: Export only mono-directional flows, discarding bi-directional flows.

:code:`--csv-separator <separator>`

      Override the default ‘|’ separator in dumps with the specified one.

:code:`--dont-drop-privileges`

      Do not drop root privileges to user ‘nobody’ when this option is specified. See al --unprivileged-user later int this manual.

:code:`--account-l2`

      NetFlow accounts IP traffic only, not counting layer 2 headers. Using this option the layer 2 headers are also accounted in flow traffic statistics.

:code:`--dump-metadata <file>`

      Dump metadata information into the specified file and quit. This option is useful when users want to know the type of each information element exported by nProbe so that (for instance) they can properly import into a database.

:code:`--zmq <socket>`

      Specify a socket (e.g., :code:`tcp://\*:5556`) that will be used to deliver flows to subscribers polling the socket. Up to 8 ZMQ endpoints can be specified by repeating the --zmq. When more than one endpoint is specified, nProbe uses an hash function to evenly balance flows among all the defined endpoints. Example:

.. code:: bash

   ./nprobe -i eth0 -n none --zmq tcp://\*:5556 --zmq tcp://\*:5557 
   ./ntopng -i tcp://127.0.0.1:5556 -i tcp://127.0.0.1:5557 -i view: tcp://127.0.0.1:5556, tcp://127.0.0.1:5557

:code:`--zmq-probe-mode`

      By default, nProbe act as a ZMQ server that delivers flows to subscribers. Using this switch, its role is reverted. This is typically used in conjunction with ntopng run in collector mode. For a thorough description refer to the section “Using nProbe with ntopng”.

:code:`--tcp <server:port>`

   Delivers flows in JSON format via TCP to the specified pair server:port.

:code:`--event-log <file>`

      Dump relevant activities (e.g. nProbe start/stop or packet drop) onto the specified file.

:code:`--enable-throughput-stats`

      When -P is used, with this option is also possible to generate throughput information. The file has the following format: <epoch> <bytes> <packets>. Each line is printed every second and it contains the number of bytes and packets observed within minute.

:code:`--ndpi-proto-ports <file>`

      Read the nDPI custom protocol and ports configuration from the specified file. Please refer to the nDPI manual for further information about the format of this file.

:code:`--disable-l7-protocol-guess`

      When nDPI is unable to detect a protocol, nProbe uses the port information to guess the protocol. This flag prevents nProbe from doing that, so protocols are detected only by nDPI without relying on default ports.

:code:`--db-engine <database engine>`

      In case flows are dumped on a MySQL database (see later on this manual) the default database engine used by nProbe is MyISAM. With this option you can use another engine (e.g. InnoDB).

:code:`--unprivileged-user <name>`

      When nprobe drops privileges (unless --dont-drop-privileges is used) the user nobody is used. It is possible to use another user by using this option.

:code:`--enable-collection-cache`

      nProbe implements a flow cache for merging packets belonging to the same flow. In flow collection the flow cache is disabled. This option enables the flow collection cache as when nProbe operates in packet capture mode. Note that this option is available only in collector/proxy mode (i.e. use -i none). 
 
:code:`--collector-passthrough`

      When you want to use nProbe as a flow proxy/collector (towards ntopng for instance) and have a 1:1 mapping between collected/exported flows this is the options to use. This because it allows you to collect flows at high speed with limited CPU usage. Note that this option is useless when --disable-cache is used.
      

:code:`--redis <host>[:<port>]`

      The redis database (when nProbe is compiled with it) is used to implement a data cache and for aggregating flow information. This option specifies the host (and optionally the port) where redis is listening. nProbe opens several connections to redis (not just one) in order to maximize performance.

:code:`--ucloud`

      This option enables the micro-cloud concept. Please refer to http://www.ntop.org/nprobe/monitoring-on-the-microcloud/ for more information.

:code:`--check-license`

      Checks if the configured license is valid (for binary nProbe’s only).

:code:`--disable-startup-checks`

      During startup nProbe obtains both the management interface IP address and its public IP address. The management interface IP address is the address of the physically-attached interface that carries nProbe  network traffic. The public IP address is the address of the management interface as it is seen from the internet. Obtaining the public IP address triggers a request to http://checkip.dyndns.org.

:code:`--dump-plugin-families`

      Dump installed plugin family names.

:code:`--minute-expire`

      Force nProbe to export active flows when a minute elapses. This is useful if you want (e.g. using -P) to have fresh flows every minute and all ending at X minutes, 0 seconds.


As some people prefer to have a configuration file containing the options that otherwise would be specified on the command line, it is also possible to start nProbe as follows:

.. code:: bash

	  nprobe <configuration file path>

where the configuration file contains the same options otherwise specified on the command line. The only difference between the command line and the configuration file is that different options need to be specified on different lines. For instance:

.. code:: bash

	  nprobe --n 127.0.0.1:2055 -i en0 -a -p

is the same as:

.. code:: bash
   
	nprobe /etc/nprobe.conf

where /etc/nprobe.conf contains the following lines:

.. code:: bash

	  # cat /etc/nprobe.conf

	  -n=127.0.0.1:2055
	  -i=en0
	  -a=
	  -p=

Note that flags with no parameter associated (e.g. --a) also need to have ‘=’ specified. 
Any standard NetFlow collector (e.g. ntop) can be used to analyze the flows generated by nProbe. When used with ntop, the nProbe can act as a remote and light traffic collector and ntop as a central network monitoring console. See chapter 3 for further information about this topic


Note on interface indexes and (router) MAC/IP addresses
-------------------------------------------------------

Flags -u and -Q are used to specify the SNMP interface identifiers for emitted flows.
However using --if-networks it is possible to specify an interface identifier to which
a MAC address or IP network is bound. The syntax of --if-networks is:
 <MAC|IP/mask>@<interfaceId> where multiple entries can be separated by a comma (,).
Example: --if-networks "AA:BB:CC:DD:EE:FF@3,192.168.0.0/24@2" or
--if-networks @<filename> where <filename> is a file path containing the networks
specified using the above format.

Further plugin available command line options
---------------------------------------------

HTTP Protocol
~~~~~~~~~~~~~

:code:`--http-dump-dir <dump dir>`

      Directory where HTTP logs will be dumped
	  
:code:`--http-content-dump-dir <dump dir>`

      Directory where HTTP content (request only) will be dumped
	  
:code:`--http-content-dump-response`

      Dump both HTTP request and response with --http-content-dump-dir
	  
:code:`--http-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
	  
:code:`--dont-hash-cookies`

      Dump cookie string instead of cookie hash
	  
:code:`--http-verbose-level <level>`

      0 - Relevant info,  1 - Very verbose (default: 1)
	  
:code:`--http-ports`

      List of ports used for http protocol (default: 80)
	  
:code:`--proxy-ports`

      List of ports used for proxy protocol (default: 3128, 8080)
	  
:code:`--http-parse-geolocation`

      Dump geolocation info if explicitly present inside mobile app protocol (e.g., "Nimbuzz")
   
DNS/LLMNR Protocol
~~~~~~~~~~~~~~~~~~

:code:`--dns-dump-dir <dump dir>`

      Directory where DNS logs will be dumped
   
SIP Plugin
~~~~~~~~~~

:code:`--sip-dump-dir <dump dir>`

      Directory where SIP logs will be dumped
	  
:code:`--sip-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   You can use @SIP@ in -T as shortcut for
   %SIP_CALL_ID %SIP_UAC %SIP_UAS %SIP_CALLING_PARTY %SIP_CALLED_PARTY %SIP_RTP_IPV4_SRC_ADDR %SIP_RTP_L4_SRC_PORT %SIP_RTP_IPV4_DST_ADDR %SIP_RTP_L4_DST_PORT %SIP_RESPONSE_CODE %SIP_REASON_CAUSE %SIP_CALL_STATE %SIP_RTP_CODECS
   
RTP Plugin
~~~~~~~~~~

:code:`--rtp-discard-late-pkts <msec>`

      Discard from stats RTP packets whose inter-arrival is greater than the specified latency.
   You can use @RTP@ in -T as shortcut for
   %RTP_SIP_CALL_ID %RTP_RTT %RTP_IN_JITTER %RTP_OUT_JITTER %RTP_IN_PKT_LOST %RTP_OUT_PKT_LOST %RTP_IN_PKT_DROP %RTP_OUT_PKT_DROP %RTP_IN_MAX_DELTA %RTP_OUT_MAX_DELTA %RTP_IN_PAYLOAD_TYPE %RTP_OUT_PAYLOAD_TYPE %RTP_IN_MOS %RTP_OUT_MOS %RTP_IN_R_FACTOR %RTP_OUT_R_FACTOR
   
FTP Protocol
~~~~~~~~~~~~

:code:`--ftp-dump-dir <dump dir>`

      Directory where FTP logs will be dumped
	  
:code:`--ftp-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
SMTP Protocol
~~~~~~~~~~~~~

:code:`--smtp-dump-dir <dump dir>`

      Directory where SMTP logs will be dumped
	  
:code:`--smtp-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
BGP Update Listener
~~~~~~~~~~~~~~~~~~~

:code:`--bgp-port <port>`

      TCP port on which BGP updates will be sent
	  
:code:`--adj-from-as-path <num>`

      Use the <num>-th ASN in the AS path to the source IP to populate field %BGP_PREV_ADJACENT_ASN, and <num>-th ASN in the AS path to the destination IP to populate field %BGP_NEXT_ADJACENT_ASN.
   
Netflow-Lite Plugin
~~~~~~~~~~~~~~~~~~~

:code:`--nflite <flow listen port low>[:<num ports>]>`

      Specify NetFlow-Lite listen port(s) (max 32)
   
GTPv0 Signaling Protocol
~~~~~~~~~~~~~~~~~~~~~~~~

:code:`--gtpv0-dump-dir <dump dir>`

      Directory where GTP logs will be dumped
	  
:code:`--gtpv0-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
GTPv1 Signaling Protocol
~~~~~~~~~~~~~~~~~~~~~~~~

:code:`--gtpv1-dump-dir <dump dir>`

      Directory where GTP logs will be dumped
	  
:code:`--gtpv1-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
	  
:code:`--gtpv1-account-imsi`

      Enable IMSI aggregation on GTPv1 signalling
	  
:code:`--gtpv1-track-non-gtp-u-traffic`

      Enable tracking of user traffic non GTP-U encapsulated triggered by GTP-U signalling (requires --ucloud)
   
GTPv2 Signaling Protocol
~~~~~~~~~~~~~~~~~~~~~~~~

:code:`--gtpv2-dump-dir <dump dir>`

      Directory where GTP logs will be dumped
	  
:code:`--gtpv2-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
	  
:code:`--gtpv2-account-imsi`

      Enable GTPv2 traffic accounting
	  
:code:`--gtpv2-track-non-gtp-u-traffic`

      Enable tracking of user traffic non GTP-U encapsulated triggered by GTP-U signalling (requires --ucloud)
   
Radius Protocol
~~~~~~~~~~~~~~~

:code:`--radius-dump-dir <dump dir>`

      Directory where Radius logs will be dumped
	  
:code:`--radius-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
Modbus Plugin
~~~~~~~~~~~~~

:code:`--modbus-dump-dir <dump dir>`

      Directory where modbus logs will be dumped
	  
:code:`--modbus-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
	  
:code:`--modbus-idle-timeout <duration>`

      Modbus idle flow timeout set to 120 seconds
   
Diameter Protocol
~~~~~~~~~~~~~~~~~

:code:`--diameter-dump-dir <dump dir>`

      Directory where Diameter logs will be dumped
	  
:code:`--diameter-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
NETBIOS Protocol
~~~~~~~~~~~~~~~~

:code:`--netbios-dump-dir <dump dir>`

      Directory where NETBIOS logs will be dumped
   
SSDP Protocol
~~~~~~~~~~~~~

:code:`--ssdp-dump-dir <dump dir>`

      Directory where SSDP logs will be dumped
   
DHCP Protocol
~~~~~~~~~~~~~

:code:`--dhcp-dump-dir <dump dir>`

      Directory where DHCP logs will be dumped
	  
:code:`--dhcp-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
IMAP Protocol
~~~~~~~~~~~~~

:code:`--imap-dump-dir <dump dir>`

      Directory where IMAP logs will be dumped
	  
:code:`--imap-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
	  
:code:`--imap-peek-headers`

      Dump both emails body and headers (default: body only)
   
POP3 Protocol
~~~~~~~~~~~~~

:code:`--pop-dump-dir <dump dir>`

      Directory where POP3 logs will be dumped
	  
:code:`--pop-exec-cmd <cmd>`

      Command executed whenever a directory has been dumped
   
Export Plugin
~~~~~~~~~~~~~

:code:`--elastic <format>`

      Enable export to ElasticSearch
Format: <index type>;<index name>;<es URL>;<es user>:<es pwd>
Note: <es user> and <es pwd> can be directly specified in the <es URL>
Note: the <index name> accepts the format supported by strftime().
Examples:

:code:`--elastic "flows;nprobe-%Y.%m.%d;http://localhost:9200/_bulk"`
:code:`--elastic "flows;nprobe-%Y.%m.%d;http://elastic:3last1cpassw0rd@localhost:9200/_bulk"`
:code:`--elastic "flows;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;elastic:3last1cpassw0rd"`
:code:`--kafka <brokers>;<topic>;[<opt topic>;<ack>;<comp>]`

      Send flows to Apache Kafka brokers obtained by metadata information 
      <host1>[:<port1>],<host2>[:<port2>]... Initial brokers list used to receive metadata information

:code:`<flow topic>    Flow topic`
:code:`<opt topic>     Flow options topic`
:code:`<0|1|-1>        0 = Don't wait for ack, 1 = Leader ack is enough, 2 = All replica must ack`

:code:`<compression> Compression type: none, gzip, snappy`

Note: <opt topic> is only used when collecting NetFlow to export option template records.
Option template records are just exported as-is, and must be configured with option --load-custom-fields.
To disable option template records export it is safe to specify none as value for <opt topic>.
			
      Example:
	  
:code:`--kafka localhost;flowsTopic;optionsTopic`
:code:`--kafka-conf [<prop=value>|list]`

      Set arbitrary librdkafka configuration property.
      Properties prefixed with "topic." are set to the topic.
      Pass "list" to print all the available properties.
      Multiple properties can be set by repeating this option.
      Examples:
	  
:code:`--kafka-conf batch.num.messages=1000`
:code:`--kafka-conf debug=msg`
:code:`--kafka-conf queue.buffering.max.ms=100`
:code:`--kafka-conf topic.auto.commit.interval.ms=200`
:code:`--kafka-conf list`
:code:`--kafka-num-producers <num>`

Create <num> parallel Kafka producers. Producers are used in round-robin to export flows. Default: 1, maximum: 4.
 
:code:`--kafka-performance-test <num>`

      Exports every flow <num>+1 times.
Use only in test environments to perform performance analyses
and measure how fast Kafka is able to ingest flows.

:code:`--kafka-add-timestamp`

      Add @timestamp field in ISO-8601 format

:code:`--mysql=<host[@port]|unix socket>:<dbname>:<prefix>:<user>:<pw>`
Enable MySQL database support configuration

:code:`--mysql-skip-db-creation`
Skip database schema creation (it is automatically created by --mysql unless this option is used).


:code:`--clickhouse=<host[@port]>:<dbname>:<prefix>:<user>:<pw>`
Dump flows into Clickhouse (Enterprise M/L only)

    
      
Custom Fields
~~~~~~~~~~~~~

:code:`--custom-fields <fields>`

      Comma-separated list of custom fields in the format <key>=<value>
      where value is a literal string/number (or a function)
      Example:
	  
:code:`--custom-fields "NAME=ntop,YEAR=2019"`

NetFlow v9/IPFIX format [-T]
----------------------------

The following options can be used to specify the format:

.. code:: bash

    ID          NetFlow Label               IPFIX Label                 Description
   ---------------------------------------------------------------------------------
   [  1][Len 4] %IN_BYTES                   %octetDeltaCount          	Incoming flow bytes (src->dst) [Aliased to %SRC_TO_DST_BYTES]
   [  2][Len 4] %IN_PKTS                    %packetDeltaCount         	Incoming flow packets (src->dst) [Aliased to %SRC_TO_DST_PKTS]
   [  4][Len 1] %PROTOCOL                   %protocolIdentifier       	IP protocol byte
   [NFv9 58500][IPFIX 35632.1028][Len 16] %PROTOCOL_MAP              	IP protocol name
   [  5][Len 1] %SRC_TOS                    %ipClassOfService         	TOS/DSCP (src->dst)
   [  6][Len 1] %TCP_FLAGS                  %tcpControlBits           	Cumulative of all flow TCP flags
   [  7][Len 2] %L4_SRC_PORT                %sourceTransportPort      	IPv4 source port
   [NFv9 58503][IPFIX 35632.1031][Len 16] %L4_SRC_PORT_MAP           	Layer 4 source port symbolic name
   [  8][Len 4] %IPV4_SRC_ADDR              %sourceIPv4Address        	IPv4 source address
   [  9][Len 1] %IPV4_SRC_MASK              %sourceIPv4PrefixLength   	IPv4 source subnet mask (/<bits>)
   [ 10][Len 4] %INPUT_SNMP                 %ingressInterface         	Input interface SNMP idx
   [ 11][Len 2] %L4_DST_PORT                %destinationTransportPort 	IPv4 destination port
   [NFv9 58507][IPFIX 35632.1035][Len 16] %L4_DST_PORT_MAP           	Layer 4 destination port symbolic name
   [NFv9 58508][IPFIX 35632.1036][Len 2] %L4_SRV_PORT               	Layer 4 server port
   [NFv9 58509][IPFIX 35632.1037][Len 16] %L4_SRV_PORT_MAP           	Layer 4 server port symbolic name
   [ 12][Len 4] %IPV4_DST_ADDR              %destinationIPv4Address   	IPv4 destination address
   [ 13][Len 1] %IPV4_DST_MASK              %destinationIPv4PrefixLength	IPv4 dest subnet mask (/<bits>)
   [ 14][Len 4] %OUTPUT_SNMP                %egressInterface          	Output interface SNMP idx
   [ 15][Len 4] %IPV4_NEXT_HOP              %ipNextHopIPv4Address     	IPv4 next hop address
   [ 16][Len 4] %SRC_AS                     %bgpSourceAsNumber        	Source BGP AS
   [ 17][Len 4] %DST_AS                     %bgpDestinationAsNumber   	Destination BGP AS
   [129][Len 4] %BGP_PREV_ADJACENT_ASN      %bgpNextAdjacentAsNumber  	Source BGP Prev AS
   [128][Len 4] %BGP_NEXT_ADJACENT_ASN      %bgpPrevAdjacentAsNumber  	Destination BGP Next AS
   [ 18][Len 4] %IPV4_BGP_NEXT_HOP          %bgpNexthopIPv4Address    	
   [ 21][Len 4] %LAST_SWITCHED              %flowEndSysUpTime         	SysUptime (msec) of the last flow pkt
   [ 22][Len 4] %FIRST_SWITCHED             %flowStartSysUpTime       	SysUptime (msec) of the first flow pkt
   [ 23][Len 4] %OUT_BYTES                  %postOctetDeltaCount      	Outgoing flow bytes (dst->src) [Aliased to %DST_TO_SRC_BYTES]
   [ 24][Len 4] %OUT_PKTS                   %postPacketDeltaCount     	Outgoing flow packets (dst->src) [Aliased to %DST_TO_SRC_PKTS]
   [ 25][Len 2] %MIN_IP_PKT_LEN             %minimumIpTotalLength     	Len of the smallest flow IP packet observed
   [ 26][Len 2] %MAX_IP_PKT_LEN             %maximumIpTotalLength     	Len of the largest flow IP packet observed
   [ 27][Len 16] %IPV6_SRC_ADDR              %sourceIPv6Address        	IPv6 source address
   [ 28][Len 16] %IPV6_DST_ADDR              %destinationIPv6Address   	IPv6 destination address
   [ 29][Len 1] %IPV6_SRC_MASK              %sourceIPv6PrefixLength   	IPv6 source mask
   [ 30][Len 1] %IPV6_DST_MASK              %destinationIPv6PrefixLength	IPv6 destination mask
   [ 32][Len 2] %ICMP_TYPE                  %icmpTypeCodeIPv4         	ICMP Type * 256 + ICMP code
   [ 34][Len 4] %SAMPLING_INTERVAL                                    	Sampling rate
   [ 35][Len 1] %SAMPLING_ALGORITHM                                   	Sampling type (deterministic/random)
   [ 36][Len 2] %FLOW_ACTIVE_TIMEOUT        %flowActiveTimeout        	Activity timeout of flow cache entries
   [ 37][Len 2] %FLOW_INACTIVE_TIMEOUT      %flowIdleTimeout          	Inactivity timeout of flow cache entries
   [ 38][Len 1] %ENGINE_TYPE                                          	Flow switching engine
   [ 39][Len 1] %ENGINE_ID                                            	Id of the flow switching engine
   [ 40][Len 4] %TOTAL_BYTES_EXP            %exportedOctetTotalCount  	Total bytes exported
   [ 41][Len 4] %TOTAL_PKTS_EXP             %exportedMessageTotalCount	Total flow packets exported
   [ 42][Len 4] %TOTAL_FLOWS_EXP            %exportedFlowRecordTotalCount	Total number of exported flows
   [ 52][Len 1] %MIN_TTL                    %minimumTTL               	Min flow TTL
   [ 53][Len 1] %MAX_TTL                    %maximumTTL               	Max flow TTL
   [ 55][Len 1] %DST_TOS                    %ipClassOfService         	TOS/DSCP (dst->src)
   [ 58][Len 2] %SRC_VLAN                   %vlanId                   	Source VLAN (inner VLAN in QinQ)
   [ 59][Len 2] %DST_VLAN                   %postVlanId               	Destination VLAN (inner VLAN in QinQ)
   [ 56][Len 6] %IN_SRC_MAC                 %sourceMacAddress         	Source MAC Address
   [ 57][Len 6] %OUT_DST_MAC                %postDestinationMacAddress	Post Destination MAC Address
   [ 80][Len 6] %IN_DST_MAC                 %destinationMacAddress    	Destination MAC Address
   [ 81][Len 6] %OUT_SRC_MAC                %postSourceMacAddress     	Post Source MAC Address
   [ 82][Len 8] %INTERFACE_NAME             %interfaceName            	Interface you are capturing from (-i)
   [ 85][Len 8] %OCTET_TOTAL                %octetTotalCount          	Total flow bytes [Aliased to %OCTETS_TOTAL]
   [ 86][Len 8] %PACKET_TOTAL               %packetTotalCount         	Total flow packets [Aliased to %PACKETS_TOTAL]
   [ 89][Len 1] %FORWARDING_STATUS          %forwardingStatus         	Forwarding status of the flow
   [243][Len 2] %DOT1Q_SRC_VLAN             %dot1qVlanId              	Source VLAN (outer VLAN in QinQ)
   [254][Len 2] %DOT1Q_DST_VLAN             %postdot1qVlanId          	Destination VLAN (outer VLAN in QinQ)
   [ 60][Len 1] %IP_PROTOCOL_VERSION        %ipVersion                	[4=IPv4][6=IPv6]
   [ 61][Len 1] %DIRECTION                  %flowDirection            	Flow direction [0=RX, 1=TX]
   [ 62][Len 16] %IPV6_NEXT_HOP              %ipNextHopIPv6Address     	IPv6 next hop address
   [ 70][Len 3] %MPLS_LABEL_1               %mplsTopLabelStackSection 	MPLS label at position 1
   [ 71][Len 3] %MPLS_LABEL_2               %mplsLabelStackSection2   	MPLS label at position 2
   [ 72][Len 3] %MPLS_LABEL_3               %mplsLabelStackSection3   	MPLS label at position 3
   [ 73][Len 3] %MPLS_LABEL_4               %mplsLabelStackSection4   	MPLS label at position 4
   [ 74][Len 3] %MPLS_LABEL_5               %mplsLabelStackSection5   	MPLS label at position 5
   [ 75][Len 3] %MPLS_LABEL_6               %mplsLabelStackSection6   	MPLS label at position 6
   [ 76][Len 3] %MPLS_LABEL_7               %mplsLabelStackSection7   	MPLS label at position 7
   [ 77][Len 3] %MPLS_LABEL_8               %mplsLabelStackSection8   	MPLS label at position 8
   [ 78][Len 3] %MPLS_LABEL_9               %mplsLabelStackSection9   	MPLS label at position 9
   [ 79][Len 3] %MPLS_LABEL_10              %mplsLabelStackSection10  	MPLS label at position 10
   [ 95][Len 4] %APPLICATION_ID             %application_id           	Application Id
   [ 96][Len 16] %APPLICATION_NAME                                     	Layer 7 protocol name
   [136][Len 1] %FLOW_END_REASON            %flowEndReason            	The reason for flow termination.
   [57640][Len 4] %SRC_PROC_PID                                         	Flow source process PID
   [57641][Len 16] %SRC_PROC_NAME                                        	Flow source process name
   [57897][Len 4] %SRC_PROC_UID                                         	Flow source process userId
   [57844][Len 16] %SRC_PROC_USER_NAME                                   	Flow source process user name
   [57845][Len 4] %SRC_FATHER_PROC_PID                                  	Flow source father process PID
   [57846][Len 16] %SRC_FATHER_PROC_NAME                                 	Flow source father process name
   [58012][Len 16] %SRC_PROC_PACKAGE_NAME                                	Flow source process package name
   [58028][Len 32] %SRC_PROC_CMDLINE                                     	Flow source process cmdline args
   [58030][Len 16] %SRC_PROC_CONTAINER_ID                                	Flow source process containerId
   [57847][Len 4] %DST_PROC_PID                                         	Flow dest process PID
   [57848][Len 16] %DST_PROC_NAME                                        	Flow dest process name
   [57898][Len 4] %DST_PROC_UID                                         	Flow dest process userId
   [57849][Len 16] %DST_PROC_USER_NAME                                   	Flow dest process user name
   [57850][Len 4] %DST_FATHER_PROC_PID                                  	Flow dest father process PID
   [57851][Len 16] %DST_FATHER_PROC_NAME                                 	Flow dest father process name
   [58013][Len 16] %DST_PROC_PACKAGE_NAME                                	Flow dest process package name
   [58029][Len 32] %DST_PROC_CMDLINE                                     	Flow dest process cmdline args
   [58031][Len 16] %DST_PROC_CONTAINER_ID                                	Flow dest process containerId
   [102][Len 2] %PACKET_SECTION_OFFSET                                	Packet section offset
   [103][Len 2] %SAMPLED_PACKET_SIZE                                  	Sampled packet size
   [104][Len 2] %SAMPLED_PACKET_ID                                    	Sampled packet id
   [130][Len 4] %EXPORTER_IPV4_ADDRESS      %exporterIPv4Address      	Flow exporter IPv4 Address
   [131][Len 16] %EXPORTER_IPV6_ADDRESS      %exporterIPv6Address      	Flow exporter IPv6 Address
   [148][Len 4] %FLOW_ID                    %flowId                   	Serial Flow Identifier
   [150][Len 4] %FLOW_START_SEC             %flowStartSeconds         	Seconds (epoch) of the first flow packet
   [151][Len 4] %FLOW_END_SEC               %flowEndSeconds           	Seconds (epoch) of the last flow packet
   [152][Len 8] %FLOW_START_MILLISECONDS    %flowStartMilliseconds    	Msec (epoch) of the first flow packet
   [154][Len 8] %FLOW_START_MICROSECONDS    %flowStartMicroseconds    	uSec (epoch) of the first flow packet
   [153][Len 8] %FLOW_END_MILLISECONDS      %flowEndMilliseconds      	Msec (epoch) of the last flow packet
   [155][Len 8] %FLOW_END_MICROSECONDS      %flowEndMicroseconds      	uSec (epoch) of the last flow packet
   [239][Len 1] %BIFLOW_DIRECTION           %biflow_direction         	1=initiator, 2=reverseInitiator
   [225][Len 4] %POST_NAT_SRC_IPV4_ADDR     %postNatSourceIPv4Address 	Post Nat Source IPv4 Address
   [226][Len 4] %POST_NAT_DST_IPV4_ADDR     %postNatDestinationIPv4Address	Post Nat Destination IPv4 Address
   [227][Len 2] %POST_NAPT_SRC_TRANSPORT_PORT %postNaptSourceTransportPort	Post Napt Source Transport Port
   [228][Len 2] %POST_NAPT_DST_TRANSPORT_PORT %postNaptDestinationTransportPort	Post Napt Destination Transport Port
   [229][Len 1] %NAT_ORIGINATING_ADDRESS_REALM %natOriginatingAddressRealm	Nat Originating Address Realm
   [230][Len 1] %NAT_EVENT                  %natEvent                 	Nat Event
   [233][Len 1] %FIREWALL_EVENT             %firewallEvent            	Flow events 0=ignore, 1=created, 2=deleted, 3=denied, 4=alert, 5=update
   [161][Len 4] %FLOW_DURATION_MILLISECONDS %flowDurationMilliseconds 	Flow duration (msec)
   [162][Len 4] %FLOW_DURATION_MICROSECONDS %flowDurationMicroseconds 	Flow duration (usec)
   [176][Len 1] %ICMP_IPV4_TYPE             %icmpTypeIPv4             	ICMP Type
   [177][Len 1] %ICMP_IPV4_CODE             %icmpCodeIPv4             	ICMP Code
   [277][Len 2] %OBSERVATION_POINT_TYPE                               	Observation point type
   [300][Len 2] %OBSERVATION_POINT_ID                                 	Observation point id
   [302][Len 2] %SELECTOR_ID                                          	Selector id
   [304][Len 2] %IPFIX_SAMPLING_ALGORITHM                             	Sampling algorithm
   [309][Len 2] %SAMPLING_SIZE                                        	Number of packets to sample
   [310][Len 2] %SAMPLING_POPULATION                                  	Sampling population
   [312][Len 2] %FRAME_LENGTH                                         	Original L2 frame length
   [318][Len 2] %PACKETS_OBSERVED                                     	Tot number of packets seen
   [319][Len 2] %PACKETS_SELECTED                                     	Number of pkts selected for sampling
   [234][Len 4] %INGRESS_VRFID              %ingressVRFID             	Ingress VRF ID
   [235][Len 4] %EGRESS_VRFID               %egressVRFID              	Egress VRF ID
   [335][Len 2] %SELECTOR_NAME                                        	Sampler name
   [361][Len 2] %PORT_RANGE_START           %portRangeStart           	NAT port range start
   [362][Len 2] %PORT_RANGE_END             %portRangeEnd             	NAT port range end
   [NFv9 57552][IPFIX 35632.80][Len 2] %SRC_FRAGMENTS             	Num fragmented packets src->dst
   [NFv9 57553][IPFIX 35632.81][Len 2] %DST_FRAGMENTS             	Num fragmented packets dst->src
   [NFv9 57595][IPFIX 35632.123][Len 4] %CLIENT_NW_LATENCY_MS      	Network TCP 3WH RTT/2 client <-> nprobe (msec)
   [NFv9 57596][IPFIX 35632.124][Len 4] %SERVER_NW_LATENCY_MS      	Network TCP 3WH RTT/2 nprobe <-> server (msec)
   [NFv9 57550][IPFIX 35632.78][Len 1] %CLIENT_TCP_FLAGS          	Cumulative of all client TCP flags
   [NFv9 57551][IPFIX 35632.79][Len 1] %SERVER_TCP_FLAGS          	Cumulative of all server TCP flags
   [NFv9 57597][IPFIX 35632.125][Len 4] %APPL_LATENCY_MS           	Application latency (msec), a.k.a. server response time
   [NFv9 57943][IPFIX 35632.471][Len 4] %NPROBE_IPV4_ADDRESS       	IPv4 address of the host were nProbe runs
   [NFv9 57554][IPFIX 35632.82][Len 4] %SRC_TO_DST_MAX_THROUGHPUT 	Src to dst max thpt (bps)
   [NFv9 57555][IPFIX 35632.83][Len 4] %SRC_TO_DST_MIN_THROUGHPUT 	Src to dst min thpt (bps)
   [NFv9 57556][IPFIX 35632.84][Len 4] %SRC_TO_DST_AVG_THROUGHPUT 	Src to dst average thpt (bps)
   [NFv9 57557][IPFIX 35632.85][Len 4] %DST_TO_SRC_MAX_THROUGHPUT 	Dst to src max thpt (bps)
   [NFv9 57558][IPFIX 35632.86][Len 4] %DST_TO_SRC_MIN_THROUGHPUT 	Dst to src min thpt (bps)
   [NFv9 57559][IPFIX 35632.87][Len 4] %DST_TO_SRC_AVG_THROUGHPUT 	Dst to src average thpt (bps)
   [NFv9 57995][IPFIX 35632.523][Len 4] %SRC_TO_DST_MAX_EST_THROUGHPUT	Src to dst max estimated TCP thpt (bps)
   [NFv9 57996][IPFIX 35632.524][Len 4] %DST_TO_SRC_MAX_EST_THROUGHPUT	Dst to src max estimated TCP thpt (bps)
   [NFv9 57560][IPFIX 35632.88][Len 4] %NUM_PKTS_UP_TO_128_BYTES  	# packets whose IP size <= 128
   [NFv9 57561][IPFIX 35632.89][Len 4] %NUM_PKTS_128_TO_256_BYTES 	# packets whose IP size > 128 and <= 256
   [NFv9 57562][IPFIX 35632.90][Len 4] %NUM_PKTS_256_TO_512_BYTES 	# packets whose IP size > 256 and < 512
   [NFv9 57563][IPFIX 35632.91][Len 4] %NUM_PKTS_512_TO_1024_BYTES	# packets whose IP size > 512 and < 1024
   [NFv9 57564][IPFIX 35632.92][Len 4] %NUM_PKTS_1024_TO_1514_BYTES	# packets whose IP size > 1024 and <= 1514
   [NFv9 57565][IPFIX 35632.93][Len 4] %NUM_PKTS_OVER_1514_BYTES  	# packets whose IP size > 1514
   [NFv9 57570][IPFIX 35632.98][Len 4] %CUMULATIVE_ICMP_TYPE      	Cumulative OR of ICMP type packets
   [NFv9 57573][IPFIX 35632.101][Len 2] %SRC_IP_COUNTRY            	Country where the src IP is located
   [NFv9 57574][IPFIX 35632.102][Len 16] %SRC_IP_CITY               	City where the src IP is located
   [NFv9 57575][IPFIX 35632.103][Len 2] %DST_IP_COUNTRY            	Country where the dst IP is located
   [NFv9 57576][IPFIX 35632.104][Len 16] %DST_IP_CITY               	City where the dst IP is located
   [NFv9 57920][IPFIX 35632.448][Len 16] %SRC_IP_LONG               	Longitude where the src IP is located
   [NFv9 57921][IPFIX 35632.449][Len 16] %SRC_IP_LAT                	Latitude where the src IP is located
   [NFv9 57922][IPFIX 35632.450][Len 16] %DST_IP_LONG               	Longitude where the dst IP is located
   [NFv9 57923][IPFIX 35632.451][Len 16] %DST_IP_LAT                	Latitude where the dst IP is located
   [NFv9 57577][IPFIX 35632.105][Len 2] %FLOW_PROTO_PORT           	L7 port that identifies the flow protocol or 0 if unknown
   [NFv9 57578][IPFIX 35632.106][Len 4] %UPSTREAM_TUNNEL_ID        	Upstream tunnel identifier (e.g. GTP TEID, VXLAN VNI) or 0 if unknown
   [NFv9 57918][IPFIX 35632.446][Len 2] %UPSTREAM_SESSION_ID       	Upstream session identifier (e.g. L2TP) or 0 if unknown
   [NFv9 57579][IPFIX 35632.107][Len 2] %LONGEST_FLOW_PKT          	Longest packet (bytes) of the flow
   [NFv9 57580][IPFIX 35632.108][Len 2] %SHORTEST_FLOW_PKT         	Shortest packet (bytes) of the flow
   [NFv9 57599][IPFIX 35632.127][Len 4] %RETRANSMITTED_IN_BYTES    	Number of retransmitted TCP flow bytes (src->dst)
   [NFv9 57581][IPFIX 35632.109][Len 4] %RETRANSMITTED_IN_PKTS     	Number of retransmitted TCP flow packets (src->dst)
   [NFv9 57600][IPFIX 35632.128][Len 4] %RETRANSMITTED_OUT_BYTES   	Number of retransmitted TCP flow bytes (dst->src)
   [NFv9 57582][IPFIX 35632.110][Len 4] %RETRANSMITTED_OUT_PKTS    	Number of retransmitted TCP flow packets (dst->src)
   [NFv9 57583][IPFIX 35632.111][Len 4] %OOORDER_IN_PKTS           	Number of out of order TCP flow packets (dst->src)
   [NFv9 57584][IPFIX 35632.112][Len 4] %OOORDER_OUT_PKTS          	Number of out of order TCP flow packets (src->dst)
   [NFv9 57585][IPFIX 35632.113][Len 1] %UNTUNNELED_PROTOCOL       	Untunneled IP protocol byte
   [NFv9 57586][IPFIX 35632.114][Len 4] %UNTUNNELED_IPV4_SRC_ADDR  	Untunneled IPv4 source address
   [NFv9 57587][IPFIX 35632.115][Len 2] %UNTUNNELED_L4_SRC_PORT    	Untunneled IPv4 source port
   [NFv9 57588][IPFIX 35632.116][Len 4] %UNTUNNELED_IPV4_DST_ADDR  	Untunneled IPv4 destination address
   [NFv9 57589][IPFIX 35632.117][Len 2] %UNTUNNELED_L4_DST_PORT    	Untunneled IPv4 destination port
   [NFv9 57590][IPFIX 35632.118][Len 2] %L7_PROTO                  	Layer 7 protocol (numeric)
   [NFv9 58032][IPFIX 35632.560][Len 1] %L7_CONFIDENCE             	nDPI confidence
   [NFv9 57591][IPFIX 35632.119][Len 16 varlen] %L7_PROTO_NAME             	Layer 7 protocol name [Aliased to %APPLICATION_NAME]
   [NFv9 57973][IPFIX 35632.501][Len 16 varlen] %L7_PROTO_CATEGORY         	Layer 7 protocol category
   [NFv9 58011][IPFIX 35632.539][Len 24 varlen] %L7_INFO                   	Layer 7 flow information
   [NFv9 57592][IPFIX 35632.120][Len 4] %DOWNSTREAM_TUNNEL_ID      	Downstream tunnel identifier (e.g. GTP TEID, VXLAN VNI) or 0 if unknown
   [NFv9 57919][IPFIX 35632.447][Len 2] %DOWNSTREAM_SESSION_ID     	Downstream session identifier (e.g. L2TP) or 0 if unknown
   [NFv9 57660][IPFIX 35632.188][Len 48 varlen] %TLS_SERVER_NAME           	TLS server name
   [NFv9 57661][IPFIX 35632.189][Len 40 varlen] %BITTORRENT_HASH           	BITTORRENT hash
   [NFv9 57593][IPFIX 35632.121][Len 32 varlen] %FLOW_USER_NAME            	Flow username of the tunnel (if known)
   [NFv9 57594][IPFIX 35632.122][Len 32 varlen] %FLOW_SERVER_NAME          	Flow server name (if known)
   [NFv9 57598][IPFIX 35632.126][Len 8 varlen] %PLUGIN_NAME               	Plugin name used by this flow (if any)
   [NFv9 57868][IPFIX 35632.396][Len 16] %UNTUNNELED_IPV6_SRC_ADDR  	Untunneled IPv6 source address
   [NFv9 57869][IPFIX 35632.397][Len 16] %UNTUNNELED_IPV6_DST_ADDR  	Untunneled IPv6 destination address
   [NFv9 57819][IPFIX 35632.347][Len 4] %NUM_PKTS_TTL_EQ_1         	# packets with TTL = 1
   [NFv9 57818][IPFIX 35632.346][Len 4] %NUM_PKTS_TTL_2_5          	# packets with TTL > 1 and TTL <= 5
   [NFv9 57806][IPFIX 35632.334][Len 4] %NUM_PKTS_TTL_5_32         	# packets with TTL > 5 and TTL <= 32
   [NFv9 57807][IPFIX 35632.335][Len 4] %NUM_PKTS_TTL_32_64        	# packets with TTL > 32 and <= 64 
   [NFv9 57808][IPFIX 35632.336][Len 4] %NUM_PKTS_TTL_64_96        	# packets with TTL > 64 and <= 96
   [NFv9 57809][IPFIX 35632.337][Len 4] %NUM_PKTS_TTL_96_128       	# packets with TTL > 96 and <= 128
   [NFv9 57810][IPFIX 35632.338][Len 4] %NUM_PKTS_TTL_128_160      	# packets with TTL > 128 and <= 160
   [NFv9 57811][IPFIX 35632.339][Len 4] %NUM_PKTS_TTL_160_192      	# packets with TTL > 160 and <= 192
   [NFv9 57812][IPFIX 35632.340][Len 4] %NUM_PKTS_TTL_192_224      	# packets with TTL > 192 and <= 224
   [NFv9 57813][IPFIX 35632.341][Len 4] %NUM_PKTS_TTL_224_255      	# packets with TTL > 224 and <= 255
   [NFv9 57821][IPFIX 35632.349][Len 37] %IN_SRC_OSI_SAP            	OSI Source SAP (OSI Traffic Only)
   [NFv9 57822][IPFIX 35632.350][Len 37] %OUT_DST_OSI_SAP           	OSI Destination SAP (OSI Traffic Only)
   [NFv9 57863][IPFIX 35632.391][Len 4] %DURATION_IN               	Client to Server stream duration (msec)
   [NFv9 57864][IPFIX 35632.392][Len 4] %DURATION_OUT              	Client to Server stream duration (msec)
   [NFv9 57887][IPFIX 35632.415][Len 2] %TCP_WIN_MIN_IN            	Min TCP Window (src->dst)
   [NFv9 57888][IPFIX 35632.416][Len 2] %TCP_WIN_MAX_IN            	Max TCP Window (src->dst)
   [NFv9 57889][IPFIX 35632.417][Len 2] %TCP_WIN_MSS_IN            	TCP Max Segment Size (src->dst)
   [NFv9 57890][IPFIX 35632.418][Len 1] %TCP_WIN_SCALE_IN          	TCP Window Scale (src->dst)
   [NFv9 57891][IPFIX 35632.419][Len 2] %TCP_WIN_MIN_OUT           	Min TCP Window (dst->src)
   [NFv9 57892][IPFIX 35632.420][Len 2] %TCP_WIN_MAX_OUT           	Max TCP Window (dst->src)
   [NFv9 57893][IPFIX 35632.421][Len 2] %TCP_WIN_MSS_OUT           	TCP Max Segment Size (dst->src)
   [NFv9 57894][IPFIX 35632.422][Len 1] %TCP_WIN_SCALE_OUT         	TCP Window Scale (dst->src)
   [NFv9 57910][IPFIX 35632.438][Len 4] %PAYLOAD_HASH              	Initial flow payload hash
   [NFv9 57915][IPFIX 35632.443][Len 16] %SRC_AS_MAP                	Organization name for SRC_AS
   [NFv9 57916][IPFIX 35632.444][Len 16] %DST_AS_MAP                	Organization name for DST_AS
   [NFv9 57944][IPFIX 35632.472][Len 8] %SRC_TO_DST_SECOND_BYTES   	Bytes/sec (src->dst)
   [NFv9 57945][IPFIX 35632.473][Len 8] %DST_TO_SRC_SECOND_BYTES   	Bytes/sec2 (dst->src)
   [NFv9 57961][IPFIX 35632.489][Len 32 varlen] %JA3C_HASH                 	JA3 client hash
   [NFv9 57962][IPFIX 35632.490][Len 32 varlen] %JA3S_HASH                 	JA3 server hash
   [NFv9 57963][IPFIX 35632.491][Len 48 varlen] %SRC_HOST_NAME             	Symbolic src host name
   [NFv9 57964][IPFIX 35632.492][Len 48 varlen] %DST_HOST_NAME             	Symbolic dst host name
   [NFv9 57965][IPFIX 35632.493][Len 2] %TLS_CIPHER                	TLS Connection Cipher
   [NFv9 57966][IPFIX 35632.494][Len 1] %TLS_UNSAFE_CIPHER         	TLS Safe(0)/unsafe(1) cipher
   [NFv9 57967][IPFIX 35632.495][Len 2] %TLS_VERSION               	TLS Version
   [NFv9 57974][IPFIX 35632.502][Len 47] %SEQ_PLEN                  	Seq of packet len (6 classes)
   [NFv9 57977][IPFIX 35632.505][Len 47] %SEQ_TDIFF                 	Seq of time diff (6 classes)
   [NFv9 57978][IPFIX 35632.506][Len 1] %SEQ_PLEN_HASH             	Seq of packet len hash
   [NFv9 57979][IPFIX 35632.507][Len 1] %SEQ_TDIFF_HASH            	Seq of time diff hash
   [NFv9 57980][IPFIX 35632.508][Len 94] %PKT_VECTOR                	Seq of packet len (+=c2s, -=s2c)
   [NFv9 57971][IPFIX 35632.499][Len 32 varlen] %HASSH_CLIENT              	HASSH client hash
   [NFv9 57972][IPFIX 35632.500][Len 32 varlen] %HASSH_SERVER              	HASSH server hash
   [NFv9 57975][IPFIX 35632.503][Len 4] %ENTROPY_CLIENT_BYTES      	Byte (src->dst) entropy * 1000
   [NFv9 57976][IPFIX 35632.504][Len 4] %ENTROPY_SERVER_BYTES      	Byte (dst->src) entropy * 1000
   [NFv9 57981][IPFIX 35632.509][Len 8] %L7_PROTO_RISK             	Layer 7 protocol risk (bitmap)
   [NFv9 57982][IPFIX 35632.510][Len 64 varlen] %L7_PROTO_RISK_NAME        	Layer 7 protocol risk (string)
   [NFv9 57999][IPFIX 35632.527][Len 2] %L7_RISK_SCORE             	Layer 7 flow risk score
   [NFv9 57994][IPFIX 35632.522][Len 2] %FLOW_VERDICT              	Flow verdict marker (0 = unknown, 1=pass, 2=drop...)
   [NFv9 57997][IPFIX 35632.525][Len 24 varlen] %SRC_HOST_LABEL            	Src host label
   [NFv9 57998][IPFIX 35632.526][Len 24 varlen] %DST_HOST_LABEL            	Dest host label
   [NFv9 58003][IPFIX 35632.531][Len 4] %SRC_TO_DST_IAT_MIN        	Min (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58004][IPFIX 35632.532][Len 4] %SRC_TO_DST_IAT_MAX        	Max (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58005][IPFIX 35632.533][Len 4] %SRC_TO_DST_IAT_AVG        	Avg (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58006][IPFIX 35632.534][Len 4] %SRC_TO_DST_IAT_STDDEV     	StdDev (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58007][IPFIX 35632.535][Len 4] %DST_TO_SRC_IAT_MIN        	Min (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58008][IPFIX 35632.536][Len 4] %DST_TO_SRC_IAT_MAX        	Max (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58009][IPFIX 35632.537][Len 4] %DST_TO_SRC_IAT_AVG        	Avg (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58010][IPFIX 35632.538][Len 4] %DST_TO_SRC_IAT_STDDEV     	StdDev (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58025][IPFIX 35632.553][Len 24 varlen] %AAA_NAT_KEY               	AAA/NAT Correlation Key
   [NFv9 58026][IPFIX 35632.554][Len 4] %L7_ERROR_CODE             	Error code (e.g. SNMP, DNS. HTTP)
   [NFv9 58027][IPFIX 35632.555][Len 48 varlen] %L7_RISK_INFO              	L7 Risk Information

Plugin HTTP Protocol templates:

.. code:: bash

   [NFv9 57652][IPFIX 35632.180][Len 128 varlen] %HTTP_URL                  	HTTP URL (IXIA URI)
   [NFv9 57832][IPFIX 35632.360][Len 4 varlen] %HTTP_METHOD               	HTTP METHOD
   [NFv9 57653][IPFIX 35632.181][Len 2] %HTTP_RET_CODE             	HTTP return code (e.g. 200, 304...)
   [NFv9 57654][IPFIX 35632.182][Len 128 varlen] %HTTP_REFERER              	HTTP Referer
   [NFv9 57655][IPFIX 35632.183][Len 256 varlen] %HTTP_UA                   	HTTP User Agent
   [NFv9 57656][IPFIX 35632.184][Len 256 varlen] %HTTP_MIME                 	HTTP Mime Type
   [NFv9 57659][IPFIX 35632.187][Len 64 varlen] %HTTP_HOST                 	HTTP(S) Host Name (IXIA Host Name)
   [NFv9 57833][IPFIX 35632.361][Len 64 varlen] %HTTP_SITE                 	HTTP server without host name
   [NFv9 57932][IPFIX 35632.460][Len 256 varlen] %HTTP_X_FORWARDED_FOR      	HTTP X-Forwarded-For
   [NFv9 57933][IPFIX 35632.461][Len 256 varlen] %HTTP_VIA                  	HTTP Via

Plugin DNS/LLMNR Protocol templates:

.. code:: bash
	  
   [NFv9 57677][IPFIX 35632.205][Len 256 varlen] %DNS_QUERY                 	DNS query
   [NFv9 57678][IPFIX 35632.206][Len 2] %DNS_QUERY_ID              	DNS query transaction Id
   [NFv9 57679][IPFIX 35632.207][Len 1] %DNS_QUERY_TYPE            	DNS query type (e.g. 1=A, 2=NS..)
   [NFv9 57680][IPFIX 35632.208][Len 1] %DNS_RET_CODE              	DNS return code (e.g. 0=no error)
   [NFv9 57681][IPFIX 35632.209][Len 1] %DNS_NUM_ANSWERS           	DNS # of returned answers
   [NFv9 57824][IPFIX 35632.352][Len 4] %DNS_TTL_ANSWER            	TTL of the first A record (if any)
   [NFv9 57870][IPFIX 35632.398][Len 256 varlen] %DNS_RESPONSE              	DNS response(s)

Plugin SIP Plugin templates:

.. code:: bash
	  
   [NFv9 57602][IPFIX 35632.130][Len 96 varlen] %SIP_CALL_ID               	SIP call-id
   [NFv9 57603][IPFIX 35632.131][Len 96 varlen] %SIP_CALLING_PARTY         	SIP Call initiator
   [NFv9 57604][IPFIX 35632.132][Len 96 varlen] %SIP_CALLED_PARTY          	SIP Called party
   [NFv9 57605][IPFIX 35632.133][Len 512] %SIP_RTP_CODECS            	SIP RTP codecs
   [NFv9 58000][IPFIX 35632.528][Len 4] %SIP_REGISTER_MAX_RRD      	SIP REGISTER max rsp delay (msec)
   [NFv9 58001][IPFIX 35632.529][Len 1] %SIP_REGISTER_NUM_OK       	SIP REGISTER number of rsp ok/authorized
   [NFv9 58002][IPFIX 35632.530][Len 1] %SIP_REGISTER_NUM_OTHER    	SIP REGISTER number of rsp not ok/authorized
   [NFv9 57606][IPFIX 35632.134][Len 4] %SIP_INVITE_TIME           	SIP time (epoch) of INVITE
   [NFv9 57607][IPFIX 35632.135][Len 4] %SIP_TRYING_TIME           	SIP time (epoch) of Trying
   [NFv9 57608][IPFIX 35632.136][Len 4] %SIP_RINGING_TIME          	SIP time (epoch) of RINGING
   [NFv9 57609][IPFIX 35632.137][Len 4] %SIP_INVITE_OK_TIME        	SIP time (epoch) of INVITE OK
   [NFv9 57610][IPFIX 35632.138][Len 4] %SIP_INVITE_FAILURE_TIME   	SIP time (epoch) of INVITE FAILURE
   [NFv9 57611][IPFIX 35632.139][Len 4] %SIP_BYE_TIME              	SIP time (epoch) of BYE
   [NFv9 57612][IPFIX 35632.140][Len 4] %SIP_BYE_OK_TIME           	SIP time (epoch) of BYE OK
   [NFv9 57613][IPFIX 35632.141][Len 4] %SIP_CANCEL_TIME           	SIP time (epoch) of CANCEL
   [NFv9 57614][IPFIX 35632.142][Len 4] %SIP_CANCEL_OK_TIME        	SIP time (epoch) of CANCEL OK
   [NFv9 57615][IPFIX 35632.143][Len 4] %SIP_RTP_IPV4_SRC_ADDR     	SIP RTP stream source IP
   [NFv9 57616][IPFIX 35632.144][Len 2] %SIP_RTP_L4_SRC_PORT       	SIP RTP stream source port
   [NFv9 57617][IPFIX 35632.145][Len 4] %SIP_RTP_IPV4_DST_ADDR     	SIP RTP stream dest IP
   [NFv9 57618][IPFIX 35632.146][Len 2] %SIP_RTP_L4_DST_PORT       	SIP RTP stream dest port
   [NFv9 57619][IPFIX 35632.147][Len 4] %SIP_RESPONSE_CODE         	SIP failure response code
   [NFv9 57620][IPFIX 35632.148][Len 4] %SIP_REASON_CAUSE          	SIP Cancel/Bye/Failure reason cause
   [NFv9 57788][IPFIX 35632.316][Len 96 varlen] %SIP_UAC                   	SIP user-agent client
   [NFv9 57789][IPFIX 35632.317][Len 96 varlen] %SIP_UAS                   	SIP user-agent server
   [NFv9 57834][IPFIX 35632.362][Len 128] %SIP_C_IP                  	SIP C IP adresses
   [NFv9 57835][IPFIX 35632.363][Len 12 varlen] %SIP_CALL_STATE            	SIP Call State

Plugin RTP Plugin templates:

.. code:: bash
	  
   [NFv9 57909][IPFIX 35632.437][Len 4] %RTP_SSRC                  	RTP Sync Source ID
   [NFv9 57622][IPFIX 35632.150][Len 4] %RTP_FIRST_SEQ             	First flow RTP Seq Number
   [NFv9 57623][IPFIX 35632.151][Len 4] %RTP_FIRST_TS              	First flow RTP timestamp
   [NFv9 57624][IPFIX 35632.152][Len 4] %RTP_LAST_SEQ              	Last flow RTP Seq Number
   [NFv9 57625][IPFIX 35632.153][Len 4] %RTP_LAST_TS               	Last flow RTP timestamp
   [NFv9 57626][IPFIX 35632.154][Len 4] %RTP_IN_JITTER             	RTP jitter (ms * 1000)
   [NFv9 57627][IPFIX 35632.155][Len 4] %RTP_OUT_JITTER            	RTP jitter (ms * 1000)
   [NFv9 57628][IPFIX 35632.156][Len 4] %RTP_IN_PKT_LOST           	Packet lost in stream (src->dst)
   [NFv9 57629][IPFIX 35632.157][Len 4] %RTP_OUT_PKT_LOST          	Packet lost in stream (dst->src)
   [NFv9 57902][IPFIX 35632.430][Len 4] %RTP_IN_PKT_DROP           	Packet discarded by Jitter Buffer (src->dst)
   [NFv9 57903][IPFIX 35632.431][Len 4] %RTP_OUT_PKT_DROP          	Packet discarded by Jitter Buffer (dst->src)
   [NFv9 57633][IPFIX 35632.161][Len 1] %RTP_IN_PAYLOAD_TYPE       	RTP payload type
   [NFv9 57630][IPFIX 35632.158][Len 1] %RTP_OUT_PAYLOAD_TYPE      	RTP payload type
   [NFv9 57631][IPFIX 35632.159][Len 4] %RTP_IN_MAX_DELTA          	Max delta (ms*100) between consecutive pkts (src->dst)
   [NFv9 57632][IPFIX 35632.160][Len 4] %RTP_OUT_MAX_DELTA         	Max delta (ms*100) between consecutive pkts (dst->src)
   [NFv9 57820][IPFIX 35632.348][Len 64 varlen] %RTP_SIP_CALL_ID           	SIP call-id corresponding to this RTP stream
   [NFv9 57906][IPFIX 35632.434][Len 4] %RTP_MOS                   	RTP pseudo-MOS (value * 100) (average both directions)
   [NFv9 57842][IPFIX 35632.370][Len 4] %RTP_IN_MOS                	RTP pseudo-MOS (value * 100) (src->dst)
   [NFv9 57904][IPFIX 35632.432][Len 4] %RTP_OUT_MOS               	RTP pseudo-MOS (value * 100) (dst->src)
   [NFv9 57908][IPFIX 35632.436][Len 4] %RTP_R_FACTOR              	RTP pseudo-R_FACTOR (value * 100) (average both directions)
   [NFv9 57843][IPFIX 35632.371][Len 4] %RTP_IN_R_FACTOR           	RTP pseudo-R_FACTOR (value * 100) (src->dst)
   [NFv9 57905][IPFIX 35632.433][Len 4] %RTP_OUT_R_FACTOR          	RTP pseudo-R_FACTOR (value * 100) (dst->src)
   [NFv9 57853][IPFIX 35632.381][Len 4] %RTP_IN_TRANSIT            	RTP Transit (value * 100) (src->dst)
   [NFv9 57854][IPFIX 35632.382][Len 4] %RTP_OUT_TRANSIT           	RTP Transit (value * 100) (dst->src)
   [NFv9 57852][IPFIX 35632.380][Len 4] %RTP_RTT                   	RTP Round Trip Time (ms)
   [NFv9 57867][IPFIX 35632.395][Len 16 varlen] %RTP_DTMF_TONES            	DTMF tones sent (if any) during the call

Plugin FTP Protocol templates:

.. code:: bash
	  
   [NFv9 57828][IPFIX 35632.356][Len 32 varlen] %FTP_LOGIN                 	FTP client login
   [NFv9 57829][IPFIX 35632.357][Len 32 varlen] %FTP_PASSWORD              	FTP client password
   [NFv9 57830][IPFIX 35632.358][Len 64 varlen] %FTP_COMMAND               	FTP client command
   [NFv9 57831][IPFIX 35632.359][Len 2] %FTP_COMMAND_RET_CODE      	FTP client command return code

Plugin SMTP Protocol templates:

.. code:: bash
	  
   [NFv9 57657][IPFIX 35632.185][Len 64 varlen] %SMTP_MAIL_FROM            	Mail sender
   [NFv9 57658][IPFIX 35632.186][Len 64 varlen] %SMTP_RCPT_TO              	Mail recipient

Plugin BGP Update Listener templates:

.. code:: bash
	  
   [NFv9 57762][IPFIX 35632.290][Len 4] %SRC_AS_PATH_1             	Src AS path position 1
   [NFv9 57763][IPFIX 35632.291][Len 4] %SRC_AS_PATH_2             	Src AS path position 2
   [NFv9 57764][IPFIX 35632.292][Len 4] %SRC_AS_PATH_3             	Src AS path position 3
   [NFv9 57765][IPFIX 35632.293][Len 4] %SRC_AS_PATH_4             	Src AS path position 4
   [NFv9 57766][IPFIX 35632.294][Len 4] %SRC_AS_PATH_5             	Src AS path position 5
   [NFv9 57767][IPFIX 35632.295][Len 4] %SRC_AS_PATH_6             	Src AS path position 6
   [NFv9 57768][IPFIX 35632.296][Len 4] %SRC_AS_PATH_7             	Src AS path position 7
   [NFv9 57769][IPFIX 35632.297][Len 4] %SRC_AS_PATH_8             	Src AS path position 8
   [NFv9 57770][IPFIX 35632.298][Len 4] %SRC_AS_PATH_9             	Src AS path position 9
   [NFv9 57771][IPFIX 35632.299][Len 4] %SRC_AS_PATH_10            	Src AS path position 10
   [NFv9 57772][IPFIX 35632.300][Len 4] %DST_AS_PATH_1             	Dest AS path position 1
   [NFv9 57773][IPFIX 35632.301][Len 4] %DST_AS_PATH_2             	Dest AS path position 2
   [NFv9 57774][IPFIX 35632.302][Len 4] %DST_AS_PATH_3             	Dest AS path position 3
   [NFv9 57775][IPFIX 35632.303][Len 4] %DST_AS_PATH_4             	Dest AS path position 4
   [NFv9 57776][IPFIX 35632.304][Len 4] %DST_AS_PATH_5             	Dest AS path position 5
   [NFv9 57777][IPFIX 35632.305][Len 4] %DST_AS_PATH_6             	Dest AS path position 6
   [NFv9 57778][IPFIX 35632.306][Len 4] %DST_AS_PATH_7             	Dest AS path position 7
   [NFv9 57779][IPFIX 35632.307][Len 4] %DST_AS_PATH_8             	Dest AS path position 8
   [NFv9 57780][IPFIX 35632.308][Len 4] %DST_AS_PATH_9             	Dest AS path position 9
   [NFv9 57781][IPFIX 35632.309][Len 4] %DST_AS_PATH_10            	Dest AS path position 10

Plugin GTPv0 Signaling Protocol templates:

.. code:: bash
	  
   [NFv9 57793][IPFIX 35632.321][Len 1] %GTPV0_REQ_MSG_TYPE        	GTPv0 Request Msg Type
   [NFv9 57794][IPFIX 35632.322][Len 1] %GTPV0_RSP_MSG_TYPE        	GTPv0 Response Msg Type
   [NFv9 57795][IPFIX 35632.323][Len 8] %GTPV0_TID                 	GTPv0 Tunnel Identifier
   [NFv9 57798][IPFIX 35632.326][Len 64] %GTPV0_APN_NAME            	GTPv0 APN Name
   [NFv9 57796][IPFIX 35632.324][Len 4] %GTPV0_END_USER_IP         	GTPv0 End User IP Address
   [NFv9 57797][IPFIX 35632.325][Len 16] %GTPV0_END_USER_MSISDN     	GTPv0 End User MSISDN
   [NFv9 57799][IPFIX 35632.327][Len 2] %GTPV0_RAI_MCC             	GTPv0 Mobile Country Code
   [NFv9 57800][IPFIX 35632.328][Len 2] %GTPV0_RAI_MNC             	GTPv0 Mobile Network Code
   [NFv9 57801][IPFIX 35632.329][Len 2] %GTPV0_RAI_CELL_LAC        	GTPv0 Cell Location Area Code
   [NFv9 57802][IPFIX 35632.330][Len 2] %GTPV0_RAI_CELL_RAC        	GTPv0 Cell Routing Area Code
   [NFv9 57803][IPFIX 35632.331][Len 1] %GTPV0_RESPONSE_CAUSE      	GTPv0 Cause of Operation

Plugin GTPv1 Signaling Protocol templates:

.. code:: bash
	  
   [NFv9 57692][IPFIX 35632.220][Len 1] %GTPV1_REQ_MSG_TYPE        	GTPv1 Request Msg Type
   [NFv9 57693][IPFIX 35632.221][Len 1] %GTPV1_RSP_MSG_TYPE        	GTPv1 Response Msg Type
   [NFv9 57694][IPFIX 35632.222][Len 4] %GTPV1_C2S_TEID_DATA       	GTPv1 Client->Server TunnelId Data
   [NFv9 57695][IPFIX 35632.223][Len 4] %GTPV1_C2S_TEID_CTRL       	GTPv1 Client->Server TunnelId Control
   [NFv9 57696][IPFIX 35632.224][Len 4] %GTPV1_S2C_TEID_DATA       	GTPv1 Server->Client TunnelId Data
   [NFv9 57697][IPFIX 35632.225][Len 4] %GTPV1_S2C_TEID_CTRL       	GTPv1 Server->Client TunnelId Control
   [NFv9 57698][IPFIX 35632.226][Len 4] %GTPV1_END_USER_IPV4       	GTPv1 End User IP Address
   [NFv9 57699][IPFIX 35632.227][Len 16] %GTPV1_END_USER_IMSI       	GTPv1 End User IMSI
   [NFv9 57700][IPFIX 35632.228][Len 16] %GTPV1_END_USER_MSISDN     	GTPv1 End User MSISDN
   [NFv9 57701][IPFIX 35632.229][Len 16] %GTPV1_END_USER_IMEI       	GTPv1 End User IMEI
   [NFv9 57702][IPFIX 35632.230][Len 64] %GTPV1_APN_NAME            	GTPv1 APN Name
   [NFv9 57708][IPFIX 35632.236][Len 1] %GTPV1_RAT_TYPE            	GTPv1 RAT Type
   [NFv9 57703][IPFIX 35632.231][Len 2] %GTPV1_RAI_MCC             	GTPv1 RAI Mobile Country Code
   [NFv9 57704][IPFIX 35632.232][Len 2] %GTPV1_RAI_MNC             	GTPv1 RAI Mobile Network Code
   [NFv9 57814][IPFIX 35632.342][Len 2] %GTPV1_RAI_LAC             	GTPv1 RAI Location Area Code
   [NFv9 57815][IPFIX 35632.343][Len 1] %GTPV1_RAI_RAC             	GTPv1 RAI Routing Area Code
   [NFv9 57816][IPFIX 35632.344][Len 2] %GTPV1_ULI_MCC             	GTPv1 ULI Mobile Country Code
   [NFv9 57817][IPFIX 35632.345][Len 2] %GTPV1_ULI_MNC             	GTPv1 ULI Mobile Network Code
   [NFv9 57705][IPFIX 35632.233][Len 2] %GTPV1_ULI_CELL_LAC        	GTPv1 ULI Cell Location Area Code
   [NFv9 57706][IPFIX 35632.234][Len 2] %GTPV1_ULI_CELL_CI         	GTPv1 ULI Cell CI
   [NFv9 57707][IPFIX 35632.235][Len 2] %GTPV1_ULI_SAC             	GTPv1 ULI SAC
   [NFv9 57804][IPFIX 35632.332][Len 1] %GTPV1_RESPONSE_CAUSE      	GTPv1 Cause of Operation

Plugin GTPv2 Signaling Protocol templates:

.. code:: bash
	  
   [NFv9 57742][IPFIX 35632.270][Len 1] %GTPV2_REQ_MSG_TYPE        	GTPv2 Request Msg Type
   [NFv9 57743][IPFIX 35632.271][Len 1] %GTPV2_RSP_MSG_TYPE        	GTPv2 Response Msg Type
   [NFv9 57744][IPFIX 35632.272][Len 4] %GTPV2_C2S_S1U_GTPU_TEID   	GTPv2 Client->Svr S1U GTPU TEID
   [NFv9 57745][IPFIX 35632.273][Len 4] %GTPV2_C2S_S1U_GTPU_IP     	GTPv2 Client->Svr S1U GTPU IP
   [NFv9 57746][IPFIX 35632.274][Len 4] %GTPV2_S2C_S1U_GTPU_TEID   	GTPv2 Srv->Client S1U GTPU TEID
   [NFv9 57907][IPFIX 35632.435][Len 17] %GTPV2_S5_S8_GTPC_TEID     	GTPv2 S5/S8 SGW GTPC TEIDs
   [NFv9 57747][IPFIX 35632.275][Len 4] %GTPV2_S2C_S1U_GTPU_IP     	GTPv2 Srv->Client S1U GTPU IP
   [NFv9 57911][IPFIX 35632.439][Len 4] %GTPV2_C2S_S5_S8_GTPU_TEID 	GTPv2 Client->Srv S5/S8 PGW GTPU TEID
   [NFv9 57912][IPFIX 35632.440][Len 4] %GTPV2_S2C_S5_S8_GTPU_TEID 	GTPv2 Srv->Client S5/S8 PGW GTPU TEID
   [NFv9 57913][IPFIX 35632.441][Len 4] %GTPV2_C2S_S5_S8_GTPU_IP   	GTPv2 Client->Srv S5/S8 PGW GTPU IP
   [NFv9 57914][IPFIX 35632.442][Len 4] %GTPV2_S2C_S5_S8_GTPU_IP   	GTPv2 Srv->Client S5/S8 PGW GTPU IP
   [NFv9 57748][IPFIX 35632.276][Len 16] %GTPV2_END_USER_IMSI       	GTPv2 End User IMSI
   [NFv9 57749][IPFIX 35632.277][Len 16] %GTPV2_END_USER_MSISDN     	GTPv2 End User MSISDN
   [NFv9 57750][IPFIX 35632.278][Len 64] %GTPV2_APN_NAME            	GTPv2 APN Name
   [NFv9 57751][IPFIX 35632.279][Len 2] %GTPV2_ULI_MCC             	GTPv2 Mobile Country Code
   [NFv9 57752][IPFIX 35632.280][Len 2] %GTPV2_ULI_MNC             	GTPv2 Mobile Network Code
   [NFv9 57753][IPFIX 35632.281][Len 2] %GTPV2_ULI_CELL_TAC        	GTPv2 Tracking Area Code
   [NFv9 57754][IPFIX 35632.282][Len 4] %GTPV2_ULI_CELL_ID         	GTPv2 Cell Identifier
   [NFv9 57805][IPFIX 35632.333][Len 1] %GTPV2_RESPONSE_CAUSE      	GTPv2 Cause of Operation
   [NFv9 57755][IPFIX 35632.283][Len 1] %GTPV2_RAT_TYPE            	GTPv2 RAT Type
   [NFv9 57756][IPFIX 35632.284][Len 4] %GTPV2_PDN_IP              	GTPV2 PDN IP Address
   [NFv9 57757][IPFIX 35632.285][Len 16] %GTPV2_END_USER_IMEI       	GTPv2 End User IMEI
   [NFv9 57926][IPFIX 35632.454][Len 4] %GTPV2_C2S_S5_S8_GTPC_IP   	GTPv2 Client->Svr S5/S8 GTPC IP
   [NFv9 57927][IPFIX 35632.455][Len 4] %GTPV2_S2C_S5_S8_GTPC_IP   	GTPv2 Svr->Client S5/S8 GTPC IP
   [NFv9 57928][IPFIX 35632.456][Len 4] %GTPV2_C2S_S5_S8_SGW_GTPU_TEID	GTPv2 Client->Srv S5/S8 SGW GTPU TEID
   [NFv9 57929][IPFIX 35632.457][Len 4] %GTPV2_S2C_S5_S8_SGW_GTPU_TEID	GTPv2 Srv->Client S5/S8 SGW GTPU TEID
   [NFv9 57930][IPFIX 35632.458][Len 4] %GTPV2_C2S_S5_S8_SGW_GTPU_IP	GTPv2 Client->Srv S5/S8 SGW GTPU IP
   [NFv9 57931][IPFIX 35632.459][Len 4] %GTPV2_S2C_S5_S8_SGW_GTPU_IP	GTPv2 Srv->Client S5/S8 SGW GTPU IP

Plugin Radius Protocol templates:

.. code:: bash
	  
   [NFv9 57712][IPFIX 35632.240][Len 1] %RADIUS_REQ_MSG_TYPE       	RADIUS Request Msg Type
   [NFv9 57713][IPFIX 35632.241][Len 1] %RADIUS_RSP_MSG_TYPE       	RADIUS Response Msg Type
   [NFv9 57714][IPFIX 35632.242][Len 32 varlen] %RADIUS_USER_NAME          	RADIUS User Name (Access Only)
   [NFv9 57715][IPFIX 35632.243][Len 32 varlen] %RADIUS_CALLING_STATION_ID 	RADIUS Calling Station Id
   [NFv9 57716][IPFIX 35632.244][Len 32 varlen] %RADIUS_CALLED_STATION_ID  	RADIUS Called Station Id
   [NFv9 57717][IPFIX 35632.245][Len 4] %RADIUS_NAS_IP_ADDR        	RADIUS NAS IP Address
   [NFv9 57718][IPFIX 35632.246][Len 24 varlen] %RADIUS_NAS_IDENTIFIER     	RADIUS NAS Identifier
   [NFv9 57719][IPFIX 35632.247][Len 16] %RADIUS_USER_IMSI          	RADIUS User IMSI (Extension)
   [NFv9 57720][IPFIX 35632.248][Len 16] %RADIUS_USER_IMEI          	RADIUS User MSISDN (Extension)
   [NFv9 57721][IPFIX 35632.249][Len 4] %RADIUS_FRAMED_IP_ADDR     	RADIUS Framed IP
   [NFv9 57722][IPFIX 35632.250][Len 24 varlen] %RADIUS_ACCT_SESSION_ID    	RADIUS Accounting Session Name
   [NFv9 57723][IPFIX 35632.251][Len 1] %RADIUS_ACCT_STATUS_TYPE   	RADIUS Accounting Status Type
   [NFv9 57724][IPFIX 35632.252][Len 4] %RADIUS_ACCT_IN_OCTETS     	RADIUS Accounting Input Octets
   [NFv9 57725][IPFIX 35632.253][Len 4] %RADIUS_ACCT_OUT_OCTETS    	RADIUS Accounting Output Octets
   [NFv9 57726][IPFIX 35632.254][Len 4] %RADIUS_ACCT_IN_PKTS       	RADIUS Accounting Input Packets
   [NFv9 57727][IPFIX 35632.255][Len 4] %RADIUS_ACCT_OUT_PKTS      	RADIUS Accounting Output Packets

Plugin Diameter Protocol templates:

.. code:: bash
	  
   [NFv9 57871][IPFIX 35632.399][Len 4] %DIAMETER_REQ_MSG_TYPE     	DIAMETER Request Msg Type
   [NFv9 57872][IPFIX 35632.400][Len 4] %DIAMETER_RSP_MSG_TYPE     	DIAMETER Response Msg Type
   [NFv9 57873][IPFIX 35632.401][Len 64 varlen] %DIAMETER_REQ_ORIGIN_HOST  	DIAMETER Origin Host Request
   [NFv9 57874][IPFIX 35632.402][Len 64 varlen] %DIAMETER_RSP_ORIGIN_HOST  	DIAMETER Origin Host Response
   [NFv9 57875][IPFIX 35632.403][Len 64 varlen] %DIAMETER_REQ_USER_NAME    	DIAMETER Request User Name
   [NFv9 57876][IPFIX 35632.404][Len 4] %DIAMETER_RSP_RESULT_CODE  	DIAMETER Response Result Code
   [NFv9 57877][IPFIX 35632.405][Len 4] %DIAMETER_EXP_RES_VENDOR_ID	DIAMETER Response Experimental Result Vendor Id
   [NFv9 57878][IPFIX 35632.406][Len 4] %DIAMETER_EXP_RES_RESULT_CODE	DIAMETER Response Experimental Result Code
   [NFv9 57917][IPFIX 35632.445][Len 4] %DIAMETER_HOP_BY_HOP_ID    	DIAMETER Hop by Hop Identifier
   [NFv9 57924][IPFIX 35632.452][Len 4] %DIAMETER_CLR_CANCEL_TYPE  	DIAMETER Cancellation Type
   [NFv9 57925][IPFIX 35632.453][Len 4] %DIAMETER_CLR_FLAGS        	DIAMETER CLR Flags
   [NFv9 57733][IPFIX 35632.261][Len 4] %DIAMETER_FRAMED_IP_ADDR   	DIAMETER Framed IP
   [NFv9 57734][IPFIX 35632.262][Len 4] %DIAMETER_SERVED_IP_ADDR   	DIAMETER Served IP
   [NFv9 57740][IPFIX 35632.268][Len 4] %DIAMETER_PDP_ADDR         	DIAMETER PDP ADRRESS IP
   [NFv9 57735][IPFIX 35632.263][Len 32 varlen] %DIAMETER_CALLING_STATION_ID	DIAMETER Calling Station Id
   [NFv9 57736][IPFIX 35632.264][Len 32 varlen] %DIAMETER_CALLED_STATION_ID	DIAMETER Called Station Id
   [NFv9 57737][IPFIX 35632.265][Len 64 varlen] %DIAMETER_SUBSCRIPTION_ID  	DIAMETER Subscription Id
   [NFv9 57738][IPFIX 35632.266][Len 64 varlen] %DIAMETER_CALLING_PARTY_ADDRESS	DIAMETER Calling Party Address
   [NFv9 57739][IPFIX 35632.267][Len 64 varlen] %DIAMETER_CALLED_PARTY_ADDRESS	DIAMETER Called Party Address

Plugin NETBIOS Protocol templates:

.. code:: bash
	  
   [NFv9 57936][IPFIX 35632.464][Len 48 varlen] %NETBIOS_QUERY_NAME        	NETBIOS Query Name
   [NFv9 57937][IPFIX 35632.465][Len 64 varlen] %NETBIOS_QUERY_TYPE        	NETBIOS Query Type
   [NFv9 57938][IPFIX 35632.466][Len 64 varlen] %NETBIOS_RESPONSE          	NETBIOS Query Response
   [NFv9 57939][IPFIX 35632.467][Len 24 varlen] %NETBIOS_QUERY_OS          	NETBIOS Query OS

Plugin SSDP Protocol templates:

.. code:: bash
	  
   [NFv9 57934][IPFIX 35632.462][Len 48 varlen] %SSDP_HOST                 	SSDP Host
   [NFv9 57935][IPFIX 35632.463][Len 64 varlen] %SSDP_USN                  	SSDP USN
   [NFv9 57940][IPFIX 35632.468][Len 64 varlen] %SSDP_SERVER               	SSDP Server
   [NFv9 57941][IPFIX 35632.469][Len 64 varlen] %SSDP_TYPE                 	SSDP Type
   [NFv9 57942][IPFIX 35632.470][Len 8 varlen] %SSDP_METHOD               	SSDP Method

Plugin DHCP Protocol templates:

.. code:: bash
	  
   [NFv9 57825][IPFIX 35632.353][Len 6] %DHCP_CLIENT_MAC           	MAC of the DHCP client
   [NFv9 57826][IPFIX 35632.354][Len 4] %DHCP_CLIENT_IP            	DHCP assigned client IPv4 address
   [NFv9 57827][IPFIX 35632.355][Len 64 varlen] %DHCP_CLIENT_NAME          	DHCP client name
   [NFv9 57895][IPFIX 35632.423][Len 32 varlen] %DHCP_REMOTE_ID            	DHCP agent remote Id
   [NFv9 57896][IPFIX 35632.424][Len 48 varlen] %DHCP_SUBSCRIBER_ID        	DHCP subscribed Id
   [NFv9 57901][IPFIX 35632.429][Len 1] %DHCP_MESSAGE_TYPE         	DHCP message type

Plugin IMAP Protocol templates:

.. code:: bash
	  
   [NFv9 57732][IPFIX 35632.260][Len 64 varlen] %IMAP_LOGIN                	Mail sender

Plugin POP3 Protocol templates:

.. code:: bash
	  
   [NFv9 57682][IPFIX 35632.210][Len 64 varlen] %POP_USER                  	POP3 user login

Plugin MySQL Plugin templates:

.. code:: bash
	  
   [NFv9 57667][IPFIX 35632.195][Len 16] %MYSQL_SERVER_VERSION      	MySQL server version
   [NFv9 57668][IPFIX 35632.196][Len 16] %MYSQL_USERNAME            	MySQL username
   [NFv9 57669][IPFIX 35632.197][Len 64] %MYSQL_DB                  	MySQL database in use
   [NFv9 57670][IPFIX 35632.198][Len 128 varlen] %MYSQL_QUERY               	MySQL Query
   [NFv9 57671][IPFIX 35632.199][Len 2] %MYSQL_RESPONSE            	MySQL server response
   [NFv9 57792][IPFIX 35632.320][Len 4] %MYSQL_APPL_LATENCY_USEC   	MySQL request->response latecy (usec)


The default template (if -T is omitted) is:

%IPV4_SRC_ADDR %IPV4_DST_ADDR %INPUT_SNMP %OUTPUT_SNMP %IN_PKTS %IN_BYTES %FIRST_SWITCHED %LAST_SWITCHED %L4_SRC_PORT %L4_DST_PORT %TCP_FLAGS %PROTOCOL %SRC_TOS %SRC_AS %DST_AS

Application Protocols
~~~~~~~~~~~~~~~~~~~~~

Major protocol (%L7_PROTO) symbolic mapping:

.. code:: bash

  0 Unknown                TCP      Unrated      Unspecified
  1 FTP_CONTROL            TCP      Unsafe       Download
  2 POP3                   TCP      Unsafe       Email
  3 SMTP                   TCP      Acceptable   Email
  4 IMAP                   TCP      Unsafe       Email
  5 DNS                    TCP/UDP  Acceptable   Network
  6 IPP                    TCP/UDP  Acceptable   System
  7 HTTP                   TCP      Acceptable   Web
  8 MDNS                   TCP      Acceptable   Network
  9 NTP                    UDP      Acceptable   System
 10 NetBIOS                TCP/UDP  Acceptable   System
 11 NFS                    TCP/UDP  Acceptable   DataTransfer
 12 SSDP                   UDP      Acceptable   System
 13 BGP                    TCP      Acceptable   Network
 14 SNMP                   UDP      Acceptable   Network
 15 XDMCP                  TCP/UDP  Acceptable   RemoteAccess
 16 SMBv1                  TCP      Dangerous    System
 17 Syslog                 TCP/UDP  Acceptable   System
 18 DHCP                   UDP      Acceptable   Network
 19 PostgreSQL             TCP      Acceptable   Database
 20 MySQL                  TCP      Acceptable   Database
 21 Hotmail                TCP      Acceptable   Email
 22 Direct_Download_Link   TCP      Potentially Dangerous Download
 23 POPS                   TCP      Safe         Email
 24 AppleJuice             TCP      Potentially Dangerous Download
 25 DirectConnect          TCP/UDP  Potentially Dangerous Download
 26 ntop                   TCP      Safe         Network
 27 COAP                   UDP      Safe         RPC
 28 VMware                 UDP      Acceptable   RemoteAccess
 29 SMTPS                  TCP      Safe         Email
 30 DTLS                   UDP      Safe         Web
 31 UBNTAC2                UDP      Safe         Network
 32 Kontiki                UDP      Potentially Dangerous Media
 33 OpenFT                 TCP      Potentially Dangerous Download
 34 FastTrack              TCP      Potentially Dangerous Download
 35 Gnutella               TCP/UDP  Potentially Dangerous Download
 36 eDonkey                TCP/UDP  Unsafe       Download
 37 BitTorrent             TCP/UDP  Acceptable   Download
 38 SkypeCall              TCP      Acceptable   VoIP
 39 Signal                 TCP      Fun          Chat
 40 Memcached              TCP/UDP  Acceptable   Network
 41 SMBv23                 TCP      Acceptable   System
 42 Mining                 UDP      Unsafe       Mining
 43 NestLogSink            TCP      Acceptable   Cloud
 44 Modbus                 TCP      Acceptable   IoT-Scada
 45 WhatsAppCall           TCP      Acceptable   VoIP
 46 DataSaver              TCP      Fun          Web
 47 Xbox                   UDP      Fun          Game
 48 QQ                     UDP      Fun          Chat
 49 TikTok                 TCP      Fun          SocialNetwork
 50 RTSP                   TCP/UDP  Fun          Media
 51 IMAPS                  TCP      Safe         Email
 52 IceCast                TCP      Fun          Media
 53 CPHA                   UDP      Fun          Network
 54 PPStream               TCP/UDP  Fun          Streaming
 55 Zattoo                 TCP/UDP  Fun          Video
 56 ShoutCast              TCP      Fun          Music
 57 Sopcast                TCP/UDP  Fun          Video
 58 Discord                TCP      Fun          Collaborative
 59 TVUplayer              TCP/UDP  Fun          Video
 60 MongoDB                TCP      Acceptable   Database
 61 QQLive                 TCP      Fun          Video
 62 Thunder                TCP/UDP  Fun          Download
 63 Soulseek               TCP      Fun          Download
 64 PS_VUE                 TCP      Acceptable   Video
 65 IRC                    TCP      Unsafe       Chat
 66 Ayiya                  UDP      Acceptable   Network
 67 Jabber                 TCP/UDP  Acceptable   Web
 68 Nats                   TCP      Acceptable   RPC
 69 AmongUs                UDP      Fun          Game
 70 Yahoo                  TCP      Safe         Web
 71 DisneyPlus             TCP      Fun          Streaming
 72 GooglePlus             TCP      Fun          SocialNetwork
 73 VRRP                   TCP      Acceptable   Network
 74 Steam                  TCP/UDP  Fun          Game
 75 HalfLife2              UDP      Fun          Game
 76 WorldOfWarcraft        TCP      Fun          Game
 77 Telnet                 TCP      Unsafe       RemoteAccess
 78 STUN                   UDP      Acceptable   Network
 79 IPsec                           Safe         VPN
 80 GRE                             Acceptable   Network
 81 ICMP                            Acceptable   Network
 82 IGMP                            Acceptable   Network
 83 EGP                             Acceptable   Network
 84 SCTP                            Acceptable   Network
 85 OSPF                            Acceptable   Network
 86 IP_in_IP                        Acceptable   Network
 87 RTP                    UDP      Acceptable   Media
 88 RDP                    TCP      Acceptable   RemoteAccess
 89 VNC                    TCP      Acceptable   RemoteAccess
 90 Tumblr                 TCP      Fun          SocialNetwork
 91 TLS                    TCP      Safe         Web
 92 SSH                    TCP      Acceptable   RemoteAccess
 93 Usenet                 TCP      Acceptable   Web
 94 MGCP                   UDP      Acceptable   VoIP
 95 IAX                    UDP      Acceptable   VoIP
 96 TFTP                   UDP      Acceptable   DataTransfer
 97 AFP                    TCP      Acceptable   DataTransfer
 98 Stealthnet             TCP      Potentially Dangerous Download
 99 Aimini                 TCP/UDP  Fun          Download
 100 SIP                    TCP/UDP  Acceptable   VoIP
 101 TruPhone               TCP      Acceptable   VoIP
 102 ICMPV6                          Acceptable   Network
 103 DHCPV6                 UDP      Acceptable   Network
 104 Armagetron             UDP      Fun          Game 
 105 Crossfire              TCP/UDP  Fun          RPC
 106 Dofus                  TCP      Fun          Game
 107 Fiesta                 TCP      Fun          Game
 108 Florensia              TCP/UDP  Fun          Game
 109 Guildwars              TCP      Fun          Game
 110 AmazonAlexa            TCP      Acceptable   VirtAssistant
 111 Kerberos               TCP/UDP  Acceptable   Network
 112 LDAP                   TCP/UDP  Acceptable   System
 113 MapleStory             TCP      Fun          Game
 114 MsSQL-TDS              TCP      Acceptable   Database
 115 PPTP                   TCP      Acceptable   VPN
 116 Warcraft3              TCP/UDP  Fun          Game
 117 WorldOfKungFu          TCP      Fun          Game
 118 Slack                  TCP      Acceptable   Collaborative
 119 Facebook               TCP      Fun          SocialNetwork
 120 Twitter                TCP      Fun          SocialNetwork
 121 Dropbox                UDP      Acceptable   Cloud
 122 GMail                  TCP      Acceptable   Email
 123 GoogleMaps             TCP      Safe         Web
 124 YouTube                TCP      Fun          Media
 125 Skype_Teams            TCP/UDP  Acceptable   VoIP
 126 Google                 TCP      Tracker/Ads  Web
 127 DCE_RPC                TCP/UDP  Acceptable   RPC
 128 NetFlow                UDP      Acceptable   Network
 129 sFlow                  UDP      Acceptable   Network
 130 HTTP_Connect           TCP      Acceptable   Web
 131 HTTP_Proxy             TCP      Acceptable   Web
 132 Citrix                 TCP      Acceptable   Network
 133 NetFlix                TCP      Fun          Video
 134 LastFM                 TCP      Fun          Music
 135 Waze                   TCP      Acceptable   Web
 136 YouTubeUpload          TCP      Fun          Media
 137 Hulu                   TCP      Fun          Streaming
 138 CHECKMK                TCP      Acceptable   DataTransfer
 139 AJP                    TCP      Acceptable   Web
 140 Apple                  TCP      Safe         Web
 141 Webex                  TCP      Acceptable   VoIP
 142 WhatsApp               TCP      Acceptable   Chat
 143 AppleiCloud            TCP      Acceptable   Web
 144 Viber                  UDP      Acceptable   VoIP
 145 AppleiTunes            TCP      Fun          Streaming
 146 Radius                 UDP      Acceptable   Network
 147 WindowsUpdate          TCP      Safe         SoftwareUpdate
 148 TeamViewer             TCP/UDP  Acceptable   RemoteAccess
 149 Tuenti                 TCP      Acceptable   VoIP
 150 LotusNotes             TCP      Acceptable   Collaborative
 151 SAP                    TCP      Acceptable   Network
 152 GTP                    UDP      Acceptable   Network
 153 WSD                    TCP      Acceptable   Network
 154 LLMNR                  TCP      Acceptable   Network
 155 RemoteScan             TCP      Potentially Dangerous Network
 156 Spotify                TCP/UDP  Acceptable   Music
 157 Messenger              TCP      Acceptable   VoIP
 158 H323                   TCP/UDP  Acceptable   VoIP
 159 OpenVPN                TCP/UDP  Acceptable   VPN
 160 NOE                    TCP/UDP  Acceptable   VoIP
 161 CiscoVPN               TCP/UDP  Acceptable   VPN
 162 TeamSpeak              TCP/UDP  Acceptable   VoIP
 163 Tor                    TCP      Potentially Dangerous VPN
 164 CiscoSkinny            TCP      Acceptable   VoIP
 165 RTCP                   TCP/UDP  Acceptable   VoIP
 166 RSYNC                  TCP      Acceptable   DataTransfer
 167 Oracle                 TCP      Acceptable   Database
 168 Corba                  TCP      Acceptable   RPC
 169 UbuntuONE              TCP      Acceptable   Cloud
 170 Whois-DAS              TCP      Acceptable   Network
 171 Collectd               TCP      Acceptable   System
 172 SOCKS                  TCP      Acceptable   Web
 173 Nintendo               UDP      Fun          Game
 174 RTMP                   TCP      Acceptable   Media
 175 FTP_DATA               TCP      Acceptable   Download
 176 Wikipedia              TCP      Safe         Web
 177 ZeroMQ                 TCP      Acceptable   RPC
 178 Amazon                 TCP      Acceptable   Web
 179 eBay                   TCP      Safe         Shopping
 180 CNN                    TCP      Safe         Web
 181 Megaco                 UDP      Acceptable   VoIP
 182 Redis                  TCP      Acceptable   Database
 183 Pinterest              TCP      Fun          SocialNetwork
 184 VHUA                   UDP      Fun          VoIP
 185 Telegram               TCP/UDP  Acceptable   Chat
 186 Vevo                   TCP      Fun          Music
 187 Pandora                TCP      Fun          Streaming
 188 QUIC                   UDP      Acceptable   Web
 189 Zoom                   TCP      Acceptable   Video
 190 EAQ                    UDP      Acceptable   Network
 191 Ookla                  TCP/UDP  Safe         Network
 192 AMQP                   TCP      Acceptable   RPC
 193 KakaoTalk              TCP      Acceptable   Chat
 194 KakaoTalk_Voice        UDP      Acceptable   VoIP
 195 Twitch                 TCP      Fun          Video
 196 DoH_DoT                TCP      Fun          Network
 197 WeChat                 TCP      Fun          Chat
 198 MPEG_TS                UDP      Fun          Media
 199 Snapchat               TCP      Fun          SocialNetwork
 200 Sina(Weibo)            TCP      Fun          SocialNetwork
 201 GoogleHangoutDuo       TCP/UDP  Acceptable   VoIP
 202 IFLIX                  TCP      Fun          Video
 203 Github                 TCP      Acceptable   Collaborative
 204 BJNP                   UDP      Acceptable   System
 205 Reddit                 TCP      Fun          SocialNetwork
 206 WireGuard              UDP      Acceptable   VPN
 207 SMPP                   TCP      Acceptable   Download
 208 DNScrypt               TCP/UDP  Safe         Network
 209 TINC                   TCP/UDP  Acceptable   VPN
 210 Deezer                 TCP      Fun          Music
 211 Instagram              TCP      Fun          SocialNetwork
 212 Microsoft              TCP      Safe         Cloud
 213 Starcraft              TCP/UDP  Fun          Game
 214 Teredo                 UDP      Acceptable   Network
 215 HotspotShield          TCP      Potentially Dangerous VPN
 216 IMO                    UDP      Acceptable   VoIP
 217 GoogleDrive            TCP      Acceptable   Cloud
 218 OCS                    TCP      Fun          Media
 219 Microsoft365           TCP      Acceptable   Collaborative
 220 Cloudflare             TCP      Acceptable   Web
 221 MS_OneDrive            TCP      Acceptable   Cloud
 222 MQTT                   TCP      Acceptable   RPC
 223 RX                     UDP      Acceptable   RPC
 224 AppleStore             TCP      Safe         SoftwareUpdate
 225 OpenDNS                TCP      Acceptable   Web
 226 Git                    TCP      Safe         Collaborative
 227 DRDA                   TCP      Acceptable   Database
 228 PlayStore              TCP      Safe         SoftwareUpdate
 229 SOMEIP                 TCP/UDP  Acceptable   RPC
 230 FIX                    TCP      Safe         RPC
 231 Playstation            TCP      Fun          Game
 232 Pastebin               TCP      Potentially Dangerous Download
 233 LinkedIn               TCP      Fun          SocialNetwork
 234 SoundCloud             TCP      Fun          Music
 235 CSGO                   UDP      Fun          Game
 236 LISP                   UDP      Acceptable   Cloud
 237 Diameter               UDP      Acceptable   Network
 238 ApplePush              TCP      Acceptable   Cloud
 239 GoogleServices         TCP      Acceptable   Web
 240 AmazonVideo            TCP/UDP  Acceptable   Cloud
 241 GoogleDocs             TCP      Acceptable   Collaborative
 242 WhatsAppFiles          TCP      Acceptable   Download
 243 TargusDataspeed        TCP/UDP  Acceptable   Network
 244 DNP3                   TCP      Acceptable   IoT-Scada
 245 IEC60870               TCP      Acceptable   IoT-Scada
 246 Bloomberg              TCP      Acceptable   Network
 247 CAPWAP                 UDP      Acceptable   Network
 248 Zabbix                 TCP      Acceptable   Network
 249 s7comm                 TCP      Acceptable   Network
 250 Teams                  TCP      Safe         Collaborative
 251 WebSocket              TCP      Acceptable   Web
 252 AnyDesk                TCP      Acceptable   RemoteAccess
 253 SOAP                   TCP      Acceptable   RPC
 254 AppleSiri              TCP      Acceptable   VirtAssistant
 255 SnapchatCall           TCP      Acceptable   VoIP
 256 HP Virtual Machine Group Management TCP      Acceptable   Network
 257 GenshinImpact          UDP      Fun          Game
 258 Activision             TCP      Fun          Game
 259 FortiClient            TCP      Safe         VPN
 260 Z39.50                 TCP      Acceptable   Network
 261 Likee                  TCP      Fun          SocialNetwork
 262 GitLab                 TCP      Fun          Collaborative
 263 AVAST SecureDNS        UDP      Safe         Network
 264 Cassandra              TCP      Acceptable   Database
 265 AmazonAWS              TCP      Acceptable   Cloud
 266 Salesforce             TCP      Safe         Cloud
 267 Vimeo                  TCP      Fun          Streaming
 268 FacebookVoip           TCP      Acceptable   VoIP
 269 SignalVoip             TCP      Acceptable   VoIP
 270 Fuze                   TCP      Acceptable   VoIP
 271 GTP_U                  TCP      Acceptable   Network
 272 GTP_C                  TCP      Acceptable   Network
 273 GTP_PRIME              TCP      Acceptable   Network
 274 Alibaba                TCP      Acceptable   Web
 275 Crashlytics            TCP      Acceptable   DataTransfer
 276 Azure                  TCP      Acceptable   Cloud
 277 iCloudPrivateRelay     TCP      Acceptable   VPN
 278 EthernetIP             TCP      Acceptable   Network
 279 Badoo                  TCP      Fun          SocialNetwork
 280 AccuWeather            TCP      Fun          Web
 281 GoogleClassroom        TCP      Safe         Collaborative
 282 HSRP                   UDP      Acceptable   Network
 283 Cybersec               TCP      Safe         Cybersecurity
 284 GoogleCloud            TCP      Acceptable   Cloud
 285 Tencent                TCP      Acceptable   SocialNetwork
 286 RakNet                 UDP      Acceptable   Game
 287 Xiaomi                 TCP      Acceptable   Web
 288 Edgecast               TCP      Acceptable   Cloud
 289 Cachefly               TCP      Acceptable   Cloud
 290 Softether              TCP      Acceptable   VPN
 291 MpegDash               TCP      Acceptable   Media
 292 Dazn                   TCP      Fun          Streaming
 293 GoTo                   TCP      Acceptable   VoIP
 294 RSH                    TCP      Unsafe       RemoteAccess
 295 1kxun                  TCP      Fun          Streaming

Usage examples
~~~~~~~~~~~~~~

1. Capture packets on eth0, and export them towards collector running at 192.168.2.25:2055

.. code:: bash

   nprobe -i eth0 -n 192.168.2.25:2055

2. Collect flows on port 9995 and export them in IPFIX format towards collector running at 192.168.2.25:2055

.. code:: bash

   nprobe -i none -3 9995 -V 10 -n 192.168.2.25:2055

3. Capture packets on eth0, and export them towards ntopng running on local host

.. code:: bash

   nprobe -i eth0 -n none -T "@NTOPNG@" --zmq tcp://127.0.0.1:1234

On 192.168.2.25:
  
.. code:: bash
  
   ntopng -itcp://127.0.01:1234

