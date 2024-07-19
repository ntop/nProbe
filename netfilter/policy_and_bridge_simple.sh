#!/usr/bin/env bash

#################
# CONFIGURATION #
#################

# Interfaces
LAN=""
WAN=""
BRIDGE="nbr0"

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
    echo "This is script will create a bridge with the provided lan/wan interfaces."
    echo "Usage:"
    echo "$0 -l <lan interface> -w <wan interface> [-b <bridge interface>]"
    exit
fi

echo "=================="
echo "Configuration:"
echo "LAN:    $LAN";
echo "WAN:    $WAN";
echo "BRIDGE: $BRIDGE";
echo "=================="

##########################
# INTERNAL CONFIGURATION #
##########################

NFQUEUE_NUM="0"

################
# SYSTEM SETUP #
################

# ENABLE iptables over bridging
modprobe br_netfilter

# Allow layer-2 bridge to do an upcall to iptables (as well as arptables or ip6tables) and have filtering travel from layer-2 (bridged frames) through layer 3
sysctl -w net.bridge.bridge-nf-call-iptables=1 > /dev/null 

##########
# BRIDGE #
##########

# CREATE THE BRIDGE INTERFACE
# Bridge creation
ip link delete ${BRIDGE} type bridge 2> /dev/null
ip link add name ${BRIDGE} type bridge

# Setup the bridge and bridged interfaces
ip link set ${BRIDGE} up
ip link set ${LAN} up
ip link set ${WAN} up

# Associate the bridge with its bridged interfaces
ip link set ${LAN} master ${BRIDGE}
ip link set ${WAN} master ${BRIDGE}

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

iptables -t mangle -A FORWARD -j NFQUEUE --queue-num ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${LAN}
iptables -t mangle -A FORWARD -j NFQUEUE --queue-num ${NFQUEUE_NUM} --queue-bypass -m physdev --physdev-in ${WAN}

# Print some info
# bridge link show ${BRIDGE}
# brctl showstp ${BRIDGE} # Make sure  port state is 'forwarding'
iptables -nvL -t mangle
