nProbe can collect proprietary flow fields as explained in https://www.ntop.org/nprobe/collecting-proprietary-flows-with-nprobe/

This directory contains configuration files for various vendors, so you can use them out of the box with nProbe

# Custom Field Value Mappings in ntopng

Custom field values can be mapped to other values. This is the case,
for example, of application ids. Typically, application ids are sent
as integers as custom field values and it may be useful to have them
mapped into their corresponding application name string.

ntopng can perform such conversion provided that an opportunely
formatted lua file is created and placed into ntopng folder

```
scripts/lua/modules/flow_field_value_maps
```

