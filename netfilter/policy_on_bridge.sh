#!/usr/bin/env bash

# This is script is meant to be used with a pre-existing bridge

#################
# CONFIGURATION #
#################

# Interfaces
LAN=""
WAN=""
BRIDGE=""

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
    echo "This is script is meant to be used with a pre-existing bridge."
    echo "Usage:"
    echo "$0 -l <lan interface> -w <wan interface> -b <bridge interface>"
    exit
fi

echo "=================="
echo "Configuration:"
echo "LAN:    $LAN";
echo "WAN:    $WAN";
echo "BRIDGE: $BRIDGE";
echo "=================="

# Keep marks in the kernel (high-performance).
# With this option, packet marks are stored into the kernel
# connections table and marked packets won't be sent
# to userspace via NFQUEUE.
ENABLE_OFFLOAD_MARKED_PACKETS=true

##########################
# INTERNAL CONFIGURATION #
##########################

# NFQUEUE number. Can specify a single number or a range using :
# to load-balance the traffic across multiple queues
NFQUEUE_NUM="0"
#NFQUEUE_NUM="0:3"

################
# SYSTEM SETUP #
################

SYSCTL_LAN=${LAN//./\/}
SYSCTL_WAN=${WAN//./\/}

# Enable IP forwarding
sysctl -w net.ipv4.ip_forward=1 > /dev/null

# ENABLE iptables over bridging
modprobe br_netfilter

# Allow layer-2 bridge to do an upcall to iptables (as well as arptables or ip6tables) and have filtering travel from layer-2 (bridged frames) through layer 3
sysctl -w net.bridge.bridge-nf-call-iptables=1 > /dev/null 

# Disable reverse path filtering
sysctl -w net.ipv4.conf.all.rp_filter=0  > /dev/null 
sysctl -w net.ipv4.conf.default.rp_filter=0 > /dev/null 
sysctl -w net.ipv4.conf."${SYSCTL_LAN}".rp_filter=0 > /dev/null 
sysctl -w net.ipv4.conf."${SYSCTL_WAN}".rp_filter=0 > /dev/null

conntrack -F

if [ "${ENABLE_OFFLOAD_MARKED_PACKETS}" = true ] ; then
    # Enable connection tracking in the kernel
    modprobe nf_conntrack
    # Enable accounting of conntrack entries
    sysctl -w net.netfilter.nf_conntrack_acct=1 > /dev/null
    # Reset all markers in the kernel connection table
    conntrack -U --mark 0 > /dev/null 2>&1
fi

##########
# BRIDGE #
##########

# ASSUMING THE BRIDGE INTERFACE IS ALREADY UP

############
# IPTABLES #
############

# CLEANUP ALL IPTABLES RULES

# Set the default policies for each of the built-in chains 
iptables -P INPUT ACCEPT
iptables -P FORWARD ACCEPT
iptables -P OUTPUT ACCEPT

# Flush all chains and tables
iptables -F
iptables -X
iptables -t nat -F
iptables -t nat -X
iptables -t mangle -F
iptables -t mangle -X

if [ "${ENABLE_OFFLOAD_MARKED_PACKETS}" = true ] ; then
    # SAVE THE PACKET MARK INTO THE CONNECTION (SO IT CAN BE RESTORED FOR NEWLY ARRIVING PACKETS)
    iptables -t mangle -A POSTROUTING -j CONNMARK --save-mark

    # READ THE MARK PREVIOUSLY SAVED INTO THE CONNECTION AND PUT IT BACK INTO THE PACKET OF THE CONNECTION
    iptables -t mangle -A PREROUTING -j CONNMARK --restore-mark

    # DROP (2)
    iptables -t mangle -A PREROUTING -m mark --mark 2 -j DROP

    # Marked packets leave the chain
    iptables -t mangle -A FORWARD -m mark ! --mark 0 -j RETURN \
	     -m comment --comment "Marked packets leave the chain and don't go to userspace via NFQUEUE"
fi

# SEND TRAFFIC WITHOUTH MARK TO NFQUEUE
if [[ ! "${NFQUEUE_NUM}" =~ ":" ]]; then
    # Single queue
    iptables -t mangle -A FORWARD -j NFQUEUE --queue-num ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${LAN}
    iptables -t mangle -A FORWARD -j NFQUEUE --queue-num ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${WAN}
else
    # Balanced across queues
    iptables -t mangle -A FORWARD -j NFQUEUE --queue-balance ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${LAN}
    iptables -t mangle -A FORWARD -j NFQUEUE --queue-balance ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${WAN}
    :
fi

# Print some info
# bridge link show ${BRIDGE}
# brctl showstp ${BRIDGE} # Make sure  port state is 'forwarding'
# iptables -nvL -t mangle
