What's New
##########

See https://github.com/ntop/nProbe/blob/master/CHANGELOG.md for changelog.

*Releases up to 7.4 (June 2016)*

- Full IPFIX support: PEN (Private Enterprise Numbers) and Variable length encoding.
- Ability to natively dump flows in FastBit format that allows to outperform relational and raw flow-based collectors.
- Ability to collect sFlow flows and turn them into flows (v5/v9/IPFIX).
- Collection of Cisco ASA flows and conversion in ‘standard’ flows.
- New design for better performance and exploitation of multicore architectures.
- Support of tunneled (including GRE, PPP and GTP) traffic and ability to export in flows inner/outer envelope/packet information.
- HTTP and MySQL protocol analysis: ability to generate logs of web and mysql activities in addition to flow export.
- BGP Plugin for establishing a BGP session with a router and generate flows with AS and AS path information.
- ElasticSearch flow export
- Kafka brokers flow export

*Release 6.15 (January 2014)*

- Updated nProbe with 6.15 features.

*Release 5.0 (February 2008)*

- Updated nBox firmware
- Updated nProbe with latest features.
- Updated ntop with latest 3.3.X version.

*Release 4.0 (July 2007)*

- Updated nBox with latest 2.6 kernel series image
- Updated nProbe with 4.9 version coverage.

*Release 3.9 (April 2005)*

- Updated nBox section

*Release 3.0.1 (February 2004)*

- Updated nBox section

*Release 3.0 (January 2004)*

- Added nProbe 3.0 coverage

*Release 2.2 (October 2003)*

- Added nBox coverage

*Release 2.1 (June 2003)*

- Added nFlow support

*Release 2.0.1 (February 2003)*

- Added the ability to save flows on disk (:code:`-P` flag)

*Release 2.0 (January 2003)*

- Added the ability to select multiple NetFlow collectors.
- Added :code:`--p` flag for ignoring TCP/UDP ports.
- Added :code:`--e` flag for slowing down flow export speed.
- Added :code:`--u` flag for identifying input NetFlow devices into emitted flows.
- Added :code:`--z` flag for preventing nProbe from emitting tiny flows.
- Added :code:`--a` flag for selecting the way flows are exported to several collectors (if defined).
- Added the ability to control an LCD display where the probe can report traffic statistics.
- Enhanced TCP flags support in exported flows.

*Release 1.3 (July 2002)*

- First public release.
