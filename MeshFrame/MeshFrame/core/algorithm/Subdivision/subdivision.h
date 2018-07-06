#pragma once
#include "../../core/TetMesh/basetmesh_orientation.h"
#include "../../core/TetMesh/vertex.h"
#include "../../core/TetMesh/tvertex.h"
#include "../../core/TetMesh/edge.h"
#include "../../core/TetMesh/tedge.h"
#include "../../core/TetMesh/face.h"
#include "../../core/TetMesh/halfface.h"
#include "../../core/TetMesh/halfedge.h"
#include "../../core/TetMesh/tet.h"
#include "../../core/TetMesh/titerators.h"

#include <list>
#include <iostream>
#include <fstream>
namespace MeshLib {
	namespace TMeshLib {
		template <typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CTMeshSubdivision : public CTMesh<TV, V, HE, TE, E, HF, F, T> {
		public:
			typedef TMeshVertexIterator<TV, V, HE, TE, E, HF, F, T> TMeshVertexIterator;
			typedef TMeshTetIterator<TV, V, HE, TE, E, HF, F, T> TMeshTetIterator;
			typedef TetHalfFaceIterator<TV, V, HE, TE, E, HF, F, T> TetHalfFaceIterator;
			typedef FaceVertexIterator<TV, V, HE, TE, E, HF, F, T> FaceVertexIterator;
			void subdivisionCentricStar(char* savePath);
		private:

		};
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshSubdivision<TV, V, HE, TE, E, HF, F, T>::subdivisionCentricStar(char * savePath)
		{
			std::list<CPoint> pointList;
			std::list<int> pointidList;
			std::list<std::vector<int>> tetList;
			
			int numV = 0;
			for (TMeshVertexIterator MVIter(this); !MVIter.end(); ++MVIter) {
				++numV;
				V* pV = *MVIter;
				pointList.push_back(pV->position());
				pointidList.push_back(pV->id());
			}

			for (TMeshTetIterator MTIter(this); !MTIter.end(); ++MTIter) {
				++numV;
				int centricID = numV;

				T* pT = *MTIter;
				CPoint V0 = pT->tvertex(0)->vert()->position();
				CPoint V1 = pT->tvertex(1)->vert()->position();
				CPoint V2 = pT->tvertex(2)->vert()->position();
				CPoint V3 = pT->tvertex(3)->vert()->position();
				CPoint centric = (V0 + V1 + V2 + V3) / 4;
				pointList.push_back(centric);
				pointidList.push_back(centricID);
				for (TetHalfFaceIterator THfIter(this, pT); !THfIter.end(); ++THfIter) {
					std::vector<int> newTet;
					F* pF = (*THfIter)->face();
					for (FaceVertexIterator FVIter(this, pF); !FVIter.end(); ++FVIter) {
						V *pV = *FVIter;
						newTet.push_back(pV->id());
					}
					newTet.push_back(centricID);
					tetList.push_back(newTet);
				}
			}

			//write to .t file
			std::ofstream outF(savePath);
			if (!outF.is_open()) {
				std::cout << "Open " << savePath << " failed." << std::endl;
				return;
			}
			//write vertices
			assert(pointidList.size() == pointidList.size());
			auto vIter = pointList.begin();
			auto vidIter = pointidList.begin();
			while (vIter != pointList.end()) {
				CPoint vPosition = *vIter;
				int vID = *vidIter;
				outF << "Vertex " << vID << " " << vPosition[0] << " " << vPosition[1] << " " << vPosition[2] << "\n";
				++vIter;
				++vidIter;
			}
			//write tets
			int tetID = 1;
			for (auto TIter = tetList.begin(); TIter != tetList.end(); ++TIter) {
				auto Tet = *TIter;
				outF << "Tet " << tetID << " " << Tet[0] << " " << Tet[1] << " " << Tet[2] << " " << Tet[3] << "\n";
				++tetID;
			}
			outF.close();
		}
	}
}