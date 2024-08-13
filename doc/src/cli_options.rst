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

:code:`--ntopng <option>`

      You can use this option to instruct nProbe to send data towards ntopng using ZMQ (available on all platforms) or Kafka (available on selected platforms).
      When ZMQ is used you can specy :code:`--ntopng zmq://<socket>:<port>` to deliver flows to ntopng connected to the specified ZMQ endpoint (see also the old option :code:`--zmq`). When more than one endpoint is defined, a hash function is used to evenly balance the flows among them.
      Example: :code:`--ntopng zmq://*:5556` or :code:`--ntopng zmq://127.0.0.1:1234`

      When Kafka is used the syntax is :code:`--ntopng kafka://<brokers>` so that you can deliver flows to ntopng connected to the specified Kafka broker in plaintext. Instead you can use :code:`--ntopng kafka-ssl://<brokers>` to deliver data in TLS/SSL. Kafka brokers are comma separated (if more than one is defined).
      Examples: :code:`--ntopng kafka://192.168.1.2` or :code:`--ntopng kafka-ssl://192.168.1.2,172.16.24.12`.

:code:`--zmq <socket>`

      Specify a socket (e.g., :code:`tcp://\*:5556`) that will be used to deliver flows to subscribers polling the socket. Up to 8 ZMQ endpoints can be specified by repeating the --zmq. When more than one endpoint is specified, nProbe uses an hash function to evenly balance flows among all the defined endpoints. Please note that this option is an alias for :code:`--ntopng` and it might be removed in future versions. Example:

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
      <host1>[:<port1>],<host2>[:<port2>]... Initial brokers list used to receive metadata information.
      Note that you can specify multiple :code:`--kafka` options and exported data
      will be sent to all configured brokers.

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
   [  1][Len 4] %IN_BYTES                   %octetDeltaCount               Incoming flow bytes (src->dst) [Aliased to %SRC_TO_DST_BYTES]
   [  2][Len 4] %IN_PKTS                    %packetDeltaCount              Incoming flow packets (src->dst) [Aliased to %SRC_TO_DST_PKTS]
   [  4][Len 1] %PROTOCOL                   %protocolIdentifier            IP protocol byte
   [NFv9 58500][IPFIX 35632.1028][Len 16] %PROTOCOL_MAP                    IP protocol name
   [  5][Len 1] %SRC_TOS                    %ipClassOfService              TOS/DSCP (src->dst)
   [  6][Len 1] %TCP_FLAGS                  %tcpControlBits                Cumulative of all flow TCP flags
   [  7][Len 2] %L4_SRC_PORT                %sourceTransportPort           IPv4 source port
   [NFv9 58503][IPFIX 35632.1031][Len 16] %L4_SRC_PORT_MAP                 Layer 4 source port symbolic name
   [  8][Len 4] %IPV4_SRC_ADDR              %sourceIPv4Address             IPv4 source address
   [  9][Len 1] %IPV4_SRC_MASK              %sourceIPv4PrefixLength        IPv4 source subnet mask (/<bits>)
   [ 10][Len 4] %INPUT_SNMP                 %ingressInterface              Input interface SNMP idx
   [ 11][Len 2] %L4_DST_PORT                %destinationTransportPort      IPv4 destination port
   [NFv9 58507][IPFIX 35632.1035][Len 16] %L4_DST_PORT_MAP                 Layer 4 destination port symbolic name
   [NFv9 58508][IPFIX 35632.1036][Len 2] %L4_SRV_PORT                      Layer 4 server port
   [NFv9 58509][IPFIX 35632.1037][Len 16] %L4_SRV_PORT_MAP                 Layer 4 server port symbolic name
   [ 12][Len 4] %IPV4_DST_ADDR              %destinationIPv4Address        IPv4 destination address
   [ 13][Len 1] %IPV4_DST_MASK              %destinationIPv4PrefixLength   IPv4 dest subnet mask (/<bits>)
   [ 14][Len 4] %OUTPUT_SNMP                %egressInterface               Output interface SNMP idx
   [ 15][Len 4] %IPV4_NEXT_HOP              %ipNextHopIPv4Address          IPv4 next hop address
   [ 16][Len 4] %SRC_AS                     %bgpSourceAsNumber             Source BGP AS
   [ 17][Len 4] %DST_AS                     %bgpDestinationAsNumber        Destination BGP AS
   [129][Len 4] %BGP_PREV_ADJACENT_ASN      %bgpNextAdjacentAsNumber       Source BGP Prev AS
   [128][Len 4] %BGP_NEXT_ADJACENT_ASN      %bgpPrevAdjacentAsNumber       Destination BGP Next AS
   [ 18][Len 4] %IPV4_BGP_NEXT_HOP          %bgpNexthopIPv4Address
   [ 21][Len 4] %LAST_SWITCHED              %flowEndSysUpTime              SysUptime (msec) of the last flow pkt
   [ 22][Len 4] %FIRST_SWITCHED             %flowStartSysUpTime            SysUptime (msec) of the first flow pkt
   [ 23][Len 4] %OUT_BYTES                  %postOctetDeltaCount           Outgoing flow bytes (dst->src) [Aliased to %DST_TO_SRC_BYTES]
   [ 24][Len 4] %OUT_PKTS                   %postPacketDeltaCount          Outgoing flow packets (dst->src) [Aliased to %DST_TO_SRC_PKTS]
   [ 25][Len 2] %MIN_IP_PKT_LEN             %minimumIpTotalLength          Len of the smallest flow IP packet observed
   [ 26][Len 2] %MAX_IP_PKT_LEN             %maximumIpTotalLength          Len of the largest flow IP packet observed
   [ 27][Len 16] %IPV6_SRC_ADDR              %sourceIPv6Address            IPv6 source address
   [ 28][Len 16] %IPV6_DST_ADDR              %destinationIPv6Address       IPv6 destination address
   [ 29][Len 1] %IPV6_SRC_MASK              %sourceIPv6PrefixLength        IPv6 source mask
   [ 30][Len 1] %IPV6_DST_MASK              %destinationIPv6PrefixLength   IPv6 destination mask
   [ 32][Len 2] %ICMP_TYPE                  %icmpTypeCodeIPv4              ICMP Type * 256 + ICMP code
   [ 34][Len 4] %SAMPLING_INTERVAL                                         Sampling rate
   [ 35][Len 1] %SAMPLING_ALGORITHM                                        Sampling type (deterministic/random)
   [ 36][Len 2] %FLOW_ACTIVE_TIMEOUT        %flowActiveTimeout             Activity timeout of flow cache entries
   [ 37][Len 2] %FLOW_INACTIVE_TIMEOUT      %flowIdleTimeout               Inactivity timeout of flow cache entries
   [ 38][Len 1] %ENGINE_TYPE                                               Flow switching engine
   [ 39][Len 1] %ENGINE_ID                                                 Id of the flow switching engine
   [ 40][Len 4] %TOTAL_BYTES_EXP            %exportedOctetTotalCount       Total bytes exported
   [ 41][Len 4] %TOTAL_PKTS_EXP             %exportedMessageTotalCount     Total flow packets exported
   [ 42][Len 4] %TOTAL_FLOWS_EXP            %exportedFlowRecordTotalCount  Total number of exported flows
   [ 52][Len 1] %MIN_TTL                    %minimumTTL                    Min flow TTL
   [ 53][Len 1] %MAX_TTL                    %maximumTTL                    Max flow TTL
   [ 55][Len 1] %DST_TOS                    %ipClassOfService              TOS/DSCP (dst->src)
   [ 58][Len 2] %SRC_VLAN                   %vlanId                        Source VLAN (inner VLAN in QinQ)
   [ 59][Len 2] %DST_VLAN                   %postVlanId                    Destination VLAN (inner VLAN in QinQ)
   [ 56][Len 6] %IN_SRC_MAC                 %sourceMacAddress              Source MAC Address
   [ 57][Len 6] %OUT_DST_MAC                %postDestinationMacAddress     Post Destination MAC Address
   [ 80][Len 6] %IN_DST_MAC                 %destinationMacAddress         Destination MAC Address
   [ 81][Len 6] %OUT_SRC_MAC                %postSourceMacAddress          Post Source MAC Address
   [ 82][Len 8] %INTERFACE_NAME             %interfaceName                 Interface you are capturing from (-i)
   [ 85][Len 8] %OCTET_TOTAL                %octetTotalCount               Total flow bytes [Aliased to %OCTETS_TOTAL]
   [ 86][Len 8] %PACKET_TOTAL               %packetTotalCount              Total flow packets [Aliased to %PACKETS_TOTAL]
   [ 89][Len 1] %FORWARDING_STATUS          %forwardingStatus              Forwarding status of the flow
   [243][Len 2] %DOT1Q_SRC_VLAN             %dot1qVlanId                   Source VLAN (outer VLAN in QinQ)
   [254][Len 2] %DOT1Q_DST_VLAN             %postdot1qVlanId               Destination VLAN (outer VLAN in QinQ)
   [ 60][Len 1] %IP_PROTOCOL_VERSION        %ipVersion                     [4=IPv4][6=IPv6]
   [ 61][Len 1] %DIRECTION                  %flowDirection                 Flow direction [0=RX, 1=TX]
   [ 62][Len 16] %IPV6_NEXT_HOP              %ipNextHopIPv6Address         IPv6 next hop address
   [ 70][Len 3] %MPLS_LABEL_1               %mplsTopLabelStackSection      MPLS label at position 1
   [ 71][Len 3] %MPLS_LABEL_2               %mplsLabelStackSection2        MPLS label at position 2
   [ 72][Len 3] %MPLS_LABEL_3               %mplsLabelStackSection3        MPLS label at position 3
   [ 73][Len 3] %MPLS_LABEL_4               %mplsLabelStackSection4        MPLS label at position 4
   [ 74][Len 3] %MPLS_LABEL_5               %mplsLabelStackSection5        MPLS label at position 5
   [ 75][Len 3] %MPLS_LABEL_6               %mplsLabelStackSection6        MPLS label at position 6
   [ 76][Len 3] %MPLS_LABEL_7               %mplsLabelStackSection7        MPLS label at position 7
   [ 77][Len 3] %MPLS_LABEL_8               %mplsLabelStackSection8        MPLS label at position 8
   [ 78][Len 3] %MPLS_LABEL_9               %mplsLabelStackSection9        MPLS label at position 9
   [ 79][Len 3] %MPLS_LABEL_10              %mplsLabelStackSection10       MPLS label at position 10
   [ 95][Len 4] %APPLICATION_ID             %application_id                Application Id
   [ 96][Len 16] %APPLICATION_NAME                                         Application Name
   [136][Len 1] %FLOW_END_REASON            %flowEndReason                 The reason for flow termination.
   [58051][Len 1] %FLOW_SOURCE                                             0=Packets, 1=NetFlow/IPFIX, 2=sFlow
   [57640][Len 4] %SRC_PROC_PID                                            Flow source proc PID
   [57641][Len 16] %SRC_PROC_NAME                                          Flow source proc name
   [57897][Len 4] %SRC_PROC_UID                                            Flow source proc userId
   [57844][Len 16] %SRC_PROC_USER_NAME                                     Flow source proc user name
   [58012][Len 16] %SRC_PROC_PKG_NAME                                      Flow source proc package name
   [58028][Len 32] %SRC_PROC_CMDLINE                                       Flow source proc cmdline args
   [58030][Len 16] %SRC_PROC_CONTAINER_ID                                  Flow source proc containerId
   [57846][Len 16] %SRC_FATHER_PROC_NAME                                   Flow src father proc name
   [58036][Len 4] %SRC_FATHER_PROC_UID                                     Flow src father proc UID
   [57845][Len 4] %SRC_FATHER_PROC_PID                                     Flow source father proc PID
   [58037][Len 16] %SRC_FATHER_PROC_USER_NAME                              Flow src father proc UID name
   [58033][Len 16] %SRC_FATHER_PROC_PKG_NAME                               Flow src father proc package name
   [57847][Len 4] %DST_PROC_PID                                            Flow dest proc PID
   [57848][Len 16] %DST_PROC_NAME                                          Flow dest proc name
   [57898][Len 4] %DST_PROC_UID                                            Flow dest proc userId
   [57849][Len 16] %DST_PROC_USER_NAME                                     Flow dest proc user name
   [58013][Len 16] %DST_PROC_PKG_NAME                                      Flow dest proc package name
   [58029][Len 32] %DST_PROC_CMDLINE                                       Flow dest proc cmdline args
   [58031][Len 16] %DST_PROC_CONTAINER_ID                                  Flow dest proc containerId
   [57850][Len 4] %DST_FATHER_PROC_PID                                     Flow dest father proc PID
   [57851][Len 16] %DST_FATHER_PROC_NAME                                   Flow dest father proc name
   [58039][Len 4] %DST_FATHER_PROC_UID                                     Flow dst father proc UID
   [58040][Len 16] %DST_FATHER_PROC_USER_NAME                              Flow dst father proc UID name
   [58035][Len 16] %DST_FATHER_PROC_PKG_NAME                               Flow dst father proc package name
   [102][Len 2] %PACKET_SECTION_OFFSET                                     Packet section offset
   [103][Len 2] %SAMPLED_PACKET_SIZE                                       Sampled packet size
   [104][Len 2] %SAMPLED_PACKET_ID                                         Sampled packet id
   [130][Len 4] %EXPORTER_IPV4_ADDRESS      %exporterIPv4Address           Flow exporter IPv4 Address
   [131][Len 16] %EXPORTER_IPV6_ADDRESS      %exporterIPv6Address          Flow exporter IPv6 Address
   [148][Len 4] %FLOW_ID                    %flowId                        Serial Flow Identifier
   [150][Len 4] %FLOW_START_SEC             %flowStartSeconds              Seconds (epoch) of the first flow packet
   [151][Len 4] %FLOW_END_SEC               %flowEndSeconds                Seconds (epoch) of the last flow packet
   [152][Len 8] %FLOW_START_MILLISECONDS    %flowStartMilliseconds         Msec (epoch) of the first flow packet
   [154][Len 8] %FLOW_START_MICROSECONDS    %flowStartMicroseconds         uSec (epoch) of the first flow packet
   [153][Len 8] %FLOW_END_MILLISECONDS      %flowEndMilliseconds           Msec (epoch) of the last flow packet
   [155][Len 8] %FLOW_END_MICROSECONDS      %flowEndMicroseconds           uSec (epoch) of the last flow packet
   [239][Len 1] %BIFLOW_DIRECTION           %biflow_direction              1=initiator, 2=reverseInitiator
   [225][Len 4] %POST_NAT_SRC_IPV4_ADDR     %postNatSourceIPv4Address      Post Nat Source IPv4 Address
   [226][Len 4] %POST_NAT_DST_IPV4_ADDR     %postNatDestinationIPv4Address Post Nat Destination IPv4 Address
   [227][Len 2] %POST_NAPT_SRC_TRANSPORT_PORT %postNaptSourceTransportPort Post Napt Source Transport Port
   [228][Len 2] %POST_NAPT_DST_TRANSPORT_PORT %postNaptDestinationTransportPort    Post Napt Destination Transport Port
   [229][Len 1] %NAT_ORIGINATING_ADDRESS_REALM %natOriginatingAddressRealm Nat Originating Address Realm
   [230][Len 1] %NAT_EVENT                  %natEvent                      Nat Event
   [233][Len 1] %FIREWALL_EVENT             %firewallEvent                 Flow events 0=ignore, 1=created, 2=deleted, 3=denied, 4=alert, 5=update
   [161][Len 4] %FLOW_DURATION_MILLISECONDS %flowDurationMilliseconds      Flow duration (msec)
   [162][Len 4] %FLOW_DURATION_MICROSECONDS %flowDurationMicroseconds      Flow duration (usec)
   [176][Len 1] %ICMP_IPV4_TYPE             %icmpTypeIPv4                  ICMP Type
   [177][Len 1] %ICMP_IPV4_CODE             %icmpCodeIPv4                  ICMP Code
   [277][Len 2] %OBSERVATION_POINT_TYPE                                    Observation point type
   [300][Len 2] %OBSERVATION_POINT_ID                                      Observation point id
   [302][Len 2] %SELECTOR_ID                                               Selector id
   [304][Len 2] %IPFIX_SAMPLING_ALGORITHM                                  Sampling algorithm
   [309][Len 2] %SAMPLING_SIZE                                             Number of packets to sample
   [310][Len 2] %SAMPLING_POPULATION                                       Sampling population
   [312][Len 2] %FRAME_LENGTH                                              Original L2 frame length
   [318][Len 2] %PACKETS_OBSERVED                                          Tot number of packets seen
   [319][Len 2] %PACKETS_SELECTED                                          Number of pkts selected for sampling
   [234][Len 4] %INGRESS_VRFID              %ingressVRFID                  Ingress VRF ID
   [235][Len 4] %EGRESS_VRFID               %egressVRFID                   Egress VRF ID
   [335][Len 2] %SELECTOR_NAME                                             Sampler name
   [361][Len 2] %PORT_RANGE_START           %portRangeStart                NAT port range start
   [362][Len 2] %PORT_RANGE_END             %portRangeEnd                  NAT port range end
   [NFv9 57552][IPFIX 35632.80][Len 2] %SRC_FRAGMENTS              Num fragmented packets src->dst
   [NFv9 57553][IPFIX 35632.81][Len 2] %DST_FRAGMENTS              Num fragmented packets dst->src
   [NFv9 57595][IPFIX 35632.123][Len 4] %CLIENT_NW_LATENCY_MS              Network TCP 3WH RTT/2 client <-> nprobe (msec)
   [NFv9 57596][IPFIX 35632.124][Len 4] %SERVER_NW_LATENCY_MS              Network TCP 3WH RTT/2 nprobe <-> server (msec)
   [NFv9 57550][IPFIX 35632.78][Len 1] %CLIENT_TCP_FLAGS           Cumulative of all client TCP flags
   [NFv9 57551][IPFIX 35632.79][Len 1] %SERVER_TCP_FLAGS           Cumulative of all server TCP flags
   [NFv9 57597][IPFIX 35632.125][Len 4] %APPL_LATENCY_MS                   Application latency (msec), a.k.a. server response time
   [NFv9 57943][IPFIX 35632.471][Len 4] %NPROBE_IPV4_ADDRESS               IPv4 address of the host were nProbe runs
   [NFv9 57554][IPFIX 35632.82][Len 4] %SRC_TO_DST_MAX_THROUGHPUT  Src to dst max thpt (bps)
   [NFv9 57555][IPFIX 35632.83][Len 4] %SRC_TO_DST_MIN_THROUGHPUT  Src to dst min thpt (bps)
   [NFv9 57556][IPFIX 35632.84][Len 4] %SRC_TO_DST_AVG_THROUGHPUT  Src to dst average thpt (bps)
   [NFv9 57557][IPFIX 35632.85][Len 4] %DST_TO_SRC_MAX_THROUGHPUT  Dst to src max thpt (bps)
   [NFv9 57558][IPFIX 35632.86][Len 4] %DST_TO_SRC_MIN_THROUGHPUT  Dst to src min thpt (bps)
   [NFv9 57559][IPFIX 35632.87][Len 4] %DST_TO_SRC_AVG_THROUGHPUT  Dst to src average thpt (bps)
   [NFv9 57995][IPFIX 35632.523][Len 4] %SRC_TO_DST_MAX_EST_THROUGHPUT     Src to dst max estimated TCP thpt (bps)
   [NFv9 57996][IPFIX 35632.524][Len 4] %DST_TO_SRC_MAX_EST_THROUGHPUT     Dst to src max estimated TCP thpt (bps)
   [NFv9 57560][IPFIX 35632.88][Len 4] %NUM_PKTS_UP_TO_128_BYTES   # packets whose IP size <= 128
   [NFv9 57561][IPFIX 35632.89][Len 4] %NUM_PKTS_128_TO_256_BYTES  # packets whose IP size > 128 and <= 256
   [NFv9 57562][IPFIX 35632.90][Len 4] %NUM_PKTS_256_TO_512_BYTES  # packets whose IP size > 256 and < 512
   [NFv9 57563][IPFIX 35632.91][Len 4] %NUM_PKTS_512_TO_1024_BYTES # packets whose IP size > 512 and < 1024
   [NFv9 57564][IPFIX 35632.92][Len 4] %NUM_PKTS_1024_TO_1514_BYTES        # packets whose IP size > 1024 and <= 1514
   [NFv9 57565][IPFIX 35632.93][Len 4] %NUM_PKTS_OVER_1514_BYTES   # packets whose IP size > 1514
   [NFv9 57570][IPFIX 35632.98][Len 4] %CUMULATIVE_ICMP_TYPE       Cumulative OR of ICMP type packets
   [NFv9 57573][IPFIX 35632.101][Len 2] %SRC_IP_COUNTRY                    Country where the src IP is located
   [NFv9 57574][IPFIX 35632.102][Len 16] %SRC_IP_CITY                      City where the src IP is located
   [NFv9 57575][IPFIX 35632.103][Len 2] %DST_IP_COUNTRY                    Country where the dst IP is located
   [NFv9 57576][IPFIX 35632.104][Len 16] %DST_IP_CITY                      City where the dst IP is located
   [NFv9 57920][IPFIX 35632.448][Len 16] %SRC_IP_LONG                      Longitude where the src IP is located
   [NFv9 57921][IPFIX 35632.449][Len 16] %SRC_IP_LAT                       Latitude where the src IP is located
   [NFv9 57922][IPFIX 35632.450][Len 16] %DST_IP_LONG                      Longitude where the dst IP is located
   [NFv9 57923][IPFIX 35632.451][Len 16] %DST_IP_LAT                       Latitude where the dst IP is located
   [NFv9 57577][IPFIX 35632.105][Len 2] %FLOW_PROTO_PORT                   L7 port that identifies the flow protocol or 0 if unknown
   [NFv9 57578][IPFIX 35632.106][Len 4] %UPSTREAM_TUNNEL_ID                Upstream tunnel identifier (e.g. GTP TEID, VXLAN VNI) or 0 if unknown
   [NFv9 57918][IPFIX 35632.446][Len 2] %UPSTREAM_SESSION_ID               Upstream session identifier (e.g. L2TP) or 0 if unknown
   [NFv9 57579][IPFIX 35632.107][Len 2] %LONGEST_FLOW_PKT                  Longest packet (bytes) of the flow
   [NFv9 57580][IPFIX 35632.108][Len 2] %SHORTEST_FLOW_PKT                 Shortest packet (bytes) of the flow
   [NFv9 57599][IPFIX 35632.127][Len 4] %RETRANSMITTED_IN_BYTES            Number of retransmitted TCP flow bytes (src->dst)
   [NFv9 57581][IPFIX 35632.109][Len 4] %RETRANSMITTED_IN_PKTS             Number of retransmitted TCP flow packets (src->dst)
   [NFv9 57600][IPFIX 35632.128][Len 4] %RETRANSMITTED_OUT_BYTES           Number of retransmitted TCP flow bytes (dst->src)
   [NFv9 57582][IPFIX 35632.110][Len 4] %RETRANSMITTED_OUT_PKTS            Number of retransmitted TCP flow packets (dst->src)
   [NFv9 57583][IPFIX 35632.111][Len 4] %OOORDER_IN_PKTS                   Number of out of order TCP flow packets (dst->src)
   [NFv9 57584][IPFIX 35632.112][Len 4] %OOORDER_OUT_PKTS                  Number of out of order TCP flow packets (src->dst)
   [NFv9 57585][IPFIX 35632.113][Len 1] %UNTUNNELED_PROTOCOL               Untunneled IP protocol byte
   [NFv9 57586][IPFIX 35632.114][Len 4] %UNTUNNELED_IPV4_SRC_ADDR          Untunneled IPv4 source address
   [NFv9 57587][IPFIX 35632.115][Len 2] %UNTUNNELED_L4_SRC_PORT            Untunneled IPv4 source port
   [NFv9 57588][IPFIX 35632.116][Len 4] %UNTUNNELED_IPV4_DST_ADDR          Untunneled IPv4 destination address
   [NFv9 57589][IPFIX 35632.117][Len 2] %UNTUNNELED_L4_DST_PORT            Untunneled IPv4 destination port
   [NFv9 57590][IPFIX 35632.118][Len 2] %L7_PROTO                          Layer 7 Protocol (numeric)
   [NFv9 58032][IPFIX 35632.560][Len 1] %L7_CONFIDENCE                     nDPI confidence
   [NFv9 58045][IPFIX 35632.573][Len 4] %FLOW_EXPORT_TIME                  Epoch of flow export
   [NFv9 57591][IPFIX 35632.119][Len 16 varlen] %L7_PROTO_NAME                     Layer 7 Protocol Name [Aliased to %APPLICATION_NAME]
   [NFv9 58046][IPFIX 35632.574][Len 1] %FLOW_CONTENT_TYPE                 Flow content (0=unk, 1=audio, 2=video...)
   [NFv9 57973][IPFIX 35632.501][Len 16 varlen] %L7_PROTO_CATEGORY                 Layer 7 Protocol Category
   [NFv9 58011][IPFIX 35632.539][Len 24 varlen] %L7_INFO                           Layer 7 Flow Information
   [NFv9 58063][IPFIX 35632.591][Len 24 varlen] %L7_DOMAIN_INFO                    Layer 7 Flow domain Information
   [NFv9 57592][IPFIX 35632.120][Len 4] %DOWNSTREAM_TUNNEL_ID              Downstream tunnel identifier (e.g. GTP TEID, VXLAN VNI) or 0 if unknown
   [NFv9 57919][IPFIX 35632.447][Len 2] %DOWNSTREAM_SESSION_ID             Downstream session identifier (e.g. L2TP) or 0 if unknown
   [NFv9 57660][IPFIX 35632.188][Len 48 varlen] %TLS_SERVER_NAME                   TLS server name
   [NFv9 57661][IPFIX 35632.189][Len 40 varlen] %BITTORRENT_HASH                   BITTORRENT hash
   [NFv9 57593][IPFIX 35632.121][Len 32 varlen] %FLOW_USER_NAME                    Flow username of the tunnel (if known)
   [NFv9 57594][IPFIX 35632.122][Len 32 varlen] %NPROBE_INSTANCE_NAME              nprobe instance name
   [NFv9 57598][IPFIX 35632.126][Len 8 varlen] %PLUGIN_NAME                Plugin name used by this flow (if any)
   [NFv9 57868][IPFIX 35632.396][Len 16] %UNTUNNELED_IPV6_SRC_ADDR         Untunneled IPv6 source address
   [NFv9 57869][IPFIX 35632.397][Len 16] %UNTUNNELED_IPV6_DST_ADDR         Untunneled IPv6 destination address
   [NFv9 57819][IPFIX 35632.347][Len 4] %NUM_PKTS_TTL_EQ_1                 # packets with TTL = 1
   [NFv9 57818][IPFIX 35632.346][Len 4] %NUM_PKTS_TTL_2_5                  # packets with TTL > 1 and TTL <= 5
   [NFv9 57806][IPFIX 35632.334][Len 4] %NUM_PKTS_TTL_5_32                 # packets with TTL > 5 and TTL <= 32
   [NFv9 57807][IPFIX 35632.335][Len 4] %NUM_PKTS_TTL_32_64                # packets with TTL > 32 and <= 64
   [NFv9 57808][IPFIX 35632.336][Len 4] %NUM_PKTS_TTL_64_96                # packets with TTL > 64 and <= 96
   [NFv9 57809][IPFIX 35632.337][Len 4] %NUM_PKTS_TTL_96_128               # packets with TTL > 96 and <= 128
   [NFv9 57810][IPFIX 35632.338][Len 4] %NUM_PKTS_TTL_128_160              # packets with TTL > 128 and <= 160
   [NFv9 57811][IPFIX 35632.339][Len 4] %NUM_PKTS_TTL_160_192              # packets with TTL > 160 and <= 192
   [NFv9 57812][IPFIX 35632.340][Len 4] %NUM_PKTS_TTL_192_224              # packets with TTL > 192 and <= 224
   [NFv9 57813][IPFIX 35632.341][Len 4] %NUM_PKTS_TTL_224_255              # packets with TTL > 224 and <= 255
   [NFv9 57821][IPFIX 35632.349][Len 37] %IN_SRC_OSI_SAP                   OSI Source SAP (OSI Traffic Only)
   [NFv9 57822][IPFIX 35632.350][Len 37] %OUT_DST_OSI_SAP                  OSI Destination SAP (OSI Traffic Only)
   [NFv9 57863][IPFIX 35632.391][Len 4] %DURATION_IN                       Client to Server stream duration (msec)
   [NFv9 57864][IPFIX 35632.392][Len 4] %DURATION_OUT                      Client to Server stream duration (msec)
   [NFv9 57887][IPFIX 35632.415][Len 2] %TCP_WIN_MIN_IN                    Min TCP Window (src->dst)
   [NFv9 57888][IPFIX 35632.416][Len 2] %TCP_WIN_MAX_IN                    Max TCP Window (src->dst)
   [NFv9 57889][IPFIX 35632.417][Len 2] %TCP_WIN_MSS_IN                    TCP Max Segment Size (src->dst)
   [NFv9 57890][IPFIX 35632.418][Len 1] %TCP_WIN_SCALE_IN                  TCP Window Scale (src->dst)
   [NFv9 57891][IPFIX 35632.419][Len 2] %TCP_WIN_MIN_OUT                   Min TCP Window (dst->src)
   [NFv9 57892][IPFIX 35632.420][Len 2] %TCP_WIN_MAX_OUT                   Max TCP Window (dst->src)
   [NFv9 57893][IPFIX 35632.421][Len 2] %TCP_WIN_MSS_OUT                   TCP Max Segment Size (dst->src)
   [NFv9 57894][IPFIX 35632.422][Len 1] %TCP_WIN_SCALE_OUT                 TCP Window Scale (dst->src)
   [NFv9 57910][IPFIX 35632.438][Len 4] %PAYLOAD_HASH                      Initial flow payload hash
   [NFv9 57915][IPFIX 35632.443][Len 16] %SRC_AS_MAP                       Organization name for SRC_AS
   [NFv9 57916][IPFIX 35632.444][Len 16] %DST_AS_MAP                       Organization name for DST_AS
   [NFv9 57944][IPFIX 35632.472][Len 8] %SRC_TO_DST_SECOND_BYTES           Bytes/sec (src->dst)
   [NFv9 57945][IPFIX 35632.473][Len 8] %DST_TO_SRC_SECOND_BYTES           Bytes/sec2 (dst->src)
   [NFv9 57961][IPFIX 35632.489][Len 32 varlen] %JA3C_HASH                         JA3 client hash
   [NFv9 58048][IPFIX 35632.576][Len 32 varlen] %JA4C_HASH                         JA4 client hash
   [NFv9 57962][IPFIX 35632.490][Len 32 varlen] %JA3S_HASH                         JA3 server hash
   [NFv9 57963][IPFIX 35632.491][Len 48 varlen] %SRC_HOST_NAME                     Symbolic src host name
   [NFv9 57964][IPFIX 35632.492][Len 48 varlen] %DST_HOST_NAME                     Symbolic dst host name
   [NFv9 57965][IPFIX 35632.493][Len 2] %TLS_CIPHER                        TLS Connection Cipher
   [NFv9 57966][IPFIX 35632.494][Len 1] %TLS_UNSAFE_CIPHER                 TLS Safe(0)/unsafe(1) cipher
   [NFv9 57967][IPFIX 35632.495][Len 2] %TLS_VERSION                       TLS Version
   [NFv9 57974][IPFIX 35632.502][Len 47] %SEQ_PLEN                         Seq of packet len (6 classes)
   [NFv9 57977][IPFIX 35632.505][Len 47] %SEQ_TDIFF                        Seq of time diff (6 classes)
   [NFv9 57978][IPFIX 35632.506][Len 1] %SEQ_PLEN_HASH                     Seq of packet len hash
   [NFv9 57979][IPFIX 35632.507][Len 1] %SEQ_TDIFF_HASH                    Seq of time diff hash
   [NFv9 57980][IPFIX 35632.508][Len 94] %PKT_VECTOR                       Seq of packet len (+=c2s, -=s2c)
   [NFv9 57971][IPFIX 35632.499][Len 32 varlen] %HASSH_CLIENT                      HASSH client hash
   [NFv9 57972][IPFIX 35632.500][Len 32 varlen] %HASSH_SERVER                      HASSH server hash
   [NFv9 57975][IPFIX 35632.503][Len 4] %ENTROPY_CLIENT_BYTES              Byte (src->dst) entropy * 1000
   [NFv9 57976][IPFIX 35632.504][Len 4] %ENTROPY_SERVER_BYTES              Byte (dst->src) entropy * 1000
   [NFv9 57981][IPFIX 35632.509][Len 8] %L7_PROTO_RISK                     Layer 7 protocol risk (bitmap)
   [NFv9 57982][IPFIX 35632.510][Len 64 varlen] %L7_PROTO_RISK_NAME                Layer 7 protocol risk (string)
   [NFv9 57999][IPFIX 35632.527][Len 2] %L7_RISK_SCORE                     Layer 7 flow Risk Score
   [NFv9 57994][IPFIX 35632.522][Len 2] %FLOW_VERDICT                      Flow verdict marker (0 = unknown, 1=pass, 2=drop...)
   [NFv9 57997][IPFIX 35632.525][Len 24 varlen] %SRC_HOST_LABEL                    Src host label
   [NFv9 57998][IPFIX 35632.526][Len 24 varlen] %DST_HOST_LABEL                    Dest host label
   [NFv9 58003][IPFIX 35632.531][Len 4] %SRC_TO_DST_IAT_MIN                Min (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58004][IPFIX 35632.532][Len 4] %SRC_TO_DST_IAT_MAX                Max (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58005][IPFIX 35632.533][Len 4] %SRC_TO_DST_IAT_AVG                Avg (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58006][IPFIX 35632.534][Len 4] %SRC_TO_DST_IAT_STDDEV             StdDev (src->dst) Pkt Inter-Arrival Time (msec)
   [NFv9 58007][IPFIX 35632.535][Len 4] %DST_TO_SRC_IAT_MIN                Min (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58008][IPFIX 35632.536][Len 4] %DST_TO_SRC_IAT_MAX                Max (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58009][IPFIX 35632.537][Len 4] %DST_TO_SRC_IAT_AVG                Avg (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58010][IPFIX 35632.538][Len 4] %DST_TO_SRC_IAT_STDDEV             StdDev (dst->src) Pkt Inter-Arrival Time (msec)
   [NFv9 58025][IPFIX 35632.553][Len 24 varlen] %AAA_NAT_KEY                       AAA/NAT Correlation Key
   [NFv9 58026][IPFIX 35632.554][Len 4] %L7_ERROR_CODE                     Error code (e.g. SNMP, DNS. HTTP)
   [NFv9 58027][IPFIX 35632.555][Len 48 varlen] %L7_RISK_INFO                      L7 Risk Information
   [NFv9 58047][IPFIX 35632.575][Len 16 varlen] %ACCOUNT_ID                        AWS VPC Account-Id (string)
   [NFv9 58058][IPFIX 35632.586][Len 1] %FLOW_ENCRYPTED                    0=cleartext/unknown, 1=encrypted (Entropy-based)
   [NFv9 58061][IPFIX 35632.589][Len 4] %UNIQUE_SOURCE_ID                  Unique nProbe + exporter flow source id
   [NFv9 58062][IPFIX 35632.590][Len 36] %NPROBE_UUID                      Unique nProbe UUID

   Plugin HTTP Protocol templates:
   [NFv9 57652][IPFIX 35632.180][Len 128 varlen] %HTTP_URL                         HTTP URL (IXIA URI)
   [NFv9 57832][IPFIX 35632.360][Len 4 varlen] %HTTP_METHOD                HTTP METHOD
   [NFv9 57653][IPFIX 35632.181][Len 2] %HTTP_RET_CODE                     HTTP return code (e.g. 200, 304...)
   [NFv9 57654][IPFIX 35632.182][Len 128 varlen] %HTTP_REFERER                     HTTP Referer
   [NFv9 57655][IPFIX 35632.183][Len 256 varlen] %HTTP_USER_AGENT                  HTTP User Agent
   [NFv9 57656][IPFIX 35632.184][Len 256 varlen] %HTTP_MIME                        HTTP Mime Type
   [NFv9 57659][IPFIX 35632.187][Len 64 varlen] %HTTP_HOST                         HTTP(S) Host Name (IXIA Host Name)
   [NFv9 57833][IPFIX 35632.361][Len 64 varlen] %HTTP_SITE                         HTTP server without host name
   [NFv9 57932][IPFIX 35632.460][Len 256 varlen] %HTTP_X_FORWARDED_FOR             HTTP X-Forwarded-For
   [NFv9 57933][IPFIX 35632.461][Len 256 varlen] %HTTP_VIA                         HTTP Via

   Plugin IMAP Protocol templates:
   [NFv9 57732][IPFIX 35632.260][Len 64 varlen] %IMAP_LOGIN                        Mail sender

   Plugin MySQL Plugin templates:
   [NFv9 57667][IPFIX 35632.195][Len 16] %MYSQL_SERVER_VERSION             MySQL server version
   [NFv9 57668][IPFIX 35632.196][Len 16] %MYSQL_USERNAME                   MySQL username
   [NFv9 57669][IPFIX 35632.197][Len 64] %MYSQL_DB                         MySQL database in use
   [NFv9 57670][IPFIX 35632.198][Len 128 varlen] %MYSQL_QUERY                      MySQL Query
   [NFv9 57671][IPFIX 35632.199][Len 2] %MYSQL_RESPONSE                    MySQL server response
   [NFv9 57792][IPFIX 35632.320][Len 4] %MYSQL_APPL_LATENCY_USEC           MySQL request->response latecy (usec)

   Plugin NETBIOS Protocol templates:
   [NFv9 57936][IPFIX 35632.464][Len 48 varlen] %NETBIOS_QUERY_NAME                NETBIOS Query Name
   [NFv9 57937][IPFIX 35632.465][Len 64 varlen] %NETBIOS_QUERY_TYPE                NETBIOS Query Type
   [NFv9 57938][IPFIX 35632.466][Len 64 varlen] %NETBIOS_RESPONSE                  NETBIOS Query Response
   [NFv9 57939][IPFIX 35632.467][Len 24 varlen] %NETBIOS_QUERY_OS                  NETBIOS Query OS

   Plugin POP3 Protocol templates:
   [NFv9 57682][IPFIX 35632.210][Len 64 varlen] %POP_USER                          POP3 user login

   Plugin Radius Protocol templates:
   [NFv9 57712][IPFIX 35632.240][Len 1] %RADIUS_REQ_MSG_TYPE               RADIUS Request Msg Type
   [NFv9 57713][IPFIX 35632.241][Len 1] %RADIUS_RSP_MSG_TYPE               RADIUS Response Msg Type
   [NFv9 57714][IPFIX 35632.242][Len 32 varlen] %RADIUS_USER_NAME                  RADIUS User Name (Access Only)
   [NFv9 57715][IPFIX 35632.243][Len 32 varlen] %RADIUS_CALLING_STATION_ID         RADIUS Calling Station Id
   [NFv9 57716][IPFIX 35632.244][Len 32 varlen] %RADIUS_CALLED_STATION_ID          RADIUS Called Station Id
   [NFv9 57717][IPFIX 35632.245][Len 4] %RADIUS_NAS_IP_ADDR                RADIUS NAS IP Address
   [NFv9 57718][IPFIX 35632.246][Len 24 varlen] %RADIUS_NAS_IDENTIFIER             RADIUS NAS Identifier
   [NFv9 57719][IPFIX 35632.247][Len 16] %RADIUS_USER_IMSI                 RADIUS User IMSI (Extension)
   [NFv9 57720][IPFIX 35632.248][Len 16] %RADIUS_USER_IMEI                 RADIUS User MSISDN (Extension)
   [NFv9 57721][IPFIX 35632.249][Len 4] %RADIUS_FRAMED_IP_ADDR             RADIUS Framed IP
   [NFv9 57722][IPFIX 35632.250][Len 24 varlen] %RADIUS_ACCT_SESSION_ID            RADIUS Accounting Session Name
   [NFv9 57723][IPFIX 35632.251][Len 1] %RADIUS_ACCT_STATUS_TYPE           RADIUS Accounting Status Type
   [NFv9 57724][IPFIX 35632.252][Len 4] %RADIUS_ACCT_IN_OCTETS             RADIUS Accounting Input Octets
   [NFv9 57725][IPFIX 35632.253][Len 4] %RADIUS_ACCT_OUT_OCTETS            RADIUS Accounting Output Octets
   [NFv9 57726][IPFIX 35632.254][Len 4] %RADIUS_ACCT_IN_PKTS               RADIUS Accounting Input Packets
   [NFv9 57727][IPFIX 35632.255][Len 4] %RADIUS_ACCT_OUT_PKTS              RADIUS Accounting Output Packets

   Plugin RTP Plugin templates:
   [NFv9 57909][IPFIX 35632.437][Len 4] %RTP_SSRC                          RTP Sync Source ID
   [NFv9 57622][IPFIX 35632.150][Len 4] %RTP_FIRST_SEQ                     First flow RTP Seq Number
   [NFv9 57623][IPFIX 35632.151][Len 4] %RTP_FIRST_TS                      First flow RTP timestamp
   [NFv9 57624][IPFIX 35632.152][Len 4] %RTP_LAST_SEQ                      Last flow RTP Seq Number
   [NFv9 57625][IPFIX 35632.153][Len 4] %RTP_LAST_TS                       Last flow RTP timestamp
   [NFv9 57626][IPFIX 35632.154][Len 4] %RTP_IN_JITTER                     RTP jitter (ms * 1000)
   [NFv9 57627][IPFIX 35632.155][Len 4] %RTP_OUT_JITTER                    RTP jitter (ms * 1000)
   [NFv9 57628][IPFIX 35632.156][Len 4] %RTP_IN_PKT_LOST                   Packet %% lost in stream (src->dst)
   [NFv9 57629][IPFIX 35632.157][Len 4] %RTP_OUT_PKT_LOST                  Packet %% lost in stream (dst->src)
   [NFv9 57902][IPFIX 35632.430][Len 4] %RTP_IN_PKT_DROP                   Packet discarded by Jitter Buffer (src->dst)
   [NFv9 57903][IPFIX 35632.431][Len 4] %RTP_OUT_PKT_DROP                  Packet discarded by Jitter Buffer (dst->src)
   [NFv9 57633][IPFIX 35632.161][Len 1] %RTP_IN_PAYLOAD_TYPE               RTP payload type
   [NFv9 57630][IPFIX 35632.158][Len 1] %RTP_OUT_PAYLOAD_TYPE              RTP payload type
   [NFv9 57631][IPFIX 35632.159][Len 4] %RTP_IN_MAX_DELTA                  Max delta (ms*100) between consecutive pkts (src->dst)
   [NFv9 57632][IPFIX 35632.160][Len 4] %RTP_OUT_MAX_DELTA                 Max delta (ms*100) between consecutive pkts (dst->src)
   [NFv9 57820][IPFIX 35632.348][Len 64 varlen] %RTP_SIP_CALL_ID                   SIP call-id corresponding to this RTP stream
   [NFv9 57906][IPFIX 35632.434][Len 4] %RTP_MOS                           RTP pseudo-MOS (value * 100) (average both directions)
   [NFv9 57842][IPFIX 35632.370][Len 4] %RTP_IN_MOS                        RTP pseudo-MOS (value * 100) (src->dst)
   [NFv9 57904][IPFIX 35632.432][Len 4] %RTP_OUT_MOS                       RTP pseudo-MOS (value * 100) (dst->src)
   [NFv9 57908][IPFIX 35632.436][Len 4] %RTP_R_FACTOR                      RTP pseudo-R_FACTOR (value * 100) (average both directions)
   [NFv9 57843][IPFIX 35632.371][Len 4] %RTP_IN_R_FACTOR                   RTP pseudo-R_FACTOR (value * 100) (src->dst)
   [NFv9 57905][IPFIX 35632.433][Len 4] %RTP_OUT_R_FACTOR                  RTP pseudo-R_FACTOR (value * 100) (dst->src)
   [NFv9 57853][IPFIX 35632.381][Len 4] %RTP_IN_TRANSIT                    RTP Transit (value * 100) (src->dst)
   [NFv9 57854][IPFIX 35632.382][Len 4] %RTP_OUT_TRANSIT                   RTP Transit (value * 100) (dst->src)
   [NFv9 57852][IPFIX 35632.380][Len 4] %RTP_RTT                           RTP Round Trip Time (ms)
   [NFv9 57867][IPFIX 35632.395][Len 16 varlen] %RTP_DTMF_TONES                    DTMF tones sent (if any) during the call
   Plugin SIP Plugin templates:
   [NFv9 57602][IPFIX 35632.130][Len 96 varlen] %SIP_CALL_ID                       SIP call-id
   [NFv9 57603][IPFIX 35632.131][Len 96 varlen] %SIP_CALLING_PARTY                 SIP Call initiator
   [NFv9 57604][IPFIX 35632.132][Len 96 varlen] %SIP_CALLED_PARTY                  SIP Called party
   [NFv9 57605][IPFIX 35632.133][Len 512] %SIP_RTP_CODECS                  SIP RTP codecs
   [NFv9 58000][IPFIX 35632.528][Len 4] %SIP_REGISTER_MAX_RRD              SIP REGISTER max rsp delay (msec)
   [NFv9 58001][IPFIX 35632.529][Len 1] %SIP_REGISTER_NUM_OK               SIP REGISTER number of rsp ok/authorized
   [NFv9 58002][IPFIX 35632.530][Len 1] %SIP_REGISTER_NUM_OTHER            SIP REGISTER number of rsp not ok/authorized
   [NFv9 57606][IPFIX 35632.134][Len 4] %SIP_INVITE_TIME                   SIP time (epoch) of INVITE
   [NFv9 57607][IPFIX 35632.135][Len 4] %SIP_TRYING_TIME                   SIP time (epoch) of Trying
   [NFv9 57608][IPFIX 35632.136][Len 4] %SIP_RINGING_TIME                  SIP time (epoch) of RINGING
   [NFv9 57609][IPFIX 35632.137][Len 4] %SIP_INVITE_OK_TIME                SIP time (epoch) of INVITE OK
   [NFv9 57610][IPFIX 35632.138][Len 4] %SIP_INVITE_FAILURE_TIME           SIP time (epoch) of INVITE FAILURE
   [NFv9 57611][IPFIX 35632.139][Len 4] %SIP_BYE_TIME                      SIP time (epoch) of BYE
   [NFv9 57612][IPFIX 35632.140][Len 4] %SIP_BYE_OK_TIME                   SIP time (epoch) of BYE OK
   [NFv9 57613][IPFIX 35632.141][Len 4] %SIP_CANCEL_TIME                   SIP time (epoch) of CANCEL
   [NFv9 57614][IPFIX 35632.142][Len 4] %SIP_CANCEL_OK_TIME                SIP time (epoch) of CANCEL OK
   [NFv9 57615][IPFIX 35632.143][Len 4] %SIP_RTP_IPV4_SRC_ADDR             SIP RTP stream source IP
   [NFv9 57616][IPFIX 35632.144][Len 2] %SIP_RTP_L4_SRC_PORT               SIP RTP stream source port
   [NFv9 57617][IPFIX 35632.145][Len 4] %SIP_RTP_IPV4_DST_ADDR             SIP RTP stream dest IP
   [NFv9 57618][IPFIX 35632.146][Len 2] %SIP_RTP_L4_DST_PORT               SIP RTP stream dest port
   [NFv9 57619][IPFIX 35632.147][Len 4] %SIP_RESPONSE_CODE                 SIP failure response code
   [NFv9 57620][IPFIX 35632.148][Len 4] %SIP_REASON_CAUSE                  SIP Cancel/Bye/Failure reason cause
   [NFv9 57788][IPFIX 35632.316][Len 96 varlen] %SIP_UAC                           SIP user-agent client
   [NFv9 57789][IPFIX 35632.317][Len 96 varlen] %SIP_UAS                           SIP user-agent server
   [NFv9 57834][IPFIX 35632.362][Len 128] %SIP_C_IP                        SIP C IP addresses
   [NFv9 57835][IPFIX 35632.363][Len 12 varlen] %SIP_CALL_STATE                    SIP Call State

   Plugin SMTP Protocol templates:
   [NFv9 57657][IPFIX 35632.185][Len 64 varlen] %SMTP_MAIL_FROM                    Mail sender
   [NFv9 57658][IPFIX 35632.186][Len 64 varlen] %SMTP_RCPT_TO                      Mail recipient

   Plugin SSDP Protocol templates:
   [NFv9 57934][IPFIX 35632.462][Len 48 varlen] %SSDP_HOST                         SSDP Host
   [NFv9 57935][IPFIX 35632.463][Len 64 varlen] %SSDP_USN                          SSDP USN
   [NFv9 57940][IPFIX 35632.468][Len 64 varlen] %SSDP_SERVER                       SSDP Server
   [NFv9 57941][IPFIX 35632.469][Len 64 varlen] %SSDP_TYPE                         SSDP Type
   [NFv9 57942][IPFIX 35632.470][Len 8 varlen] %SSDP_METHOD                SSDP Method

