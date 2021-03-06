#ifndef _torrent_test_packets_H_
#define _torrent_test_packets_H_


static char *raw_packet_ethernet_ip_udp_torrent_dht = 
	"\x52\x54\x00\x12\x35\x02\x08\x00\x27\x57\xbf\x32\x08\x00\x45\x00"
	"\x00\x56\x00\x00\x40\x00\x40\x11\xe3\x3c\x0a\x00\x02\x0f\x58\xbe"
	"\xf2\x8d\xc8\xd5\x1a\xe1\x00\x42\x57\xae\x64\x31\x3a\x61\x64\x32"
	"\x3a\x69\x64\x32\x30\x3a\xc4\x99\x79\x9e\x05\x44\x67\xc6\x0d\xce"
	"\xa4\x4d\x6d\x8a\x16\xfa\x7f\x23\x5a\x67\x65\x31\x3a\x71\x34\x3a"
	"\x70\x69\x6e\x67\x31\x3a\x74\x34\x3a\x70\x6e\x00\x00\x31\x3a\x79"
	"\x31\x3a\x71\x65";

static int raw_packet_ethernet_ip_udp_torrent_dht_length = 100;

static char *raw_packet_ethernet_ip_tcp_torrent =
	"\x52\x54\x00\x12\x35\x02\x08\x00\x27\x57\xbf\x32\x08\x00\x45\x00"
	"\x00\x6c\xb8\xa8\x40\x00\x40\x06\xbe\x3a\x0a\x00\x02\x0f\x47\x43"
	"\x70\x57\xd6\x2a\x8c\x2d\x47\x1a\x97\x5d\x01\x10\x76\x02\x50\x18"
	"\x39\x08\xc4\x07\x00\x00\x13\x42\x69\x74\x54\x6f\x72\x72\x65\x6e"
	"\x74\x20\x70\x72\x6f\x74\x6f\x63\x6f\x6c\x80\x00\x00\x00\x00\x13"
	"\x00\x00\x01\x75\xbc\x86\x17\xf5\x31\x5b\x79\x2e\xc8\xb5\xc3\xb9"
	"\xfa\xa2\xa8\xd4\x34\xcd\x2d\x41\x5a\x34\x33\x30\x36\x2d\x66\x4b"
	"\x49\x77\x53\x67\x54\x4e\x4e\x4c\x51\x50";

static int raw_packet_ethernet_ip_tcp_torrent_length =122;

#endif
