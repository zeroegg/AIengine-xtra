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
#ifndef _FrequencyGroupItem_H_
#define _FrequencyGroupItem_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <utility>
#include <cstring>
#include "Frequencies.h"
#include "../flow/FlowManager.h"
#include <boost/format.hpp>

using namespace std;

class FrequencyGroupItem
{
public:
	explicit FrequencyGroupItem() 
	{
		freqs_ = SharedPointer<Frequencies>(new Frequencies());
		total_items_ = 0;
		total_flows_bytes_ = 0;
	}	
	virtual ~FrequencyGroupItem() {};
		
	void incTotalItems() { ++total_items_;};
	void addTotalFlowsBytes(int32_t bytes) { total_flows_bytes_ += bytes;};

	void sumFrequencies(SharedPointer<Frequencies> freqs) 
	{ 
		Frequencies *freq_ptr = freqs_.get();

		*freq_ptr = *freq_ptr + *freqs.get();
	}

	void addFlow(SharedPointer<Flow> flow) { flow_list_.push_back(flow);};	
	int getTotalItems() { return total_items_;};
	int32_t getTotalFlowsBytes() { return total_flows_bytes_;};	
	SharedPointer<Frequencies> getFrequencies() { return freqs_;};	
	std::vector<WeakPointer<Flow>> &getReferenceFlows() { return flow_list_;};
private:		
	int total_items_;
	int32_t total_flows_bytes_;
	SharedPointer<Frequencies> freqs_;
	std::vector<WeakPointer<Flow>> flow_list_;
};

typedef std::shared_ptr<FrequencyGroupItem> FrequencyGroupItemPtr;

#endif
