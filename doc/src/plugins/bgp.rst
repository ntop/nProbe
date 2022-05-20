BGP Plugin
##########

Using the nProbe BGP plugin it is possible to export the first ten
Autonomous Systems (ASes) in both the AS-path to the client and to the
server of each flow.

AS-paths, which are well-known mandatory BGP attributes, are
determined by establishing a BGP session with a BGP-router. BGP
sessions are established by an helper script
:code:`bgp_probe_client.pl` which encapsulates all of the
functionality needed to establish and maintain a BGP peering session
and exchange routing update information. Specifically,
the script:

  - Establishes a BGP session with a BGP-router
  - Reads BGP updates to extract AS-paths
  - Sends AS-paths to nProbe


.. figure:: ../img/nprobe_plugins_bgp.png
  :align: center
  :alt: BGP with nProbe and :code:`bgp_probe_client.pl`

  BGP with nProbe and :code:`bgp_probe_client.pl`


The script can be `freely downloaded from here
<https://github.com/ntop/nProbe/blob/master/bgp/bgp_probe_client.pl>`_.

Once the script has been downloaded, it has to be configured with the
IP address and AS of the BGP router, and with the IP address and port
of the host running nProbe.

The configuration is done simply by editing the first lines of the
downloaded script and involves the following variables:

  - :code:`$local_ip` sets the BGP ID (IP address) of the local peer
    of the BGP session. It takes a string in IP dotted decimal
    notation.
  - :code:`$local_as` sets the BGP AS number of the local peer of the
    BGP session. It takes an integer value in the range of a 16-bit
    unsigned integer.
  - :code:`$remote_ip` sets the BGP ID (IP address) of the remote peer
    of the BGP session. It takes a string in IP dotted decimal
    notation.
  - :code:`$remote_as` sets the BGP AS number of the remote peer of the
    BGP session. It takes an integer value in the range of a 16-bit
    unsigned integer.
  - :code:`$nprobe_ip` sets the IP address of the host running
    nProbe. The script will attempt to connect to :code:`$nprobe_ip`
    on :code:`$nprobe_port` to exchange AS-paths information.
  - :code:`$nprobe_port` sets the port on which the nProbe is
    listening for incoming BGP AS-paths. This must be the same port as
    specified in the nProbe option :code:`--bgp-port`.

An example configuration is as follows:

  .. code:: bash

	    # BGP
	    my $local_ip = '192.168.48.2';
	    my $local_as = 65498;
	    my $remote_ip = '192.168.48.1';
	    my $remote_as = 2597;

	    # nProbe
	    my $nprobe_ip   = '127.0.0.1';
	    my $nprobe_port = 4096; 

In the configuration above, the remote peer AS-:code:`2597` BGP-router is at
:code:`192.168.48.1`. The script will connect to the remote peer
announcing itself as AS-:code:`65498` peer
:code:`192.168.48.2`. nProbe it is assumed to be running on the same
:code:`localhost` of the script, listening for connections on port :code:`4096`.

Once the configuration is done, nProbe can be started using option
:code:`--bgp-port <port>`, where :code:`<port>` is set to the value of
:code:`$nprobe_port`. Finally, the script can be run so that it will
connect to nProbe on the specified port.

This plugin defines the following information elements that can be
used in the template as any other regular information element:

.. code:: bash 

	  %SRC_AS_PATH_1                    Src AS path position 1
	  %SRC_AS_PATH_2                    Src AS path position 2
	  %SRC_AS_PATH_3                    Src AS path position 3
	  %SRC_AS_PATH_4                    Src AS path position 4
	  %SRC_AS_PATH_5                    Src AS path position 5
	  %SRC_AS_PATH_6                    Src AS path position 6
	  %SRC_AS_PATH_7                    Src AS path position 7
	  %SRC_AS_PATH_8                    Src AS path position 8
	  %SRC_AS_PATH_9                    Src AS path position 9
	  %SRC_AS_PATH_10                   Src AS path position 10
	  %DST_AS_PATH_1                    Dest AS path position 1
	  %DST_AS_PATH_2                    Dest AS path position 2
	  %DST_AS_PATH_3                    Dest AS path position 3
	  %DST_AS_PATH_4                    Dest AS path position 4
	  %DST_AS_PATH_5                    Dest AS path position 5
	  %DST_AS_PATH_6                    Dest AS path position 6
	  %DST_AS_PATH_7                    Dest AS path position 7
	  %DST_AS_PATH_8                    Dest AS path position 8
	  %DST_AS_PATH_9                    Dest AS path position 9
	  %DST_AS_PATH_10                   Dest AS path position 10

Example
=======

.. code:: bash

	  $ sudo nprobe --bgp-port 9999 --zmq "tcp://*:5556" -i none \
	    -n none --collector-port 2055 \
	    -T "@NTOPNG@ %SRC_TOS %DST_TOS %INPUT_SNMP %OUTPUT_SNMP \
	    %SRC_AS_PATH_1 %SRC_AS_PATH_2 %SRC_AS_PATH_3 %SRC_AS_PATH_4 \
	    %SRC_AS_PATH_5 %SRC_AS_PH_6 %SRC_AS_PATH_7 %SRC_AS_PATH_8 \
	    %DST_AS_PATH_1 %DST_AS_PATH_2 %DST_AS_PATH_3 %DST_AS_PATH_4 \
	    %DST_AS_PATH_5 %DST_AS_PATH_6 %DST_AS_PATH_7 %DST_AS_PATH_8"

The nProbe instance executed above exports via ZMQ (:code:`--zmq
"tcp://*:5556"`) NetFlow data collected on port :code:`2055`
(:code:`--collector-port 2055`), including the first 8 ASes in the
AS-path to the client and to the server. nProbe expects
:code:`bgp_probe_client.pl` to be running and exporting on port
:code:`9999` (:code:`--bgp-port 9999`).
