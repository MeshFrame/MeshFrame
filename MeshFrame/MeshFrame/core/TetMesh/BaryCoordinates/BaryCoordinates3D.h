#pragma once

#define DIMENSION_BARYCOORD_3D 3
#include <MeshLib\core\TetMesh\TInterface.h>
#include <MeshLib\core\TetMesh\TIterators.h>
#include <MeshLib\core\TetMesh\Face.h>
#include <MeshLib\core\TetMesh\Edge.h>
#include <MeshLib\core\TetMesh\HalfEdge.h>
#include <MeshLib\core\TetMesh\HalfFace.h>
#include <MeshLib\core\TetMesh\vertex.h>
#include <vector>

namespace MeshLib
{
	namespace TMeshLib
	{
		class CBaryCoordinates3D {
		public:
			CBaryCoordinates3D()  {};
			CBaryCoordinates3D(CHalfFace * pHF)
			{
				int i = 0;
				for (auto pHE : TIt::HF_HEIterator(pHF)) {
					setpHE(pHE, i);
					CVertex* pV = TIf::HalfEdgeTarget(TIf::HalfEdgeNext(pHE));
					setpV(pV, i);
					++i;
				}
			}

			CVertex* getpV(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpVerticeCorresponding[i]; };
			CHalfEdge* getpHE(int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); return vpHalfEdgeCorresponding[i]; };

			void setpV(CVertex* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpVerticeCorresponding[i] = value; };
			void setpHE(CHalfEdge* value, int i) { assert(i >= 0 && i < DIMENSION_BARYCOORD_3D); vpHalfEdgeCorresponding[i] = value; };

			CPoint bary2Decartes(CPoint bary) 
			{ 
				return (bary[0] * vpVerticeCorresponding[0]->position()
					+ bary[1] * vpVerticeCorresponding[1]->position()
					+ bary[2] * vpVerticeCorresponding[2]->position());
			};
		private:
			typedef TInterface<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet> TIf;
			typedef TIterators<TIf> TIt;

			CVertex* vpVerticeCorresponding[DIMENSION_BARYCOORD_3D];
			CHalfEdge* vpHalfEdgeCorresponding[DIMENSION_BARYCOORD_3D];
		};
	}
}