#pragma once

#define DIMENSION_BARYCOORD_3D 3

#include "..\Geometry\Point.h"
#include "..\Mesh\Face.h"
#include "..\Mesh\Edge.h"
#include "..\Mesh\HalfEdge.h"
#include "..\Mesh\vertex.h"
#include <vector>

namespace MeshLib
{
	namespace TMeshLib
	{
		class CBaryCoordinates3D {
		public:
			CBaryCoordinates3D()  {};
			CBaryCoordinates3D(CFace *pF);

			CVertex* getCorrespondingPVertex(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpVerticeCorresponding[i]; };
			CPoint getCorrespondingPoint(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpVerticeCorresponding[i]->point(); };
			CHalfEdge* getCorrespondingPHalfface(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpHalfEdgeCorresponding[i]; };
			CPoint bray2Descartes(CPoint bary) { return (bary[0]*getCorrespondingPoint(0) + bary[1]*getCorrespondingPoint(1) + bary[2]*getCorrespondingPoint(2)); };
			int getVertexID(CVertex *pV);

			void setpVertice(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpVerticeCorresponding[i] = value; };
			void setpHalfEdge(CHalfEdge* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpHalfEdgeCorresponding[i] = value; };
		private:
			CVertex* vpVerticeCorresponding[DIMENSION_BARYCOORD_3D];
			CHalfEdge* vpHalfEdgeCorresponding[DIMENSION_BARYCOORD_3D];
		};
		TMeshLib::CBaryCoordinates3D::CBaryCoordinates3D(CFace * pF)
		{
			typedef FaceHalfedgeIterator<CVertex, CEdge, CFace, CHalfEdge> FaceHalfedgeIterator;
			int i = 0;
			for (FaceHalfedgeIterator FHIter(pF); !FHIter.end(); ++FHIter) {
				CHalfEdge *pH = *FHIter;
				setpHalfEdge(pH, i);
				CHalfEdge *pNextH = pH->he_next();
				CVertex *pV = pH->target();
				setpVertice(pV, i);
				++i;

			}
		}
		int CBaryCoordinates3D::getVertexID(CVertex * pV)
		{
			int ID = -1;
			for (int i = 0; i < DIMENSION_BARYCOORD_3D; ++i) {
				if (pV == getCorrespondingPVertex(i))
				{
					ID = i;
				}
			}
			return ID;
		}
	}
}