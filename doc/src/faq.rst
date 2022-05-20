Frequently Asked Questions
##########################

- Q: I'm sending 60 bytes ping packets using ‘ping --s 60' but nProbe reports 92 bytes packets.

  - A: nProbe counts the packet size at IP level. An ICMP Echo Request packet with 60 bytes payload is 92 bytes long. 

- Q: I need to capture traffic from several interfaces but nProbe allows just one interface to be used. What can I do?

  - A: You can start several instances of nProbe, each on a different network interface. 

- Q: nProbe is exporting flows too fast and my collector cannot keep up with it. How can I slow down nProbe export rate?

  - A: nProbe has been for high-speed networks (1Gb and above) so its export rate can be high due to traffic conditions. There are several solutions available:

    - Specify a minimum intra-flow delay (-e flag)
    - Use several collectors and send them flows in round robin (-n flag) in order to balance load among the collectors. 
