#pragma once

#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

namespace Skel::Net
{
	class Address;
	inline constexpr uint16	 MAX_PLAYERS = 8;

#ifndef SERVER
	// Not const because ImGui editable
	inline			float	 FAKE_LAG_S = 0.0f; //seconds
	inline			float	 FAKE_JITTER_S = 0.0f; //seconds
	inline			float	 FAKE_PACKET_LOSS = 0.0f; //seconds
#endif

#ifdef SERVER
	inline			 float	 FAKE_LAG_S = 0.0f; //seconds
	inline			 float	 FAKE_JITTER_S = 0.0f; //seconds
	inline			 float	 FAKE_PACKET_LOSS = 0.0f; //seconds
#endif


	inline constexpr uint16	 SYNC_SAMPLES = 30; // how many sync packets are sent out
	inline constexpr uint8	 SNAPSHOT_PER_SEC = 20; // how many snapshot packets are sent out

	inline constexpr uint64	 SERVER_TICK_PER_SEC = 60;

	inline constexpr double	 SNAPSHOT_RATE = 1.0 / double(SNAPSHOT_PER_SEC);

	inline constexpr double	 SNAPSHOT_INTER_BUFFER = 0.1; // seconds to delay snapshots for interpolation

	inline constexpr double	 CLIENT_TIMEOUT_TIME = 5.0; // seconds before giving up on server
	inline constexpr double	 SERVER_TIMEOUT_TIME_PER_CLIENT = 5.0; // seconds before server gives up on client

	inline constexpr bool	 ENABLE_CLIENT_TIMEOUT = false;// true;
	inline constexpr bool	 ENABLE_SERVER_TIMEOUT_PER_CLIENT = false;// true;


	inline constexpr int	 PREDICTED_STATES = 512; // ticks to keep in memory
	inline constexpr int	 INPUTS_PACKED = 16;		 // amount of previous inputs to send

	void Init();
	Address GetServerAddress();
}