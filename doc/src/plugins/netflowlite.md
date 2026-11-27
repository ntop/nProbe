---
title: "NetFlow-Lite Plugin"
description: "This plugin collects NetFlow-Lite flows and uses them as (simulated) packets as if they where received from a captured device. As the plugin acts as a..."
outline: [2, 3]
---
# NetFlow-Lite Plugin {#netflow-lite-plugin}

This plugin collects NetFlow-Lite flows and uses them as (simulated) packets as if they where received from a captured device. As the plugin acts as a collector for flows sent in NF-Lite format, you need to specify the listening port and an optional number of sequential ports to which flows will be sent. The more ports the more performance can be achieved.

```bash
--nflite <flow listen port low>[:<num ports>]>     | Specify NetFlow-Lite listen port(s) (max 32)
```
