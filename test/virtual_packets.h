#ifndef _virtual_packets_H_
#define _virtual_packets_H_

// A Mini database of different Virtual Packets of VxLan 


// Ethernet and Arp request
static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_arp_request  = 
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00\x45\x00"
	"\x00\x46\x04\xd2\x00\x00\x7f\x11\x36\xd6\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x23\x45\x23\x45\x00\x32\x00\x00\x08\x00\x00\x00\x74\x91"
	"\x27\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x00"
	"\x45\x00\x00\x14\x04\xd2\x00\x00\x7f\x00\x37\x19\x00\x00\x00\x00"
	"\x00\x00\x00\x00";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_arp_request_length = 92;

// Ethernet and IP and ICMP echo reply
static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_icmp_reply =
	"\x36\xdc\x85\x1e\xb3\x40\x00\x16\x3e\x08\x71\xcf\x08\x00\x45\x00"
	"\x00\x86\xb8\x03\x40\x00\x3e\x11\x6e\x0f\xc0\xa8\xca\x01\xc0\xa8"
	"\xcb\x01\x80\x7e\x12\xb5\x00\x72\x00\x00\x08\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x16\x3e\x37\xf6\x04\x00\x30\x88\x01\x00\x02\x08\x00"
	"\x45\x00\x00\x54\xb8\xb3\x00\x00\x40\x01\xaa\x9b\xc0\xa8\xcb\x05"
	"\xc0\xa8\xcb\x03\x00\x00\xfe\xf2\x05\x0c\x00\x01\xfc\xe2\x97\x51"
	"\x00\x00\x00\x00\xa6\xf8\x02\x00\x00\x00\x00\x00\x10\x11\x12\x13"
	"\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23"
	"\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33"
	"\x34\x35\x36\x37";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_icmp_reply_length = 148;

// Ethernet and IP and UDP and DNS request
// DNS request to github.com
// The VNI is 0

static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_udp_dns_request =
        "\x36\xdc\x85\x1e\xb3\x40\x00\x16\x3e\x08\x71\xcf\x08\x00\x45\x00"
        "\x00\x86\xb8\x03\x40\x00\x3e\x11\x6e\x0f\xc0\xa8\xca\x01\xc0\xa8"
        "\xcb\x01\x80\x7e\x12\xb5\x00\x46\x00\x00\x08\x00\x00\x00\x00\x00"
        "\x00\x00"
	"\x24\x76\x7d\x39\xb8\x71\x8c\xa9\x82\x49\x43\x5c\x08\x00\x45\x00"
	"\x00\x38\x64\xc0\x40\x00\x40\x11\x1a\xd5\xc0\xa8\x01\x0d\x59\x65"
	"\xa0\x05\xba\xf8\x00\x35\x00\x24\x74\xcd\x08\xc4\x01\x00\x00\x01"
	"\x00\x00\x00\x00\x00\x00\x06\x67\x69\x74\x68\x75\x62\x03\x63\x6f"
	"\x6d\x00\x00\x01\x00\x01";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_udp_dns_request_length = 120;

// Ethernet and IP and UDP and DNS request
// DNS request to gitgit.com
// The VNI is 12340 

static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_udp_dns_request_2 =
        "\x36\xdc\x85\x1e\xb3\x40\x00\x16\x3e\x08\x71\xcf\x08\x00\x45\x00"
        "\x00\x86\xb8\x03\x40\x00\x3e\x11\x6e\x0f\xc0\xa8\xca\x01\xc0\xa8"
        "\xcb\x01\x80\x7e\x12\xb5\x00\x46\x00\x00\x08\x00\x00\x00\x00\x30"
        "\x34\x00"
        "\x24\x76\x7d\x39\xb8\x71\x8c\xa9\x82\x49\x43\x5c\x08\x00\x45\x00"
        "\x00\x38\x64\xc0\x40\x00\x40\x11\x1a\xd5\xc0\xa8\x01\x0d\x59\x65"
        "\xa0\x05\xba\xf8\x00\x35\x00\x24\x74\xcd\x08\xc4\x01\x00\x00\x01"
        "\x00\x00\x00\x00\x00\x00\x06\x67\x69\x74\x67\x69\x74\x03\x63\x6f"
        "\x6d\x00\x00\x01\x00\x01";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_udp_dns_request_2_length = 120;

// Ethernet IP and ICMP request with GRE

