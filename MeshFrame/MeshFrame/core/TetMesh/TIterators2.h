#pragma once
#include <set>
#include <list>
#include <assert.h>
#include <iterator>
#include <memory>
#include "BaseTMesh.h"
namespace MeshLib
{
	namespace TMeshLib
	{
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		struct TIteratorCore {
			typedef CTMesh< TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType> TMeshType;
			typedef TMeshType* TMeshPtr;

			typedef TVertexType* TVPtr;
			typedef VertexType* VPtr;
			typedef HalfEdgeType* HEPtr;
			typedef TEdgeType* TEPtr;
			typedef TetType* TPtr;
			typedef EdgeType* EPtr;
			typedef HalfFaceType* HFPtr;
			typedef FaceType* FPtr;
			/*!
			\brief TMesh Vertex Iterator

			go through all the vertices in the TetMesh
			*/
			class TM_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				TM_VIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->vertices().begin()) {};
				/*! dereferencing */
				VPtr value() { return *m_iter; };
				VPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_VIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_VIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_VIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_VIterator operator++(int) { TM_VIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_VIterator begin() { return TM_VIterator(m_pMesh); };
				TM_VIterator end() { return TM_VIterator(m_pMesh, m_pMesh->vertices().end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->vertices().end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_VIterator(TMeshPtr pTMesh, MPIterator<VertexType> iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! TetMesh pointer */
				TMeshPtr       m_pMesh;
				/*! Current vertex list iterator */
				//int             m_id;
				MPIterator<VertexType> m_iter;
			};

			/*!
			\brief TVertex Vertex Iterator

			go through all the adjacent vertices in of a vertex
			*/
			class V_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				V_VIterator(VPtr pV) : m_pV(pV), pEdges(TMeshType::VertexEdgeList(pV))
				{
					m_iter = pEdges->begin();;
				}

				/*! dereferencing */
				VPtr value() { 
					EPtr e = *m_iter;
					VPtr v1 = TMeshType::EdgeVertex1(e);
					VPtr v2 = TMeshType::EdgeVertex2(e);
					return v1 == m_pV ? v2 : v1; 
				};
				VPtr operator*() {
					EPtr e = *m_iter;
					VPtr v1 = TMeshType::EdgeVertex1(e);
					VPtr v2 = TMeshType::EdgeVertex2(e);
					return v1 == m_pV ? v2 : v1;
				};

				/*! bool operator*/
				bool operator==(const V_VIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const V_VIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				V_VIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				V_VIterator  operator++(int) { V_VIterator tmp(m_pV, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				V_VIterator begin() { return V_VIterator(m_pV); };
				V_VIterator end() { return V_VIterator(m_pV, pEdges->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				V_VIterator(VPtr pV, typename std::vector<EPtr>::iterator iter) :m_pV(pV), pEdges(TMeshType::VertexEdgeList(pV)), m_iter(iter) {};
				/*! Pointer to the center vertex*/
				VPtr m_pV;
				/*! center vertex */
				std::vector<EPtr> * pEdges;
				/*! set iterator */
				typename std::vector<EPtr>::iterator m_iter;

			};

			/*!
			\brief MeshTet Iterator

			go through all the Tets in a mesh
			*/
			class TM_TIterator : public std::iterator<std::forward_iterator_tag, TPtr> {
			public:
				TM_TIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->tets().begin()) {};
				/*! dereferencing */
				TPtr value() { return *m_iter; };
				TPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_TIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_TIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_TIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_TIterator operator++(int) { TM_TIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_TIterator begin() { return TM_TIterator(m_pMesh); };
				TM_TIterator end() { return TM_TIterator(m_pMesh, m_pMesh->tets().end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->tets().end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_TIterator(TMeshPtr pTMesh, MPIterator<TetType> iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! pointer to a TetMesh */
				TMeshPtr m_pMesh;
				/*! tet iterator */
				MPIterator<TetType> m_iter;
			};

			/*!
			\brief TMeshType::TetHalfFace Iterator

			go through all the halfaces of a Tet in a mesh
			*/
			class T_HFIterator : public std::iterator<std::forward_iterator_tag, HFPtr>
			{
			public:
				T_HFIterator(TPtr pT) : m_pTet(pT), m_id(0) {};

				/*! dereferencing */
				HFPtr operator*() { assert(m_id < 4); return TMeshType::TetHalfFace(m_pTet, m_id); };
				HFPtr value() { assert(m_id < 4);  return TMeshType::TetHalfFace(m_pTet, m_id); };

				/*! bool operator*/
				bool operator==(const T_HFIterator& otherIter) { return m_id == otherIter.m_id; };
				bool operator!=(const T_HFIterator& otherIter) { return m_id != otherIter.m_id; };

				/*! iterate ++ */
				T_HFIterator& operator++() { ++m_id; return *this; };
				/*! ++iterator */
				T_HFIterator operator++(int) { T_HFIterator tmp(m_pMesh, m_iter); ++m_id; return tmp; };

				/*! return the begin and end iterators*/
				T_HFIterator begin() { return T_HFIterator(m_pTet); };
				T_HFIterator end() { return T_HFIterator(m_pTet, 4); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == 4; };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				T_HFIterator(TPtr pT, int id) : m_pTet(pT), m_id(id) {};
				/*! current tet */
				TPtr             m_pTet;
				/*! current half_face */
				int                m_id;
			};

			/*!
			\brief Mesh Edge Iterator
			go through all the edges of a Tet in a mesh
			*/
			class TM_EIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
			public :
				TM_EIterator(TMeshPtr pTMesh) : m_pMesh(pTMesh), m_iter(pTMesh->edges().begin()) {};
				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_EIterator operator++(int) { TM_EIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_EIterator begin() { return TM_EIterator(m_pMesh); };
				TM_EIterator end() { return TM_EIterator(m_pMesh, m_pMesh->edges().end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pMesh->edges().end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_EIterator(TMeshPtr pTMesh, MPIterator<EdgeType> iter) : m_pMesh(pTMesh), m_iter(iter) {};
				/*! pointer to the mesh */
				TMeshPtr m_pMesh;
				/*! edge list iterator */
				MPIterator<EdgeType> m_iter;
			};

			/*!
			\brief Edge->TEdge Iterator
			go through all the tedges of an edge
			*/
			class E_TEIterator : public std::iterator<std::forward_iterator_tag, TEPtr> {
			public:
				E_TEIterator(EPtr pE) : m_pEdge(pE), m_iter(TMeshType::EdgeTEdgeList(pE)->begin()) {};
				/*! dereferencing */
				TEPtr value() { return *m_iter; };
				TEPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const E_TEIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const E_TEIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				E_TEIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				E_TEIterator operator++(int) { TM_EIterator tmp(m_pMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				E_TEIterator begin() { return E_TEIterator(m_pEdge); };
				E_TEIterator end() { return E_TEIterator(m_pEdge, TMeshType::EdgeTEdgeList(m_pEdge)->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == TMeshType::EdgeTEdgeList(m_pEdge)->end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				E_TEIterator(EPtr pE, typename std::vector<TEPtr>::iterator iter) : m_pEdge(pE), m_iter(iter) {};
				/*! pointer to the edge */
				EPtr m_pEdge;
				/*! TEdge list iterator */
				typename std::vector<TEPtr>::iterator m_iter;
			};

			/*!
			\brief Vertex->Edge Iterator
			go through all the edges of a vertex
			*/
			class V_EIterator : public std::iterator<std::forward_iterator_tag, EPtr>{
			public:
				V_EIterator(VPtr pV) : m_pV(pV), m_iter(TMeshType::VertexEdgeList(pV)->begin()) {};

				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const V_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const V_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				V_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				V_EIterator  operator++(int) { V_EIterator tmp(m_pV, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				V_EIterator begin() { return V_EIterator(m_pV); };
				V_EIterator end() { return V_EIterator(m_pV, TMeshType::VertexEdgeList(m_pV)->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				V_EIterator(VPtr pV, typename std::list<EPtr>::iterator iter) :m_pV(pV), m_iter(iter) {};
				/*! Edge List of the point */
				VPtr m_pV;
				/*! Edge list iterator */
				typename std::vector<EPtr>::iterator m_iter;
			};

			/*!
			\brief Tet->Edge Iterator
			go through all the edges of a Tet
			*/
			class T_EIterator : public std::iterator<std::forward_iterator_tag, EPtr> {
			public:
				T_EIterator(TPtr pT) :
					m_pT(pT),
					pEdges(new std::set<EPtr>)
				{
					for (int i = 0; i < 4; i++)
					{
						HFPtr pHF = TMeshType::TetHalfFace(pT, i);
						HEPtr pHE = TMeshType::HalfFaceHalfEdge(pHF);
						for (int j = 0; j < 3; j++)
						{
							TEPtr pTE = TMeshType::HalfEdgeTEdge(pHE);
							EPtr pE = TMeshType::TEdgeEdge(pTE);
							pEdges->insert(pE);
							pHE = TMeshType::HalfEdgeNext(pHE);
						}
					}
					m_iter = pEdges->begin();
				};

				/*! dereferencing */
				EPtr value() { return *m_iter; };
				EPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const T_EIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const T_EIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				T_EIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				T_EIterator  operator++(int) { T_EIterator tmp(m_pT, pEdges, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				T_EIterator begin() { return T_EIterator(m_pT, pEdges, pEdges->begin()); };
				T_EIterator end() { return T_EIterator(m_pT, pEdges, pEdges->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pEdges->end(); };

			protected:

				T_EIterator(TPtr pT, std::shared_ptr<std::set<EPtr>> edges, typename std::set<EPtr>::iterator iter) : m_pT(pT), m_iter(iter), pEdges(edges) {};

				/*! Pointer to the tet */
				TPtr m_pT;
				/*! set of edges adjacent to the central vertex */
				std::shared_ptr<std::set<EPtr>> pEdges;
				/*! edge set iterator */
				typename std::set<EPtr>::iterator m_iter;
			};
			/*!
			\brief Tet->Edge Iterator
			go through all the edges of a Tet
			*/
			class T_TEIterator : public std::iterator<std::forward_iterator_tag, TEPtr> {
			public:
				T_TEIterator(TPtr pT) : 
					m_pT(pT), 
					pTEdges(new std::set<TEPtr>)
				{
					for (int i = 0; i < 4; i++)
					{
						HFPtr pHF = TMeshType::TetHalfFace(pT, i);
						HEPtr pHE = TMeshType::HalfFaceHalfEdge(pHF);
						for (int j = 0; j < 3; j++)
						{
							TEPtr pTE = TMeshType::HalfEdgeTEdge(pHE);
							pTEdges->insert(pTE);
							pHE = TMeshType::HalfEdgeNext(pHE);
						}
					}
					m_iter = pTEdges->begin();
				};

				/*! dereferencing */
				TEPtr value() { return *m_iter; };
				TEPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const T_TEIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const T_TEIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				T_TEIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				T_TEIterator  operator++(int) { T_TEIterator tmp(m_pT, pTEdges, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				T_TEIterator begin() { return T_TEIterator(m_pT, pTEdges, pTEdges->begin()); };
				T_TEIterator end() { return T_TEIterator(m_pT, pTEdges, pTEdges->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pTEdges->end(); };

			protected:

				T_TEIterator(TPtr pT, std::shared_ptr<std::set<TEPtr>> pNewTEdges, typename std::set<TEPtr>::iterator iter) : m_pT(pT), m_iter(iter), pTEdges(pNewTEdges) {};

				/*! Pointer to the tet */
				TPtr m_pT;
				/*! set of edges adjacent to the central vertex */
				std::shared_ptr<std::set<TEPtr>> pTEdges;
				/*! edge set iterator */
				typename std::set<TEPtr>::iterator m_iter;
			};
			/*!
			\brief Mesh->Face Iterator
			go through all the faces of a Mesh
			*/
			class TM_FIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
			public:
				TM_FIterator(TMeshPtr pTMesh) : m_pTMesh(pTMesh), m_iter(m_pTMesh->faces().begin()) {};

				/*! dereferencing */
				FPtr value() { return *m_iter; };
				FPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_FIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_FIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_FIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_FIterator operator++(int) { TM_FIterator tmp(m_pTMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_FIterator begin() { return TM_FIterator(m_pTMesh); };
				TM_FIterator end() { return TM_FIterator(m_pTMesh, m_pTMesh->faces().end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pTMesh->faces().end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_FIterator(TMeshPtr pTMesh, typename std::list<FPtr>::iterator iter) : m_pTMesh(pTMesh), m_iter(iter) {};
				/*! pointer to the mesh */
				TMeshPtr m_pTMesh;
				/*! edge list iterator */
				typename std::list<FPtr>::iterator m_iter;
			};

			/*!
			\brief Mesh->HalfFace Iterator
			go through all the halffaces of a Mesh
			*/
			class TM_HFIterator : public std::iterator<std::forward_iterator_tag, HFPtr> {
			public:
				TM_HFIterator(TMeshPtr pTMesh) : m_pTMesh(pTMesh), m_iter(m_pTMesh->half_faces().begin()) {};

				/*! dereferencing */
				HFPtr value() { return *m_iter; };
				HFPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const TM_HFIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const TM_HFIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! iterate ++ */
				TM_HFIterator& operator++() { ++m_iter; return *this; };
				/*! ++iterator */
				TM_HFIterator operator++(int) { TM_HFIterator tmp(m_pTMesh, m_iter); ++m_iter; return tmp; };

				/*! return the begin and end iterators*/
				TM_HFIterator begin() { return TM_HFIterator(m_pTMesh); };
				TM_HFIterator end() { return TM_HFIterator(m_pTMesh, m_pTMesh->half_faces().end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == m_pTMesh->half_faces().end(); };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TM_HFIterator(TMeshPtr pTMesh, MPIterator<HalfFaceType> iter) : m_pTMesh(pTMesh), m_iter(iter) {};
				/*! pointer to the mesh */
				TMeshPtr m_pTMesh;
				/*! edge list iterator */
				MPIterator<HalfFaceType> m_iter;
			};

			/*!
			\brief Edge->Face Iterator
			go through all the faces adjacent to an edge
			*/
			class E_FIterator : public std::iterator<std::forward_iterator_tag, FPtr> {
			public:
				E_FIterator(EPtr pE):
					pFaces(new std::set<FPtr>)
				{
					std::list<TEPtr> * TEdges = TMeshType::EdgeTEdgeList(pE);
					for (TEPtr pT : *TEdges)
					{
						HEPtr pL = TEdgeLeftHalfEdge(pT);
						HEPtr pR = TEdgeRightHalfEdge(pT);
								    
						HFPtr pHF = TMeshType::HalfEdgeHalfFace(pL);
						FPtr   pF = HalfFaceFace(pHF);
						pFaces->insert(pF);

						pHF = TMeshType::HalfEdgeHalfFace(pR);
						pF  = HalfFaceFace(pHF);
						pFaces->insert(pF);
					}

					m_iter = pFaces->begin();
				}

				/*! dereferencing */
				FPtr value() { return *m_iter; };
				FPtr operator*() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const E_FIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const E_FIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! Iterator ++ */
				E_FIterator& operator++() { ++m_iter; return *this; };
				/*! ++ Iterator */
				E_FIterator  operator++(int) { E_FIterator tmp(pFaces, m_iter);  ++m_iter; return tmp; }

				/*! return the begin and end iterators*/
				E_FIterator begin() { return E_FIterator(pFaces, pFaces->begin()); };
				E_FIterator end() { return E_FIterator(pFaces, pFaces->end()); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_iter == pFaces->end(); };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				E_FIterator(std::shared_ptr<std::set<FPtr>> faces, typename std::set<FPtr>::iterator iter) : m_iter(iter), pFaces(faces) {};
				/*! set of edges adjacent to the central vertex */
				std::shared_ptr<std::set<FPtr>> pFaces;
				/*! edge set iterator */
				typename std::set<FPtr>::iterator m_iter;
			};


			/*!
			\brief HalfFace->vertex Iterator
			go through all the vertices of a HalfFace
			*/
			class HF_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				HF_VIterator(HFPtr pHF) : m_pHF(pHF), m_pHE(TMeshType::HalfFaceHalfEdge(m_pHF)) {};
				
				/*! dereferencing */
				VPtr operator*() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTarget(m_pHE); };
				VPtr value() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTarget(m_pHE); };

				/*! bool operator*/
				bool operator==(const HF_VIterator& otherIter) { return m_pHE == otherIter.m_pHE; };
				bool operator!=(const HF_VIterator& otherIter) { return m_pHE != otherIter.m_pHE; };

				/*! iterator ++ */
				HF_VIterator& operator++()
				{
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::HalfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return *this;
				};
				/*! ++ iterator */
				HF_VIterator operator++(int)
				{
					HF_VIterator tmp(pHF, pHE);
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::HalfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return tmp;
				};

				/*! return the begin and end iterators*/
				HF_VIterator begin() { return HF_VIterator(m_pHF); };
				HF_VIterator end() { return HF_VIterator(m_pHF, NULL); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_PHE == NULL; };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				HF_VIterator(HFPtr pHF, HEPtr pHE) : m_pHF(pHF), m_pHE(pHE) {};
				/*! Pointer to the halfface */
				HFPtr m_pHF;
				/*! Pointer to the halfedge */
				HEPtr m_pHE;
			};


			/*!
			\brief HalfFace->HalfEdge Iterator
			go through all the vertices of a HalfFace
			*/
			class HF_HEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
			public:
				HF_HEIterator(HFPtr pHF) : m_pHF(pHF), m_pHE(TMeshType::HalfFaceHalfEdge(m_pHF)) {};

				/*! dereferencing */
				HEPtr operator*() { assert(m_pHE != NULL); return m_pHE; };
				HEPtr value() { assert(m_pHE != NULL); return m_pHE; };

				/*! bool operator*/
				bool operator==(const HF_HEIterator& otherIter) { return m_pHE == otherIter.m_pHE; };
				bool operator!=(const HF_HEIterator& otherIter) { return m_pHE != otherIter.m_pHE; };

				/*! iterator ++ */
				HF_HEIterator& operator++()
				{
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::HalfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return *this;
				};
				/*! ++ iterator */
				HF_HEIterator operator++(int)
				{
					HF_HEIterator tmp(pHF, pHE);
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::HalfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return tmp;
				};

				/*! return the begin and end iterators*/
				HF_HEIterator begin() { return HF_HEIterator(m_pHF); };
				HF_HEIterator end() { return HF_HEIterator(m_pHF, NULL); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_PHE == NULL; };

			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				HF_HEIterator(HFPtr pHF, HEPtr pHE) : m_pHF(pHF), m_pHE(pHE) {};
				/*! Pointer to the halfface */
				HFPtr m_pHF;
				/*! Pointer to the halfedge */
				HEPtr m_pHE;
			};

			/*!
			\brief Face->vertex Iterator
			go through all the vertices of a Face
			*/
			class F_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				F_VIterator(FPtr pF) : m_pF(pF), m_pHF(TMeshType::FaceLeftHalfFace(pF)), m_pHE(TMeshType::HalfFaceHalfEdge(m_pHF)) {};
				/*! dereferencing */
				VPtr operator*() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTarget(m_pHE); };
				VPtr value() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTarget(m_pHE); };

				/*! bool operator*/
				bool operator==(const F_VIterator& otherIter) { return m_pHE == otherIter.m_pHE; };
				bool operator!=(const F_VIterator& otherIter) { return m_pHE != otherIter.m_pHE; };

				/*! iterator ++ */
				F_VIterator& operator++()
				{
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::HalfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return *this;
				};
				/*! ++ iterator */
				F_VIterator operator++(int)
				{
					F_VIterator tmp(m_pF, pHE);
					m_pHE = TMeshType::HalfEdgeNext(m_pHE);
					if (m_pHE == TMeshType::alfFaceHalfEdge(m_pHF))
					{
						m_pHE = NULL;
					}
					return tmp;
				};

				/*! return the begin and end iterators*/
				F_VIterator begin() { return F_VIterator(m_pF); };
				F_VIterator end() { return F_VIterator(m_pF, NULL); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_PHE == NULL; };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				F_VIterator(FPtr pF, HEPtr pHE) : m_pF(pF), m_pHF(TMeshType::FaceLeftHalfFace(pF)), m_pHE(pHE) {};
				/*! Pointer to the halfface */
				FPtr  m_pF;
				/*! Pointer to the halfface */
				HFPtr m_pHF;
				/*! Pointer to the halfedge */
				HEPtr  m_pHE;
			};

			/*!
			\brief TVertex->InHalfEdge Iterator
			go through all the InHalfEdge of a TVertex
			*/
			class TV_InHEIterator : public std::iterator<std::forward_iterator_tag, HEPtr> {
			public:
				TV_InHEIterator(TVPtr pTV) : m_pTV(pTV), m_pHE(TMeshType::HalfEdgeDual( TMeshType::TVertexHalfEdge(pTV) )), m_pHF(TMeshType::HalfEdgeHalfFace(m_pHE)) {};
				/*! dereferencing */
				HEPtr operator*() { assert(m_pHE != NULL); return m_pHE; };
				HEPtr value() { assert(m_pHE != NULL); return m_pHE; };

				/*! bool operator*/
				bool operator==(const TV_InHEIterator& otherIter) { return m_pHE == otherIter.m_pHE; };
				bool operator!=(const TV_InHEIterator& otherIter) { return m_pHE != otherIter.m_pHE; };

				/*! iterator ++ */
				TV_InHEIterator& operator++()
				{
					m_pHE = TMeshType::HalfEdgeDual( TMeshType::HalfEdgeNext(m_pHE) );
					if (m_pHF == TMeshType::HalfEdgeHalfFace(m_pHE))
					{
						m_pHE = NULL;
					}
					return *this;
				};
				/*! ++ iterator */
				TV_InHEIterator operator++(int unused)
				{
					TV_InHEIterator tmp(m_pTV, m_pHF, m_pHE);
					m_pHE = TMeshType::HalfEdgeDual(TMeshType::HalfEdgeNext(m_pHE));
					if (m_pHF == TMeshType::HalfEdgeHalfFace(m_pHE))
					{
						m_pHE = NULL;
					}
					return tmp;
				};

				/*! return the begin and end iterators*/
				TV_InHEIterator begin() { return TV_InHEIterator(m_pTV); };
				TV_InHEIterator end() { return TV_InHEIterator(m_pTV, m_pHF, NULL); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_PHE == NULL; };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TV_InHEIterator(TVPtr pTV, HFPtr pHF, HEPtr pHE) : m_pTV(pTV), m_pHE(pHE), m_pHF(pHF) {};
				/*! Pointer to the TVertex */
				TVPtr m_pTV;
				/*! Pointer to the halfedge */
				HEPtr m_pHE;
				HFPtr m_pHF;
			};

			/*!
			\brief TVertex->TEdge Iterator
			go through all the TEdge of a TVertex
			*/
			class TV_TEIterator : public std::iterator<std::forward_iterator_tag, TEPtr> {
			public:
				TV_TEIterator(TVPtr pTV) : m_pTV(pTV), m_pHE(TMeshType::HalfEdgeDual(TMeshType::TVertexHalfEdge(pTV))), m_pHF(TMeshType::HalfEdgeHalfFace(m_pHE)) {};
				/*! dereferencing */
				TEPtr operator*() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTEdge(m_pHE); };
				TEPtr value() { assert(m_pHE != NULL); return TMeshType::HalfEdgeTEdge(m_pHE); };

				/*! bool operator*/
				bool operator==(const TV_TEIterator& otherIter) { return m_pHE == otherIter.m_pHE; };
				bool operator!=(const TV_TEIterator& otherIter) { return m_pHE != otherIter.m_pHE; };

				/*! iterator ++ */
				TV_TEIterator& operator++()
				{
					m_pHE = TMeshType::HalfEdgeDual(TMeshType::HalfEdgeNext(m_pHE));
					if (m_pHF == TMeshType::HalfEdgeHalfFace(m_pHE))
					{
						m_pHE = NULL;
					}
					return *this;
				};
				/*! ++ iterator */
				TV_TEIterator operator++(int unused)
				{
					TV_TEIterator tmp(m_pTV, m_pHE);
					m_pHE = TMeshType::HalfEdgeDual(TMeshType::HalfEdgeNext(m_pHE));
					if (m_pHF == TMeshType::HalfEdgeHalfFace(m_pHE))
					{
						m_pHE = NULL;
					}
					return tmp;
				};

				/*! return the begin and end iterators*/
				TV_TEIterator begin() { return TV_TEIterator(m_pTV); };
				TV_TEIterator end() { return TV_TEIterator(m_pTV, m_pHF, NULL); };

				/*! formal style end() method, return whether the iterator has reached the end of the container*/
				bool reachEnd() { return m_PHE == NULL; };
			protected:
				/*! Private construction function, only used to generate begin, end and tmp iterator*/
				TV_TEIterator(TVPtr pTV, HFPtr pHF, HEPtr pHE) : m_pTV(pTV), m_pHE(pHE), m_pHF(pHF) {};
				/*! Pointer to the TVertex */
				TVPtr m_pTV;
				/*! Pointer to the halfedge */
				HEPtr m_pHE;
				HFPtr m_pHF;
			};

			/*!
			\brief Vertex->TVertex Iterator
			go through all the tvertices of a vertex
			*/
			class V_TVIterator : public std::iterator<std::forward_iterator_tag, TVPtr> {
			public:
				V_TVIterator(VPtr pV)
				{
					m_pV = pV;
					m_iter = TMeshType::VertexTVertexList(pV)->begin();
				};

				/*! dereferencing */
				TVPtr operator*() { return *m_iter; };
				TVPtr value() { return *m_iter; };

				/*! bool operator*/
				bool operator==(const V_TVIterator& otherIter) { return m_iter == otherIter.m_iter; };
				bool operator!=(const V_TVIterator& otherIter) { return m_iter != otherIter.m_iter; };

				/*! ++iterator */
				V_TVIterator& operator++() { ++m_iter; return *this; };
				/*! iterator++ */
				V_TVIterator operator++(int unused) { 
					V_TVIterator tmp(m_pV, m_iter); 
					++m_iter; 
					return tmp;
				};

				/*! return the begin and end iterators*/
				V_TVIterator begin() { return V_TVIterator(m_pV); };
				V_TVIterator end() { return V_TVIterator(m_pV, TMeshType::VertexTVertexList(m_pV)->end()); };


				/*! formal style, verify if the end has been reached */
				bool reachEnd() { return m_iter == TMeshType::VertexTVertexList(m_pV)->end(); };
				
			protected:

				V_TVIterator(VPtr pV, typename std::vector<TVPtr>::iterator iter) : m_pV(pV), m_iter(iter) {};
				/*! pointer to the vertex */
				VPtr m_pV;
				/*! TVertex list iterator */
				typename std::vector<TVPtr>::iterator m_iter;

			};

			class T_TVIterator :  public std::iterator<std::forward_iterator_tag, TVPtr>{
			public :
				T_TVIterator(TPtr pT) : m_pT(pT), m_ID(0) {};

				/*! dereferencing */
				TVPtr operator*() { assert(m_ID < 4); return TMeshType::TetTVertex(m_pT, m_ID); };
				TVPtr value() { assert(m_ID < 4); return TMeshType::TetTVertex(m_pT, m_ID); };

				/*! bool operator*/
				bool operator==(const T_TVIterator& otherIter) { return m_ID == otherIter.m_ID; };
				bool operator!=(const T_TVIterator& otherIter) { return m_ID != otherIter.m_ID; };

				/*! ++iterator */
				T_TVIterator& operator++() { ++m_ID; return *this; };
				/*! iterator++ */
				T_TVIterator operator++(int unused) {
					T_TVIterator tmp(m_pT, m_ID);
					++m_iter;
					return tmp;
				};

				/*! return the begin and end iterators*/
				T_TVIterator begin() { return T_TVIterator(m_pT); };
				T_TVIterator end() { return T_TVIterator(m_pT, 4); };


				/*! formal style, verify if the end has been reached */
				bool reachEnd() { return m_ID == 4; };

			protected:
				T_TVIterator(TPtr pT, int id) : m_pT(pT), m_ID(id) {};

				TPtr m_pT;
				int m_ID;
			};

			class T_VIterator : public std::iterator<std::forward_iterator_tag, VPtr> {
			public:
				T_VIterator(TPtr pT) : m_pT(pT), m_ID(0) {};

				/*! dereferencing */
				VPtr operator*() { assert(m_ID < 4); return TMeshType::TetVertex(m_pT, m_ID); };
				VPtr value() { assert(m_ID < 4); return TMeshType::TetVertex(m_pT, m_ID); };

				/*! bool operator*/
				bool operator==(const T_VIterator& otherIter) { return m_ID == otherIter.m_ID; };
				bool operator!=(const T_VIterator& otherIter) { return m_ID != otherIter.m_ID; };

				/*! ++iterator */
				T_VIterator& operator++() { ++m_ID; return *this; };
				/*! iterator++ */
				T_VIterator operator++(int unused) {
					T_VIterator tmp(m_pT, m_ID);
					++m_iter;
					return tmp;
				};

				/*! return the begin and end iterators*/
				T_VIterator begin() { return T_VIterator(m_pT); };
				T_VIterator end() { return T_VIterator(m_pT, 4); };


				/*! formal style, verify if the end has been reached */
				bool reachEnd() { return m_ID == 4; };

			protected:
				T_VIterator(TPtr pT, int id) : m_pT(pT), m_ID(id) {};

				TPtr m_pT;
				int m_ID;
			};
		};

		template <typename TMeshType>
		struct TIterators : public TIteratorCore<typename TMeshType::TVType, typename TMeshType::VType, typename TMeshType::HEType,
			typename TMeshType::TEType, typename TMeshType::EType, typename TMeshType::HFType, typename TMeshType::FType, typename TMeshType::TType> {
		};
		/*
		TVType
		VType
		HEType
		TEType
		TType
		EType
		HFType
		FType
		*/
	}
}