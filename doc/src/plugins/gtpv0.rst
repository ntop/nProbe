GTPv0 Plugin
############

This plugin dissects GTPv0 signaling information (GTP-C) and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	  %GTPV0_REQ_MSG_TYPE               GTPv0 Request Msg Type
	  %GTPV0_RSP_MSG_TYPE               GTPv0 Response Msg Type
	  %GTPV0_TID                        GTPv0 Tunnel Identifier
	  %GTPV0_APN_NAME                   GTPv0 APN Name
	  %GTPV0_END_USER_IP                GTPv0 End User IP Address
	  %GTPV0_END_USER_MSISDN            GTPv0 End User MSISDN
	  %GTPV0_RAI_MCC                    GTPv0 Mobile Country Code
	  %GTPV0_RAI_MNC                    GTPv0 Mobile Network Code
	  %GTPV0_RAI_CELL_LAC               GTPv0 Cell Location Area Code
	  %GTPV0_RAI_CELL_RAC               GTPv0 Cell Routing Area Code
	  %GTPV0_RESPONSE_CAUSE             GTPv0 Cause of Operation

The plugin supports the following command line options that are used to specify where the (optional) GTP log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

	  --gtpv0-dump-dir <dump dir> Directory where GTP logs will be dumped
	  --gtpv0-exec-cmd <cmd>    Command executed whenever a directory has been dumped

Please note that GTP-U is not handled by this plugin but rather by the nProbe core when the --tunnel option is used.
