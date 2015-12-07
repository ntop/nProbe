# nProbe + Elasticsearch & Kibana
This document provides useful information and examples for using nProbe metrics with Elasticsearch and Kibana.

Requirements:

* nProbe Pro + Export Plugin for Elasticsearch Bulk
* nProbe Plugin(s) matching the desired Dashboard _(ie: DNS, HTTP, etc)_
* Elasticsearch 2.x _(cluster or single server)_
* Kibana 4.2+ _(cluster or single server)_

<br>

### Setup
----------------------


#### Import Demo Dashboards
A script is provided to automatically load all nProbe demo dashboards and settings:
```
# ./load.sh -url http://localhost:9200 
```

Once imported, Kibana objects should become available:
![](https://cloud.githubusercontent.com/assets/1423657/11626806/0305688e-9ce6-11e5-8d63-817e511f15be.png)

#### Index Template
Before shipping nProbe metrics in Kibana, an index template is required to let Elasticsearch know which fields should be analyzed in which way. The provided template file can be adjusted and loaded with the following command:
```
curl -XPUT localhost:9200/_template/nprobe_template --data @template/nprobe-es-template.json
```
#### Index Pattern
Once the metrics are indexed in ES, a matching Kibana index pattern should be created as follows:

###### Path: ```Settings > Indices ```
![](https://cloud.githubusercontent.com/assets/1423657/11626807/030a0768-9ce6-11e5-8115-5c8f20841186.png)

#### Scripted Fields
Users can optionally generate scripted fields "on-the-fly" in Kibana when needed. 
The following example generate new flow fields with sum of IN/OUT Bytes and Packets:

![](https://cloud.githubusercontent.com/assets/1423657/11626821/18a51fb8-9ce6-11e5-986d-5d232486dd96.png)


----------------------
<br><br>

### Dashboard Examples
The provided Dashboards are a work-in-progress and community contributions are extremely welcome!

----------------------

##### DNS
Start nProbe exporting ```%DNS_*``` fields:
```
nprobe --elastic "dns;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;" -T "%IPV4_SRC_ADDR %L4_SRC_PORT %IPV4_DST_ADDR %L4_DST_PORT %PROTOCOL %IN_BYTES %OUT_BYTES %FIRST_SWITCHED %LAST_SWITCHED %IN_PKTS %OUT_PKTS %IP_PROTOCOL_VERSION %APPLICATION_ID %L7_PROTO_NAME %ICMP_TYPE %SRC_IP_COUNTRY %DST_IP_COUNTRY %APPL_LATENCY_MS %DNS_QUERY %DNS_QUERY_ID %DNS_QUERY_TYPE %DNS_RET_CODE %DNS_NUM_ANSWERS %DNS_TTL_ANSWER %DNS_RESPONSE" ....
```
![](https://cloud.githubusercontent.com/assets/1423657/11629699/f63840f4-9cf9-11e5-90cb-0b3ad709367f.png)
----------------------

##### HTTP
Start nProbe exporting ```%HTTP_*``` fields:
```
nprobe --elastic "http;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;" -T "%IPV4_SRC_ADDR %L4_SRC_PORT %IPV4_DST_ADDR %L4_DST_PORT %PROTOCOL %IN_BYTES %OUT_BYTES %FIRST_SWITCHED %LAST_SWITCHED %IN_PKTS %OUT_PKTS %IP_PROTOCOL_VERSION %APPLICATION_ID %L7_PROTO_NAME %ICMP_TYPE %SRC_IP_COUNTRY %DST_IP_COUNTRY %APPL_LATENCY_MS %HTTP_URL %HTTP_METHOD %HTTP_RET_CODE %HTTP_REFERER %HTTP_UA %HTTP_MIME %HTTP_HOST %HTTP_SITE" ....
```
![](https://cloud.githubusercontent.com/assets/1423657/11629700/f6385c7e-9cf9-11e5-84dd-810eb60c9c23.png)
----------------------

