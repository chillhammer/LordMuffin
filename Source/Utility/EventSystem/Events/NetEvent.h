#pragma once
#pragma once

#include <EventSystem/Event.h>


namespace Skel
{
	/**
		A Net Event is related to network.
		All of these events must be connected to network
	**/

	EVENT_CLASS_SIMPLE(ClientConnect,
		ClientConnectEvent(ARGS(uint16 id)) : ARGS(ClientID(id)) {};
		uint16 ClientID;
	);

	EVENT_CLASS_SIMPLE(ClientDisconnect,
		ClientDisconnectEvent(ARGS(uint16 id)) : ARGS(ClientID(id)) {};
		uint16 ClientID;
	);
}