#pragma once
#include "..\..\MeshLib\core\Geometry\Point2.h"
#include "..\..\MeshLib\core\Mesh\Face.h"
#include "..\..\MeshLib\core\Mesh\Edge.h"
#include "..\..\MeshLib\core\Mesh\HalfEdge.h"
#include "..\..\MeshLib\core\Mesh\BaseMesh.h"
#include <vector>

#define DIMENSION_BARYCOORD_2D 2
namespace MeshLib 
{
	class CBaryCoordinates2D
	{
	public:
		CBaryCoordinates2D(): 
			vpVerticeCorresponding(DIMENSION_BARYCOORD_2D), 
			coordinates(0.0, 0.0) {};
		
		double getCoordinate(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_2D); return coordinates[i]; };
		CVertex* getCorrespondingPVertex(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_2D); return vpVerticeCorresponding[i]; };
		void setCoordinate(double value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_2D); coordinates[i] = value; };

		void setCoordinate(CPoint2 value) { assert( value[0] + value[1] < 1.0000001 && 0.9999999 < value[0] + value[1] );  coordinates = value; };
		void setVpVertice(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_2D); vpVerticeCorresponding[i] = value; };
	private:
		CPoint2 coordinates;
		std::vector<CVertex*> vpVerticeCorresponding;
		
	};
}
