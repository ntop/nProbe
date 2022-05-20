Flow Information Elements
#########################

The --T flag enabled users to specify the format of NetFlow v9/IPFIX flows. The format options currently supported by nProbe are those specified in the NetFlow v9 RFC, namely (in square brackets it is specified the field Id as defined in the RFC). As nProbe can be extended by means of plugins, further information elements can be defined based on plugin presence. Following is the exhaustive list of all options available.

.. code:: bash

   ID   NetFlow Label               IPFIX Label                   Description
  -------------------------------------------------------------------------------
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
  [ 18][Len 4] %BGP_IPV4_NEXT_HOP          %bgpNexthopIPv4Address    	
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
  [243][Len 2] %DOT1Q_SRC_VLAN             %dot1qVlanId              	Source VLAN (outer VLAN in QinQ)
  [254][Len 2] %DOT1Q_DST_VLAN             %postdot1qVlanId          	Destination VLAN (outer VLAN in QinQ)
  [ 60][Len 1] %IP_PROTOCOL_VERSION        %ipVersion                	[4=IPv4][6=IPv6]
  [ 61][Len 1] %DIRECTION                  %flowDirection            	Flow direction [0=src->dst, 1=dst->src]
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
  [ 95][Len 4] %APPLICATION_ID             %application_id           	Cisco Application Id
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
  [234][Len 4] %INGRESS_VRFID              %ingressVRFID             	Ingress VRF ID
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
  [335][Len 2] %SELECTOR_NAME                                        	Sampler name
  [NFv9 57552][IPFIX 35632.80][Len 2] %SRC_FRAGMENTS             	Num fragmented packets src->dst
  [NFv9 57553][IPFIX 35632.81][Len 2] %DST_FRAGMENTS             	Num fragmented packets dst->src
  [NFv9 57595][IPFIX 35632.123][Len 4] %CLIENT_NW_LATENCY_MS      	Network RTT/2 client <-> nprobe (msec)
  [NFv9 57596][IPFIX 35632.124][Len 4] %SERVER_NW_LATENCY_MS      	Network RTT/2 nprobe <-> server (msec)
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
  [NFv9 57591][IPFIX 35632.119][Len 16 varlen] %L7_PROTO_NAME             	Layer 7 protocol name
  [NFv9 57592][IPFIX 35632.120][Len 4] %DOWNSTREAM_TUNNEL_ID      	Downstream tunnel identifier (e.g. GTP TEID, VXLAN VNI) or 0 if unknown
  [NFv9 57919][IPFIX 35632.447][Len 2] %DOWNSTREAM_SESSION_ID     	Downstream session identifier (e.g. L2TP) or 0 if unknown
  [NFv9 57660][IPFIX 35632.188][Len 48 varlen] %SSL_SERVER_NAME           	SSL server name
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
  [NFv9 57944][IPFIX 35632.472][Len 8] %SRC_TO_DST_SECOND_BYTES   	Bytes/sec (src->dst) [pro only]
  [NFv9 57945][IPFIX 35632.473][Len 8] %DST_TO_SRC_SECOND_BYTES   	Bytes/sec2 (dst->src) [pro only]
  [NFv9 57961][IPFIX 35632.489][Len 32 varlen] %JA3C_HASH              	JA3 client hash
  [NFv9 57962][IPFIX 35632.490][Len 32 varlen] %JA3S_HASH              	JA3 server hash
  [NFv9 57963][IPFIX 35632.491][Len 48 varlen] %SRC_HOST_NAME          	Symbolic src host name
  [NFv9 57964][IPFIX 35632.492][Len 48 varlen] %DST_HOST_NAME          	Symbolic dst host name
  [NFv9 57965][IPFIX 35632.493][Len 2] %SSL_CIPHER                	SSL Connection Cipher
  [NFv9 57966][IPFIX 35632.494][Len 1] %SSL_UNSAFE_CIPHER         	SSL Safe(0)/unsafe(1) cipher
  [NFv9 57967][IPFIX 35632.495][Len 2] %SSL_VERSION               	SSL Version


Plugin information elements can be found later on this manual.

If you want to specify NetFlow v9 flows in a format similar to v5 flows you can do as follows:

.. code:: bash

	  nprobe -T "%IPV4_SRC_ADDR %IPV4_DST_ADDR %IPV4_NEXT_HOP %INPUT_SNMP %OUTPUT_SNMP %IN_PKTS %IN_BYTES %FIRST_SWITCHED %LAST_SWITCHED %L4_SRC_PORT %L4_DST_PORT %TCP_FLAGS %PROTOCOL %SRC_TOS %SRC_AS %DST_AS %SRC_MASK %DST_MASK"

Note that the fields start with a % and are separated by a space.
