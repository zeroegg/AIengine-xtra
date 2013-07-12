#ifndef _DNSProtocol_H_
#define _DNSProtocol_H_

#ifdef __FAVOR_BSD
#undef __FAVOR_BSD
#endif // __FAVOR_BSD

#include "../Multiplexer.h"
#include "../FlowForwarder.h"
#include "../Protocol.h"
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

class DNSProtocol: public Protocol 
{
public:
    	explicit DNSProtocol():ssl_header_(nullptr),total_bytes_(0) { name_="DNSProtocol";};
    	virtual ~DNSProtocol() {};
	
	static const u_int16_t id = 0;
	static const int header_size = 2;
	int getHeaderSize() const { return header_size;};

	int32_t getTotalBytes() const { return total_bytes_; };
	uint64_t getTotalPackets() const { return total_packets_;};
	uint64_t getTotalValidatedPackets() const { return total_validated_packets_;};
	uint64_t getTotalMalformedPackets() const { return total_malformed_packets_;};

        const char *getName() { return name_.c_str();};

	void processPacket(Packet& packet){};
	void processFlow(Flow *flow);
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);};

        void setMultiplexer(MultiplexerPtrWeak mux) { mux_ = mux; };
        MultiplexerPtrWeak getMultiplexer() { mux_;};

        void setFlowForwarder(FlowForwarderPtrWeak ff) { flow_forwarder_= ff; };
        FlowForwarderPtrWeak getFlowForwarder() { return flow_forwarder_;};

        void setHeader(unsigned char *raw_packet)
        {
                ssl_header_ = raw_packet;
        }

	// Condition for say that a payload is DNS 
	bool dnsChecker(Packet &packet) 
	{
		// I dont like this idea of ports but...
		if((packet.getSourcePort() == 53)||(packet.getDestinationPort() == 53))
		{
			setHeader(packet.getPayload());
			++total_validated_packets_; 
			return true;
		}
		else
		{
			++total_malformed_packets_;
			return false;
		}
	}


private:
	FlowForwarderPtrWeak flow_forwarder_;	
	MultiplexerPtrWeak mux_;
	unsigned char *ssl_header_;
        int32_t total_bytes_;
};

typedef std::shared_ptr<DNSProtocol> DNSProtocolPtr;

#endif
