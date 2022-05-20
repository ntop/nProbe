BPF Packet Filtering Expressions
################################

This section has been extracted from the tcpdump man page and it describes the syntax of BPF filters you can specify using the --f flag.

The expression consists of one or more primitives. Primitives usually consist of an id (name or number) preceded by one or more qualifiers. There are three different kinds of qualifier:

*type*

Qualifiers say what kind of thing the id name or number refers to. Possible types are host, net and port. E.g., `host foo`, `net 128.3`, `port 20`. If there is no type qualifier, host is assumed.

*dir*

Qualifiers specify a particular transfer direction to and/or from id. Possible directions are src, dst, src or dst and src and dst. E.g., `src foo`, `dst net 128.3`, `src or dst port ftp-data`. If there is no dir qualifier, src or dst is assumed.

*proto*

Qualifiers restrict the match to a particular protocol. Possible protos are: ether, fddi, ip, arp, rarp, decnet, lat, moprc, mopdl, tcp and udp. E.g., `ether src foo`, `arp net 128.3`, `tcp port 21`. If there is no proto qualifier, all protocols consistent with the type are assumed. E.g., `src foo` means `(ip or arp or rarp) src foo` (except the latter is not legal syntax), `net bar` means `(ip or arp or rarp) net bar` and `port 53` means `(tcp or udp) port 53`. 
[`fddi` is actually an alias for `ether`; the parser treats them identically as meaning `the data link level used on the specified network interface.` FDDI headers contain Ethernet-like source and destination addresses, and often contain Ethernet-like packet types, so you can filter on these FDDI fields just as with the analogous Ethernet fields. FDDI headers also contain other fields, but you cannot name them explicitly in a filter expression.] 
In addition to the above, there are some special `primitive` keywords that don't follow the pattern: gateway, broadcast, less, greater and arithmetic expressions. All of these are described below. 
More complex filter expressions are built up by using the words and, or and not to combine primitives. E.g., `host foo and not port ftp and not port ftp-data`. To save typing, identical qualifier lists can be omitted. E.g., `tcp dst port ftp or ftp-data or domain` is exactly the same as `tcp dst port ftp or tcp dst port ftp-data or tcp dst port domain`. 

Allowable primitives
--------------------

*dst host host*

True if the IP destination field of the packet is host, which may be either an address or a name. 

*src host host*

True if the IP source field of the packet is host. 

*host host*

True if either the IP source or destination of the packet is host. Any of the above host expressions can be prepended with the keywords, ip, arp, or rarp as in: ip host host which is equivalent to: ether proto \ip and host host If host is a name with multiple IP addresses, each address will be checked for a match. 

*ether dst ehost*

True if the ethernet destination address is ehost. Ehost may be either a name from /etc/ethers or a number.

*ether src ehost*

True if the ethernet source address is ehost. 

*ether host ehost*

True if either the ethernet source or destination address is ehost. 

*gateway host*

True if the packet used host as a gateway. I.e., the ethernet source or destination address was host but neither the IP source nor the IP destination was host. Host must be a name and must be found in both /etc/hosts and /etc/ethers. (An equivalent expression is ether host ehost and not host host which can be used with either names or numbers for host / ehost.) 

*dst net net*

True if the IP destination address of the packet has a network number of net, which may be either an address or a name. 

*src net net*

True if the IP source address of the packet has a network number of net. 

*net net*

True if either the IP source or destination address of the packet has a network number of net. 

*dst port port*

True if the packet is ip/tcp or ip/udp and has a destination port value of port. The port can be a number or a name used in /etc/services. If a name is used, both the port number and protocol are checked. If a number or ambiguous name is used, only the port number is checked (e.g., dst port 513 will print both tcp/login traffic and udp/who traffic, and port domain will print both tcp/domain and udp/domain traffic). 

*src port port*

True if the packet has a source port value of port. 

*port port*

True if either the source or destination port of the packet is port. Any of the above port expressions can be prepended with the keywords, tcp or udp, as in: tcp src port port which matches only tcp packets. 

*less length*

True if the packet has a length less than or equal to length. This is equivalent to: len <= length.

*greater length*

True if the packet has a length greater than or equal to length. This is equivalent to: len >= length.

*ip proto protocol*

True if the packet is an ip packet of protocol type protocol. Protocol can be a number or one of the names icmp, udp, nd, or tcp. Note that the identifiers tcp, udp, and icmp are also keywords and must be escaped via backslash (\), which is \\ in the C-shell. 

*ether broadcast*

True if the packet is an ethernet broadcast packet. The ether keyword is optional. 

*ip broadcast*

True if the packet is an IP broadcast packet. It checks for both the all-zeroes and all-ones broadcast conventions, and looks up the local subnet mask. 

*ether multicast*

True if the packet is an ethernet multicast packet. The ether keyword is optional. This is shorthand for `ether[0] & 1 != 0`. 

