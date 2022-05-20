Exporting to Apache Kafka
#########################

nProbe, if configured with the export plugin, can send flows to one or more Kafka brokers in a cluster. Initially, the nProbe tries to contact one or more user-specified brokers to retrieve Kafka cluster metadata. Metadata include the list of brokers available in the cluster that are responsible for a given user-specified topic. Once the nProbe has the list of brokers available, it starts sending flows to them. The user can also decide to compress data and specify if he/she:

  - Doesn't want to wait for ACKs 
  - Wants to receive an ACK only from the Kafka leader
  - Wants to receive an ACK from every replica 


*Example*

Assuming there is a Zookeeper on localhost port 2181 that manages the Kafka cluster, it is possible to ask for the creation of a new Kafka topic. Let's say the topic is called “topicFlows” and we want it to be split across three partitions with replication factor two. The command that has to be issued is the following

.. code:: bash

   ./bin/kafka-topics.sh --zookeeper localhost:2181 --create --topic topicFlows --partitions 3 --replication-factor 2


Now that the Kafka topic has been created, it is possible to execute nProbe and tell the instance to export to Kafka topic "topicFlows". We also tell the instance a list of three brokers running on localhost (on ports 9092, 9093 and 9094 respectively) that will be queried at the beginning to obtain kafka cluster/topic metadata.

.. code:: bash

   nprobe --kafka "127.0.0.1:9092,127.0.0.1:9093,127.0.0.1:9094;topicFlows"

   
At this point the nProbe instance is actively exporting flows to the Kafka cluster. To verify that everything is running properly, we can take messages out of the Kafka topic with the following command

.. code:: bash

   ./bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topic topicFlows --from-beginning 

Additional information and fine-tuning of Kafka export can be found at
the following blog posts:

- https://www.ntop.org/tutorials/stream-that-flow-how-to-inject-nprobe-cento-flows-in-a-kafka-cluster/
- https://www.ntop.org/nprobe/improved-nprobe-kafka-export-support-theory-and-practice/

Custom Configuration Properties
===============================

nProbe allows custom configuration properties to be set. Being able to
set configuration properties is key as it allows to decide if
highthroughput is the name of the game, or if a low latency service is
required.

Among all the configuration properties available, for the
sake of performance tuning, it is worth mentioning:

- :code:`batch.num.messages`: the minimum number of messages – flows,
  in the nProbe parlance – to wait for to accumulate in the queue
  before sending off a message set.
- :code:`queue.buffering.max.ms`: how long to wait for
  :code:`batch.num.messages` to fill up in the queue. A lower value improves
  latency  at the cost of lower throughput and higher per-message
  overhead. A higher value improves throughput at the expense of latency.

Configuration properties are set using nProbe option
:code:`--kafka-conf`, followed by the property name and the property
value, separated by an equal :code:`=` sign. The option can be
repeated multiple times, when multiple properties need to be set.

For example, to set the :code:`queue.buffering.max.ms` to 1000 ms, the
autocommit interval :code:`topic.auto.commit.interval.ms` to 2000 ms,
and the messages debug, one can run nProbe adding the following:

.. code:: bash

	  --kafka-conf debug=msg --kafka-conf queue.buffering.max.ms=1000 --kafka-conf=topic.auto.commit.interval.ms=2000

The full list of configuration properties that can be configured is
available at
https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md


TLS/SSL Encryption
==================

To add TLS/SSL encryption to Kafka, both the brokers and the nProbe
client need to be opportunely configured as described in
https://github.com/edenhill/librdkafka/wiki/Using-SSL-with-librdkafka.

Specifically, configuration properties such as :code:`ssl.ca.location`
and :code:`ssl.key.location` can be set using nProbe option
:code:`--kafka-conf` as described in the section above.

Quick Start
===========

For the impatient, please follow the instructions below to build a basic
Kafka installation and run it.

Download Kafka:

.. code:: bash

   wget https://downloads.apache.org/kafka/2.8.0/kafka_2.13-2.8.0.tgz
   tar -xzf kafka_2.13-2.8.0.tgz

Run the Kafka Zookeeper:

.. code:: bash

   cd kafka_2.13-2.8.0
   bin/zookeeper-server-start.sh config/zookeeper.properties

In another terminal run the Kafka server:

.. code:: bash

   cd kafka_2.13-2.8.0
   bin/kafka-server-start.sh config/server.properties

Create a topic:

.. code:: bash

   cd kafka_2.13-2.8.0
   bin/kafka-topics.sh --zookeeper localhost:2181 --create --topic topicFlows --partitions 3 --replication-factor 1

Run nProbe:

.. code:: bash

   sudo nprobe -i eno1 --kafka "127.0.0.1:9092;topicFlows"

