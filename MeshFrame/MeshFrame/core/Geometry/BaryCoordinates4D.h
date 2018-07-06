#pragma once

#define DIMENSION_BARYCOORD_4D 4

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
		class CBaryCoordinates4D {
		public:
			CBaryCoordinates4D() :
				coordinates(0.0, 0.0, 0.0, 0.0) {};

			double getCoordinate(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); return coordinates[i]; };
			CVertex* getCorrespondingPVertex(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); return vpVerticeCorresponding[i]; };
			CHalfFace* getCorrespondingPHalfface(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); return vpHalffaceCorresponding[i]; };

			void setCoordinate(double value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); coordinates[i] = value; };

			void setCoordinate(CPoint4 value) { assert(value[0] + value[1] < 1.0000001 && 0.9999999 < value[0] + value[1]);  coordinates = value; };
			void setVpVertice(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); vpVerticeCorresponding[i] = value; };
			void setVpHalfface(CHalfFace* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_4D); vpHalffaceCorresponding[i] = value; };
		private:
			CPoint4 coordinates;
			CVertex* vpVerticeCorresponding[DIMENSION_BARYCOORD_4D];
			CHalfFace* vpHalffaceCorresponding[DIMENSION_BARYCOORD_4D];
		};

	}
}