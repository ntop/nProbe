---
title: "SMTP Plugin"
description: "This plugin dissects SMTP traffic and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements."
outline: [2, 3]
---
# SMTP Plugin {#smtp-plugin}

This plugin dissects SMTP traffic and saves it in dump files as well export the information via NetFlow/IPFIX using the following information elements.

```bash
[NFv9 57657][IPFIX 35632.185][Len 64 varlen] %SMTP_MAIL_FROM                    Mail sender
[NFv9 57658][IPFIX 35632.186][Len 64 varlen] %SMTP_RCPT_TO                      Mail recipient
```

SMTP plugin support option `--smtp-dump-dir <dump dir>`. When this option is used, nProbe writes SMTP dump files in `<dump dir>`. Multiple files are created in a hierarchical YYYY/MM/DD directory tree and each file is at most 1000-lines long.

An extra command can be executed every time nProbe ends dumping a directory using option `--smtp-exec-cmd <cmd>`. Command `<cmd>` is executed using system call `system()`.

::: info
As this plugin dissects traffic packets, it is only available when nProbe is used in probe mode.
:::
