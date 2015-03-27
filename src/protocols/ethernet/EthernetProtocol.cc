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
#include "EthernetProtocol.h"
#include <iomanip> // setw

namespace aiengine {

// Just used when there is ethernet on the middle part of a stack
// Check vxlan for further details

bool EthernetProtocol::processPacket(Packet& packet) { 

	++total_packets_;
	MultiplexerPtr mux = mux_.lock();

        if (mux) {
                mux->setNextProtocolIdentifier(getEthernetType());

                mux->setHeaderSize(header_size);
                packet.setPrevHeaderSize(header_size);
	}
	return true;
}
	
void EthernetProtocol::statistics(std::basic_ostream<char>& out) { 

	if (stats_level_ > 0) {
		out << getName() << "(" << this <<") statistics" << std::endl;
		out << "\t" << "Total packets:          " << std::setw(10) << total_packets_ <<std::endl;
		out << "\t" << "Total bytes:        " << std::setw(14) << total_bytes_ <<std::endl;
		if (stats_level_> 1) {
			out << "\t" << "Total validated packets:" << std::setw(10) << total_validated_packets_ <<std::endl;
			out << "\t" << "Total malformed packets:" << std::setw(10) << total_malformed_packets_ <<std::endl;
			if (stats_level_> 2) {
				if(mux_.lock())
					mux_.lock()->statistics(out);
			}
		}
	}
}

#ifdef PYTHON_BINDING

boost::python::dict EthernetProtocol::getCounters() const {
	boost::python::dict counters;

	counters["packets"] = total_packets_;
	counters["bytes"] = total_bytes_;

        return counters;
}

#endif


} // namespace aiengine 
