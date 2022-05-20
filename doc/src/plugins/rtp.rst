RTP Plugin
##########

This plugin dissects RTP traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	%RTP_SSRC                  		RTP Sync Source ID
	%RTP_FIRST_SEQ             		First flow RTP Seq Number
 	%RTP_FIRST_TS              		First flow RTP timestamp
 	%RTP_LAST_SEQ              		Last flow RTP Seq Number
 	%RTP_LAST_TS               		Last flow RTP timestamp
 	%RTP_IN_JITTER             		RTP jitter (ms * 1000)
 	%RTP_OUT_JITTER            		RTP jitter (ms * 1000)
 	%RTP_IN_PKT_LOST           		Packet lost in stream (src->dst)
 	%RTP_OUT_PKT_LOST          		Packet lost in stream (dst->src)
 	%RTP_IN_PKT_DROP           		Packet discarded by Jitter Buffer (src->dst)
 	%RTP_OUT_PKT_DROP          		Packet discarded by Jitter Buffer (dst->src)
 	%RTP_IN_PAYLOAD_TYPE       		RTP payload type
 	%RTP_OUT_PAYLOAD_TYPE      		RTP payload type
 	%RTP_IN_MAX_DELTA          		Max delta (ms*100) between consecutive pkts (src->dst)
 	%RTP_OUT_MAX_DELTA         		Max delta (ms*100) between consecutive pkts (dst->src)
 	%RTP_SIP_CALL_ID           	        SIP call-id corresponding to this RTP stream
 	%RTP_MOS                   		RTP pseudo-MOS (value * 100) (average both directions)
 	%RTP_IN_MOS                		RTP pseudo-MOS (value * 100) (src->dst)
 	%RTP_OUT_MOS               		RTP pseudo-MOS (value * 100) (dst->src)
 	%RTP_R_FACTOR              		RTP pseudo-R_FACTOR (value * 100) (average both directions)
 	%RTP_IN_R_FACTOR           		RTP pseudo-R_FACTOR (value * 100) (src->dst)
 	%RTP_OUT_R_FACTOR          		RTP pseudo-R_FACTOR (value * 100) (dst->src)
 	%RTP_IN_TRANSIT            		RTP Transit (value * 100) (src->dst)
 	%RTP_OUT_TRANSIT           		RTP Transit (value * 100) (dst->src)
 	%RTP_RTT                   		RTP Round Trip Time (ms)
 	%RTP_DTMF_TONES            	        DTMF tones sent (if any) during the call

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
	  
