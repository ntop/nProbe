#!/usr/bin/perl

#use strict;

use Getopt::Std;

use Net::BGP::Process;
use Net::BGP::Peer;
use Net::BGP::Refresh;

use threads;
use Thread::Queue;

#############

#
# Configuration parameters
#

# BGP
my $local_ip = '192.168.48.2';
my $local_as = 65498;
my $remote_ip = '192.168.48.1';
my $remote_as = 2597;

# nProbe
my $nprobe_ip   = '127.0.0.1';
my $nprobe_port = 4096;

#############

my $max_queue_len = 32768;
my $debug = 0;
my $dump_file = "";

%options=();
getopts("i:d:vh",\%options);

help()                   if defined $options{h};
$debug = 1               if defined $options{v};
$dump_file = $options{d} if defined $options{d};
($nprobe_ip,$nprobe_port) = split(/:/, $options{i}) if defined $options{i};

############

my $bgp  = Net::BGP::Process->new();
my $peer = Net::BGP::Peer->new(
    Start    => 1,
    ThisID   => $local_ip,
    ThisAS   => $local_as,
    PeerID   => $remote_ip,
    PeerAS   => $remote_as,
    Passive  => 1,
    UpdateCallback       => \&my_update_callback
    );

my $refresh = Net::BGP::Refresh->new(
    AFI      => Net::BGP::AFI_IP4,
    SAFI     => Net::BGP::SAFI_BOTH,
    );

my %as_paths = ();
my $num_updates : shared = 0;
my $num_dropped_updates : shared = 0;
my $cmdQueue = Thread::Queue->new;

my $socket;

############################

sub openSocket() {
    $socket = IO::Socket::INET->new(PeerAddr => $nprobe_ip,
				    PeerPort => $nprobe_port,
				    Proto    => "tcp");

    if(defined $socket) {
	print "New socket open...\n";
    } else {
	print "Couldn't connect to $nprobe_host:$nprobe_port : $@\n";
	sleep 1;
    }
}

sub processCmds {
    my $max_queue_len = 0;
    my $OUT;

    if($dump_file ne "") {
	# Dump mode
	open OUT, '>', $dump_file or die $!;
    }

    while (my $cmd = $cmdQueue->dequeue()) {
	my $num = $cmdQueue->pending();
	if($num > $max_queue_len) { $max_queue_len = $num; }
	if($debug) { print $cmd."\n"; }

	if($dump_file ne "") {
	    # Dump mode
	    print OUT $cmd;
	} else {
	    # Socket mode

	    if(not defined $socket) {
		openSocket();
	    }

	    if(defined $socket) {
		my $bytes_sent = $socket->send($cmd);

		if((not defined $bytes_sent) || ($bytes_sent == 0)) {
		    print "Socket was closed by remote peer\n";
		    close($socket);
		    openSocket();
		}
	    }
	}
    }
}

my $i=0;
my $num_threads = 1;

for($i=0; $i<$num_threads; $i++) {
    my $thr = threads->new(\&processCmds);
    $thr->detach; # Now we officially don't care any more
}

############################

$bgp->add_peer($peer);
$peer->refresh($refresh);
$peer->start();
$bgp->event_loop();

sub my_update_callback
{
    my ($peer,$update) = @_;
    my %h;
    my $as_path;

    #print "Update from [$peer][$update]\n";

    ################################

    # Remove duplicates entries
    my @path = uniq(split(/ /, $update->{_as_path}));

    #shift(@path); # Delete top element

    my $target_as = $path[$#path];
    #pop(@path); # Delete last element from array (i.e. remove target_as)

    if(!($target_as =~ m/^{/)) {
	my $old_val = $as_paths{$target_as};

	# Format: (number of elements)@(elem 1),(elem 2)....
	$as_path = ($#path+1)."@".join(",", @path);

	#print $as_path."\n";
	if($old_val ne $as_path) {
	    $as_paths{$target_as} = $as_path;
	}

	#if($debug) { print $as_path."\n"; } else { print "."; }
    } else {
	# Something bad happened
	return;
    }

    ########################

    my @nlri = @{$update->nlri()};
    if($debug) { print "[$num_updates] [ "; }
    foreach (@nlri) {
	if($debug) { print $_." "; }
	my $net = $_;

	if ($net =~ m/^(\d+)\.(\d+)\.(\d+)\.(\d+)\/(\d+)$/) {
	    $cmd = "+".$net."=".$as_path."\n";
	    if($debug) { print $cmd; }
	    if($cmdQueue->pending() < $max_queue_len) {
		$cmdQueue->enqueue($cmd);
		$num_updates++;
	    } else {
		$num_dropped_updates++;
	    }
	}
    }

   ########################

    my @withdrawn = @{$update->withdrawn()};
    if($debug) { print "[$num_updates] [ "; }
    foreach (@withdrawn) {
	if($debug) { print $_." "; }
	my $net = $_;

	if ($net =~ m/^(\d+)\.(\d+)\.(\d+)\.(\d+)\/(\d+)$/) {
	    $cmd = "-".$net."=".$as_path."\n";
	    if($debug) { print $cmd; }

	    if($cmdQueue->pending() < $max_queue_len) {
		$cmdQueue->enqueue($cmd);
		$num_updates++;
	    } else {
		$num_dropped_updates++;
	    }
	}
    }

}

############

sub uniq {
    my %seen = ();
    my @r = ();
    foreach my $a (@_) {
        unless ($seen{$a}) {
            push @r, $a;
            $seen{$a} = 1;
        }
    }
    return @r;
}

############

sub help {
    print "bgp_probe_client.pl [-i <probe host:port>] [-d <dump file>] [-v] [-h]\n";
    exit 0;
}
