Wifi over Ethernet
==================

This is an experimental project I created to explore a silly idea that was
probably doomed from the start. Without a lot of knowledge of 802.11 wifi
protocols, I wanted to see if I could enable wireless devices to communicate
with each other over distances far beyond their normal operating range by
using wifi stations to capture interesting 802.11 frames and distributing
those frames over a VPN to other similar wifi stations that will re-transmit
the original frames without any processing.

The idea was to distribute the raw frames over an OpenVPN tap network, so this
virtual ethernet must have a MTU size equal to the size of the largest 802.11
frame + an ethernet header. OpenVPN can do this, but it's not a common
configuration.

The code found here contains the repeater functionality, which inclues only the
following:

  * Listen on a wifi interface then broadcast captured frames via a given
    distribution ethernet interface.

  * Listen to the distribution network and transmit received frames through
    the wifi interface.

It is assumed that your wifi interface is already set up with the proper
parameters and is in monitor mode. It is also assumed that your distribution
network VPN is already established and configured.

I should also note that the code here is in rough shape and although the
distribution and re-transmission functionality does work, I was never able to
establish useful associations between wireless devices across the tunnel. 
However, access points configured to broadcast their availability were indeed
visible to devices across the tunnel, so I guess that's kind of cool.

I never had the time to research it further, but I suspect that either the
802.11 protocols cannot tolerate the added latency or there's some problem
related to moving wifi frames between independent collision domains. I also
suspect that this might be less of a problem for unsecured networks, but I
haven't actually attempted that because my particular use case involved
secure communications, exclusively.

## Documentation

It's not that complex.  Just read the source code.

## Why?

I wanted to use the wifi features of my Nintendo 3DS video game console (like
Street Pass) to interact with my distant friends that I rarely encounter in
person.

### Copyrights

Original work is Copyright (C) 2013 Erik Elmore <erik@erikelmore.com>

### License
See LICENSE file for full text.

> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
> 
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU General Public License for more details.
> 
> You should have received a copy of the GNU General Public License
> along with this program. If not, see <http://www.gnu.org/licenses/>.
