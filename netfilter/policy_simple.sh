#!/usr/bin/env bash

# This is a simple script redirecting traffic to a Netfilter queue 0

iptables -F
iptables -t nat -F
iptables -t mangle -F
iptables -A OUTPUT -p icmp -j NFQUEUE --queue-num 0 --queue-bypass
iptables -A OUTPUT -p udp --sport 53 -j NFQUEUE --queue-num 0 --queue-bypass
iptables -A OUTPUT -p udp --dport 53 -j NFQUEUE --queue-num 0 --queue-bypass