The default template (if -T is omitted) is:

%IPV4_SRC_ADDR %IPV4_DST_ADDR %INPUT_SNMP %OUTPUT_SNMP %IN_PKTS %IN_BYTES %FIRST_SWITCHED %LAST_SWITCHED %L4_SRC_PORT %L4_DST_PORT %TCP_FLAGS %PROTOCOL %SRC_TOS %SRC_AS %DST_AS

In case @NTOPNG@ is used with -T, such template it is expanded according to the
operational mode:
1. probe mode (-i <interface>)
   %IN_SRC_MAC %OUT_DST_MAC %INPUT_SNMP %OUTPUT_SNMP %SRC_VLAN %IPV4_SRC_ADDR %IPV4_DST_ADDR %L4_SRC_PORT %L4_DST_PORT %IPV6_SRC_ADDR %IPV6_DST_ADDR %SRC_TOS %DST_TOS %IP_PROTOCOL_VERSION %PROTOCOL %L7_PROTO %L7_CONFIDENCE %IN_BYTES %IN_PKTS %OUT_BYTES %OUT_PKTS %FIRST_SWITCHED %LAST_SWITCHED %CLIENT_TCP_FLAGS %SERVER_TCP_FLAGS %L7_PROTO_RISK %L7_RISK_SCORE %EXPORTER_IPV4_ADDRESS %DIRECTION %SAMPLING_INTERVAL %TOTAL_FLOWS_EXP %NPROBE_IPV4_ADDRESS %NPROBE_INSTANCE_NAME %FLOW_SOURCE %JA4C_HASH %UNIQUE_SOURCE_ID %CLIENT_NW_LATENCY_MS %SERVER_NW_LATENCY_MS %APPL_LATENCY_MS %TCP_WIN_MAX_IN %TCP_WIN_MAX_OUT %OOORDER_IN_PKTS %OOORDER_OUT_PKTS %RETRANSMITTED_IN_PKTS %RETRANSMITTED_OUT_PKTS %SRC_FRAGMENTS %DST_FRAGMENTS %L7_INFO %DNS_QUERY %DNS_QUERY_TYPE %DNS_RET_CODE %HTTP_URL %HTTP_SITE %HTTP_METHOD %HTTP_RET_CODE %TLS_SERVER_NAME %BITTORRENT_HASH %SRC_TOS %DST_TOS %HTTP_USER_AGENT %L7_RISK_INFO

