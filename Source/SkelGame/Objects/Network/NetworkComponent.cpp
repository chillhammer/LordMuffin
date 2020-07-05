#include "SkelPCH.h"
#include "NetworkComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<NetworkComponent>("Network")
			.constructor<>(registration::public_access);
	}

	void NetworkComponent::Update()
	{
		// Handle the appropriate packett. Their implementations are in another file
#ifdef SERVER
		HandleServerPackets();
#else
		HandleClientPackets();
#endif
	}
}
