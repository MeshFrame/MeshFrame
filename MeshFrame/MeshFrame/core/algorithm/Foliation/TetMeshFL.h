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
			void foliationBreadthFirstBackTrace(std::list<T*> & sourceList);
			void foliationBreadthFirstGreedy(std::list<T*> & sourceList);
			bool findShellingBackTrace();
			std::list<T*> markBoundaryFaceSource(int sinkID);
			T* markBoundaryFaceSourceReverse(int sinkID);
			void foliationBreadthFirstGreedyReverse(T* pSink);
			void tetCentricMapping();
			void saveShellingOrder(char *path);

		private:
			std::list<T*> simplexList;
			std::list<T*> shellingOrder;
			void makeCandidateList(std::list<T*> &_candidateList);
			T* randomChooseShelling(std::list<T*> &tList);
			T* greedilyChooseShelling(std::list<T*> &tList);
			T* greedilyChooseShellingBackTrace(std::list<T*> &tList);
				
			bool isShelling(T* newSimplex);
			bool backTraceShelling(T* nextSimplex);
			bool foliationDone = false;
			void putInCandidateList(T * introducingSimplex, T* candidateSimplex);
			void putInShellingList(T* nextSimplex);
			void removeFromCandidateList(typename std::list<T*>::iterator FLItr);
			void removeFromShellingList(typename std::list<T*>::iterator FLItr);

		};
		
		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::foliationBreadthFirstRandom(std::list<T*> & sourceList)
		{	
			static std::default_random_engine generator(time(NULL));
			static std::uniform_int_distribution<int> randInt(0, sourceList.size() - 1);
			cout << "Assign direction field vai shelling begin." << endl;

			std::list<T*>::iterator FLItr = sourceList.begin();
			T* beginsource;
			int k = randInt(generator);
			for (int i = 0; i < k; ++i) {
				++FLItr;
			}
			beginsource = *FLItr;
			beginsource->inList = true;
			simplexList.push_front(beginsource);
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
						if (!nextSimplex->inList && !nextSimplex->hasDeleted()) {
							simplexList.push_back(nextSimplex);
							nextSimplex->inList = true;
						}
					}
				}
			}
			foliationDone = true;
			cout << "Congratulations, the dirction assignment succeeded." << endl;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::foliationBreadthFirstBackTrace(std::list<T*>& sourceList)
		{	
			static std::default_random_engine generator(time(NULL));
			static std::uniform_int_distribution<int> randInt(0, sourceList.size() - 1);
			cout << "Assign direction field vai shelling begin, shelling method: Greedy, From Boundary." << endl;

			std::list<T*>::iterator FLItr = sourceList.begin();
			T* beginsource;
			int k = randInt(generator);
			for (int i = 0; i < k; ++i) {
				++FLItr;
			}
			beginsource = *FLItr;
			simplexList.push_front(beginsource);

			while (!simplexList.empty()) {
				//choose a simplex from candidate list simplexList, and judge if it's a bishelling
				T* simplex;
				std::list<T*>::iterator FLItr;
				for (FLItr = simplexList.begin(); FLItr != simplexList.end(); ++FLItr) {
					simplex = *FLItr;
					if (isShelling(simplex)) {
						if (simplex->assignDirectionFLFlag(this)) {
							break;
						}
					}
				}
				if (FLItr == simplexList.end()) {
					//no aviliable next shelling now, trace back
					--FLItr;
					T* pSimpleToDelete = *FLItr;
					//remove pSimpleToDelete from shelling list, but don't add it back to candidate list
					removeFromShellingList(FLItr);
					//remove simplices in candidate list introduced by pSimpleToDelete
					for (auto itr = pSimpleToDelete->introducingList.begin(); itr != pSimpleToDelete->introducingList.end(); ++itr) {
						int id = *itr;
						auto ritr = simplexList.end();
						--ritr;
						for (; ritr != simplexList.begin(); --ritr) {
							if ((*ritr)->id() == id) {
								removeFromCandidateList(ritr);
								break;
							}

						}

					}
				}
				else {
					putInShellingList(simplex);
					removeFromCandidateList(FLItr);
				}
				
				//Debug
				//std::cout << simplex->id() << std::endl;
				//getchar();
				
				

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
						if (!nextSimplex->inList && !nextSimplex->hasDeleted()) {
							putInCandidateList(simplex, nextSimplex);
						}
					}
				}
			}
			foliationDone = true;
			cout << "Congratulations, the dirction assignment succeeded." << endl;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::foliationBreadthFirstGreedy(std::list<T*>& sourceList)
		{
			static std::default_random_engine generator(time(NULL));
			static std::uniform_int_distribution<int> randInt(0, sourceList.size() - 1);
			cout << "Assign direction field vai shelling begin, shelling method: Greedy, From Boundary." << endl;

			std::list<T*>::iterator FLItr = sourceList.begin();
			T* beginsource;
			int k = randInt(generator);
			for (int i = 0; i < k; ++i) {
				++FLItr;
			}
			beginsource = *FLItr;
			simplexList.push_front(beginsource);

			while (!simplexList.empty()) {

				T* simplex = greedilyChooseShelling(simplexList);;
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
						if (!nextSimplex->inList && !nextSimplex->hasDeleted()) {
							simplexList.push_back(nextSimplex);
							nextSimplex->inList = true;
						}
					}
				}
			}
			foliationDone = true;
			cout << "Congratulations, the dirction assignment succeeded." << endl;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool CTMeshFL<TV, V, HE, TE, E, HF, F, T>::findShellingBackTrace()
		{
			for (TMeshTetIterator MTIter(this); !MTIter.end(); ++MTIter) {
				shellingOrder.clear();
				std::cout << "Start from: " << (*MTIter)->id() << std::endl;
				putInShellingList(*MTIter);
				std::list<T*> _candidateList; 
				makeCandidateList(_candidateList);
				for (auto cTIter = _candidateList.begin(); cTIter != _candidateList.end(); ++cTIter) {
					T *pT = *cTIter;
					if (isShelling(pT)) {
						if (backTraceShelling(pT))
							std::cout << "Shelling from: " << (*MTIter)->id() << " Succeeded." << std::endl;
							return true;
					}
				}
				std::cout << "Shelling from: " << (*MTIter)->id() << " Failed." << std::endl;
				return false;
			}
			
			return false;
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
						pTet->markSinkFace();
						pTet->markDeleted();
						pFace->setBorderType(BorderType::sink);
						pFace->setOriginalType(BorderType::sink);
					}
					else if (pFace->boundary()) {
						pFace->setBorderType(BorderType::source);
						pFace->setOriginalType(BorderType::source);
						sourceList.push_back(pTet);
						break;
					}
				}
			}
			return sourceList;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline T* CTMeshFL<TV, V, HE, TE, E, HF, F, T>::markBoundaryFaceSourceReverse(int sinkID)
		{
			T* pSink;
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
						pSink = pTet;
						pTet->markDeleted();
						pTet->markSinkFace();
						pFace->setBorderType(BorderType::sink);
						pFace->setOriginalType(BorderType::sink);
					}
					else if (pFace->boundary()) {
						pFace->setBorderType(BorderType::source);
						pFace->setOriginalType(BorderType::source);
						
					}
				}
			}
			return pSink;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::foliationBreadthFirstGreedyReverse(T * pSink)
		{
			cout << "Assign direction field vai shelling begin, shelling method: Greedy, Reverse." << endl;
			for (TetHalfFaceIterator THFItr(this, pSink); !THFItr.end(); ++THFItr) {
				HF* pHF = *THFItr;
				F* pFace = (F*)pHF->face();
				if (pFace->getBorderType() == BorderType::interior) {
					pFace->setBorderType(BorderType::sink);
				}
				HF* pDualHF = (HF*)pHF->dual();
				if (pDualHF != NULL) {
					T* nextSimplex = (T*)pDualHF->tet();
					if (!nextSimplex->inList && !nextSimplex->hasDeleted()) {
						simplexList.push_back(nextSimplex);
						nextSimplex->inList = true;
					}
				}
			}


			while (!simplexList.empty()) {

				T* simplex = greedilyChooseShelling(simplexList);;
				//T* simplex = randomChooseShelling(simplexList);;
				//Debug
				//std::cout << simplex->id() << std::endl;
				//getchar();
				shellingOrder.push_back(simplex);
				simplex->markDeleted();
				simplex->assignDirectionFLReverse(this);

				//for (MeshVertexIterator viter(this); !viter.end(); ++viter)
				for (TetHalfFaceIterator THFItr(this, simplex); !THFItr.end(); ++THFItr) {
					HF* pHF = *THFItr;
					F* pFace = (F*)pHF->face();
					if (pFace->getBorderType() == BorderType::interior) {
						pFace->setBorderType(BorderType::sink);
					}
					HF* pDualHF = (HF*)pHF->dual();
					if (pDualHF != NULL) {
						T* nextSimplex = (T*)pDualHF->tet();
						if (!nextSimplex->inList && !nextSimplex->hasDeleted()) {
							simplexList.push_back(nextSimplex);
							nextSimplex->inList = true;
						}
					}
				}
			}
			foliationDone = true;
			cout << "Congratulations, the dirction assignment succeeded." << endl;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::tetCentricMapping()
		{
			std::cout << "Mapping Tet centric to R2." << std::endl;
			CPoint4 center(0.3333333333333, 0.3333333333333, 0.3333333333333, 0.3333333333333);
			for (TMeshTetIterator MTItr(this); !MTItr.end(); ++MTItr) {
				T* pT = *MTItr;
				CPoint parameters = pT->paraMapping(center);
				pT->setCentricParameters(parameters);
				ShowDialog(std::cout << "Face:" << pT->id() << std::endl;);
				ShowDialog(std::cout << parameters[0] << "," << parameters[1] << "," << parameters[2] << std::endl;);
				//getchar();
			}

		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::saveShellingOrder(char * path)
		{
			std::ofstream outF(path);
			if (!outF.is_open()) {
				std::cout << "Open " << path << " failed." << std::endl;
				return;
			}
			for (auto shellingIter = shellingOrder.begin(); shellingIter != shellingOrder.end(); ++shellingIter) {
				T* pT = *shellingIter;
				outF << pT->id() << " ";
			}
			outF.close();
		}
		

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		void CTMeshFL<TV, V, HE, TE, E, HF, F, T>::makeCandidateList(std::list<T*> &_candidateList)
		{
			for (auto shellingIter = shellingOrder.begin(); shellingIter != shellingOrder.end(); ++shellingIter) {
				T *pT = *shellingIter;
				for (TetHalfFaceIterator THFItr(this, pT); !THFItr.end(); ++THFItr) {
					HF* pHF = *THFItr;
					HF* pDualHF = (HF*)pHF->dual();
					if (pDualHF != NULL) {
						T* nextSimplex = (T*)pDualHF->tet();
						if (!nextSimplex->hasDeleted()) {
							_candidateList.push_back(nextSimplex);
						}
					}
				}
			}
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline T * CTMeshFL<TV, V, HE, TE, E, HF, F, T>::randomChooseShelling(std::list<T*> &tList)
		{
			static std::default_random_engine generator(time(NULL));
			static std::uniform_int_distribution<int> randInt(0, tList.size() - 1);
			std::list<T*>::iterator FLItr;
			T* simplex;
			do {
				FLItr = tList.begin();
				int k = randInt(generator);
				for (int i = 0; i < k; ++i) {
					++FLItr;
				}
				simplex = *FLItr;

			} while (!isShelling(simplex));
			tList.erase(FLItr);
			simplex->inList = false;
			return simplex;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline T * CTMeshFL<TV, V, HE, TE, E, HF, F, T>::greedilyChooseShelling(std::list<T*>& tList)
		{			
			T* simplex;
			std::list<T*>::iterator FLItr;
			for (FLItr = tList.begin(); FLItr != tList.end(); ++FLItr) {
				simplex = *FLItr;
				if (isShelling(simplex)) {
					break;
				}
			}
			assert(FLItr != tList.end());
			tList.erase(FLItr);
			simplex->inList = false;
			return simplex;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline T * CTMeshFL<TV, V, HE, TE, E, HF, F, T>::greedilyChooseShellingBackTrace(std::list<T*>& tList)
		{
			T* simplex;
			std::list<T*>::iterator FLItr;
			for (FLItr = tList.begin(); FLItr != tList.end(); ++FLItr) {
				simplex = *FLItr;
				if (isShelling(simplex)) {
					break;
				}
			}
			if (FLItr != tList.end()) {
				//no aviliable next shelling now, trace back
			}
			tList.erase(FLItr);
			simplex->inList = false;
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
				E *pE   = *TEItr;
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

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		bool CTMeshFL<TV, V, HE, TE, E, HF, F, T>::backTraceShelling(T * nextSimplex)
		{
			static int minTraceLevel = 1000000000;
			putInShellingList(nextSimplex);
			std::list<T*> _candidateList;
			makeCandidateList(_candidateList);
			if (_candidateList.empty())
				return true;
			int i = 1;
			for (auto cTIter = _candidateList.begin(); cTIter != _candidateList.end(); ++cTIter) {
				T *pT = *cTIter;
				//ShowDialog(std::cout << pT->id() << "in list. " << i << " of " << _candidateList.size() << "Level:" << shellingOrder.size() << "\n";);
				if (isShelling(pT)) {
					if (backTraceShelling(pT))
						return true;
				}
				i++;
			}
			auto shellingOrderListEnd = shellingOrder.end();
			shellingOrderListEnd--;
			removeFromShellingList(shellingOrderListEnd );
			if (shellingOrder.size() < minTraceLevel) {
				minTraceLevel = shellingOrder.size();
				ShowDialog(std::cout << "Back Trace." << "Level:" << shellingOrder.size() << "|Candidate:" << _candidateList.size() << "\n";);
			}
			return false;
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void TMeshLib::CTMeshFL<TV, V, HE, TE, E, HF, F, T>::putInCandidateList(T * introducingSimplex, T * candidateSimplex)
		{
			simplexList.push_back(candidateSimplex);
			candidateSimplex->inList = true;
			candidateSimplex->introducingSimplexId = introducingSimplex->id();
			introducingSimplex->introducingList.push_back(candidateSimplex->id());
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void TMeshLib::CTMeshFL<TV, V, HE, TE, E, HF, F, T>::putInShellingList(T * nextSimplex)
		{
			shellingOrder.push_back(nextSimplex);
			nextSimplex->markDeleted();

		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void TMeshLib::CTMeshFL<TV, V, HE, TE, E, HF, F, T>::removeFromCandidateList(typename std::list<T*>::iterator FLItr)
		{
			T* simplex = *FLItr;
			simplex->inList = false;
			simplex->introducingSimplexId = -1;
			simplexList.erase(FLItr);
		}

		template<typename TV, typename V, typename HE, typename TE, typename E, typename HF, typename F, typename T>
		inline void TMeshLib::CTMeshFL<TV, V, HE, TE, E, HF, F, T>::removeFromShellingList(typename std::list<T*>::iterator FLItr)
		{
			T* simplex = *FLItr;
			simplex->markUndeleted();
			simplex->introducingList.clear();
			shellingOrder.erase(FLItr);
		}


		
	}
}