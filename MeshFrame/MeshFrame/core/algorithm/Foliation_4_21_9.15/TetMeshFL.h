#pragma once
#include "../../core/TetMesh/TMeshLibHeaders.h"
#include "TFaceFL.h"
#include "tetFL.h"
#include "../../algorithm/Foliation/BorderType.h"

#include <list>
#include <random>
#include <iostream>

using std::cout;
using std::endl;
namespace MeshLib
{
	namespace TMeshLib
	{
		template <typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		class CTMeshFL : public CTMesh<TV, V, HE, TE, E, HF, F, T> {
		public:
			typedef TetHalfFaceIterator<TV, V, HE, TE, E, HF, F, T> TetHalfFaceIterator;
			typedef TMeshTetIterator< TV, V, HE, TE, E, HF, F, T> TMeshTetIterator;
			typedef TVertexInHalfEdgeIterator< TV, V, HE, TE, E, HF, F, T> TVertexInHalfEdgeIterator;
			typedef VertexTVertexIterator< TV, V, HE, TE, E, HF, F, T> VertexTVertexIterator;
			typedef TetEdgeIterator< TV, V, HE, TE, E, HF, F, T> TetEdgeIterator;
			void foliationBreadthFirstRandom(std::list<T*> & sourceList);
			std::list<T*> markBoundaryFaceSource(int sinkID);


		private:
			std::list<T*> simplexList;
			std::list<T*> shellingOrder;
			T* randomChooseShelling(std::list<T*> &tList);
			bool isShelling(T* newSimplex);
			bool foliationDone = false;
		
		};
		
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::foliationBreadthFirstRandom(std::list<T*> & sourceList)
		{	
			cout << "Assign direction field vai shelling begin." << endl;
			simplexList = sourceList;
			while (!simplexList.empty()) {

				T* simplex = randomChooseShelling(simplexList);;
				//Debug
				//std::cout << simplex->id() << std::endl;
				//getchar();
				shellingOrder.push_back(simplex);
				simplex->markDeleted();

				simplex->assignDirectionFL(this);

				//for (MeshVertexIterator viter(this); !viter.end(); ++viter)
				for (TetHalfFaceIterator THFItr(this, simplex); !THFItr.end(); ++THFItr) {
					HF* pHF = *THFItr;
					F* pFace = (F*)pHF->face();
					if (pFace->getBorderType() == BorderType::interior) {
						pFace->setBorderType(BorderType::source);
					}
					HF* pDualHF = (HF*)pHF->dual();
					if (pDualHF != NULL) {
						T* nextSimplex = (T*)pDualHF->tet();
						auto pos = find(simplexList.cbegin(), simplexList.cend(), nextSimplex);
						if (!nextSimplex->hasDeleted() && pos == simplexList.cend())
							simplexList.push_back(nextSimplex);
					}
				}
			}
			foliationDone = true;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline std::list<T*> CTMeshFL<TV, V, HE, TE, E, HF, F, T>::markBoundaryFaceSource(int sinkID)
		{
			std::list<T*> sourceList;
			F* pFace;
			T* pTet;
			for (TMeshTetIterator TMTItr(this); !TMTItr.end(); ++TMTItr) {
				pTet = *TMTItr;
				for (TetHalfFaceIterator THFItr(this, pTet); !THFItr.end(); ++THFItr)
				{
					HF* pHF = *THFItr;
					pFace = (F*)pHF->face();
					if (pTet->id() == sinkID)
					{
						pTet->markDeleted();
						pFace->setBorderType(BorderType::sink);
					}
					else if (pFace->boundary()) {
						pFace->setBorderType(BorderType::source);
						sourceList.push_back(pTet);
						break;
					}
				}
			}
			return sourceList;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline T * CTMeshFL<TV, V, HE, TE, E, HF, F, T>::randomChooseShelling(std::list<T*> &tList)
		{
			static std::default_random_engine generator(time(NULL));
			std::uniform_int_distribution<int> randInt(0, tList.size() - 1);
			std::list<T*>::iterator FLItr;
			T* simplex;
			do {
				FLItr = simplexList.begin();
				int k = randInt(generator);
				for (int i = 0; i < k; ++i) {
					++FLItr;
				}
				simplex = *FLItr;

			} while (!isShelling(simplex));
			tList.erase(FLItr);
			return simplex;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline bool CTMeshFL<TV, V, HE, TE, E, HF, F, T>::isShelling(T * newSimplex)
		{
			//check if there is 0-dimensional intersection
			bool possibleSingular;
			for (int i = 0; i < 4; i++)
			{
				possibleSingular = true;
				TV* pTV = newSimplex->tvertex(i);
				for (TVertexInHalfEdgeIterator TvHfItr(this, pTV); !TvHfItr.end(); ++TvHfItr ) {
					HE* pHe = *TvHfItr;
					if (pHe->half_face()->dual() != NULL)
					{
						T* pAdjacentTet = (T*)pHe->half_face()->dual()->tet();
						if (pAdjacentTet->hasDeleted()) {
							possibleSingular = false;
						}
					}
				}
				if (possibleSingular)
				{
					V* pV = pTV->vert();
					for (VertexTVertexIterator VtVItr(this,pV); !VtVItr.end(); ++VtVItr)
					{
						TV* pTV = *VtVItr;
						T* pTet = (T*)pTV->tet();
						if (pTet->hasDeleted())
						{
							return false;
						}
					}
				}
			}

			//check if there is 1-dimensional intersection
			for (TetEdgeIterator TEItr(this, newSimplex); !TEItr.end(); ++TEItr) {
				possibleSingular = true;
				E *pE = *TEItr;
				TE *pTE = NULL;
				//find the TEdge corresponding to newSimplex 
				for (auto ETEItr = pE->edges()->begin(); ETEItr != pE->edges()->end(); ++ETEItr) {
					TE *pTEtemp = (TE*) *ETEItr;
					if (pTEtemp->tet()->id() == newSimplex->id()) {
						pTE = pTEtemp;
						break;
					}
				}
				assert(pTE != NULL);
				HF* pHF1 = (HF*)pTE->left()->half_face()->dual();
				HF* pHF2 = (HF*)pTE->right()->half_face()->dual();
				if (pHF1 != NULL) {
					T* pTtemp = (T*)pHF1->tet();
					if (pTtemp->hasDeleted())
						possibleSingular = false;
				}

				if (pHF2 != NULL) {
					T* pTtemp = (T*)pHF2->tet();
					if (pTtemp->hasDeleted())
						possibleSingular = false;
				}
				if (possibleSingular) {
					for (auto ETEItr = pE->edges()->begin(); ETEItr != pE->edges()->end(); ++ETEItr) {
						TE *pTEtemp = (TE*) *ETEItr;
						T* pAdjacentT = (T*)pTEtemp->tet();
						if (pAdjacentT->hasDeleted()){
							return false;
						}
					}
				}
			}	
			return true;
		}


		
	}
}