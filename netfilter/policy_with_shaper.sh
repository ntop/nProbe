#!/bin/bash

# This is script is meant to be used to create a bridge and configure Netfilter and tc (traffic limiting)

#
# https://www.digitalocean.com/community/tutorials/a-deep-dive-into-iptables-and-netfilter-architecture
#

# Interfaces
LAN=""
WAN=""
BRIDGE="br0"

while getopts l:w:b:h flag
do
    case "${flag}" in
        l) LAN=${OPTARG};;
        w) WAN=${OPTARG};;
        b) BRIDGE=${OPTARG};;
        h) ;;
    esac
done

if [ -z "$LAN" ] || [ -z "$WAN" ] || [ -z "$BRIDGE" ]; then
    echo "This is script is meant to be used to create a bridge ($BRIDGE)"
    echo "and configure Netfilter and tc (traffic limiting)"
    echo "Usage:"
    echo "$0 -l <lan interface> -w <wan interface>"
    exit
fi

echo "=================="
echo "Configuration:"
echo "LAN:    $LAN";
echo "WAN:    $WAN";
echo "BRIDGE: $BRIDGE";
echo "=================="

# 10 Mbit
WAN_SPEED=10000

STANDARD_BANDWIDTH_POOL=$((WAN_SPEED*80/100))
LOW_BANDWIDTH_POOL=$((WAN_SPEED-STANDARD_BANDWIDTH_POOL))

# Enable iptables over bridging file
modprobe br_netfilter

# Routing needed by DNAT (DNS)
echo 1 > /proc/sys/net/ipv4/ip_forward

iptables -F
iptables -t nat -F
iptables -t mangle -F

# Read CONNMARK and set it in mark
iptables -t mangle -A PREROUTING -j CONNMARK --restore-mark

# DROP (2)
iptables -t mangle -A PREROUTING -m mark --mark 2 -j DROP

# Save CONNMARK (1st rule of POSTROUTING)
iptables -t mangle -A POSTROUTING -j CONNMARK --save-mark

###################### Shaping ######################

tc qdisc del dev $WAN root 2>/dev/null
tc qdisc add dev $WAN root handle 1: htb default 100
tc class add dev $WAN parent 1: classid 1:1 htb rate "$WAN_SPEED"kbit ceil "$WAN_SPEED"kbit
tc class add dev $WAN parent 1: classid 1:11 htb rate "$STANDARD_BANDWIDTH_POOL"kbit ceil "$WAN_SPEED"kbit burst 15k
tc qdisc add dev $WAN parent 1:11 handle 11: sfq perturb 10
tc filter add dev $WAN parent 1:11 protocol ip handle 51 flow hash keys nfct-src divisor 254
# ACCEPT(1) = normal bandwidth
iptables -t mangle -A POSTROUTING -m mark --mark 1 -j CLASSIFY --set-class 1:11
iptables -t mangle -A POSTROUTING -m mark --mark 1 -j RETURN
tc class add dev $WAN parent 1: classid 1:13 htb rate "$LOW_BANDWIDTH_POOL"kbit ceil "$WAN_SPEED"kbit burst 15k
tc qdisc add dev $WAN parent 1:13 handle 13: sfq perturb 10
tc filter add dev $WAN parent 1:13 protocol ip handle 53 flow hash keys nfct-src divisor 254

# SLOW(3) = low bandwidth
iptables -t mangle -A POSTROUTING -m mark --mark 3 -j CLASSIFY --set-class 1:13
iptables -t mangle -A POSTROUTING -m mark --mark 3 -j RETURN

# Send traffic to NFQUEUE
iptables -t mangle -A FORWARD -m mark --mark 0 -j NFQUEUE --queue-num 0 --queue-bypass -m physdev --physdev-in $LAN
iptables -t mangle -A FORWARD -m mark --mark 0 -j NFQUEUE --queue-num 0 --queue-bypass -m physdev --physdev-out $WAN

######################## DNS ########################

# Reset DNS chain
# Define the child DNS (https://dns.norton.com/configureRouter.html)
#iptables -t nat -A PREROUTING -m mark --mark 1 -j ACCEPT
iptables -t nat -A PREROUTING -i $BRIDGE -m mark --mark 97 -j DNAT --to-destination 199.85.126.20
# Define the global DNS
iptables -t nat -A PREROUTING -i $BRIDGE -m mark --mark 98 -j DNAT --to-destination 8.8.4.4

###################################################

# Show all
iptables -nvL -t mangle

