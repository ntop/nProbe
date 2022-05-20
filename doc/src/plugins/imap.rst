IMAP Plugin
###########

This plugin dissects IMAP traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information element.

.. code:: bash

	  %IMAP_LOGIN                       Mail sender

The plugin supports the following command line options that are used to specify where the (optional) log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

	  --imap-dump-dir <dump dir>        Directory where IMAP logs will be dumped
	  --imap-exec-cmd <cmd>     Command executed whenever a directory has been dumped
	  --imap-peek-headers           Dump both emails body and headers (default: body only)
	  
.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
