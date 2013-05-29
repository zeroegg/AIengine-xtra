#include <string>
#include "../Protocol.h"
#include "../Multiplexer.h"
#include "../ethernet/EthernetProtocol.h"
#include "../vlan/VLanProtocol.h"
#include "IPProtocol.h"


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE iptest 
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE (ip_suite) // name of the test suite is stringtest

BOOST_AUTO_TEST_CASE (test1_ip)
{
	IPProtocol *ip = new IPProtocol();

	BOOST_CHECK(ip->getTotalPackets() == 0);

	delete ip;	
}

BOOST_AUTO_TEST_CASE (test2_ip) // ethernet -> ip 
{
        EthernetProtocol *eth = new EthernetProtocol();
        MultiplexerPtr mux_eth = MultiplexerPtr(new Multiplexer());
	IPProtocol *ip = new IPProtocol();
        MultiplexerPtr mux_ip = MultiplexerPtr(new Multiplexer());

	// ethernet ->ip ->udp -> dns
	char *raw_packet = "\x00\x0c\x29\x2e\x3c\x2a\x90\x84\x0d\x62\xd8\x04\x08\x00\x45\x00"
		"\x00\x3d\x8a\x0d\x00\x00\xec\x11\xf4\x4f\xc0\xa8\x01\x76\x50\x3a"
		"\x3d\xfa\xe9\xb3\x00\x35\x00\x29\x05\x94\x84\xd3\x01\x00\x00\x01"
		"\x00\x00\x00\x00\x00\x00\x02\x63\x68\x04\x70\x6f\x6f\x6c\x03\x6e"
		"\x74\x70\x03\x6f\x72\x67\x00\x00\x01\x00\x01";
	int length = 75;
	unsigned char *packet = reinterpret_cast <unsigned char*> (raw_packet);
	
	//configure the eth 
	eth->setMultiplexer(mux_eth);
        mux_eth->setHeaderSize(eth->header_size);
        mux_eth->addChecker(std::bind(&EthernetProtocol::ethernetChecker,eth));

	// configure the ip
	ip->setMultiplexer(mux_ip);
        mux_ip->setHeaderSize(ip->header_size);
        mux_ip->addChecker(std::bind(&IPProtocol::ipChecker,ip));

	// configure the multiplexers
        mux_eth->addUpMultiplexer(mux_ip,ETHERTYPE_IP);
        mux_ip->addDownMultiplexer(mux_eth);	

        // Sets the raw packet to a valid ethernet header
        eth->setEthernetHeader(packet);
        BOOST_CHECK(eth->getEthernetType() == ETH_P_IP);

	// executing the packet
	// forward the packet through the multiplexers
        mux_eth->setPacketInfo(0,packet,length);
        mux_eth->forward();	

	delete ip;
	delete eth;
}

BOOST_AUTO_TEST_CASE (test3_ip) // ethernet -> vlan -> ip 
{
        EthernetProtocol *eth = new EthernetProtocol();
        MultiplexerPtr mux_eth = MultiplexerPtr(new Multiplexer());
        VLanProtocol *vlan = new VLanProtocol();
        MultiplexerPtr mux_vlan = MultiplexerPtr(new Multiplexer());
	IPProtocol *ip = new IPProtocol();
        MultiplexerPtr mux_ip = MultiplexerPtr(new Multiplexer());

	// ethernet -> vlan ->ip ->udp -> dns
	char *raw_packet = "\x00\x0c\x29\x2e\x3c\x2a\x90\x84\x0d\x62\xd8\x04" "\x81\x00\x02\x5e\x08\x00"
		"\x45\x00"
		"\x00\x3d\x8a\x0d\x00\x00\xec\x11\xf4\x4f\xc0\xa8\x01\x76\x50\x3a"
		"\x3d\xfa\xe9\xb3\x00\x35\x00\x29\x05\x94\x84\xd3\x01\x00\x00\x01"
		"\x00\x00\x00\x00\x00\x00\x02\x63\x68\x04\x70\x6f\x6f\x6c\x03\x6e"
		"\x74\x70\x03\x6f\x72\x67\x00\x00\x01\x00\x01";
	int length = 79;
	unsigned char *packet = reinterpret_cast <unsigned char*> (raw_packet);
	
	//configure the eth 
	eth->setMultiplexer(mux_eth);
        mux_eth->setHeaderSize(eth->header_size);
        mux_eth->addChecker(std::bind(&EthernetProtocol::ethernetChecker,eth));

	//configure the vlan 
	vlan->setMultiplexer(mux_vlan);
        mux_vlan->setHeaderSize(vlan->header_size);
        mux_vlan->addChecker(std::bind(&VLanProtocol::vlanChecker,vlan));

	// configure the ip
	ip->setMultiplexer(mux_ip);
        mux_ip->setHeaderSize(ip->header_size);
        mux_ip->addChecker(std::bind(&IPProtocol::ipChecker,ip));

	// configure the multiplexers
        mux_eth->addUpMultiplexer(mux_vlan,ETH_P_8021Q);
        mux_vlan->addDownMultiplexer(mux_eth);	
        mux_vlan->addUpMultiplexer(mux_ip,ETHERTYPE_IP);
        mux_ip->addDownMultiplexer(mux_vlan);	

	// executing the packet
	// forward the packet through the multiplexers
        mux_eth->setPacketInfo(0,packet,length);
        mux_eth->forward();	

	BOOST_CHECK(vlan->getTotalPackets() == 1);
	BOOST_CHECK(ip->getTotalPackets() == 1);

        BOOST_CHECK(mux_eth->getTotalForwardPackets() == 1);
        BOOST_CHECK(mux_vlan->getTotalForwardPackets() == 1);
        BOOST_CHECK(mux_ip->getTotalForwardPackets() == 0);
        BOOST_CHECK(mux_ip->getTotalFailPackets() == 1);

        BOOST_CHECK(mux_eth->getPacketLength() == length);
        BOOST_CHECK(mux_vlan->getPacketLength() == length - 14);
        BOOST_CHECK(mux_ip->getPacketLength() == length - 14 - 20);

        //vl->setVLanHeader(mux_vlan->getRawPacket());
        BOOST_CHECK(eth->getEthernetType() == ETH_P_8021Q);
        BOOST_CHECK(vlan->getEthernetType() == ETH_P_IP);

	delete ip;
	delete vlan;
	delete eth;
}

BOOST_AUTO_TEST_SUITE_END( )

