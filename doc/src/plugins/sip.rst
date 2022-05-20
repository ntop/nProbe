SIP Plugin
##########

This plugin dissects SIP traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	%SIP_CALL_ID               SIP call-id
	%SIP_CALLING_PARTY         SIP Call initiator
	%SIP_CALLED_PARTY          SIP Called party
	%SIP_RTP_CODECS            SIP RTP codecs
	%SIP_INVITE_TIME           SIP time (epoch) of INVITE
	%SIP_TRYING_TIME           SIP time (epoch) of Trying
	%SIP_RINGING_TIME          SIP time (epoch) of RINGING
	%SIP_INVITE_OK_TIME        SIP time (epoch) of INVITE OK
	%SIP_INVITE_FAILURE_TIME   SIP time (epoch) of INVITE FAILURE
	%SIP_BYE_TIME              SIP time (epoch) of BYE
	%SIP_BYE_OK_TIME           SIP time (epoch) of BYE OK
	%SIP_CANCEL_TIME           SIP time (epoch) of CANCEL
	%SIP_CANCEL_OK_TIME        SIP time (epoch) of CANCEL OK
	%SIP_RTP_IPV4_SRC_ADDR     SIP RTP stream source IP
	%SIP_RTP_L4_SRC_PORT       SIP RTP stream source port
	%SIP_RTP_IPV4_DST_ADDR     SIP RTP stream dest IP
	%SIP_RTP_L4_DST_PORT       SIP RTP stream dest port
	%SIP_RESPONSE_CODE         SIP failure response code
	%SIP_REASON_CAUSE          SIP Cancel/Bye/Failure reason cause
	%SIP_UAC                   SIP user-agent client
	%SIP_UAS                   SIP user-agent server
	%SIP_C_IP                  SIP C IP adresses
	%SIP_CALL_STATE            SIP Call State


A User Agent Client (UAC) is an entity that sends SIP requests and
receives SIP responses.  For example, a SIP telephone is a UAC because
it sends an INVITE request to create a voice call.  A User Agent
Server (UAS) is an entity that receives SIP requests and sends SIP
responses.  A UAS will send SIP REGISTER requests, but these are not
considered to be session creation messages.  A  SIP telephone is also
a UAS because it accepts INVITE requests in order to ring the
telephone and alert the user.  Because of their dual roles, User Agent
Client (:code:`%SIP_UAC`) and Sser Agent Server (:code:`%SIP_UAS`) are
expressed relative to the client and server of the flow.

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