2. collector mode (-3 <port>)
      %IN_SRC_MAC %OUT_DST_MAC %INPUT_SNMP %OUTPUT_SNMP %SRC_VLAN %IPV4_SRC_ADDR %IPV4_DST_ADDR %L4_SRC_PORT %L4_DST_PORT %IPV6_SRC_ADDR %IPV6_DST_ADDR %SRC_TOS %DST_TOS %IP_PROTOCOL_VERSION %PROTOCOL %L7_PROTO %L7_CONFIDENCE %IN_BYTES %IN_PKTS %OUT_BYTES %OUT_PKTS %FIRST_SWITCHED %LAST_SWITCHED %CLIENT_TCP_FLAGS %SERVER_TCP_FLAGS %L7_PROTO_RISK %L7_RISK_SCORE %EXPORTER_IPV4_ADDRESS %DIRECTION %SAMPLING_INTERVAL %TOTAL_FLOWS_EXP %NPROBE_IPV4_ADDRESS %NPROBE_INSTANCE_NAME %FLOW_SOURCE %JA4C_HASH %UNIQUE_SOURCE_ID %POST_NAT_SRC_IPV4_ADDR %POST_NAT_DST_IPV4_ADDR %POST_NAPT_SRC_TRANSPORT_PORT %POST_NAPT_DST_TRANSPORT_PORT


