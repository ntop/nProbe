Using TLS In Flow Collection/Export
####################################


By default nProbe uses UDP to collect and export flows. However it also supports

  - UDP
  - TCP
  - TLS
  - SCTP (on selected platforms, only for flow export)


You can select the protocol with -n (flow export) and -3 (flow collection) using the format <method>://<address>. Example:

  - :code:`udp://127.0.0.1:1234`
  - :code:`tls://127.0.0.1:1234`

When using TLS, it is compulsory to have a valid TLS certificate that is validated during handshake. The following sections explain how to generate a TLS certificate and what are the command line options available for TLS.

    
TLS Certificates Validation
###########################

When nProbe is used for flow collection over TLS, nProbe needs a valid certificate file. While you can generate certificates using commercial CA (Certification Authorities), sometimes you want to use a self-signed TLS certificate for testing. It can be generated as follows: :code:`openssl req -x509 -nodes -newkey rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365`

The above command will generate two files: key.pem that is the private file, and cert.pem that is the certificate file. Such files needs to be stored on a prototected place and they can be passed to nProbe as follows:

  - :code:`--tls-priv-key <key.pem path>`
  - :code:`--tls-cert <cert.pem path>`

For example the following command collects flows on localhost:2055: :code:`nprobe --tls-priv-key tls/key.pem --tls-cert tls/cert.pem -n none -i none -b 2 -3 tls://localhost:2055`

nProbe can export flows to the above command as follows: :code:`nprobe -i en3 -b2 -t 3 -d 3 -s 3 -n tls://localhost:2055`

If the nProbe collector has been started with a self-signed or non-valid (e.g. expired) certificate, the :code:`--tls-insecure` option can be used to skip TLS certificate validation. Please remember to set --tls-insecure before -n.
