#pragma once

// Declare types here. Seperate from class declarations since Packet.h relies on this
#define CUSTOM_PACKETS PACKET_JOIN_REQUEST = 0, PACKET_JOIN_ACCEPT = 1, PACKET_JOIN_DECLINED = 2, \
						PACKET_INPUT = 3