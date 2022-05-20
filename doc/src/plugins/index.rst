nProbe Plugins
##############

nProbe has been designed as an engine that processes packets and compute basic statistics, and plugins that extend the core with additional capabilities. Each plugin dissects a specific traffic (e.g. SMTP email traffic), but you can enable the use of multiple plugins simultaneously.


.. note::

    nProbe based on the template configuration (-T) will selectively enable plugins and define as many templates as necessary. Their number depends on the plugins enabled and on the fact that you might enable IPv4 and/or IPv6 traffic support.

.. warning::

    Plugins require a license to be enabled. See :ref:`licensing` for further information. Plugin-specific template elements will be ignored if the corresponding plugin is not licensed.


Plugins help can be printed using the option :code:`-H`.

.. code:: bash

    nprobe -H

This will print an extended help with all the available plugins, along with their respective options and supported flow information elements. For example, for the HTTP plugin, the following information will be printed when using :code:`nprobe -H`:

.. code:: bash

    [HTTP Protocol]
      --http-dump-dir <dump dir>                         | Directory where HTTP logs will be dumped
      --http-content-dump-dir <dump dir>                 | Directory where HTTP content (request only) will be dumped
      --http-content-dump-response                       | Dump both HTTP request and response with --http-content-dump-dir
      --http-exec-cmd <cmd>                              | Command executed whenever a directory has been dumped
      --dont-hash-cookies                                | Dump cookie string instead of cookie hash
      --http-verbose-level <level>                       | 0 - Relevant info,  1 - Very verbose (default: 1)
      --http-ports                                       | List of ports used for http protocol (default: 80)
      --proxy-ports                                      | List of ports used for proxy protocol (default: 3128, 8080)
      --http-parse-geolocation                           | Dump geolocation info if explicitly present inside mobile app protocol (e.g., "Nimbuzz")

    Plugin HTTP Protocol templates:
    [NFv9 57652][IPFIX 35632.180][Len 128 varlen] %HTTP_URL                         HTTP URL (IXIA URI)
    [NFv9 57832][IPFIX 35632.360][Len 4 varlen] %HTTP_METHOD                        HTTP METHOD
    [NFv9 57653][IPFIX 35632.181][Len 2] %HTTP_RET_CODE                             HTTP return code (e.g. 200, 304...)
    [NFv9 57654][IPFIX 35632.182][Len 128 varlen] %HTTP_REFERER                     HTTP Referer
    [NFv9 57655][IPFIX 35632.183][Len 256 varlen] %HTTP_UA                          HTTP User Agent
    [NFv9 57656][IPFIX 35632.184][Len 256 varlen] %HTTP_MIME                        HTTP Mime Type
    [NFv9 57659][IPFIX 35632.187][Len 64 varlen] %HTTP_HOST                         HTTP(S) Host Name (IXIA Host Name)
    [NFv9 57833][IPFIX 35632.361][Len 64 varlen] %HTTP_SITE                         HTTP server without host name
    [NFv9 57932][IPFIX 35632.460][Len 256 varlen] %HTTP_X_FORWARDED_FOR             HTTP X-Forwarded-For
    [NFv9 57933][IPFIX 35632.461][Len 256 varlen] %HTTP_VIA                         HTTP Via

The following sections cover the configuration and information elements provided by each individual plugin. Most plugins are available also in source format, but sometimes due to license restrictions (e.g. the plugin has been sponsored by a company that does not want others to access the source code) we are unable to release all plugins in source format.

.. toctree::
    :maxdepth: 2

    bgp
    diameter
    dns
    dhcp
    export
    gtpv0
    gtpv1
    gtpv2
    http
    imap
    pop3
    radius
    rtp
    sip
    smtp
    netflowlite
    modbus
    custom
    
