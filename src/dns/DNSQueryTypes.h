/*
 * AIEngine a deep packet inspector reverse engineering engine.
 *
 * Copyright (C) 2013  Luis Campo Giralte
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
 * Written by Luis Campo Giralte <luis.camp0.2009@gmail.com> 2013
 *
 */
#ifndef SRC_DNS_DNSQUERYTYPES_H_
#define SRC_DNS_DNSQUERYTYPES_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unordered_map>

namespace aiengine {

#define DNS_STANDARD_QUERY 0x0100
#define DNS_DYNAMIC_UPDATE 0x2800

// Some of the most used Dns types
enum class DNSQueryTypes : std::uint16_t {
	DNS_TYPE_A =            1,
	DNS_TYPE_NS =           2,
	DNS_TYPE_CNAME =        5,
	DNS_TYPE_SOA =          6,
	DNS_TYPE_MB =           7,
	DNS_TYPE_MG =           8,
	DNS_TYPE_MR =           9,
	DNS_TYPE_NULL =         10,
	DNS_TYPE_PTR =          12,
	DNS_TYPE_MX =           15,
	DNS_TYPE_TXT =          16,
	DNS_TYPE_AAAA =         28,
	DNS_TYPE_LOC =          29,
	DNS_TYPE_SRV =          33,
	DNS_TYPE_DS =           43,
	DNS_TYPE_DNSKEY =       48,
	DNS_TYPE_ANY =          255
};

const std::unordered_map<std::uint16_t,std::string> DNSQueryTypeToString {
	{ static_cast<std::uint16_t>(DNSQueryTypes::DNS_TYPE_A), "A" }
};

} // namespace aiengine 

#endif  // SRC_DNS_DNSQUERYTYPES_H_ 
