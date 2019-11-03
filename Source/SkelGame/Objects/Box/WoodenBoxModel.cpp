#include <skelpch.h>
#include <Resources/ResourceManager.h>
#include "BoxMesh.h"
#include "WoodenBoxModel.h"

namespace Skel
{
	WoodenBoxModel::WoodenBoxModel() : SingleMeshModel(*Resources.GetMesh("WoodenBox"))
	{

	}
}