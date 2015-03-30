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

// gets rid of annoying "deprecated conversion from string constant blah blah" warning
#pragma GCC diagnostic ignored "-Wwrite-strings"

#ifndef SRC_PROTOCOLS_TCPGENERIC_TCPGENERICPROTOCOL_H_
#define SRC_PROTOCOLS_TCPGENERIC_TCPGENERICPROTOCOL_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LIBLOG4CXX
#include "log4cxx/logger.h"
#endif
#include "Protocol.h"
#include "regex/RegexManager.h"
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

namespace aiengine {

class TCPGenericProtocol: public Protocol 
{
public:
    	explicit TCPGenericProtocol():Protocol(TCPGenericProtocol::default_name),stats_level_(0),
		tcp_generic_header_(nullptr),total_bytes_(0) {}

    	virtual ~TCPGenericProtocol() {}

	static constexpr char *default_name = "TCPGenericProtocol";	
	static const uint16_t id = 0;
	static const int header_size = 0;
	int getHeaderSize() const { return header_size;}

	int64_t getTotalBytes() const { return total_bytes_; }
	int64_t getTotalPackets() const { return total_packets_;}
	int64_t getTotalValidatedPackets() const { return total_validated_packets_;}
	int64_t getTotalMalformedPackets() const { return total_malformed_packets_;}

	bool processPacket(Packet& packet) { return true; }
	void processFlow(Flow *flow);
	
	void setStatisticsLevel(int level) { stats_level_ = level;}
	void statistics(std::basic_ostream<char>& out);
	void statistics() { statistics(std::cout);}

        void releaseCache() {} // No need to free cache

        void setHeader(unsigned char *raw_packet) {
        
                tcp_generic_header_ = raw_packet;
        }

	// Condition for say that a payload is for generic tcp 
	// Accepts all!
	bool tcpGenericChecker(Packet &packet){ 
	
		setHeader(packet.getPayload());
		++total_validated_packets_; 
		return true;
	}

	void setRegexManager(SharedPointer<RegexManager> sig) { sigs_ = sig;} 
	unsigned char *getPayload() const { return tcp_generic_header_;}

	int64_t getAllocatedMemory() const { return sizeof(TCPGenericProtocol); }

#ifdef PYTHON_BINDING
        boost::python::dict getCounters() const;
#endif

private:
#ifdef HAVE_LIBLOG4CXX
	static log4cxx::LoggerPtr logger;
#endif
	int stats_level_;
	unsigned char *tcp_generic_header_;
        int64_t total_bytes_;
	SharedPointer<RegexManager> sigs_;
};

typedef std::shared_ptr<TCPGenericProtocol> TCPGenericProtocolPtr;
typedef std::weak_ptr<TCPGenericProtocol> TCPGenericProtocolPtrWeak;

} // namespace aiengine

#endif  // SRC_PROTOCOLS_TCPGENERIC_TCPGENERICPROTOCOL_H_
