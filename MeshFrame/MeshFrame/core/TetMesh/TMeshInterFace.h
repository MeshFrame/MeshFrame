#pragma once
#include "../../core/TetMesh/titerators.h"

namespace MeshLib {
	namespace TMeshLib {
		template< typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CTMeshInterface
		{
		public:
			typedef TetHalfFaceIterator<TV, V, HE, TE, E, HF, F, T> TetHalfFaceIterator;
		};
	}
}