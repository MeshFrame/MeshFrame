#pragma once

#define DIMENSION_BARYCOORD_3D 3

#include "..\..\core\Geometry\Point4.h"
#include "..\..\core\TetMesh\Face.h"
#include "..\..\core\TetMesh\Edge.h"
#include "..\..\core\TetMesh\HalfEdge.h"
#include "..\..\core\TetMesh\Halfface.h"
#include "..\..\core\TetMesh\vertex.h"
#include <vector>

namespace MeshLib
{
	namespace TMeshLib
	{
		class CBaryCoordinates3D {
		public:
			CBaryCoordinates3D()  {};

			CVertex* getCorrespondingPVertex(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpVerticeCorresponding[i]; };
			CHalfFace* getCorrespondingPHalfface(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpHalffaceCorresponding[i]; };

			void setVpVertice(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpVerticeCorresponding[i] = value; };
			void setVpHalfface(CHalfFace* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpHalffaceCorresponding[i] = value; };
		private:
			CVertex* vpVerticeCorresponding[DIMENSION_BARYCOORD_3D];
			CHalfFace* vpHalffaceCorresponding[DIMENSION_BARYCOORD_3D];
		};

	}
}