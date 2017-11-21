nprobe is controlled using utility `systemctl` on operating systems
and distributions that use the `systemd` service manager.

Upon successful package installation, the nprobe service is
automatically started on the loopback interface. The service uses a
configuration file that is located at `/etc/nprobe/nprobe.conf` and
that is populated with some defaults during installation. The
configuration file can be edited and extended with any configuration
option supported by nprobe. A service restart is required after
configuration file modifications.

The nprobe service is always started on boot by default. The service
must be disabled to prevent this behavior.

## The nprobe service configuration file

The configuration file is located at `/etc/nprobe/nprobe.conf`.

## Controlling nprobe

To start, stop and restart the nprobe service type:

```
# systemctl start nprobe
# systemctl stop nprobe
# systemctl restart nprobe
```

To prevent nprobe from starting on boot type:

```
# systemctl disable nprobe
```

To start nprobe on boot, assuming it has previously been disabled,
type:

```
# systemctl enable nprobe
```

To check the status of the service, including its output and PID, type:

```
# systemctl status nprobe
```

## Instantiated nprobe services

There are circumstances under which multiple instances of the nprobe
service may run on the same host. To manage a particular `<instance>`
of the service, append an `@<instance>` to the `nprobe` service name.

Typically, `<instance>` corresponds to an interface name (e.g., `eno1`)
or to `none` when nprobe is used in collector mode. This convention
allows an easy identification of the purpose of each
service. Nonetheless, any string is acceptable as value for `<instance>`.

The `<instance>` uniquely identifies a service and its corresponding
configuration file that is located under
`/etc/nprobe/nprobe-<instance>.conf`.

For example, to start two nprobe services, one on interface `eno1` and
another to collect netflow, one can add two configuration files:

```
/etc/nprobe/nprobe-eno1.conf
/etc/nprobe/nprobe-none.conf
```

And then start the services with:

```
# systemctl start nprobe@eno1
# systemctl start nprobe@none
```

Optionally, one may want to start the services on boot with:

```
# systemctl enable nprobe@eno1
# systemctl enable nprobe@none
```

The status of the services above can be controlled with:

```
# systemctl status nprobe@eno1
# systemctl status nprobe@none
```
