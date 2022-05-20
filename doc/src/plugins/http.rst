HTTP Plugin
###########

This plugin dissects HTTP traffic information and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

.. code:: bash

	[NFv9 57652][IPFIX 35632.180][Len 128 varlen] %HTTP_URL                         HTTP URL (IXIA URI)
	[NFv9 57832][IPFIX 35632.360][Len 4 varlen] %HTTP_METHOD                HTTP METHOD
	[NFv9 57653][IPFIX 35632.181][Len 2] %HTTP_RET_CODE                     HTTP return code (e.g. 200, 304...)
	[NFv9 57654][IPFIX 35632.182][Len 128 varlen] %HTTP_REFERER                     HTTP Referer
	[NFv9 57655][IPFIX 35632.183][Len 256 varlen] %HTTP_UA                          HTTP User Agent
	[NFv9 57656][IPFIX 35632.184][Len 256 varlen] %HTTP_MIME                        HTTP Mime Type
	[NFv9 57659][IPFIX 35632.187][Len 64 varlen] %HTTP_HOST                         HTTP(S) Host Name (IXIA Host Name)
	[NFv9 57833][IPFIX 35632.361][Len 64 varlen] %HTTP_SITE                         HTTP server without host name
	[NFv9 57932][IPFIX 35632.460][Len 256 varlen] %HTTP_X_FORWARDED_FOR             HTTP X-Forwarded-For
	[NFv9 57933][IPFIX 35632.461][Len 256 varlen] %HTTP_VIA                         HTTP Via

The plugin supports the following command line options that are used to specify where the (optional) HTTP log file is saved. As previously described for -P, dumps are nested in directories. It is possible to instruct nProbe to execute a command when a directory (not a log file) if fully dumped (i.e. nProbe has moved to the next directory in time order).

.. code:: bash

  --http-dump-dir <dump dir>                         | Directory where HTTP logs will be dumped
  --http-content-dump-dir <dump dir>                 | Directory where HTTP content (request only) will be dumped
  --http-content-dump-response                       | Dump both HTTP request and response with --http-content-dump-dir
  --http-exec-cmd <cmd>                              | Command executed whenever a directory has been dumped
  --dont-hash-cookies                                | Dump cookie string instead of cookie hash
  --http-verbose-level <level>                       | 0 - Relevant info,  1 - Very verbose (default: 1)
  --http-ports                                       | List of ports used for http protocol (default: 80)
  --proxy-ports                                      | List of ports used for proxy protocol (default: 3128, 8080)
  --http-parse-geolocation                           | Dump geolocation info if explicitly present inside mobile app protocol (e.g., "Nimbuzz")
