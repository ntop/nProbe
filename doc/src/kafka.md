---
title: "Using Kafka with nProbe"
description: "nProbe can use Apache Kafka to export flow information to both As you know, nProbe has always been able to export flows to ntopng in using ZMQ. Recently..."
outline: [2, 3]
---

<a id="usingnprobewithkafka"></a>

# Using Kafka with nProbe {#using-kafka-with-nprobe}

nProbe can use Apache Kafka to export flow information to both
: - flow collectors (flows are exported in JSON format). You can read more about this subject at [page](https://www.ntop.org/guides/nprobe/case_study/exporting_to_kafka.html).
  - ntopng

As you know, nProbe has always been able to export flows to ntopng in using ZMQ. Recently Kafka support has been added to nProbe in order to use Kafka as a ZMQ alternative. This new option has the advantage of allowing nProbe instances to take advanateg of the Kafka architecture in terms of flow persistency (when ntopng is down for maintenance for instance) and workload.

## Exporting Flows To ntopng {#exporting-flows-to-ntopng}

nProbe can export flows to ntopng using the :`--ntopng` command line option:

> - \:`--ntopng zmq://....` is used for ZMQ
> - \:`--ntopng kafka://....` (plaintext) or :`--ntopng kafka-ssl://....` (SSL/TLS) is used for exporting flows towards ntopng using Kafka .

When using Kafka, you need to specify the (comma separated value) list of kafka brokers. Example:

> - \:`--ntopng kafka://192.168.1.2`
> - \:`--ntopng kafka-ssl://192.168.1.2,172.16.24.12`
