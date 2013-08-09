#include "HTTPProtocol.h"
#include <iomanip> // setw

void HTTPProtocol::extractHostValue(Flow *flow, const char *header)
{     
	boost::cmatch result;

        if(boost::regex_search(header,result,http_host_))
        {
        	std::string host_raw(result[0].first, result[0].second);
                std::string host(host_raw,6,host_raw.length()-8); // remove also the \r\n

                HTTPHostPtr host_ptr = flow->http_host.lock();

                if(!host_ptr) // There is no Host object attached to the flow
                {
                        host_ptr = host_cache_->acquire().lock();
                        if(host_ptr)
			{
                      		host_ptr->setName(host); 
		         	flow->http_host = host_ptr;
				std::cout << "yes" << std::endl;
			}
                }else{ std::cout << "joder " << std::endl;}

                HostMapType::iterator it = host_map_.find(host);

               	if(it == host_map_.end())
                {
                	host_map_[host] = 0;
                }
                ++host_map_[host];
	}
}

void HTTPProtocol::extractUserAgentValue(Flow *flow, const char *header)
{
	boost::cmatch result;

	if(boost::regex_search(header,result,http_ua_))
	{
		std::string ua_raw(result[0].first, result[0].second);
		std::string ua(ua_raw,12,ua_raw.length()-14); // remove also the \r\n

		UAMapType::iterator it = ua_map_.find(ua);
		if(it == ua_map_.end())
		{
			ua_map_[ua] = 0;
		}
		++ua_map_[ua];
		//flow->http_ua = ua;
	}
}


void HTTPProtocol::processFlow(Flow *flow)
{
	++total_packets_;	
	total_bytes_ += flow->packet->getLength();
	++flow->total_packets_l7;

	// Is the first packet accepted and processed
	if(flow->total_packets_l7 == 1) 
	{
		const char *header = reinterpret_cast <const char*> (flow->packet->getPayload());
	
		extractHostValue(flow,header);	
		extractUserAgentValue(flow,header);
	}
}

void HTTPProtocol::statistics(std::basic_ostream<char>& out)
{
        out << "HTTPProtocol(" << this << ") statistics" << std::dec <<  std::endl;
        out << "\t" << "Total packets:          " << std::setw(10) << total_packets_ <<std::endl;
        out << "\t" << "Total validated packets:" << std::setw(10) << total_validated_packets_ <<std::endl;
        out << "\t" << "Total malformed packets:" << std::setw(10) << total_malformed_packets_ <<std::endl;
        out << "\t" << "Total bytes:            " << std::setw(10) << total_bytes_ <<std::endl;
        if(flow_forwarder_.lock())
                flow_forwarder_.lock()->statistics(out);

	out << "Hosts used" << std::endl;
	for(auto it = host_map_.begin(); it!=host_map_.end(); ++it)
	{
		out << "\t\tHost:" << (*it).first <<":" << (*it).second << std::endl;
	}
	out << "UserAgents used" << std::endl;
	for(auto it = ua_map_.begin(); it!=ua_map_.end(); ++it)
	{
		out << "\t\tUserAgent:" << (*it).first <<":" << (*it).second << std::endl;
	}
}

