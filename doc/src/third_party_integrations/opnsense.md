---
title: "OPNsense"
description: "nProbe can be installed on OPNsense as plugin using the Web interface (recommended)"
outline: [2, 3]
---

<a id="opnsenseintegration"></a>

# OPNsense {#opnsense}

nProbe can be installed on OPNsense as plugin using the Web interface (recommended)
or using the command line. In both cases the ntop repository for FreeBSD should be
configured as described at <https://packages.ntop.org/FreeBSD/>.

## Repository Configuration {#repository-configuration}

Log into the OPNsense Shell as administrator (select option 8).

![OPNsense Shell](../img/opnsense_shell.png "OPNsense Shell")

And install the repository using the command provided at <https://packages.ntop.org/FreeBSD/>
(you can cut&paste the command below).

```bash
pkg add https://packages.ntop.org/FreeBSD/FreeBSD:13:amd64/latest/ntop-1.0.pkg
```

::: info
On older OPNsense versions the package prefix is .txz so the command will be pkg add <https://packages.ntop.org/FreeBSD/FreeBSD:12:amd64/latest/ntop-1.0.txz>
:::

The output should look like the below.

![ntop Repo Installation](../img/opnsense_repo_installation.png "ntop Repository Installation")

## Plugin Installation {#plugin-installation}

::: info
Plugins installation in OPNsense requires you to log in as administrator.
:::

Go to the *System* > *Firmware* > *Plugins* page and click on the *Check for updates*.

![Check for updates](../img/opnsense_check_for_updates.png "Plugins - Check For Updates")

The *os-nprobe* plugin should appear in the list and can be installed by
clicking on the *+* symbol.

![Plugins Installation](../img/opnsense_plugins_nprobe.png "nProbe Plugin Installation")

## License Configuration {#license-configuration}

Go to *Services* > *nProbe* > *Settings* > *License* to get all the information required
by the license generator (*Version* and *System ID*).

![nProbe Info](../img/opnsense_nprobe_info.png "nProbe Info")

The license can be installed through the same page by pasting it in the *License Key*
box and saving the configuration. The service should be restarted in the *General* page.

## nProbe Configuration {#nprobe-configuration}

Going to *Services* > *nProbe* > *Settings* > *General* it is possible to configure
the nProbe service. A basic configuration usually includes the below steps:

> 1. Enable the service by checking *Enable nProbe*
> 2. Select the capture *Interface*

![nProbe Configuration](../img/opnsense_nprobe_conf.png "nProbe Configuration")

Save the configuration and run the service.

By default nProbe listens for collectors over ZMQ on a default port. Select the *advanced mode*
to change the port or to enable the *Probe Mode* which is usually useful when deploying nProbe
behind a NAT/Firewall, to let it connect to the collector as a client.

Please note that it is possible to use nProbe to deliver traffic information to a local ntopng
instance by installing the *os-ntopng-enterprise* plugin and enabling the *Connect to nProbe*
flag in the *ntopng Enterprise* plugin configuration page. Please take a look at the
[ntopng plugin guide](https://www.ntop.org/guides/ntopng/third_party_integrations/opnsense.html)
for further information.
