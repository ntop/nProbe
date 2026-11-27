---
title: "Hardware Sizing"
description: "Indications on process, memory and disk required are given below for networks with three different sizes."
outline: [2, 3]
---
# Hardware Sizing {#hardware-sizing}

Indications on process, memory and disk required are given below for networks with three different sizes.

## Packets to Flow {#packets-to-flow}

The table below highlights the requirements for capturing packets and transforming them in flows.

|  | Small Network | Medium Network | Large Network | Very Large Network |
|---|---|---|---|---|
| Traffic | &lt; 100 Mbps | Between 100 Mbps and 1 Gbps | Between 1 and 10 Gbps | Above 10 Gbps |
| Application | 1 x nProbe | 2 x nProbes | 4-8 x nProbes | Use nProbe Cento |
| Traffic Capture | None | 2 RSS Queues + PF_RING | 4-8 RSS Queues + ZC | 8 RSS + PF_RING ZC |
| Flow Export Rate | &lt; 100 FPS | &lt; 1000 FPS | &lt; 3000 FPS | 3000+ FPS |
| Active Flow Cache | Thousands | Hundreds of Thousands | A few Millions | Tenth of Millions |
| CPU Type | 2 cores | 2 cores+ | 4 cores+ | 8 cores+ |
| Memory | 2 GB | 2 GB+ | 4-8 GB+ | 16 GB+ |

Note: FPS stands for flows/sec

## Flow Collection/Proxy {#flow-collection-proxy}

In flow collection nProbe performance changes according to the number of device exporting to it. In case of large
number of flows to be collected, it is advised to load-balance flows across multiple nProbe instances. This by
partitioning probe devices across multiple nprobe instances. In flow collection flows are usually saved on disk or
dumped on a database, thus adding disk speed into the equation. The expected performance figures - per core / per nProbe instance -
range from 50 to 90k FPS when usinf the nProbe cache for merging flows. If you want to bypass the cache (-–collector-passthrough)
you can expect to go over over 200k FPS. [Here](https://www.ntop.org/nprobe/introducing-nprobe-9-0-traffic-behaviour-analysis-and-high-speed-flow-collection-even-behind-a-firewall/) can find a detailed report about nProbe performance

## Plugins {#plugins}

Enabling plugins increases requirements based on traffic type. Increase +25% system load enablign plugins such as (but not limited to) DNS, VoIP and email.

## Disk {#disk}

Unless you decide to store data on disk (eg. using -P) no disk space is really necessary to operate nProbe.

## Operating System {#operating-system}

Even though nprobe supports both Linux, MacOS and Windows, we advise to use Linux for best perfomance.

## Multiple nProbe per Host Deployment {#multiple-nprobe-per-host-deployment}

If you have more traffic you can scale up by spawning multiple instances (make sure you
set core affinity to avoiding nProbe to step on each other toes).