static char *raw_packet_ethernet_ip_gre_ethernet_ip_icmp_request =
	"\x00\x16\x3e\x08\x61\xcf\x4a\x2c\x9c\x1d\x5b\x47\x08\x00\x45\x00"
	"\x00\x7a\x26\xae\x40\x00\x40\x2f\x01\x53\xc0\xa8\xc9\x01\xc0\xa8"
	"\xc8\x01\x00\x00\x65\x58\x00\x30\x88\x00\x00\x02\x00\x16\x3e\x37"
	"\xe6\x04\x08\x00\x45\x00\x00\x54\x00\x00\x40\x00\x40\x01\x27\x4f"
	"\xc0\xa8\xc9\x03\xc0\xa8\xc9\x05\x08\x00\x9a\x13\x05\x0a\x00\x01"
	"\x81\xe2\x97\x51\x00\x00\x00\x00\x78\xda\x08\x00\x00\x00\x00\x00"
	"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
	"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
	"\x30\x31\x32\x33\x34\x35\x36\x37";

static int raw_packet_ethernet_ip_gre_ethernet_ip_icmp_request_length = 136;

// Ethernet IP and ICMP replay with GRE, the response of the previous packet

static char *raw_packet_ethernet_ip_gre_ethernet_ip_icmp_reply =
	"\x4a\x2c\x9c\x1d\x5b\x47\x00\x16\x3e\x08\x61\xcf\x08\x00\x45\x00"
	"\x00\x7a\xcc\x44\x40\x00\x3e\x2f\x5d\xbc\xc0\xa8\xc8\x01\xc0\xa8"
	"\xc9\x01\x00\x00\x65\x58\x00\x16\x3e\x37\xe6\x04\x00\x30\x88\x00"
	"\x00\x02\x08\x00\x45\x00\x00\x54\x04\xf2\x00\x00\x40\x01\x62\x5d"
	"\xc0\xa8\xc9\x05\xc0\xa8\xc9\x03\x00\x00\xa2\x13\x05\x0a\x00\x01"
	"\x81\xe2\x97\x51\x00\x00\x00\x00\x78\xda\x08\x00\x00\x00\x00\x00"
	"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
	"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
	"\x30\x31\x32\x33\x34\x35\x36\x37";

static int raw_packet_ethernet_ip_gre_ethernet_ip_icmp_reply_length = 136;


// Ethernet IP TCP and FTP syn packet
// virtual ipsrc 192.168.1.23 ipdst 192.168.1.100

static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_tcp_syn = 
	"\xa1\xa2\xa3\xa4\xa5\xa6\x01\x02\x03\x04\x05\x06\x08\x00\x45\x00"
	"\x00\x62\x00\x00\x00\x00\x40\x11\xf7\x37\xc0\xa8\x01\x02\xc0\xa8"
	"\x01\x01\xa0\x40\x12\xb5\x00\x4e\xf1\x90\x08\x00\x00\x00\x00\x00"
	"\x01\x00\x0f\x12\x00\x00\x05\x06\xf0\x02\xa0\xa4\xa5\xa6\x08\x00"
	"\x45\x00\x00\x30\x00\x00\x00\x00\x40\x06\xf6\xfc\xc0\xa8\x01\x17"
	"\xc0\xa8\x01\x64\x00\x14\x04\x0a\x40\xa7\x04\x28\x00\x00\x00\x00"
	"\x70\x02\xc0\x00\xf6\x65\x00\x00\x02\x04\x05\xb4\x01\x01\x04\x02";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_tcp_syn_length = 112;

// Ethernet IP TCP and FTP syn/ack packet, response to the previous packet
// virtual ipsrc 192.168.1.100 ipdst 192.168.1.23

static char *raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_tcp_synack =
	"\x01\x02\x03\x04\x05\x06\xa1\xa2\xa3\xa4\xa5\xa6\x08\x00\x45\x00"
	"\x00\x62\x00\x00\x00\x00\x40\x11\xf7\x37\xc0\xa8\x01\x01\xc0\xa8"
	"\x01\x02\x12\xb5\xa0\x40\x00\x4e\xf1\x90\x08\x00\x00\x00\x00\x00"
	"\x01\x00\xf0\x02\xa0\xa4\xa5\xa6\x0f\x12\x00\x00\x05\x06\x08\x00"
	"\x45\x00\x00\x30\x00\x00\x00\x00\x40\x06\xf6\xfc\xc0\xa8\x01\x64"
	"\xc0\xa8\x01\x17\x04\x0a\x00\x14\x06\xf5\x46\x6f\x40\xa7\x04\x29"
	"\x70\x12\xfc\x00\x6c\xf0\x00\x00\x02\x04\x05\xb4\x01\x01\x04\x02";

static int raw_packet_ethernet_ip_udp_vxlan_ethernet_ip_tcp_synack_length = 112;


#endif
