Export Plugin
#############

nProbe allows you to export data do external sources. This plugin allows to export flows to ElasticSearch and kafka. Following are the configuration options required:

.. code:: bash

  --elastic <format>         | Enable export to ElasticSearch
                             | Format: <index type>;<index name>;<es URL>;<es user>:<es pwd>
                             | Example:
                             | --elastic "flows;nprobe-%Y.%m.%d;http://localhost:9200/_bulk;user:pwd"
                             | Note: the <index name> accepts the format supported by strftime().

  --kafka <brokers>;<topic>[;<ack>;<compr>]
                             | Send flows to Apache Kafka brokers obtained by metadata information
                             | <host1>[:<port1>],<host2>[:<port2>]... Initial brokers list used to
                             | receive metadata information
                             | <topic>       Message topic
                             | <0|1|-1>      0=Don't wait for ack
                             |               1=Leader ack is enough
                             |               -1=All replica must ack
                             | <compression> Compression type: none, gzip, snappy
                             | Example --kafka localhost;test;0;gzip

The exported fields are the ones specified into the nProbe template parameter `-T`. See `nprobe -H` for
the full list of supported fields.

You can read more here:

- https://www.ntop.org/ntopng/ntopng-goes-elastic-introducing-elasticsearch-6-support
- https://www.ntop.org/ntopng/exploring-your-traffic-using-ntopng-with-elasticsearchkibana/

