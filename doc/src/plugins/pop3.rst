POP3 Plugin
###########

This plugin dissects POP3 traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information element.

.. code:: bash

	  %POP_USER                         POP3 user login

The plugin supports the following command line options that are used to specify where the (optional) log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

   --pop-dump-dir <dump dir>  Directory where POP3 logs will be dumped
   --pop-exec-cmd <cmd>      Command executed whenever a directory has been dumped

.. note::

	As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
