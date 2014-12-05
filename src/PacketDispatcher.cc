/*
 * AIEngine a deep packet inspector reverse engineering engine.
 *
 * Copyright (C) 2013-2014  Luis Campo Giralte
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
#include "PacketDispatcher.h"

namespace aiengine {

#ifdef HAVE_LIBLOG4CXX
log4cxx::LoggerPtr PacketDispatcher::logger(log4cxx::Logger::getLogger("aiengine.packetdispatcher"));
#endif

void PacketDispatcher::info_message(std::string msg) {

#ifdef HAVE_LIBLOG4CXX
        LOG4CXX_INFO(logger, msg);
#else
        std::chrono::system_clock::time_point time_point = std::chrono::system_clock::now();
        std::time_t now = std::chrono::system_clock::to_time_t(time_point);
#ifdef __clang__
        std::cout << "[" << std::put_time(std::localtime(&now), "%D %X") << "] ";
#else
        char mbstr[100];
        std::strftime(mbstr, 100, "%D %X", std::localtime(&now));
        std::cout << "[" << mbstr << "] ";
#endif
        std::cout << msg << std::endl;
#endif
}


void PacketDispatcher::setDefaultMultiplexer(MultiplexerPtr mux) {

	defMux_ = mux;
	eth_ = std::dynamic_pointer_cast<EthernetProtocol>(defMux_->getProtocol());
}


void PacketDispatcher::open_device(std::string device) {

	char errorbuf[PCAP_ERRBUF_SIZE];
#ifdef __FREEBSD__
	int timeout = 1000; // miliseconds
#else
	int timeout = -1;
#endif

	pcap_ = pcap_open_live(device.c_str(), PACKET_RECVBUFSIZE, 0, timeout, errorbuf);
	if (pcap_ == nullptr) {
#ifdef HAVE_LIBLOG4CXX
		LOG4CXX_ERROR(logger,"Device:" <<device.c_str() << " error:" << errorbuf );
#else
		std::cerr << "Device:" << device.c_str() << " error:" << errorbuf << std::endl;
#endif
		device_is_ready_ = false;
		exit(-1);
		return;
	}
	int ifd = pcap_get_selectable_fd(pcap_);
	if (pcap_setnonblock(pcap_, 1, errorbuf) == 1) {
		device_is_ready_ = false;
		return;
	}
	stream_ = PcapStreamPtr(new PcapStream(io_service_));
			
	stream_->assign(::dup(ifd));
	device_is_ready_ = true;
	input_name_ = device;
}

void PacketDispatcher::close_device(void) {

	if (device_is_ready_) {
		stream_->close();
		pcap_close(pcap_);
		device_is_ready_ = false;
	}
}

void PacketDispatcher::open_pcap_file(std::string filename) {

	char errorbuf[PCAP_ERRBUF_SIZE];

        pcap_ = pcap_open_offline(filename.c_str(),errorbuf);
        if (pcap_ == nullptr) {
		pcap_file_ready_ = false;
#ifdef HAVE_LIBLOG4CXX
		LOG4CXX_ERROR(logger,"Unknown pcapfile:" << filename.c_str());
#else
		std::cerr << "Unkown pcapfile:" << filename.c_str() << std::endl;
#endif
		exit(-1);
	} else {
		pcap_file_ready_ = true;
		input_name_ = filename;
	}
}

void PacketDispatcher::close_pcap_file(void) {

	if (pcap_file_ready_) {
		pcap_close(pcap_);
		pcap_file_ready_ = false;
	}
}

void PacketDispatcher::do_read(boost::system::error_code ec) {

	int len = pcap_next_ex(pcap_,&header_,&pkt_data_);
	if (len >= 0) {
		forward_raw_packet((unsigned char*)pkt_data_,header_->len,header_->ts.tv_sec);
	}

// This prevents a problem on the boost asio signal
// remove this if when boost will be bigger than 1.50
#ifdef PYTHON_BINDING
#if BOOST_VERSION >= 104800 && BOOST_VERSION < 105000
	if (PyErr_CheckSignals() == -1) {
		std::cout << "Throwing exception from python." << std::endl;
		throw std::runtime_error("Python exception\n");
       	}
#endif
#endif

	if (!ec || ec == boost::asio::error::would_block)
      		start_operations();
	// else error but not handler
}

void PacketDispatcher::forward_raw_packet(unsigned char *packet,int length, time_t packet_time) {

	++total_packets_;
	total_bytes_ += length;

	if (defMux_) {
		current_packet_.setPayload(packet);
		current_packet_.setPayloadLength(length);
		current_packet_.setPrevHeaderSize(0);
		current_packet_.setPacketTime(packet_time);

		if (defMux_->acceptPacket(current_packet_)) {
			defMux_->setPacket(&current_packet_);
			defMux_->setNextProtocolIdentifier(eth_->getEthernetType());
			defMux_->forwardPacket(current_packet_);
                }
	}
}

void PacketDispatcher::start_operations(void) {

	read_in_progress_ = false;
	if (!read_in_progress_) {
		read_in_progress_ = true;

		stream_->async_read_some(boost::asio::null_buffers(),
                	boost::bind(&PacketDispatcher::do_read, this,
                                boost::asio::placeholders::error));
#ifdef PYTHON_BINDING
		user_shell_->readUserInput();
#endif
	}
}

void PacketDispatcher::run_pcap(void) {

        std::ostringstream msg;
        msg << "Processing packets from file " << input_name_.c_str();

       	info_message(msg.str());

	status_ = PacketDispatcherStatus::RUNNING;
	while (pcap_next_ex(pcap_,&header_,&pkt_data_) >= 0) {
		forward_raw_packet((unsigned char*)pkt_data_,header_->len,header_->ts.tv_sec);
	}
	status_ = PacketDispatcherStatus::STOP;
}


void PacketDispatcher::run_device(void) {

	if (device_is_ready_) {

        	std::ostringstream msg;
        	msg << "Processing packets from device " << input_name_.c_str();

        	info_message(msg.str());

		try {
			status_ = PacketDispatcherStatus::RUNNING;
			start_operations();
			io_service_.run();
		} catch (std::exception& e) {
        		std::cerr << e.what() << std::endl;
        	}
		status_ = PacketDispatcherStatus::STOP;
	} else {

                std::ostringstream msg;
                msg << "The device is not ready to run";

                info_message(msg.str());
	}
}

void PacketDispatcher::open(const std::string &source) {

	std::ifstream infile(source);

	device_is_ready_ = false;
	pcap_file_ready_ = false;

	if (infile.good()) { // The source is a file
		open_pcap_file(source);
	} else {
		open_device(source);
	}
}

void PacketDispatcher::run(void) {

	if (device_is_ready_) {
		run_device();
	} else {
		if(pcap_file_ready_) {
			run_pcap();
		}
	}
}

void PacketDispatcher::close(void) {

        if (device_is_ready_) {
                close_device();
        } else {
                if (pcap_file_ready_) {
                        close_pcap_file();
                }
        }
}

void PacketDispatcher::setPcapFilter(const std::string &filter) {

	if ((device_is_ready_)or(pcap_file_ready_)) {
		struct bpf_program fp;
		char *c_filter = const_cast<char*>(filter.c_str());

		if (pcap_compile(pcap_, &fp, c_filter, 1, PCAP_NETMASK_UNKNOWN) == 0) {
			
			if (pcap_setfilter(pcap_,&fp) == 0) {
				std::ostringstream msg;
                		msg << "Pcap filter set:" << filter;

                		info_message(msg.str());
			}
		}
	}
}

#ifdef PYTHON_BINDING

void PacketDispatcher::scheduler_handler(boost::system::error_code error) {

	// Check if the timer have been cancel
        if (error ==  boost::asio::error::operation_aborted) {
            	return;
        }

	// Update the values of the timer and reschedule
        timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(scheduler_seconds_));
        timer_->async_wait(boost::bind(&PacketDispatcher::scheduler_handler, this,
                boost::asio::placeholders::error));

	PyGILState_STATE state(PyGILState_Ensure());
        try {
        	// TODO: Fix
                boost::python::call<void>(scheduler_callback_);
	} catch (std::exception &e) {
        	std::cout << "ERROR:" << e.what() << std::endl;
        }
        PyGILState_Release(state);

	return;
}



void PacketDispatcher::forwardPacket(const std::string &packet, int length) {

	const unsigned char *pkt = reinterpret_cast<const unsigned char *>(packet.c_str());

	// TODO: pass the time to the method forward_raw_packet from the
	// python binding
	forward_raw_packet((unsigned char*)pkt,length,0);
	return;
}

void PacketDispatcher::enableShell(bool enable) {

	user_shell_->enableShell(enable);
}

void PacketDispatcher::unsetStack() {

	stack_name_ = "None";
       	defMux_.reset();
}

void PacketDispatcher::setScheduler(PyObject *callback, int seconds) {

	if (timer_) // cancel the timer if exists
		timer_->cancel();

	// reset the values of the scheduler
	if ((callback == Py_None)or(seconds <= 0)) {
		scheduler_set_ = false;
		scheduler_seconds_ = 0;
               	if (scheduler_callback_)
			Py_XDECREF(scheduler_callback_);

		timer_.reset();
	} else {
        	if (!PyCallable_Check(callback)) {
                	std::cerr << "Object is not callable." << std::endl;
        	} else {
	
			if ( scheduler_callback_ ) Py_XDECREF(scheduler_callback_);
                	scheduler_callback_ = callback;
                	Py_XINCREF(scheduler_callback_);
                	scheduler_set_ = true;
			scheduler_seconds_ = seconds;

			// reset the shared pointer and create a new timer.
			timer_.reset(new boost::asio::deadline_timer(io_service_,
				boost::posix_time::seconds(scheduler_seconds_)));

                        timer_->expires_at(timer_->expires_at() + boost::posix_time::seconds(scheduler_seconds_));
                        timer_->async_wait(boost::bind(&PacketDispatcher::scheduler_handler, this,
                                boost::asio::placeholders::error));
        	}
	}
}

#endif

std::ostream& operator<< (std::ostream& out, const PacketDispatcher& pdis) {

	out << "PacketDispatcher(" << &pdis <<") statistics" << std::endl;
	out << "\t" << "Connected to " << pdis.stack_name_ <<std::endl;
	out << "\t" << "Total packets:          " << std::setw(10) << pdis.total_packets_ <<std::endl;
	out << "\t" << "Total bytes:        " << std::setw(14) << pdis.total_bytes_ <<std::endl;

        return out;
}

void PacketDispatcher::status(void) {

	std::ostringstream msg;
        msg << "PacketDispatcher ";
	if (status_ == PacketDispatcherStatus::RUNNING)
		msg << "running";
	else
		msg << "stoped";
	msg << ", plug to " << stack_name_;
	msg << ", packets " << total_packets_ << ", bytes " << total_bytes_;

        info_message(msg.str());
}

} // namespace aiengine
