Radius Plugin
#############

This plugin dissects Radius (unencrypted) traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	  %RADIUS_REQ_MSG_TYPE              RADIUS Request Msg Type
	  %RADIUS_RSP_MSG_TYPE              RADIUS Response Msg Type
	  %RADIUS_USER_NAME                 RADIUS User Name (Access Only)
	  %RADIUS_CALLING_STATION_ID        RADIUS Calling Station Id
	  %RADIUS_CALLED_STATION_ID         RADIUS Called Station Id
	  %RADIUS_NAS_IP_ADDR               RADIUS NAS IP Address
	  %RADIUS_NAS_IDENTIFIER            RADIUS NAS Identifier
	  %RADIUS_USER_IMSI                 RADIUS User IMSI (Extension)
	  %RADIUS_USER_IMEI                 RADIUS User MSISDN (Extension)
	  %RADIUS_FRAMED_IP_ADDR            RADIUS Framed IP
	  %RADIUS_ACCT_SESSION_ID           RADIUS Accounting Session Name
	  %RADIUS_ACCT_STATUS_TYPE          RADIUS Accounting Status Type
	  %RADIUS_ACCT_IN_OCTETS            RADIUS Accounting Input Octets
	  %RADIUS_ACCT_OUT_OCTETS           RADIUS Accounting Output Octets
	  %RADIUS_ACCT_IN_PKTS              RADIUS Accounting Input Packets
	  %RADIUS_ACCT_OUT_PKTS             RADIUS Accounting Output Packets

The plugin supports the following command line options that are used to specify where the (optional) log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

	  --radius-dump-dir <dump dir>    Directory where Radius logs will be dumped
	  --radius-exec-cmd <cmd>  Command executed whenever a directory has been dumped

Note that 3GPP radius extensions are supported by the plugin.

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
