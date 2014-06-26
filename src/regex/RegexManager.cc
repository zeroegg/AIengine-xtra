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
#include <iostream>
#include "RegexManager.h"

namespace aiengine {

void RegexManager::addRegex(const std::string name,const std::string expression) {

	SharedPointer<Regex> sig = SharedPointer<Regex>(new Regex(name,expression));

	addRegex(sig);
}

void RegexManager::addRegex(SharedPointer<Regex> sig) {

	signatures_.push_back(sig);
}

void RegexManager::evaluate(const std::string& data, bool *result) {

	current_signature_.reset();

//	Something strange happens on the freebsd version that the
//	use of the std::find_if generates a sigsegv
//        std::find_if(signatures_.begin(),
//                signatures_.end(),  [&](SharedPointer<Regex>& sig) {
//
//        });
	
	for (auto it = signatures_.begin(); it != signatures_.end(); ++it) {
		SharedPointer<Regex> sig = *it;
	
		if (sig->evaluate(data)) {
			++total_matched_signatures_;
			current_signature_ = sig;
			(*result) = true;
			break;
		}
      	} 
	return;
}

std::ostream& operator<< (std::ostream& out, const RegexManager& sig) {

	out << "RegexManager(" << &sig << ") statistics" << std::dec <<  std::endl;	
	for (auto it = sig.signatures_.begin(); it != sig.signatures_.end(); ++it) {
		SharedPointer<Regex> sig = (*it);
		std::ostringstream tabs;	

		bool no_more_regex = false;

		while (no_more_regex == false) {
			tabs << "\t";
			out << tabs.str() << "Regex:" << sig->getName() << " matches:" << sig->getMatchs() << std::endl;
			if (sig->isTerminal() == false) { 
				no_more_regex = false;
				sig = sig->getNextRegex();
			} else {
				no_more_regex = true;
			}
		}	
	}
	return out;
}

} // namespace aiengine