*ip multicast*

True if the packet is an IP multicast packet. 

*ether proto protocol*

True if the packet is of ether type protocol. Protocol can be a number or a name like ip, arp, or rarp. Note these identifiers are also keywords and must be escaped via backslash (\). [In the case of FDDI (e.g., `fddi protocol arp`), the protocol identification comes from the 802.2 Logical Link Control (LLC) header, which is usually layered on top of the FDDI header. ntop assumes, when filtering on the protocol identifier, that all FDDI packets include an LLC header, and that the LLC header is in so-called SNAP format.] 

*decnet src host*

True if the DECNET source address is host, which may be an address of the form `10.123`, or a DECNET host name. [DECNET host name support is only available on Ultrix systems that are configured to run DECNET.] 

*decnet dst host*

True if the DECNET destination address is host. 

*decnet host host*

True if either the DECNET source or destination address is host. 

*ip, arp, rarp, decnet*

Abbreviations for: ether proto p where p is one of the above protocols.

*lat, moprc, mopdl*

Abbreviations for: ether proto p where p is one of the above protocols. Note that ntop does not currently know how to parse these protocols. 

*tcp, udp, icmp*

Abbreviations for: ip proto p where p is one of the above protocols. 

*expr relop expr*

True if the relation holds, where relop is one of >, <, >=, <=, =, !=, and expr is an arithmetic expression composed of integer constants (expressed in standard C syntax), the normal binary operators [+, -, *, /, &, |], a length operator, and special packet data accessors. To access data inside the packet, use the following syntax: proto [ expr : size ] Proto is one of ether, fddi, ip, arp, rarp, tcp, udp, or icmp, and indicates the protocol layer for the index operation. The byte offset, relative to the indicated protocol layer, is given by expr. Size is optional and indicates the number of bytes in the field of interest; it can be either one, two, or four, and defaults to one. The length operator, indicated by the keyword len, gives the length of the packet. 
For example, `ether[0] & 1 != 0` catches all multicast traffic. The expression `ip[0] & 0xf != 5` catches all IP packets with options. The expression `ip[6:2] & 0x1fff = 0` catches only unfragmented datagrams and frag zero of fragmented datagrams. This check is implicitly applied to the tcp and udp index operations. For instance, tcp[0] always means the first byte of the TCP header, and never means the first byte of an intervening fragment.

Primitives may be combined using: 

  - A parenthesized group of primitives and operators 
  - (parentheses are special to the Shell and must be escaped). 
  - Negation (`!` or `not`). 
  - Concatenation (`&&` or `and`). 
  - Alternation (`||` or `or`). 

Negation has highest precedence. Alternation and concatenation have equal precedence and associate left to right. Note that explicit and tokens, not juxtaposition, are now required for concatenation. If an identifier is given without a keyword, the most recent keyword is assumed. For example, not host vs and ace is short for not host vs and host ace which should not be confused with not ( host vs or ace ). Expression arguments can be passed to nProbe as either a single argument or as multiple arguments, whichever is more convenient. Generally, if the expression contains Shell metacharacters, it is easier to pass it as a single, quoted argument. Multiple arguments are concatenated with spaces before being parsed. 


Examples
--------
To select all packets arriving at or departing from sundown:

.. code:: bash

	  nprobe -f "host sundown"

To select traffic between helios and either hot or ace: 

.. code:: bash

	  nprobe -f "host helios and ( hot or ace )"

To select all IP packets between ace and any host except helios: 

.. code:: bash

	  nprobe -f "ip host ace and not helios"

To select all traffic between local hosts and hosts at Berkeley: 

.. code:: bash

	  nprobe -f "net ucb-ether"

To select all ftp traffic through internet gateway snup: (note that the expression is quoted to prevent the shell from (mis-)interpreting the parentheses):

.. code:: bash

	  nprobe -f "gateway snup and (port ftp or ftp-data)"

To select traffic neither sourced from nor destined for local hosts (if you gateway to one other net, this stuff should never make it onto your local net).

.. code:: bash

	  nprobe -f " ip and not net localnet"

To select the start and end packets (the SYN and FIN packets) of each TCP conversation that involves a non-local host. 

.. code:: bash

	  nprobe -f "tcp[13] & 3 != 0 and not src and dst net localnet"

To select IP packets longer than 576 bytes sent through gateway snup: 

.. code:: bash

	  nprobe -f "gateway snup and ip[2:2] > 576"

To select IP broadcast or multicast packets that were not sent via ethernet broadcast or multicast: 

.. code:: bash

	  nprobe -f "ether[0] & 1 = 0 and ip[16] >= 224"

To select all ICMP packets that are not echo requests/replies (i.e., not ping packets): 

.. code:: bash

	  nprobe -f "icmp[0] != 8 and icmp[0] != 0"
