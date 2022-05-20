GTPv1 Plugin
############

This plugin dissects GTPv1 signaling information (GTP-C) and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	  %GTPV1_REQ_MSG_TYPE               GTPv1 Request Msg Type
	  %GTPV1_RSP_MSG_TYPE               GTPv1 Response Msg Type
	  %GTPV1_C2S_TEID_DATA              GTPv1 Client->Server TunnelId Data
	  %GTPV1_C2S_TEID_CTRL              GTPv1 Client->Server TunnelId Control
	  %GTPV1_S2C_TEID_DATA              GTPv1 Server->Client TunnelId Data
	  %GTPV1_S2C_TEID_CTRL              GTPv1 Server->Client TunnelId Control
	  %GTPV1_END_USER_IPV4              GTPv1 End User IPv4 Address
	  %GTPV1_END_USER_IMSI              GTPv1 End User IMSI
	  %GTPV1_END_USER_MSISDN            GTPv1 End User MSISDN
	  %GTPV1_END_USER_IMEI              GTPv1 End User IMEI
	  %GTPV1_APN_NAME                   GTPv1 APN Name
	  %GTPV1_RAI_MCC                    GTPv1 RAI Mobile Country Code
	  %GTPV1_RAI_MNC                    GTPv1 RAI Mobile Network Code
	  %GTPV1_RAI_LAC                    GTPv1 RAI Location Area Code
	  %GTPV1_RAI_RAC                    GTPv1 RAI Routing Area Code
	  %GTPV1_ULI_MCC                    GTPv1 ULI Mobile Country Code
	  %GTPV1_ULI_MNC                    GTPv1 ULI Mobile Network Code
	  %GTPV1_ULI_CELL_LAC               GTPv1 ULI Cell Location Area Code
	  %GTPV1_ULI_CELL_CI                GTPv1 ULI Cell CI
	  %GTPV1_ULI_SAC                    GTPv1 ULI SAC
	  %GTPV1_RESPONSE_CAUSE             GTPv1 Cause of Operation

The plugin supports the following command line options that are used to specify where the (optional) GTP log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

	  --gtpv1-dump-dir <dump dir> Directory where GTP logs will be dumped
	  --gtpv1-exec-cmd <cmd>    Command executed whenever a directory has been dumped

Please note that GTP-U is not handled by this plugin but rather by the nProbe core when the --tunnel option is used.

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
