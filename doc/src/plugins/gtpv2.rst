GTPv2 Plugin
############

This plugin dissects GTPv2 signaling information (GTP-C) and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	  %GTPV2_REQ_MSG_TYPE               GTPv2 Request Msg Type
	  %GTPV2_RSP_MSG_TYPE               GTPv2 Response Msg Type
	  %GTPV2_C2S_S1U_GTPU_TEID          GTPv2 Client->Svr S1U GTPU TEID
	  %GTPV2_C2S_S1U_GTPU_IP            GTPv2 Client->Svr S1U GTPU IP
	  %GTPV2_S2C_S1U_GTPU_TEID          GTPv2 Srv->Client S1U GTPU TEID
	  %GTPV2_S2C_S1U_GTPU_IP            GTPv2 Srv->Client S1U GTPU IP
	  %GTPV2_END_USER_IMSI              GTPv2 End User IMSI
	  %GTPV2_END_USER_MSISDN            GTPv2 End User MSISDN
	  %GTPV2_APN_NAME                   GTPv2 APN Name
	  %GTPV2_ULI_MCC                    GTPv2 Mobile Country Code
	  %GTPV2_ULI_MNC                    GTPv2 Mobile Network Code
	  %GTPV2_ULI_CELL_TAC               GTPv2 Tracking Area Code
	  %GTPV2_ULI_CELL_ID                GTPv2 Cell Identifier
	  %GTPV2_RESPONSE_CAUSE             GTPv2 Cause of Operation

The plugin supports the following command line options that are used to specify where the (optional) GTP log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

	  --gtpv2-dump-dir <dump dir> Directory where GTP logs will be dumped
	  --gtpv2-exec-cmd <cmd>    Command executed whenever a directory has been dumped

Please note that GTP-U is not handled by this plugin but rather by the nProbe core when the --tunnel option is used.

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
