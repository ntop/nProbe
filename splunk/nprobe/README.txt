This free app is meant for:

- Use with Splunk® Free
- Receive Flow data from nProbe

This App from ntop.org allows you to ingest Flow data in Splunk® Free.

Get started in 3 easy steps:

1. Install and enable the app by completing the setup page
2. Send your Flow to the default listening port: 3333 via nProbe 
3. Begin collecting and analyzing Flow with Splnk!

nprobe -T "%IPV4_SRC_ADDR %L4_SRC_PORT %IPV4_DST_ADDR %L4_DST_PORT %PROTOCOL %IN_BYTES %OUT_BYTES %FIRST_SWITCHED %LAST_SWITCHED %HTTP_URL %HTTP_METHOD %HTTP_RET_CODE %HTTP_REFERER %HTTP_UA %HTTP_MIME %HTTP_HOST %HTTP_SITE %IN_PKTS %OUT_PKTS %IP_PROTOCOL_VERSION %APPLICATION_ID %L7_PROTO_NAME %ICMP_TYPE" --tcp "127.0.0.1:3333" -b 2 -i eth0 --json-labels

Here is how it works:

Flow is sent from your nProbe instance over the TCP protocol to port 3333. The nProbe bundled with the app receives the data and converts it into JSON messages. The data is sent to the local Splunk TCP data input 3333 which was created for you during the app installation.

Please read the Quick Start Guide ($SPLUNK_HOME/ect/apps/nprobe/appserver/static).