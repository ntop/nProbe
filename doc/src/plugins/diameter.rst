SIP Plugin
##########

This plugin dissects Diameter traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	%DIAMETER_REQ_MSG_TYPE		 DIAMETER Request Msg Type
	%DIAMETER_RSP_MSG_TYPE     	 DIAMETER Response Msg Type
	%DIAMETER_REQ_ORIGIN_HOST  	 DIAMETER Origin Host Request
	%DIAMETER_RSP_ORIGIN_HOST  	 DIAMETER Origin Host Response
	%DIAMETER_REQ_USER_NAME    	 DIAMETER Request User Name
	%DIAMETER_RSP_RESULT_CODE  	 DIAMETER Response Result Code
	%DIAMETER_EXP_RES_VENDOR_ID	 DIAMETER Response Experimental Result Vendor Id
	%DIAMETER_EXP_RES_RESULT_CODE	 DIAMETER Response Experimental Result Code
	%DIAMETER_HOP_BY_HOP_ID    	 DIAMETER Hop by Hop Identifier
	%DIAMETER_CLR_CANCEL_TYPE  	 DIAMETER Cancellation Type
	%DIAMETER_CLR_FLAGS        	 DIAMETER CLR Flags
	%DIAMETER_FRAMED_IP_ADDR   	 DIAMETER Framed IP
	%DIAMETER_SERVED_IP_ADDR   	 DIAMETER Served IP
	%DIAMETER_CALLING_STATION_ID	 DIAMETER Calling Station Id
	%DIAMETER_CALLED_STATION_ID	 DIAMETER Called Station Id
	%DIAMETER_SUBSCRIPTION_ID  	 DIAMETER Subscription Id
	%DIAMETER_CALLING_PARTY_ADDRESS	 DIAMETER Calling Party Address
	%DIAMETER_CALLED_PARTY_ADDRESS	 DIAMETER Called Party Address

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
