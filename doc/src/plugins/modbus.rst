Modbus Plugin
#############

This plugin dissects Modbus (https://en.wikipedia.org/wiki/Modbus) over TCP traffic information and saves it in dump files. No information is exported via NetFlow/IPFIX.

The plugin supports the following command line options that are used to specify where the (optional) Modbus log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

  --modbus-dump-dir <dump dir>       Directory where modbus logs will be dumped
  --modbus-exec-cmd <cmd>            Command executed whenever a directory has been dumped
  --modbus-idle-timeout <duration>   Modbus idle flow timeout set to 120 seconds
       
.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
