#!/usr/bin/env python

""" Example for detect troyan activity on the network """

__author__ = "Luis Campo Giralte"
__copyright__ = "Copyright (C) 2013-2016 by Luis Campo Giralte"
__revision__ = "$Id$"
__version__ = "0.1"
import sys
import os
sys.path.append("../src/")
import pyaiengine

def callback_troyan_activity(flow):
    ip = flow.src_ip

    print("Detected OSX_DocksterTrojan on ip:",ip)

def callback_domain(flow):

    d = flow.dnsinfo
    if (d):
        print("Suspicious Domain (%s) from %s" % (d.domainname,flow.srcip))
        print("Add specific regex for OSX_DocksterTrojan")

        reg = pyaiengine.Regex("OSX_DocksterTrojan regex activity",
            "^\xff\xff\xff\xff\xc2\x1f\x96\x9b\x5f\x03\xd3\x3d\x43\xe0\x4f\x8f")

        reg.callback = callback_troyan_activity
        r_mng.add_regex(reg)

        # Something of the references of python are wrong
        # do not remove this call, fix on future.
        st.tcp_regex_manager = r_mng


if __name__ == '__main__':

    # Load an instance of a Network Stack on Mobile network
    st = pyaiengine.StackLan()

    dm = pyaiengine.DomainNameManager()

    dom = pyaiengine.DomainName("OSX_DocksterTrojan suspicious domain",
        "itsec.eicp.net")
    dom.callback = callback_domain
    dm.add_domain_name(dom)

    r_mng = pyaiengine.RegexManager()

    st.set_domain_name_manager(dm,"DNSProtocol")
    st.tcp_regex_manager = r_mng

    st.tcp_flows = 327680
    st.udp_flows = 163840

    with pyaiengine.PacketDispatcher("/home/luis/pcapfiles/troyan/OSX_DocksterTrojan.pcap") as pd:
        pd.stack = st
        pd.run()

    # Dump on file the statistics of the stack
    st.stats_level = 5
    f = open("statistics.log","w")
    f.write(str(st))
    f.close()

    sys.exit(0)

