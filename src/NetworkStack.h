/*
 * AIEngine a deep packet inspector reverse engineering engine.
 *
 * Copyright (C) 2013-2015  Luis Campo Giralte
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 * Written by Luis Campo Giralte <luis.camp0.2009@gmail.com> 
 *
 */
#ifndef SRC_NETWORKSTACK_H_
#define SRC_NETWORKSTACK_H_

#include <iostream>
#include <fstream>
#include "Multiplexer.h"
#include "names/DomainNameManager.h"
#include "regex/RegexManager.h"
#include "flow/FlowManager.h"
#include "DatabaseAdaptor.h"
#include "ipset/IPSetManager.h"
#include "./protocols/tcp/TCPProtocol.h"
#include "./protocols/udp/UDPProtocol.h"
#include "./protocols/tcpgeneric/TCPGenericProtocol.h"
#include "./protocols/udpgeneric/UDPGenericProtocol.h"
#include "./protocols/dns/DNSProtocol.h"
#include "./protocols/sip/SIPProtocol.h"
#include "./protocols/dhcp/DHCPProtocol.h"
#include "./protocols/ntp/NTPProtocol.h"
#include "./protocols/snmp/SNMPProtocol.h"
#include "./protocols/ssl/SSLProtocol.h"
#include "./protocols/http/HTTPProtocol.h"
#include "./protocols/smtp/SMTPProtocol.h"
#include "./protocols/imap/IMAPProtocol.h"
#include "./protocols/pop/POPProtocol.h"
#include "protocols/frequency/FrequencyProtocol.h"

namespace aiengine {

typedef std::pair<std::string,ProtocolPtr> ProtocolPair;
typedef std::map<std::string,ProtocolPtr> ProtocolMap;
typedef std::vector<ProtocolPair> ProtocolVector;

class NetworkStack 
{
public:
    	NetworkStack();
    	virtual ~NetworkStack() {}

	virtual void showFlows(std::basic_ostream<char>& out) = 0;
	virtual void showFlows() = 0;

        void statistics(std::basic_ostream<char>& out) { out << *this; }
        void statistics() { statistics(std::cout);}
	void statistics(const std::string &name);

	virtual const char* getName() = 0;
	virtual void setName(char *name) = 0;

	virtual void setLinkLayerMultiplexer(MultiplexerPtrWeak mux) = 0;
	virtual MultiplexerPtrWeak getLinkLayerMultiplexer() = 0; 

	virtual void setTotalTCPFlows(int value) = 0;
	virtual int getTotalTCPFlows() const = 0;
	virtual void setTotalUDPFlows(int value) = 0;
	virtual int getTotalUDPFlows() const = 0;

	virtual void setTCPRegexManager(const SharedPointer<RegexManager>& sig) = 0;	
	virtual void setUDPRegexManager(const SharedPointer<RegexManager>& sig) = 0;	

	virtual void enableFrequencyEngine(bool enable) = 0;
	virtual void enableNIDSEngine(bool enable) = 0;
	virtual void enableLinkLayerTagging(std::string type) = 0;

	virtual void setFlowsTimeout(int timeout) = 0;
	virtual int getFlowsTimeout() const = 0;

	// Release the memory of the caches of every protocol on the stack
	void releaseCache(const std::string &name);
	void releaseCaches();

	void enableFlowForwarders(const FlowForwarderPtr& ff, std::initializer_list<FlowForwarderPtr> fps);
	void disableFlowForwarders(const FlowForwarderPtr& ff, std::initializer_list<FlowForwarderPtr> fps);

#ifdef PYTHON_BINDING

	virtual FlowManager& getTCPFlowManager() = 0;
	virtual FlowManager& getUDPFlowManager() = 0;

	void setDomainNameManager(DomainNameManager& dnm, const std::string& name);
	void setDomainNameManager(DomainNameManager& dnm, const std::string& name, bool allow);
	
	void setTCPDatabaseAdaptor(boost::python::object &dbptr);
	void setTCPDatabaseAdaptor(boost::python::object &dbptr,int packet_sampling);
	void setUDPDatabaseAdaptor(boost::python::object &dbptr);
	void setUDPDatabaseAdaptor(boost::python::object &dbptr,int packet_sampling);
	
	virtual void setTCPIPSetManager(IPSetManager& ipset_mng) = 0;
	virtual void setUDPIPSetManager(IPSetManager& ipset_mng) = 0;

	boost::python::dict getCounters(const std::string &name);

#else
	virtual void setTCPIPSetManager(SharedPointer<IPSetManager> ipset_mng) = 0;
	virtual void setUDPIPSetManager(SharedPointer<IPSetManager> ipset_mng) = 0;

	virtual FlowManagerPtrWeak getTCPFlowManager() = 0;
	virtual FlowManagerPtrWeak getUDPFlowManager() = 0;
#endif

	void addProtocol(ProtocolPtr proto); 
	void setStatisticsLevel(int level); 
	int getStatisticsLevel() const { return stats_level_; }

	friend std::ostream& operator<< (std::ostream& out, const NetworkStack& ns);

	// Protocols shared with all the stacks, layer 7
        HTTPProtocolPtr http;
        SSLProtocolPtr ssl;
        DNSProtocolPtr dns;
        SIPProtocolPtr sip;
        DHCPProtocolPtr dhcp;
        NTPProtocolPtr ntp;
        SNMPProtocolPtr snmp;
        SMTPProtocolPtr smtp;
        IMAPProtocolPtr imap;
        POPProtocolPtr pop;
        TCPGenericProtocolPtr tcp_generic;
        UDPGenericProtocolPtr udp_generic;
        FrequencyProtocolPtr freqs_tcp;
        FrequencyProtocolPtr freqs_udp;

        FlowForwarderPtr ff_http;
        FlowForwarderPtr ff_ssl;
        FlowForwarderPtr ff_dns;
        FlowForwarderPtr ff_sip;
        FlowForwarderPtr ff_dhcp;
        FlowForwarderPtr ff_ntp,ff_snmp;
        FlowForwarderPtr ff_smtp;
        FlowForwarderPtr ff_imap;
        FlowForwarderPtr ff_pop;
        FlowForwarderPtr ff_tcp_generic;
        FlowForwarderPtr ff_udp_generic;
        FlowForwarderPtr ff_tcp_freqs;
        FlowForwarderPtr ff_udp_freqs;

private:
	template <class T> 
	void set_domain_name_manager(DomainNameManager& dnm, bool allow);

	ProtocolPtr get_protocol(const std::string &name);

	int stats_level_;
	ProtocolMap proto_map_;
	ProtocolVector proto_vector_;
	std::vector<DomainNameManagerPtr> domain_mng_list_;
};

typedef std::shared_ptr <NetworkStack> NetworkStackPtr;

} // namespace aiengine

#endif  // SRC_NETWORKSTACK_H_
