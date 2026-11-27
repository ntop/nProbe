---
title: "Collecting Proprietary Flow Fields"
description: "nProbe is not only able to collect standard flow fields, that is, those defined in the NetFlow/IPFIX RFCs, it is also able to collect custom"
outline: [2, 3]
---
# Collecting Proprietary Flow Fields {#collecting-proprietary-flow-fields}

nProbe is not only able to collect standard flow fields, that is, those
defined in the NetFlow/IPFIX RFCs, it is also able to collect custom
and proprietary flow fields. Proprietary fields are ofen used by
vendors to export data using standard NetFlow v9 or IPFIX for the
transport. Cisco Application Visibility and Control (AVC), Cisco
Medianet, IXIA IxFlow, and SonicWall AppFlow are just a few examples.

Proprietary fields can be declared using a plain text file that has to
be input to nProbe and they will be treated by nProbe as if they were
regular, standard fields. This means that those fields can be exported
simply by specifying them in the template `-T`.

The plain text file has one or more lines in the following format:

```bash
Name          STANDARD_ALIAS  PEN     FieldId         Len     Format
```

- `Name` is just a mnemonic name assigned to the proprietary
  field that is used to identify the field within nProbe, for example
  when indicating the field in the template.

- `STANDARD_ALIAS` is a mapping to a standard field. `NONE`
  can be specified when no mapping to a standard field is
  available. Typically, it is useful to specify this mapping to convert
  proprietary fields into standard, well-known ones.

- `PEN` is the Private Enterprise Number of the proprietary
  field. If the field has no PEN it is safe to specify `0`.

- `FieldId` is the integer number that, along with the PEN,
  uniquely identifies the proprietary field.

- `Len` is the length of the field, expressed in bytes.

- `Format` specifies how to interpret the bytes of the field:

  - `dump_as_uint` interprets the field as an unsigned integer
  - `dump_as_ipv4_address` interprets the field as an IPv4
    address
  - `dump_as_mac_address` interprets the field as a mac address
  - `dump_as_hex` interprets the field as an hexadecimal value
  - `dump_as_ascii` as a text string in ASCII format.

## Available Custom Flow Fields Definitions {#available-custom-flow-fields-definitions}

nProbe comes with preconfigured vendor flow definitions, so users can
immediately collect popular flow fields without having to create a
custom flow configuration. The list of available configurations is
available at <https://github.com/ntop/nProbe/tree/master/custom_fields>

## Example {#example}

A snippet of a text file with the declaration of custom fields for a few
vendors is available below:

```bash
#
# Name                STANDARD_ALIAS  PEN     FieldId         Len     Format
#
# Notes
# - STANDARD_EQUIVALENT
#   If this field is equivalenet/alias of a standard field, put its name here
#   or put NONE if there is no standard equivalent element
#
# Notes
# - Use 0 for no PEN
#
#
# Available formats
# - dump_as_uint
# - dump_as_ipv4_address
# - dump_as_mac_address
# - dump_as_hex
# - dump_as_ascii
#

# Gigamon
HttpReqUrl                    NONE    26866   1       64      dump_as_ascii
DnsQueryName                  NONE    26866   204     64      dump_as_ascii
DnsResponseIpv4Address        NONE    26866   207     4       dump_as_ipv4_address
```

Let's assume proprietary fields `HttpReqUrl` and
`DnsQueryName` have to be exported. one can put the
declarations in a text file `custom-fields.txt` and start nProbe
as

```bash
./nprobe --load-custom-fields "custom-fields.txt" -T  "@NTOPNG@ %HttpReqUrl %DnsQueryName" ...
```

::: info
This is an nProbe Professional feature only.
:::