Application Protocols
~~~~~~~~~~~~~~~~~~~~~

Major protocol (%L7_PROTO) symbolic mapping:

.. code:: bash

 Id Protocol               Layer_4  Breed        Category
 0        0 Unknown                TCP        X        Unrated      Unspecified        -                               -
 1        1 FTP_CONTROL            TCP        X        Unsafe       Download           -                               21
 2        2 POP3                   TCP        X        Unsafe       Email              -                               110
 3        3 SMTP                   TCP        X        Acceptable   Email              -                               25,587
 4        4 IMAP                   TCP        X        Unsafe       Email              -                               143
 5        5 DNS                    TCP/UDP    X        Acceptable   Network            53                              53
 6        6 IPP                    TCP/UDP    X        Acceptable   System             -                               -
 7        7 HTTP                   TCP        X        Acceptable   Web                -                               80
 8        8 MDNS                   TCP        X        Acceptable   Network            5353,5354                       -
 9        9 NTP                    UDP        X        Acceptable   System             123                             -
 10       10 NetBIOS                TCP/UDP    X        Acceptable   System             137,138,139                     139
 11       11 NFS                    TCP/UDP    X        Acceptable   DataTransfer       2049                            2049
 12       12 SSDP                   UDP        X        Acceptable   System             -                               -
 13       13 BGP                    TCP        X        Acceptable   Network            -                               179,2605
 14       14 SNMP                   UDP        X        Acceptable   Network            161,162                         -
 15       15 XDMCP                  TCP/UDP    X        Acceptable   RemoteAccess       177                             177
 16       16 SMBv1                  TCP        X        Dangerous    System             -                               445
 17       17 Syslog                 TCP/UDP    X        Acceptable   System             514                             514,601,6514
 18       18 DHCP                   UDP        X        Acceptable   Network            67,68                           -
 19       19 PostgreSQL             TCP        X        Acceptable   Database           -                               5432
 20       20 MySQL                  TCP        X        Acceptable   Database           -                               3306
 21       21 Outlook                TCP                 Acceptable   Email              -                               -
 22       22 VK                     TCP                 Fun          SocialNetwork      -                               -
 23       23 POPS                   TCP                 Safe         Email              -                               995
 24       24 Tailscale              UDP                 Acceptable   VPN                41641                           -
 25       25 Yandex                 TCP                 Safe         Web                -                               -
 26       26 ntop                   TCP                 Safe         Network            -                               -
 27       27 COAP                   UDP        X        Safe         RPC                5683,5684                       -
 28       28 VMware                 UDP        X        Acceptable   RemoteAccess       902,903                         903
 29       29 SMTPS                  TCP                 Safe         Email              -                               465
 30       30 DTLS                   UDP        X        Safe         Web                -                               -
 31       31 UBNTAC2                UDP        X        Safe         Network            10001                           -
 32       32 BFCP                   TCP/UDP    X        Acceptable   Video              -                               -
 33       33 YandexMail             TCP                 Safe         Email              -                               -
 34       34 YandexMusic            TCP                 Fun          Music              -                               -
 35       35 Gnutella               TCP/UDP    X        Potentially Dangerous Download           -                               -
 36       36 eDonkey                TCP        X        Unsafe       Download           -                               -
 37       37 BitTorrent             TCP/UDP    X        Acceptable   Download           6771,51413                      51413,53646
 38       38 Skype_TeamsCall        TCP                 Acceptable   VoIP               -                               -
 39       39 Signal                 TCP                 Fun          Chat               -                               -
 40       40 Memcached              TCP/UDP    X        Acceptable   Network            11211                           11211
 41       41 SMBv23                 TCP        X        Acceptable   System             -                               445
 42       42 Mining                 TCP                 Unsafe       Mining             -                               -
 43       43 NestLogSink            TCP                 Acceptable   Cloud              -                               11095
 44       44 Modbus                 TCP        X        Acceptable   IoT-Scada          -                               502
 45       45 WhatsAppCall           TCP                 Acceptable   VoIP               -                               -
 46       46 DataSaver              TCP                 Fun          Web                -                               -
 47       47 Xbox                   UDP                 Fun          Game               -                               -
 48       48 QQ                     UDP                 Fun          Chat               -                               -
 49       49 TikTok                 TCP                 Fun          SocialNetwork      -                               -
 50       50 RTSP                   TCP/UDP    X        Fun          Media              554                             554
 51       51 IMAPS                  TCP                 Safe         Email              -                               993
 52       52 IceCast                TCP        X        Fun          Media              -                               -
 53       53 CPHA                   UDP                 Fun          Network            8116                            -
 54       54 iQIYI                  UDP        X        Fun          Streaming          -                               -
 55       55 Zattoo                 TCP/UDP             Fun          Video              -                               -
 56       56 YandexMarket           TCP                 Safe         Shopping           -                               -
 57       57 YandexDisk             TCP                 Safe         Cloud              -                               -
 58       58 Discord                UDP                 Fun          Collaborative      -                               -
 59       59 AdobeConnect           TCP                 Acceptable   Video              -                               -
 60       60 MongoDB                TCP        X        Acceptable   Database           -                               27017
 61       61 Pluralsight            TCP                 Fun          Video              -                               -
 62       62 YandexCloud            TCP                 Safe         Cloud              -                               -
 63       63 OCSP                   TCP                 Safe         Network            -                               -
 64       64 VXLAN                  UDP        X        Acceptable   Network            4789                            -
 65       65 IRC                    TCP        X        Unsafe       Chat               194                             194
 66       66 MerakiCloud            UDP        X        Acceptable   Network            -                               -
 67       67 Jabber                 TCP        X        Acceptable   Web                -                               -
 68       68 Nats                   TCP        X        Acceptable   RPC                -                               -
 69       69 AmongUs                UDP                 Fun          Game               22023                           -
 70       70 Yahoo                  TCP                 Safe         Web                -                               -
 71       71 DisneyPlus             TCP                 Fun          Streaming          -                               -
 72       72 HART-IP                TCP/UDP    X        Acceptable   IoT-Scada          -                               5094
 73       73 VRRP                   TCP        X        Acceptable   Network            -                               -
 74       74 Steam                  UDP                 Fun          Game               -                               -
 75       75 HalfLife2              UDP        X        Fun          Game               -                               -
 76       76 WorldOfWarcraft        TCP                 Fun          Game               -                               -
 77       77 Telnet                 TCP        X        Unsafe       RemoteAccess       -                               23
 78       78 STUN                   TCP/UDP    X        Acceptable   Network            3478                            -
 79       79 IPSec                  UDP        X        Safe         VPN                500,4500                        500
 80       80 GRE                               X        Acceptable   Network            -                               -
 81       81 ICMP                              X        Acceptable   Network            -                               -
 82       82 IGMP                              X        Acceptable   Network            -                               -
 83       83 EGP                               X        Acceptable   Network            -                               -
 84       84 SCTP                              X        Acceptable   Network            -                               -
 85       85 OSPF                              X        Acceptable   Network            -                               2604
 86       86 IP_in_IP                          X        Acceptable   Network            -                               -
 87       87 RTP                    TCP/UDP    X        Acceptable   Media              -                               -
 88       88 RDP                    TCP/UDP    X        Acceptable   RemoteAccess       3389                            3389
 89       89 VNC                    TCP        X        Acceptable   RemoteAccess       -                               5900,5901,5800
 90       90 Tumblr                 TCP                 Fun          SocialNetwork      -                               -
 91       91 TLS                    TCP        X        Safe         Web                -                               443
 92       92 SSH                    TCP        X        Acceptable   RemoteAccess       -                               22
 93       93 Usenet                 TCP        X        Acceptable   Web                -                               -
 94       94 MGCP                   UDP        X        Acceptable   VoIP               -                               -
 95       95 IAX                    UDP        X        Acceptable   VoIP               4569                            4569
 96       96 TFTP                   UDP        X        Acceptable   DataTransfer       69                              -
 97       97 AFP                    TCP        X        Acceptable   DataTransfer       548                             548
 98       98 YandexMetrika          TCP                 Safe         Web                -                               -
 99       99 YandexDirect           TCP                 Tracker/Ads  Advertisement      -                               -
 100      100 SIP                    TCP/UDP    X        Acceptable   VoIP               5060,5061                       5060,5061
 101      101 TruPhone               TCP                 Acceptable   VoIP               -                               -
 102      102 ICMPV6                            X        Acceptable   Network            -                               -
 103      103 DHCPV6                 UDP        X        Acceptable   Network            -                               -
 104      104 Armagetron             UDP        X        Fun          Game               -                               -
 105      105 Crossfire              TCP/UDP             Fun          RPC                -                               -
 106      106 Dofus                  TCP        X        Fun          Game               -                               -
 107      107 ADS_Analytic_Track     TCP                 Tracker/Ads  Advertisement      -                               -
 108      108 AdultContent           TCP                 Acceptable   AdultContent       -                               -
 109      109 Guildwars              TCP        X        Fun          Game               -                               -
 110      110 AmazonAlexa            TCP                 Acceptable   VirtAssistant      -                               -
 111      111 Kerberos               TCP/UDP    X        Acceptable   Network            88                              88
 112      112 LDAP                   TCP/UDP    X        Acceptable   System             389                             389
 113      113 MapleStory             TCP                 Fun          Game               -                               -
 114      114 MsSQL-TDS              TCP        X        Acceptable   Database           -                               1433,1434
 115      115 PPTP                   TCP        X        Acceptable   VPN                -                               -
 116      116 Warcraft3              TCP/UDP    X        Fun          Game               -                               -
 117      117 WorldOfKungFu          TCP        X        Fun          Game               -                               -
 118      118 Slack                  TCP                 Acceptable   Collaborative      -                               -
 119      119 Facebook               TCP                 Fun          SocialNetwork      -                               -
 120      120 Twitter                TCP                 Fun          SocialNetwork      -                               -
 121      121 Dropbox                UDP                 Acceptable   Cloud              17500                           -
 122      122 GMail                  TCP                 Acceptable   Email              -                               -
 123      123 GoogleMaps             TCP                 Safe         Web                -                               -
 124      124 YouTube                TCP                 Fun          Media              -                               -
 125      125 Skype_Teams            TCP                 Acceptable   VoIP               -                               -
 126      126 Google                 TCP                 Acceptable   Web                -                               -
 127      127 MS-RPCH                TCP        X        Acceptable   RPC                -                               -
 128      128 NetFlow                UDP        X        Acceptable   Network            2055                            -
 129      129 sFlow                  UDP        X        Acceptable   Network            6343                            -
 130      130 HTTP_Connect           TCP        X        Acceptable   Web                -                               8080
 131      131 HTTP_Proxy             TCP        X        Acceptable   Web                -                               8080,3128
 132      132 Citrix                 TCP                 Acceptable   Network            -                               1494,2598
 133      133 NetFlix                TCP                 Fun          Video              -                               -
 134      134 LastFM                 TCP                 Fun          Music              -                               -
 135      135 Waze                   TCP                 Acceptable   Web                -                               -
 136      136 YouTubeUpload          TCP                 Fun          Media              -                               -
 137      137 Hulu                   TCP                 Fun          Streaming          -                               -
 138      138 CHECKMK                TCP        X        Acceptable   DataTransfer       -                               6556
 139      139 AJP                    TCP        X        Acceptable   Web                -                               8009,8010
 140      140 Apple                  TCP                 Safe         Web                -                               -
 141      141 Webex                  TCP                 Acceptable   VoIP               -                               -
 142      142 WhatsApp               TCP                 Acceptable   Chat               -                               -
 143      143 AppleiCloud            TCP                 Acceptable   Web                -                               -
 144      144 Viber                  TCP/UDP             Fun          VoIP               7985,7987,5242,5243,4244        7985,5242,5243,4244
 145      145 AppleiTunes            TCP                 Fun          Streaming          -                               -
 146      146 Radius                 UDP        X        Acceptable   Network            1812,1813                       1812,1813
 147      147 WindowsUpdate          TCP                 Safe         SoftwareUpdate     -                               -
 148      148 TeamViewer             TCP/UDP             Acceptable   RemoteAccess       5938                            5938
 149      149 EthernetGlobalData     UDP        X        Acceptable   IoT-Scada          -                               -
 150      150 LotusNotes             TCP        X        Acceptable   Collaborative      -                               1352
 151      151 SAP                    TCP        X        Acceptable   Network            -                               3201
 152      152 GTP                    UDP        X        Acceptable   Network            2152,2123                       -
 153      153 WSD                    UDP        X        Acceptable   Network            3702                            -
 154      154 LLMNR                  TCP        X        Acceptable   Network            5355                            5355
 155      155 TocaBoca               UDP        X        Fun          Game               5055                            -
 156      156 Spotify                TCP/UDP             Fun          Music              -                               -
 157      157 FacebookMessenger      TCP                 Acceptable   Chat               -                               -
 158      158 H323                   TCP/UDP    X        Acceptable   VoIP               1719,1720                       1719,1720
 159      159 OpenVPN                TCP/UDP             Acceptable   VPN                1194                            1194
 160      160 NOE                    UDP        X        Acceptable   VoIP               -                               -
 161      161 CiscoVPN               UDP        X        Acceptable   VPN                10000                           10000,8008
 162      162 TeamSpeak              TCP/UDP    X        Fun          VoIP               -                               -
 163      163 Tor                    TCP                 Potentially Dangerous VPN                -                               -
 164      164 CiscoSkinny            TCP        X        Acceptable   VoIP               -                               2000
 165      165 RTCP                   TCP        X        Acceptable   VoIP               -                               -
 166      166 RSYNC                  TCP        X        Acceptable   DataTransfer       -                               873
 167      167 Oracle                 TCP        X        Acceptable   Database           -                               1521
 168      168 Corba                  TCP/UDP    X        Acceptable   RPC                -                               -
 169      169 UbuntuONE              TCP                 Acceptable   Cloud              -                               -
 170      170 Whois-DAS              TCP        X        Acceptable   Network            -                               43,4343
 171      171 SD-RTN                 UDP        X        Acceptable   Media              -                               -
 172      172 SOCKS                  TCP        X        Acceptable   Web                1080                            1080
 173      173 Nintendo               UDP                 Fun          Game               -                               -
 174      174 RTMP                   TCP        X        Acceptable   Media              -                               1935
 175      175 FTP_DATA               TCP        X        Acceptable   Download           -                               20
 176      176 Wikipedia              TCP                 Safe         Web                -                               -
 177      177 ZeroMQ                 TCP        X        Acceptable   RPC                -                               -
 178      178 Amazon                 TCP                 Acceptable   Web                -                               -
 179      179 eBay                   TCP                 Safe         Shopping           -                               -
 180      180 CNN                    TCP                 Safe         Web                -                               -
 181      181 Megaco                 UDP        X        Acceptable   VoIP               2944                            -
 182      182 RESP                   TCP        X        Acceptable   Database           -                               6379
 183      183 Pinterest              TCP                 Fun          SocialNetwork      -                               -
 184      184 VHUA                   UDP        X        Fun          VoIP               58267                           -
 185      185 Telegram               TCP/UDP             Acceptable   Chat               -                               -
 186      186 CoD_Mobile             UDP        X        Fun          Game               -                               -
 187      187 Pandora                TCP                 Fun          Streaming          -                               -
 188      188 QUIC                   UDP        X        Acceptable   Web                443                             -
 189      189 Zoom                   UDP                 Acceptable   Video              -                               -
 190      190 EAQ                    UDP        X        Acceptable   Network            6000                            -
 191      191 Ookla                  TCP/UDP             Safe         Network            -                               -
 192      192 AMQP                   TCP        X        Acceptable   RPC                -                               -
 193      193 KakaoTalk              TCP                 Acceptable   Chat               -                               -
 194      194 KakaoTalk_Voice        UDP        X        Acceptable   VoIP               -                               -
 195      195 Twitch                 TCP                 Fun          Video              -                               -
 196      196 DoH_DoT                TCP                 Acceptable   Network            784,853                         853
 197      197 WeChat                 TCP                 Fun          Chat               -                               -
 198      198 MPEG_TS                UDP        X        Fun          Media              -                               -
 199      199 Snapchat               TCP                 Fun          SocialNetwork      -                               -
 200      200 Sina                   TCP                 Fun          SocialNetwork      -                               -
 201      201 GoogleMeet             TCP                 Acceptable   Chat               -                               -
 202      202 IFLIX                  TCP                 Fun          Video              -                               -
 203      203 Github                 TCP                 Acceptable   Collaborative      -                               -
 204      204 BJNP                   UDP        X        Acceptable   System             8612                            -
 205      205 Reddit                 TCP                 Fun          SocialNetwork      -                               -
 206      206 WireGuard              UDP        X        Acceptable   VPN                51820                           -
 207      207 SMPP                   TCP        X        Acceptable   Download           -                               -
 208      208 DNScrypt               TCP/UDP             Acceptable   Network            -                               -
 209      209 TINC                   TCP/UDP    X        Acceptable   VPN                655                             655
 210      210 Deezer                 TCP                 Fun          Music              -                               -
 211      211 Instagram              TCP                 Fun          SocialNetwork      -                               -
 212      212 Microsoft              TCP                 Safe         Cloud              -                               -
 213      213 Starcraft              TCP/UDP    X        Fun          Game               1119                            1119
 214      214 Teredo                 UDP        X        Acceptable   Network            -                               -
 215      215 HotspotShield          TCP                 Potentially Dangerous VPN                -                               -
 216      216 IMO                    UDP        X        Acceptable   VoIP               -                               -
 217      217 GoogleDrive            TCP                 Acceptable   Cloud              -                               -
 218      218 OCS                    TCP                 Fun          Media              -                               -
 219      219 Microsoft365           TCP                 Acceptable   Collaborative      -                               -
 220      220 Cloudflare             TCP                 Acceptable   Web                -                               -
 221      221 MS_OneDrive            TCP                 Acceptable   Cloud              -                               -
 222      222 MQTT                   TCP        X        Acceptable   RPC                -                               1883,8883
 223      223 RX                     UDP        X        Acceptable   RPC                -                               -
 224      224 AppleStore             TCP                 Safe         SoftwareUpdate     -                               -
 225      225 OpenDNS                TCP                 Acceptable   Web                -                               -
 226      226 Git                    TCP        X        Safe         Collaborative      -                               9418
 227      227 DRDA                   TCP        X        Acceptable   Database           -                               -
 228      228 PlayStore              TCP                 Safe         SoftwareUpdate     -                               -
 229      229 SOMEIP                 TCP/UDP    X        Acceptable   RPC                30491,30501,30490               30491,30501
 230      230 FIX                    TCP        X        Safe         RPC                -                               -
 231      231 Playstation            TCP                 Fun          Game               -                               -
 232      232 Pastebin               TCP                 Potentially Dangerous Download           -                               -
 233      233 LinkedIn               TCP                 Fun          SocialNetwork      -                               -
 234      234 SoundCloud             TCP                 Fun          Music              -                               -
 235      235 SteamDatagramRelay     UDP        X        Fun          Game               -                               -
 236      236 LISP                   TCP/UDP    X        Acceptable   Cloud              4342,4341                       -
 237      237 Diameter               TCP        X        Acceptable   Network            -                               3868
 238      238 ApplePush              TCP                 Acceptable   Cloud              -                               -
 239      239 GoogleServices         TCP                 Acceptable   Web                -                               -
 240      240 AmazonVideo            TCP/UDP             Fun          Cloud              -                               -
 241      241 GoogleDocs             TCP                 Acceptable   Collaborative      -                               -
 242      242 WhatsAppFiles          TCP                 Acceptable   Download           -                               -
 243      243 TargusDataspeed        TCP        X        Acceptable   Network            5001,5201                       5001,5201
 244      244 DNP3                   TCP        X        Acceptable   IoT-Scada          -                               20000
 245      245 IEC60870               TCP        X        Acceptable   IoT-Scada          -                               2404
 246      246 Bloomberg              TCP                 Acceptable   Network            -                               -
 247      247 CAPWAP                 UDP        X        Acceptable   Network            5246,5247                       -
 248      248 Zabbix                 TCP        X        Acceptable   Network            -                               10050,10051
 249      249 S7Comm                 TCP        X        Acceptable   IoT-Scada          -                               -
 250      250 Teams                  TCP                 Safe         Collaborative      -                               -
 251      251 WebSocket              TCP        X        Acceptable   Web                -                               -
 252      252 AnyDesk                TCP                 Acceptable   RemoteAccess       -                               -
 253      253 SOAP                   TCP        X        Acceptable   RPC                -                               -
 254      254 AppleSiri              TCP                 Acceptable   VirtAssistant      -                               -
 255      255 SnapchatCall           TCP                 Acceptable   VoIP               -                               -
 256      256 HP_VIRTGRP             TCP        X        Acceptable   Network            -                               -
 257      257 GenshinImpact          TCP/UDP    X        Fun          Game               22102                           -
 258      258 Activision             UDP                 Fun          Game               -                               -
 259      259 FortiClient            TCP                 Safe         VPN                -                               8013,8014
 260      260 Z3950                  TCP        X        Acceptable   Network            -                               210
 261      261 Likee                  TCP                 Fun          SocialNetwork      -                               -
 262      262 GitLab                 TCP                 Fun          Collaborative      -                               -
 263      263 AVASTSecureDNS         UDP                 Safe         Network            -                               -
 264      264 Cassandra              TCP        X        Acceptable   Database           -                               7000,9042
 265      265 AmazonAWS              TCP                 Acceptable   Cloud              -                               -
 266      266 Salesforce             TCP                 Safe         Cloud              -                               -
 267      267 Vimeo                  TCP                 Fun          Streaming          -                               -
 268      268 FacebookVoip           TCP                 Acceptable   VoIP               -                               -
 269      269 SignalVoip             TCP                 Acceptable   VoIP               -                               -
 270      270 Fuze                   TCP                 Acceptable   VoIP               -                               -
 271      271 GTP_U                  TCP        X        Acceptable   Network            -                               -
 272      272 GTP_C                  TCP        X        Acceptable   Network            -                               -
 273      273 GTP_PRIME              TCP        X        Acceptable   Network            -                               -
 274      274 Alibaba                TCP                 Acceptable   Web                -                               -
 275      275 Crashlytics            TCP                 Acceptable   DataTransfer       -                               -
 276      276 Azure                  TCP                 Acceptable   Cloud              -                               -
 277      277 iCloudPrivateRelay     TCP                 Acceptable   VPN                -                               -
 278      278 EthernetIP             TCP        X        Acceptable   Network            -                               44818
 279      279 Badoo                  TCP                 Fun          SocialNetwork      -                               -
 280      280 AccuWeather            TCP                 Fun          Web                -                               -
 281      281 GoogleClassroom        TCP                 Safe         Collaborative      -                               -
 282      282 HSRP                   UDP        X        Acceptable   Network            1985                            -
 283      283 Cybersec               TCP                 Safe         Cybersecurity      -                               -
 284      284 GoogleCloud            TCP                 Acceptable   Cloud              -                               -
 285      285 Tencent                TCP                 Fun          SocialNetwork      -                               -
 286      286 RakNet                 UDP        X        Fun          Game               -                               -
 287      287 Xiaomi                 TCP                 Acceptable   Web                -                               -
 288      288 Edgecast               TCP                 Acceptable   Cloud              -                               -
 289      289 Cachefly               TCP                 Acceptable   Cloud              -                               -
 290      290 Softether              UDP                 Acceptable   VPN                -                               -
 291      291 MpegDash               TCP                 Fun          Media              -                               -
 292      292 Dazn                   TCP                 Fun          Streaming          -                               -
 293      293 GoTo                   TCP                 Acceptable   VoIP               -                               -
 294      294 RSH                    TCP        X        Unsafe       RemoteAccess       -                               -
 295      295 1kxun                  TCP                 Fun          Streaming          -                               -
 296      296 PGM                               X        Acceptable   Network            -                               -
 297      297 IP_PIM                            X        Acceptable   Network            -                               -
 298      298 collectd               UDP        X        Acceptable   System             25826                           -
 299      299 TunnelBear             TCP                 Acceptable   VPN                -                               -
 300      300 CloudflareWarp         UDP                 Acceptable   VPN                -                               -
 301      301 i3D                    UDP        X        Fun          Game               -                               -
 302      302 RiotGames              UDP        X        Fun          Game               -                               -
 303      303 Psiphon                TCP                 Acceptable   VPN                -                               -
 304      304 UltraSurf              TCP        X        Acceptable   VPN                -                               -
 305      305 Threema                TCP        X        Fun          Chat               -                               -
 306      306 AliCloud               TCP        X        Acceptable   Cloud              -                               -
 307      307 AVAST                  TCP        X        Safe         Network            -                               -
 308      308 TiVoConnect            TCP/UDP    X        Fun          Network            2190                            2190
 309      309 Kismet                 TCP        X        Acceptable   Network            -                               -
 310      310 FastCGI                TCP        X        Safe         Network            -                               -
 311      311 FTPS                   TCP        X        Unsafe       Download           -                               -
 312      312 NAT-PMP                UDP        X        Acceptable   Network            5351                            -
 313      313 Syncthing              UDP        X        Fun          Download           -                               -
 314      314 CryNetwork             UDP        X        Fun          Game               -                               -
 315      315 Line                   TCP        X        Acceptable   Chat               -                               -
 316      316 LineCall               UDP        X        Acceptable   VoIP               -                               -
 317      317 AppleTVPlus            TCP                 Fun          Streaming          -                               -
 318      318 DirecTV                TCP                 Fun          Streaming          -                               -
 319      319 HBO                    TCP                 Fun          Streaming          -                               -
 320      320 Vudu                   TCP                 Fun          Streaming          -                               -
 321      321 Showtime               TCP                 Fun          Streaming          -                               -
 322      322 Dailymotion            TCP                 Fun          Streaming          -                               -
 323      323 Livestream             TCP                 Fun          Streaming          -                               -
 324      324 Tencentvideo           TCP                 Fun          Streaming          -                               -
 325      325 IHeartRadio            TCP                 Fun          Music              -                               -
 326      326 Tidal                  TCP                 Fun          Music              -                               -
 327      327 TuneIn                 TCP                 Fun          Music              -                               -
 328      328 SiriusXMRadio          TCP                 Fun          Music              -                               -
 329      329 Munin                  TCP        X        Acceptable   System             -                               4949
 330      330 Elasticsearch          TCP        X        Acceptable   System             -                               -
 331      331 TuyaLP                 UDP        X        Acceptable   IoT-Scada          6667                            -
 332      332 TPLINK_SHP             TCP/UDP    X        Acceptable   IoT-Scada          9999                            9999
 333      333 Source_Engine          UDP        X        Fun          Game               27015                           -
 334      334 BACnet                 UDP        X        Safe         IoT-Scada          47808                           -
 335      335 OICQ                   UDP        X        Acceptable   Chat               8000                            -
 336      336 Heroes_of_the_Storm    UDP        X        Fun          Game               -                               -
 337      337 FbookReelStory         TCP                 Fun          SocialNetwork      -                               -
 338      338 SRTP                   TCP        X        Acceptable   Media              -                               -
 339      339 OperaVPN               TCP                 Acceptable   VPN                -                               -
 340      340 EpicGames              UDP                 Fun          Game               -                               -
 341      341 GeForceNow             TCP                 Fun          Game               -                               -
 342      342 Nvidia                 TCP                 Safe         Web                -                               -
 343      343 BITCOIN                TCP        X        Acceptable   Crypto_Currency    -                               8333
 344      344 ProtonVPN              TCP                 Acceptable   VPN                -                               -
 345      345 Thrift                 TCP/UDP    X        Acceptable   RPC                -                               -
 346      346 Roblox                 TCP                 Fun          Game               -                               -
 347      347 Service_Location_Protocol TCP/UDP    X        Acceptable   RPC                427                             427
 348      348 Mullvad                TCP                 Acceptable   VPN                -                               -
 349      349 HTTP2                  TCP        X        Safe         Web                -                               -
 350      350 HAProxy                TCP        X        Safe         Web                -                               -
 351      351 RMCP                   UDP        X        Safe         System             623                             -
 352      352 Controller_Area_Network TCP/UDP    X        Safe         System             -                               -
 353      353 Protobuf               TCP/UDP    X        Safe         Network            -                               -
 354      354 ETHEREUM               TCP/UDP    X        Acceptable   Crypto_Currency    -                               30303
 355      355 TelegramVoip           TCP                 Acceptable   VoIP               -                               -
 356      356 SinaWeibo              TCP                 Fun          SocialNetwork      -                               -
 357      357 TeslaServices          TCP                 Acceptable   Network            -                               -
 358      358 PTPv2                  UDP        X        Acceptable   System             319,320                         -
 359      359 RTPS                   UDP        X        Acceptable   RPC                7401                            -
 360      360 OPC-UA                 TCP        X        Acceptable   IoT-Scada          -                               4840
 361      361 S7CommPlus             TCP        X        Acceptable   IoT-Scada          -                               -
 362      362 FINS                   TCP/UDP    X        Acceptable   IoT-Scada          9600                            9600
 363      363 EtherSIO               UDP        X        Acceptable   IoT-Scada          6060                            -
 364      364 UMAS                   TCP        X        Acceptable   IoT-Scada          -                               -
 365      365 BeckhoffADS            TCP        X        Acceptable   IoT-Scada          -                               48898
 366      366 ISO9506-1-MMS          TCP        X        Acceptable   IoT-Scada          -                               -
 367      367 IEEE-C37118            TCP/UDP    X        Acceptable   IoT-Scada          4713                            4712
 368      368 Ether-S-Bus            UDP        X        Acceptable   IoT-Scada          5050                            -
 369      369 Monero                 TCP                 Acceptable   Crypto_Currency    -                               -
 370      370 DCERPC                 TCP/UDP    X        Acceptable   RPC                135                             135
 371      371 PROFINET_IO            UDP        X        Acceptable   IoT-Scada          -                               -
 372      372 HiSLIP                 TCP        X        Acceptable   IoT-Scada          -                               4880
 373      373 UFTP                   UDP        X        Acceptable   Download           1044                            -
 374      374 OpenFlow               TCP        X        Acceptable   Network            -                               6653
 375      375 JSON-RPC               TCP        X        Acceptable   RPC                -                               -
 376      376 WebDAV                 TCP        X        Acceptable   Collaborative      -                               -
 377      377 Kafka                  TCP        X        Acceptable   RPC                -                               9092
 378      378 NoMachine              TCP/UDP    X        Acceptable   RemoteAccess       4000                            4000
 379      379 IEC62056               TCP/UDP    X        Acceptable   IoT-Scada          4059                            4059
 380      380 HL7                    TCP        X        Acceptable   RPC                -                               2575
 381      381 Ceph                   TCP        X        Acceptable   DataTransfer       -                               3300,6789
 382      382 GoogleChat             TCP                 Acceptable   Chat               -                               -
 383      383 Roughtime              TCP/UDP    X        Acceptable   System             2002                            2002
 384      384 PrivateInternetAccess  TCP                 Acceptable   VPN                -                               -
 385      385 KCP                    TCP/UDP    X        Acceptable   Network            -                               -
 386      386 Dota2                  TCP                 Fun          Game               -                               -
 387      387 Mumble                 UDP        X        Fun          VoIP               -                               -
 388      388 Yojimbo                UDP        X        Fun          Game               -                               -
 389      389 ElectronicArts         TCP                 Fun          Game               -                               -
 390      390 STOMP                  TCP        X        Acceptable   RPC                -                               61613
 391      391 Radmin                 TCP        X        Acceptable   RemoteAccess       -                               4899
 392      392 Raft                   TCP        X        Acceptable   Network            -                               -
 393      393 CIP                    UDP        X        Acceptable   IoT-Scada          2222                            -
 394      394 Gearman                TCP        X        Acceptable   RPC                -                               4730
 395      395 TencentGames           TCP        X        Fun          Game               -                               -
 396      396 GaijinEntertainment    UDP        X        Fun          Game               20011                           -
 397      397 ANSI_C1222             TCP/UDP    X        Acceptable   IoT-Scada          1153                            1153
 398      398 Huawei                 TCP                 Acceptable   Web                -                               -
 399      399 HuaweiCloud            TCP                 Acceptable   Cloud              -                               -
 400      400 DLEP                   TCP/UDP    X        Acceptable   Network            854                             854
 401      401 BFD                    UDP        X        Acceptable   Network            3784,3785                       -
 402      402 NetEaseGames           UDP        X        Fun          Game               -                               -
 403      403 PathofExile            TCP        X        Fun          Game               -                               -
 404      404 GoogleCall             TCP                 Acceptable   VoIP               -                               -
 405      405 PFCP                   UDP        X        Acceptable   Network            8805                            -
 406      406 FLUTE                  UDP        X        Acceptable   Download           -                               -
 407      407 LoLWildRift            UDP        X        Fun          Game               -                               -
 408      408 TES_Online             TCP        X        Fun          Game               -                               -
 409      409 LDP                    TCP/UDP    X        Acceptable   Network            646                             646
 410      410 KNXnet_IP              TCP/UDP    X        Acceptable   IoT-Scada          3671                            3671
 411      411 Bluesky                TCP                 Fun          SocialNetwork      -                               -
 412      412 Mastodon               TCP                 Fun          SocialNetwork      -                               -
 413      413 Threads                TCP                 Fun          SocialNetwork      -                               -
 414      414 ViberVoip              TCP                 Acceptable   VoIP               -                               -
 415      415 ZUG                    UDP        X        Acceptable   Crypto_Currency    -                               -
 416      416 JRMI                   TCP        X        Acceptable   RPC                -                               1099
 417      417 RipeAtlas              UDP        X        Acceptable   Network            -                               -
 418      418 HLS                    TCP        X        Fun          Media              -                               -
 419      419 ClickHouse             TCP        X        Acceptable   Database           -                               -
 420      420 Nano                   TCP        X        Acceptable   Crypto_Currency    -                               7075
 421      421 OpenWire               TCP        X        Acceptable   RPC                -                               61616
 

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
