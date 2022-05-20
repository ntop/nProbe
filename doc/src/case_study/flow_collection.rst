.. _UnderstandingFlowCollection:

Understanding How Flow Collection Works
#######################################

nProbe supports the collection of NetFlow v5 and v9, jFlow, IPFIX and
sFlow. Specifically, it extract flows carried in NetFlow v5 and v9, jFlow and
IPFIX, whereas it creates flows starting from the sampled packets
carried within sFlow.

Once the flows are extracted or created, they can be processed and
exported as discussed thoroughly in the present guide, regardless of
the source they are coming from. In other words, after the collection,
a flow for nProbe is just an element with certain fields (e.g., client ip, server
ip, client to server bytes, server to client bytes, and so on) and it
will no longer matter if it has been extracted from NetFlow or created
out of sFlow.

After the collection, flows are temporarily placed by nProbe in an
internal flow cache. Once in the cache, flows may have their traffic
counters updated, for example when multiple NetFlow packets carry
updates for the same flow or when multiple sFlow sampled packets are
actually samples of the same flow.

Flows stay in the cache for a certain amount of time, which is
configurable using timeout options :code:`-t`
and :code:`-d`. After this time, flows are exported downstream
according to the specified configuration (for example to text files,
Kafka, ElasticSearch and so on) and limited to the fields specified in
the template with option :code:`-T`.

Placing flows in the cache and updating their traffic counters before
the export may have the beneficial effect to reduce the volume of
exported flows as, rather than exporting a flow multiple times, a
single flow carrying the (total) updated traffic counters is exported
just one time. However, this comes at the cost of using extra memory
and extra time to handle the cache and do lookups into it. Therefore,
when it is not necessary or when it is not desired to have collected
flows placed into the cache, time and memory can be saved by
completely bypassing the internal flows cache using option :code:`--collector-passthrough`.

Option :code:`--collector-passthrough` exports flows to the configured
ZMQ endpoints as-is, right after their collection, without placing
them into the cache. Flows are exported as-is with all their fields as
found in the NetFlow/jFlow/IPFIX. This option only works with NetFlow/jFlow/IPFIX
(i.e., sFlow is not supported), causes the template :code:`-T` to be
ignored, and only exports to ZMQ. Using :code:`--collector-passthrough`
gives the highest collection throughput.

Rather than using :code:`--collector-passthrough`, one can decide to
just use :code:`--disable-cache`. Just disabling the cache rather than
using the :code:`--collector-passthrough` yields a lower throughput but
does not have the limitations discussed above. Hence, the
:code:`-T` template option will be honoured and flows will be exported to
any of the configured downstream destinations (not just ZMQ).

Filtering and Reforging Collected NetFlow Flows
===============================================

Sometimes when collecting NetFlow flows there are two needs:

- Discard specific flows coming from selected interfaces/routers
- Reforge NetFlow probe IP address as it might have been modified (e.g. through NAT)

nProbe allows you to do that using the :code:`--collector-nf-reforge <path>` command line option. The
format of the reforge file is the one shown in the example below (note that fields are tab separated):

.. code:: bash

       #
       # NOTE
       # - ReforgedIP set to * means no IP reforge
       # - ListOfAllowedInterfaces set to * means all inIfIfx
       # - valid ListOfAllowedInterfaces entries must be in the 0-63 range
       #
       # ExporterIP    ReforgedIP      ListOfAllowedInterfaces
       127.0.0.1       10.0.24.25      12
       192.168.1.1     192.168.1.1     1,2,3,4
       192.168.1.21    *               1,12,43,98

For those routers for which you want to apply some filtering/reforging you need to specify a line
in the file (to date only IPv4 is supported) where you specify the reforged IP address (or * for using
the original IP and this not reforging it) and the list of allowed ingressInterfaceId's that are accepted
for this exported. In the first line, only the interface 12 as allowed, meaning that all other collected flows
originated by other interfaceIds will be discarded and not considered by nProbe. For exporters not listed
in the file, no IP reforging and no interfaceId filtering is performed (i.e. everything will go through).
