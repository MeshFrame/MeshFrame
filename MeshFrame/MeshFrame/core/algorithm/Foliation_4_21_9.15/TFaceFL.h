#pragma once
#include "../../core/TetMesh/TMeshLibHeaders.h"
#include "../../algorithm/Foliation/BorderType.h"
#include "../../algorithm/Foliation/BaryCoordinates3D.h"

namespace MeshLib
{
	namespace TMeshLib
	{
		class CTFaceFL : public CFace, public BorderType::CBorderType, CBaryCoordinates3D{
		};
	}
}