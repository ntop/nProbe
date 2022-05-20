Scripting nProbe Using Lua
##########################

nProbe Professional allows the user to script custom actions and behaviors right before a flow is exported out. Scripted actions and behaviors are executed on a per-protocol basis and require the corresponding plugin. For example, scripting actions and behaviors for HTTP flows requires the HTTP plugin installed and available. Similarly, scripting actions and behaviors for DNS flows requires the DNS plugin to be enabled and available.

Scripts have to be written in Lua language (https://www.lua.org/about.html). Lua is a lightweight scripting language that is dynamically typed, runs by interpreting byte-code with a register-based virtual machine, and has automatic memory management with incremental garbage collection, making it ideal for configuration, scripting, and rapid prototyping.

Lua scripts are specified using nProbe option --lua. For example, to tell nProbe to execute script dnsSearch.lua located in folder lua/ one can run the following command

.. code:: bash

	  ./nprobe -i eth1 --lua lua/dnsSearch.lua

But how does nProbe know which function to call? The answer is pretty straightforward: it looks for Lua functions with specific name patterns inside the Lua script received as input.


Lua Functions Naming Convention
-------------------------------

nProbe looks for Lua functions that are structured as

.. code:: bash

	  check<Protocol>Flow

Every nProbe plugin will inspect the input Lua file and try to find its corresponding function. Plugins must be correctly enabled and set up in order to properly call Lua functions.  Plugins that support Lua scripts are outlined in the following table

+------------+---------------------+------------------------+ 
| Protocol   | Lua Function Name   | Lua Global Table Name  | 
+============+=====================+========================+ 
| DHCP       | checkDHCPFlow       | dhcp                   | 
+------------+---------------------+------------------------+ 
| DNS        | checkDNSFlow        | dns                    |
+------------+---------------------+------------------------+ 
| FTP        | checkFTPFlow        | ftp                    | 
+------------+---------------------+------------------------+ 
| GTPV1      | checkGTPV1Flow      | gtpv1                  | 
+------------+---------------------+------------------------+ 
| HTTP       | checkHTTPFlow       | http                   | 
+------------+---------------------+------------------------+ 
| IMAP       | checkIMAPFlow       | imap                   | 
+------------+---------------------+------------------------+ 
| POP        | checkPOPFlow        | pop                    | 
+------------+---------------------+------------------------+ 
| RADIUS     | checkRADIUSFlow     | radius                 | 
+------------+---------------------+------------------------+ 
| SIP        | checkSIPFlow        | sip                    |
+------------+---------------------+------------------------+ 
| SMTP       | checkSMTPFlow       | smtp                   | 
+------------+---------------------+------------------------+

Every plugin, for every flow -- before calling, if available, its corresponding Lua function -- pushes into the Lua stack a global table that contains:

  - Flow elements that are common to any flow
  - Additional elements that are peculiar to flows captured with the plugin

Elements are pushed in the global Lua table as pairs Key - Value.

Common Lua Flow Elements
------------------------

Every flow has some basic elements that are made available, in a global lua table, my plugin. Common information include source and destination ports, for example. The list of common information fields are outlined in the following table


+-----------------------+------------------------------------------------------------------------------------------------+
| Common Element Name   | Description                                                                                    |
+=======================+================================================================================================+
| nprobe.pid            | The process id of the nProbe instance                                                          |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.id               | A unique integer identification to uniquely represent the flow                                 |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.from             | The flow source IP address                                                                     | 
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.to               | The flow destination IP address                                                                |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.sport            | The flow source Layer-3 port                                                                   |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.dport            | The flow destination Layer-3 port                                                              |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.duration         | The duration of the flow in seconds                                                            |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.beginTime        | The epoch of the flow start time                                                               |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.endTime          | The epoch of the flow end time                                                                 |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.l7_proto         | A unique integer number to represent the Layer-7 protocol                                      |
+-----------------------+------------------------------------------------------------------------------------------------+
| flow.protocol         | A unique, standard, integer number to represent the Layer-4 protocol as approved by IANA       | 
+-----------------------+------------------------------------------------------------------------------------------------+


Plugin-Specific Lua Flow Elements
---------------------------------

These additional fields vary depending on the plugin that called the Lua function. Every plugin prefixes its additional fields with a name that resembles the plugin.

For example, the DNS plugin adds the following fields


+--------------------------------------+---------------------------------------------------------------------------------+
| Additional DNS Plugin Element Name   | Description                                                                     |
+======================================+=================================================================================+
| dns.dns_client                       | The IP address of the client that made the DNS request                          |
+--------------------------------------+---------------------------------------------------------------------------------+
| dns.as                               |The Autonomous System of the client that made the DNS request                    |
+--------------------------------------+---------------------------------------------------------------------------------+
| dns.clientcountry                    | The country of the client that made the DNS request                             |
+--------------------------------------+---------------------------------------------------------------------------------+
| dns.clientcity                       | The city of the client that made the DNS request                                |
+--------------------------------------+---------------------------------------------------------------------------------+
| dns.query                            | The DNS query performed                                                         |
+--------------------------------------+---------------------------------------------------------------------------------+
| dns.answers                          | The DNS answers received                                                        |
+--------------------------------------+---------------------------------------------------------------------------------+


Examples
--------

In the remainder of this section are shown examples of nProbe Lua scripting.

*Intercepting DNS Flows*

Let's say a custom action has to be performed right before the export of any DNS flow. Let's also assume, in the interest of pedagogical clarity, that the custom action consists in printing to the console the global flow Lua table dns that has been pushed in by nProbe. To accomplish the action outlined above it suffices to create a Lua script with the following contents

.. code:: bash

  function checkDNSFlow()
     for k, v in pairs(dns) do
        print('k: '..k..' v: '..v..'\n')
     end
  end


The script above iterates over the global dns table and prints all the key-value pairs to the console, one per line. The execution occurs one time for every DNS flow detected, right before exporting the flow. Assuming the script is saved to file dnsSearch.lua under the lua/ directory, then nProbe can be started as follow

.. code:: bash

	  ./nprobe -i en4 --lua lua/dnsSearch.lua  --dns-dump-dir /tmp


Console logs will confirm the DNS plugin is enabled and that the Lua interpreter correctly parsed dnsSearch.lua:

.. code:: bash

	  [...]
	  [LUA] Successfully interpreted lua/dnsSearch.lua
	  DNS log files will be saved in /tmp
	  1 plugin(s) enabled
	  [...]

At this point it is possible fire a query to a DNS server

.. code:: bash

	  $ dig google.it @8.8.8.8


nProbe, right after dig termination, will output to the console the following information

.. code:: bash

	  [...]
	  k: flow.duration v: 1
	  k: dns.dns_client v: 192.168.2.130
	  k: flow.sport v: 54209
	  k: flow.protocol v: 17
	  k: dns.query v: google.it
	  k: flow.endTime v: 1466425499
	  k: flow.beginTime v: 1466425499
	  k: flow.dport v: 53
	  k: dns.answers v:
	  k: dns.as v: 0
	  k: flow.to v: 8.8.8.8
	  k: nprobe.pid v: 3278
	  k: flow.id v: 42
	  k: flow.from v: 192.168.2.130
	  [...]

	  
