#pragma once

// Declare types here. Seperate from class declarations since Packet.h relies on this
#define CUSTOM_PACKETS PACKET_JOIN_REQUEST = 0, PACKET_JOIN_ACCEPT = 1, PACKET_JOIN_DECLINED = 2, \
						PACKET_INPUT = 3, PACKET_SNAPSHOT = 4, PACKET_SYNC_REQUEST = 5, PACKET_SYNC_SERVER_TIME = 6, \
						PACKET_QUIT = 7
