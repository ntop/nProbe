DHCP Plugin
###########

This plugin dissects DHCP traffic and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	[NFv9 57825][IPFIX 35632.353][Len 6] %DHCP_CLIENT_MAC                   MAC of the DHCP client
	[NFv9 57826][IPFIX 35632.354][Len 4] %DHCP_CLIENT_IP                    DHCP assigned client IPv4 address
	[NFv9 57827][IPFIX 35632.355][Len 64 varlen] %DHCP_CLIENT_NAME          DHCP client name
	[NFv9 57895][IPFIX 35632.423][Len 32 varlen] %DHCP_REMOTE_ID            DHCP agent remote Id
	[NFv9 57896][IPFIX 35632.424][Len 48 varlen] %DHCP_SUBSCRIBER_ID        DHCP subscribed Id
	[NFv9 57901][IPFIX 35632.429][Len 1] %DHCP_MESSAGE_TYPE                 DHCP message type

DHCP plugin support option :code:`--dhcp-dump-dir <dump dir>`. When this option is used, nProbe writes DHCP dump files in :code:`<dump dir>`. Multiple files are created in a hierarchical YYYY/MM/DD directory tree and each file is at most 1000-lines long.

An example of a created file is:

.. code:: bash

	#
	 When[uint]    MessageType[ascii:16]   DHCP_Server[ascii:32]   ClientMAC[ascii:32]     ClientIP[ascii:32]      ClientName[ascii:32]    TransactionId   LeaseTime[uint] SubscriberId[ascii:32]  AgentRemoteId[ascii:48]
	#
	1593701239      Offer(2)        192.168.2.1     00:25:90:D4:CC:F9       192.168.2.222           0xF8494A18      86400
	1593701239      ACK(5)  192.168.2.1     00:25:90:D4:CC:F9       192.168.2.222           0xF8494A18      86400
	1593701250      ACK(5)  192.168.2.1     00:25:90:D4:CC:F9       192.168.2.222           0x4EEC7C3B      86400
	1593701256      ACK(5)  192.168.2.1     00:25:90:D4:CC:F9       192.168.2.222           0xBA0E6A54      86400

An extra command can be executed every time nProbe ends dumping a directory using option :code:`--dhcp-exec-cmd <cmd>`. Command :code:`<cmd>` is executed using system call :code:`system()`.

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
	