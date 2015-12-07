# nProbe + Kibana
This document provides useful information and examples for using nProbe metrics with Elasticsearch and Kibana.

Requirements:

* nProbe Pro + Export Plugin for Elasticsearch Bulk
* nProbe Plugin(s) matching the desired Dashboard _(ie: DNS, HTTP, etc)_
* Elasticsearch 2.x _(cluster or single server)_
* Kibana 4.2+ _(cluster or single server)_

### First Setup
Before using nProbe metrics in Kibana you need to load the index template, which lets Elasticsearch know which fields should be analyzed in which way.
The provided template file can be adjusted and loaded with the following command:
```
curl -XPUT localhost:9200/_template/nprobe_template --data @template/nprobe-es-template.json
```

### Dashboard Examples
The following example Plugin Dashboards can be imported in Kibana 4 <br>
The Dashboards are a work-in-progress and community contributions are extremely welcome!

#### DNS
Start nProbe exporting ```%DNS_*``` fields:
```
nprobe --elastic "dns;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;" -T "%IPV4_SRC_ADDR %L4_SRC_PORT %IPV4_DST_ADDR %L4_DST_PORT %PROTOCOL %IN_BYTES %OUT_BYTES %FIRST_SWITCHED %LAST_SWITCHED %IN_PKTS %OUT_PKTS %IP_PROTOCOL_VERSION %APPLICATION_ID %L7_PROTO_NAME %ICMP_TYPE %SRC_IP_COUNTRY %DST_IP_COUNTRY %APPL_LATENCY_MS %DNS_QUERY %DNS_QUERY_ID %DNS_QUERY_TYPE %DNS_RET_CODE %DNS_NUM_ANSWERS %DNS_TTL_ANSWER %DNS_RESPONSE" ....
```

#### HTTP
Start nProbe exporting ```%HTTP_*``` fields:
```
nprobe --elastic "http;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;" -T "%IPV4_SRC_ADDR %L4_SRC_PORT %IPV4_DST_ADDR %L4_DST_PORT %PROTOCOL %IN_BYTES %OUT_BYTES %FIRST_SWITCHED %LAST_SWITCHED %IN_PKTS %OUT_PKTS %IP_PROTOCOL_VERSION %APPLICATION_ID %L7_PROTO_NAME %ICMP_TYPE %SRC_IP_COUNTRY %DST_IP_COUNTRY %APPL_LATENCY_MS %HTTP_URL %HTTP_METHOD %HTTP_RET_CODE %HTTP_REFERER %HTTP_UA %HTTP_MIME %HTTP_HOST %HTTP_SITE" ....
```

