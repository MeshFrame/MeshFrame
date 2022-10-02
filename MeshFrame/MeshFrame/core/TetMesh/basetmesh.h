/*!
*      \file BaseTetMesh.h
*      \brief Base TetMesh Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tetrahedral meshes
*	   \author David Gu, Jerome Jiang
*      \date 10/01/2011
*	   \modified 4/16/2015
*
*/

#ifndef _TMESHLIB_BASE_TET_MESH_H_
#define _TMESHLIB_BASE_TET_MESH_H_

#include <assert.h>
#include <list>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <iterator>

#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Parser/StrUtil_fast.h"
#include "../Parser/IOFuncDef.h"
#include "../Memory/MemoryPool.h"
#include "../Memory/Array.h"

#include "TProps.h"

#ifndef MAX_LINE 
#define MAX_LINE 2048
#endif

#define TMESH_ARRAY_PRE_ALLOC_SIZE 32
namespace MeshLib
{
	namespace TMeshLib
	{

		/*!
		* \brief CBaseTMesh, base class for all types of tet-mesh classes
		*
		*  This is the fundamental class for tet-meshes. All the geometric objects are connected by pointers,
		*  vertex, edge, face, tet are connected by halffaces. The mesh class has file IO functionalities,
		*  supporting .tet file formats. It offers Euler operators, each geometric primative
		*  can access its neighbors freely.
		*
		* \tparam VertexType   vertex   class, derived from TMeshLib::VertexType     class
		* \tparam TVertexType  tetrahedron vertex   class, derived from TMeshLib::TVertexType   class
		* \tparam HalfEdgeType halfedge class, derived from TMeshLib::HalfEdgeType class
		* \tparam TEdgeType	tetrahedron edge class, derived from TMeshLib::TEdgeType class
		* \tparam EdgeType     edge     class, derived from MeshLib::EdgeType     class
		* \tparam FaceType     face     class, derived from TMeshLib::FaceType     class
		* \tparam HalfFaceType half face     class, derived from TMeshLib::HalfFaceType     class
		* \tparam TetType      tetrahedron class, derived from TMeshLib::TetType class
		*/

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		class CTMesh
		{
		public:
			typedef TVertexType     TVType;
			typedef VertexType      VType;
			typedef HalfEdgeType    HEType;
			typedef TEdgeType       TEType;
			typedef TetType         TType;
			typedef EdgeType        EType;
			typedef HalfFaceType    HFType;
			typedef FaceType        FType;

			typedef TVertexType* TVPtr;
			typedef VertexType* VPtr;
			typedef HalfEdgeType* HEPtr;
			typedef TEdgeType* TEPtr;
			typedef TetType* TPtr;
			typedef EdgeType* EPtr;
			typedef HalfFaceType* HFPtr;
			typedef FaceType* FPtr;

			typedef  std::pair<int, VertexType*>		VMapPair;
			typedef  std::pair<int, TetType*>			TMapPair;


			typedef MemoryPool<VertexType>				VContainer;
			typedef MemoryPool<TVertexType>				TVContainer;
			typedef MemoryPool<FaceType>				FContainer;
			typedef MemoryPool<HalfFaceType>			HFContainer;
			typedef MemoryPool<EdgeType>				EContainer;
			typedef MemoryPool<TEdgeType>				TEContainer;
			typedef MemoryPool<HalfEdgeType>			HEContainer;
			typedef MemoryPool<TetType>					TContainer;

			typedef CTMesh<TVType, VType, HEType, TEType, EType, HFType, FType, TType>* Ptr;

			typedef CPArray<HFPtr, TMESH_ARRAY_PRE_ALLOC_SIZE> HFArray;
			typedef CPArray<TEPtr, TMESH_ARRAY_PRE_ALLOC_SIZE> TEArray;

			/*!
				CTMesh constructor
				*/
			CTMesh() { std::ios::sync_with_stdio(false); };
			/*!
				CTMesh desctructor
				*/
			~CTMesh() { _clear(); };
			/*!
				Load tet mesh from a ".tet" file
				*/
			void _load(const char * input, bool checkOrientation = false);
			/*!
				Load tet mesh from a ".t" file
				*/
			void _load_t(const char * input, bool checkOrientation = false);

			/*!
			Load tet mesh from a ".t" file
			*/
			void _load_vtArray(const std::vector<std::array<double, 3>>& verts, const std::vector<std::array<int, 4>>& tetVIds, bool checkOrientation = false);

			/*!
				Write tet mesh to a file
				*/
			void _write(const char *);

			/*!
			Write tet mesh to a .t file
			*/
			void _write_t(const char * filename, bool highPrecision = false);


			/*!
			Write tet mesh to a .vtk file
			*/
			void _write_vtk(const char* filename, bool highPrecision = false);

			/*!
			Write tet mesh to a .vtk file
			*/
			void _write_tet_list_to_vtk(const char* filename, std::vector<TPtr> tets, bool highPrecision = false);

			/*!
				access the list of half faces
				*/
			HFContainer & half_faces() { return mHFContainer; };
			/*!
				access the list of edges
				*/
			EContainer       & edges() { return mEContainer; };
			/*!
				access list of faces
				*/
			FContainer        & faces() { return mFContainer; };

			HEContainer& half_edges() { return mHEContainer; };

			TEContainer& tedges() { return mTEContainer; };

			/*!
			access list of vertices
			*/
			VContainer & vertices() { return mVContainer; };
			TVContainer & tvertices() { return mTVContainer; };

			/*! number of tets */
			int numTets() { return m_nTets; };

			/*! number of edges */
			int numEdges() { return mEContainer.size(); };

			/*! number of vertices */
			int numVertices() { return m_nVertices; };

			/*! max vertex id*/
			int maxVertexId() { return m_maxVertexId; };

			/*! Access the array of tets */
			TContainer& tets() { return mTContainer; };
			const TContainer & tets() const { return mTContainer; };

			/*! access the vertex with ID */
			VertexType * idVertex(int id) { return m_map_Vertices[id]; };

			/*! access the tet with ID */
			TetType      * idTet(int id) { return m_map_Tets[id]; };


			//Access Vertex data members
			/*! Vertex->Edge List */
			static std::vector<EdgeType*>* VertexEdgeList(VertexType* pVertex);
			/*! Vertex->TEdge List */
			TEArray& VertexTEdgeList(VertexType* pVertex);
			/*! Vertex->HalfFace List */
			static std::list<HalfFaceType*>* VertexHalfFaceList(VertexType* pVertex);
			/*! Vertex->TVertex List */
			static std::vector<TVertexType*>* VertexTVertexList(VertexType* pVertex);

			/*! Vertex->Edge */
			static EdgeType* VertexEdge(VertexType* v1, VertexType* v2);

			//Access TVertex data memebers
			static VertexType* TVertexVertex(TVertexType* pTVertex);
			static TetType* TVertexTet(TVertexType* pTVertex);
			static HalfEdgeType* TVertexHalfEdge(TVertexType* pTVertex);
			static HalfFaceType* TVertexOppositeHalfFace(TVertexType* pTVertex);

			//Access TEdge data memebers
			static HalfEdgeType* TEdgeLeftHalfEdge(TEdgeType* pTEdge);
			static HalfEdgeType* TEdgeRightHalfEdge(TEdgeType* pTEdge);
			static EdgeType* TEdgeEdge(TEdgeType* pTEdge);
			static TEdgeType* TEdgeDualTEdge(TEdgeType* pTEdge);
			static TetType* TEdgeTet(TEdgeType* pTEdge);
			static HalfFaceType* TEdgeLeftHalfFace(TEdgeType* pTEdge);
			static HalfFaceType* TEdgeRightHalfFace(TEdgeType* pTEdge);

			//Access HalfEdge data members
			/*! HalfEdge->source vertex */
			static VertexType* HalfEdgeSource(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->target vertex */
			static VertexType* HalfEdgeTarget(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->source tvertex */
			static TVertexType* HalfEdgeTSource(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->target tvertex */
			static TVertexType* HalfEdgeTTarget(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->tet */
			static TetType* HalfEdgeTet(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->dual halfedge */
			static HalfEdgeType* HalfEdgeDual(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->next HalfEdge */
			static HalfEdgeType* HalfEdgeNext(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->prev HalfEdge */
			static HalfEdgeType* HalfEdgePrev(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->Edge Edge */
			static EdgeType* HalfEdgeEdge(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->TEdge TEdge */
			static TEdgeType* HalfEdgeTEdge(HalfEdgeType* pHalfEdge);
			/*! HalfEdge->HalfFace */
			static HalfFaceType* HalfEdgeHalfFace(HalfEdgeType* pHalfEdge);
			/*! Turn halfedge into vector in CPoint */
			static CPoint HalfEdgeVec(HalfEdgeType* pHalfEdge);

			//Access Edge data members
			/*! TEdge list of the edge */
			static std::vector<TEdgeType*>* EdgeTEdgeList(EdgeType* pEdge);
			/*! Edge->Vertex1 */
			static VertexType* EdgeVertex1(EdgeType* pEdge);
			/*! Edge->Vertex2 */
			static VertexType* EdgeVertex2(EdgeType* pEdge);
			/*! length of the edge*/
			static double EdgeLength(EdgeType* pEdge);
			/*! squared length of the edge*/
			static double EdgeLengthSquare(EdgeType* pEdge);

			//Access HalfFace data memebers
			/*! HalfFace->HalfEdge */
			static HalfEdgeType* HalfFaceHalfEdge(HalfFaceType* pHalfFace);
			/*! HalfFace->face */
			static FaceType* HalfFaceFace(HalfFaceType* pHalfFace);
			/*! HalfFace->Tet */
			static TetType* HalfFaceTet(HalfFaceType* pHalfFace);
			/*! HalfFace->dual half face */
			static HalfFaceType* HalfFaceDual(HalfFaceType* pHalfFace);
			/*! HalfFace's opposite tvertex, i.e, the tvertex not contain in the halfface */
			static TVertexType* HalfFaceOppositeTVertex(HalfFaceType* pHalfFace);
			static void HalfFace3Points(HalfFaceType* pHF, CPoint* v);
			static CPoint HalfFaceNormal(HalfFaceType* pHF);
			static CPoint HalfFaceOrientedArea(HalfFaceType* pHF);
			static bool PointInTet(TPtr pT, const CPoint& p);

			//Operations of new a object
			/*! New a vertex in tmesh */
			VertexType* newVertex();

			/*! Create a vertex with input id, used in the process of reading .t file, id is user defined
			\return pointer to the new vertex
			*/
			VertexType* createVertexWithId(int id);

			/*! Create a vertex with automatically assigned index, faster than createVertexWithId
			* the index is its index in its containter, its id will be set to =index
						\return pointer to the new vertex
						*/
			VertexType* createVertexWithIndex();

			TVertexType* createTVertex();

			/*! Create a face with automatically assigned index, 
			* the index is the index in its containter, its id will be set to =index
			*
			\return pointer to the new face
			*/
			FaceType* createFace();


			/*! Create a halfface with automatically assigned index,
			* the index is its index in its containter, its id will be set to =index
			\return pointer to the new halfface
			*/
			HalfFaceType* createHalfFaceWithIndex();

			/*! Create a halfedge with automatically assigned index,
			* the index is its index in its containter, its id will be set to =index
			\return pointer to the new halfedge
			*/
			HalfEdgeType* createHalfEdgeWithIndex();

			/*! Create a edge with automatically assigned index,
			* the index is its index in its containter, its id will be set to =index
			\return pointer to the new edge
			*/
			EdgeType* createEdgeWithIndex();

			/*! Create a tedge with automatically assigned index,
			* the index is its index in its containter, its id will be set to =index
			\return pointer to the new tedge
			*/
			TEdgeType* createTEdgeWithIndex();

			/*! Create a tedge with automatically assigned index,
			* the index is its index in its containter, its id will be set to =index
				\return pointer to the new tedge
				*/
			TetType* createTetWithIndex();


			/*! Create a vertex with input id, used in the process of reading .t file, id is user defined
				\return pointer to the new tedge
				*/
			TetType* createTetWithId(int id);

			//Face
			/*! access the left half face of a face */
			static HalfFaceType* FaceLeftHalfFace(FaceType* pFace);
			/*! access the right half face of a face */
			static HalfFaceType* FaceRightHalfFace(FaceType* pFace);

			//Tetrahedron

			/*! access the j-th half edge of a tet */
			static HalfFaceType* TetHalfFace(TetType* pT, int j);
			/*! access the j-th tvertex of a tet */
			static TVertexType* TetTVertex(TetType* pT, int j);
			/*! access the j-th vertex of a tet */
			static VertexType* TetVertex(TetType* pT, int j);
			static double TetOrientedVolume(TetType* pT);
			static CPoint TetCentroid(TetType* pT);


			void reinitializeVIds();


		protected:

			/*!
			construct tetrahedron
			\tparam v array of vertex ids
			\tparam pT retulting tetrahedron
			*/

			void  _construct_tet(TetType* pT, int tID, int * v);
			void  _construct_tet_orientation(TetType* pT, int tId, int  v[4]);
			/*! construct faces */
			void  _construct_faces();
			/*! construct edges */
			void  _construct_edges();
			/*!  construct half face
			\tparam array of vertex pointers
			*/
			HalfFaceType*   _construct_half_face(TVertexType **);

			/*! release all the memory allocations */
			void _clear();



		protected:
			/*!
			list of faces
			*/
			FContainer        mFContainer;
			/*!
			list of half faces
			*/
			HFContainer	 mHFContainer;
			/*!
			list of half edges
			*/
			HEContainer	 mHEContainer;
			/*!
			list of edges
			*/
			EContainer       mEContainer;
			/*!
			list of tetrahedra
			*/
			TEContainer		 mTEContainer;

			/*!
			 array of vertices
			 */
			VContainer		 mVContainer;

			/*!
			 array of vertices
			 */
			TVContainer		 mTVContainer;
			//VertexType *				 mVContainer;

			/*!
			map of VertexType id and pointer
			*/
			std::map<int, VertexType *> m_map_Vertices;

			/*!
			array of tets
			*/
			TContainer		 mTContainer;
			//TetType*                    mTContainer;

			std::map<int, TetType*>     m_map_Tets;

			/*! number of vertices */
			int m_nVertices;

			/*! number of tets */
			int m_nTets;

			/*! number of edges */
			int m_nEdges;

			/*! max vertex id */
			int m_maxVertexId;


			MAKE_PROP_OF(V);
			MAKE_PROP_OF(E);
			MAKE_PROP_OF(F);
			MAKE_PROP_OF(T);
			MAKE_PROP_OF(HF);
			MAKE_PROP_OF(HE);

			VPropHandle<HFArray> mVHFArrayHandle;
			VPropHandle<TEArray> mVTEArrayHandle;

		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_clear()
		{

		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load(const char * input, bool checkOrientation)
		{
			std::ostringstream oss;
			std::istringstream iss;

			m_maxVertexId = 0;

			std::fstream is(input, std::fstream::in);

			if (is.fail())
			{
				fprintf(stderr, "Error in opening file %s\n", input);
				return;
			}

			char buffer[MAX_LINE];
			{
				//read in the first line
				is.getline(buffer, MAX_LINE);
				//read in the number of vertices: "number vertices"
				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();
				m_nVertices = strutilTetMesh::parseString<int>(token);
				stokenizer.nextToken();
				token = stokenizer.getToken();
				if (token != "vertices")
				{
					fprintf(stderr, "Error in file format %s\n", input);
					return;
				}
			}

			//read in the second line
			is.getline(buffer, MAX_LINE);
			//read in the number of vertices: "number tets"
			std::string line(buffer);
			line = strutilTetMesh::trim(line);
			strutilTetMesh::Tokenizer stokenizer(line, " \r\n");
			stokenizer.nextToken();
			std::string token = stokenizer.getToken();
			m_nTets = strutilTetMesh::parseString<int>(token);
			stokenizer.nextToken();
			token = stokenizer.getToken();
			if (token != "tets")
			{
				fprintf(stderr, "Error in file format %s\n", input);
				return;
			}

			//read in the vertices
			for (int i = 0; i < m_nVertices && is.getline(buffer, MAX_LINE); i++)
			{
				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutilTetMesh::parseString<float>(token);
				}

				VertexType * v = createVertexWithIndex();
				v->position() = p;

				if (!stokenizer.nextToken("\t\r\n")) continue;
				std::string token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					v->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}


			//read in tets 
			for (int id = 0; id < m_nTets && is.getline(buffer, MAX_LINE); id++)
			{
				int vId[4];

				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				//skip the first "4" in the line
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();
				int tid = strutilTetMesh::parseString<int>(token);

				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutilTetMesh::parseString<int>(token);
				}

				TetType * pT = createEdgeWithIndex();

				if (checkOrientation) {
					_construct_tet_orientation(pT, tid, vId);
				}
				else {
					_construct_tet(pT, tid, vId);
				}
			}

			_construct_faces();
			_construct_edges();

			is.close();

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (auto fIter = mFContainer.begin(); fIter != mFContainer.end(); ++fIter)
			{
				FPtr pF = *fIter;
				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					HalfFaceType * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);
					HalfEdgeType * pHE = (HalfEdgeType *)pH->half_edge();

					for (int i = 0; i < 3; ++i)
					{
						EdgeType * pE = HalfEdgeEdge(pHE);
						int vid = pH->key(i);
						VertexType * v = idVertex(vid);
						v->boundary() = true;
						pE->boundary() = true;
						pHE = HalfEdgeNext(pHE);
					}
				}
			}
		};


		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load_t(const char * input, bool checkOrientation)
		{
			std::ostringstream oss;
			std::istringstream iss;

			addVProp(mVHFArrayHandle);
			addVProp(mVTEArrayHandle);

			m_maxVertexId = 0;

			std::fstream is(input, std::fstream::in);

			if (is.fail())
			{
				fprintf(stderr, "Error in opening file %s\n", input);
				return;
			}

			char buffer[MAX_LINE];

			m_nVertices = 0;
			m_nTets = 0;
			m_nEdges = 0;

			while (!is.eof())
			{
				is.getline(buffer, MAX_LINE);
				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token == "Vertex") m_nVertices++;
				if (token == "Tet") m_nTets++;
				if (token == "Edge") m_nEdges++;
			}

			is.clear();              // forget we hit the end of file
			is.seekg(0, std::ios::beg);   // move to the start of the file

			//read in the vertices
			for (int i = 0; i < m_nVertices && is.getline(buffer, MAX_LINE); i++)
			{
				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Vertex")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}


				stokenizer.nextToken();
				token = stokenizer.getToken();
				int vid = strutilTetMesh::parseString<int>(token, iss);

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutilTetMesh::parseString<float>(token, iss);
				}

				VertexType * v = createVertexWithId(vid);
				v->position() = p;

				if (!stokenizer.nextToken("\t\r\n")) continue;
				token = stokenizer.getToken();

				//int sp = (int)token.find("{");
				//int ep = (int)token.find("}");

				//if (sp >= 0 && ep >= 0)
				//{
				//	v->string() = token.substr(sp + 1, ep - sp - 1);
				//}
			}


			//read in tets 
			for (int id = 0; id < m_nTets && is.getline(buffer, MAX_LINE); id++)
			{
				int vId[4];

				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Tet")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}

				//skip the first "4" in the line
				stokenizer.nextToken();
				token = stokenizer.getToken();
				int tid = strutilTetMesh::parseString<int>(token, iss);


				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutilTetMesh::parseString<int>(token, iss);
				}

				TetType * pT = createTetWithId(tid);

				if (checkOrientation) {
					_construct_tet_orientation(pT, tid, vId);
				}
				else {
					_construct_tet(pT, tid, vId);
				}
				// read in string
				if (!stokenizer.nextToken("\t\r\n")) continue;
				//token = stokenizer.getToken();

				//int sp = (int)token.find("{");
				//int ep = (int)token.find("}");

				//if (sp >= 0 && ep >= 0)
				//{
				//	pT->string() = token.substr(sp + 1, ep - sp - 1);
				//}
			}

			_construct_faces();
			_construct_edges();

			for (int id = 0; id < m_nEdges && is.getline(buffer, MAX_LINE); id++)
			{
				std::string line(buffer);
				line = strutilTetMesh::trim(line);
				strutilTetMesh::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Edge")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id1 = strutilTetMesh::parseString<int>(token, iss);

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id2 = strutilTetMesh::parseString<int>(token, iss);

				VertexType * pV1 = idVertex(id1);
				VertexType * pV2 = idVertex(id2);

				EdgeType * pE = VertexEdge(pV1, pV2);

				if (!stokenizer.nextToken("\t\r\n"))
				{
					continue;
				}

				token = stokenizer.getToken();

				//int sp = (int)token.find("{");
				//int ep = (int)token.find("}");

				//if (sp >= 0 && ep >= 0)
				//{
				//	pE->string() = token.substr(sp + 1, ep - sp - 1);
				//}
			}

			m_nEdges = (int)mEContainer.size();

			is.close();

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (auto fIter = mFContainer.begin(); fIter != mFContainer.end(); ++fIter)
			{
				FPtr pF = *fIter;
				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					HalfFaceType * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);
					//added by Anka, mark edge as boundary
					HalfEdgeType * pHE = (HalfEdgeType *)pH->half_edge();

					for (int i = 0; i < 3; ++i)
					{
						EdgeType * pE = HalfEdgeEdge(pHE);
						int vid = pH->key(i);
						VertexType * v = idVertex(vid);
						v->boundary() = true;
						pE->boundary() = true;
						pHE = HalfEdgeNext(pHE);
					}
				}
			}

			// read in traits
			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				pV->edges()->shrink_to_fit();
				pV->tvertices()->shrink_to_fit();
				pV->_from_string();
			}

			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				TetType * pT = *tIter;
				pT->_from_string();
			}

			for (auto eIter = mEContainer.begin(); eIter != mEContainer.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				pE->_from_string();
			}

			removeVProp(mVTEArrayHandle);

		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_load_vtArray(
			const std::vector<std::array<double, 3>>& verts, const std::vector<std::array<int, 4>>& tetVIds, bool checkOrientation)
		{
			addVProp(mVHFArrayHandle);
			addVProp(mVTEArrayHandle);

			m_maxVertexId = verts.size()-1;

			m_nVertices = verts.size();
			m_nTets = tetVIds.size();
			m_nEdges = 0;

			//read in the vertices
			for (int i = 0; i < m_nVertices; i++)
			{
				int vid = i;

				CPoint p(verts[i][0], verts[i][1], verts[i][2]);

				VertexType* v = createVertexWithId(vid);
				v->position() = p;
			}


			//read in tets 
			for (int id = 0; id < m_nTets; id++)
			{
				int tid = id;
				int vIds[4] = { tetVIds[id][0], tetVIds[id][1], tetVIds[id][2], tetVIds[id][3]};

				TetType* pT = createTetWithId(tid);

				if (checkOrientation) {
					_construct_tet_orientation(pT, tid, vIds);
				}
				else {
					_construct_tet(pT, tid, vIds);
				}

			}

			_construct_faces();
			_construct_edges();

			m_nEdges = (int)mEContainer.size();

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType* pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (auto fIter = mFContainer.begin(); fIter != mFContainer.end(); ++fIter)
			{
				FPtr pF = *fIter;
				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					HalfFaceType* pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);
					//added by Anka, mark edge as boundary
					HalfEdgeType* pHE = (HalfEdgeType*)pH->half_edge();

					for (int i = 0; i < 3; ++i)
					{
						EdgeType* pE = HalfEdgeEdge(pHE);
						int vid = pH->key(i);
						VertexType* v = idVertex(vid);
						v->boundary() = true;
						pE->boundary() = true;
						pHE = HalfEdgeNext(pHE);
					}
				}
			}

			// read in traits
			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType* pV = *vIter;
				pV->edges()->shrink_to_fit();
				pV->tvertices()->shrink_to_fit()
				pV->_from_string();
			}

			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				TetType* pT = *tIter;
				pT->_from_string();
			}

			for (auto eIter = mEContainer.begin(); eIter != mEContainer.end(); eIter++)
			{
				EdgeType* pE = *eIter;
				pE->_from_string();
			}

			removeVProp(mVTEArrayHandle);
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>

		HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_half_face(TVertexType ** pTV)
		{
			HalfFaceType* pHF = createHalfFaceWithIndex();

			VertexType * pV[3];

			for (int i = 0; i < 3; i++)
			{
				pV[i] = TVertexVertex(pTV[i]);
			}

			HalfEdgeType * pH[3];
			for (int i = 0; i < 3; i++)
			{
				pH[i] = createHalfEdgeWithIndex();

				pH[i]->SetHalfFace(pHF);
				//pH[i]->
				
				//SetSource(pTV[i]);
				pH[i]->SetTarget(pTV[(i + 1) % 3]);
				pTV[i]->set_halfedge(pH[i]);
			}

			for (int i = 0; i < 3; i++)
			{
				pH[i]->SetNext(pH[(i + 1) % 3]);
				pH[i]->SetPrev(pH[(i + 2) % 3]);
			}

			pHF->SetHalfEdge(pH[0]);

			for (int i = 0; i < 3; i++)
			{
				pHF->key(i) = pV[i]->id();
			}

			//bubble

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2 - i; j++)
				{
					if (pHF->key(j) > pHF->key(j + 1))
					{
						int tmp = pHF->key(j);
						pHF->key(j) = pHF->key(j + 1);
						pHF->key(j + 1) = tmp;
					}
				}
			}

			assert(pHF->key(0) < pHF->key(1));
			assert(pHF->key(1) < pHF->key(2));

			VertexType * pv = m_map_Vertices[pHF->key(0)];
			pv->HalfFaces()->push_back(pHF);

			return pHF;
		};

		//construct faces
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_faces()
		{
			VertexType * pV = NULL;

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				pV = *vIter;

				std::list<HalfFaceType*> * pL = VertexHalfFaceList(pV);

				while (!pL->empty())
				{
					HalfFaceType * pF = pL->front();
					pL->pop_front();
					FaceType* f = createFace();
					f->SetLeft(pF);
					pF->SetFace(f);

					for (std::list<HalfFaceType*>::iterator it = pL->begin(); it != pL->end(); it++)
					{
						HalfFaceType * pH = *it;

						if (*pH == *pF)
						{
							pH->SetDual(pF);
							pF->SetDual(pH);
							f->SetRight(pH);
							pH->SetFace(f);
							break;
						}
					}

					if (pF->dual() != NULL)
					{
						pL->remove(HalfFaceDual(pF));
					}
				}
			}
		};

		//construct edges
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_edges()
		{
			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				TEArray& vteArr = VertexTEdgeList(pV);

				while (vteArr.size() != 0)
				{
					TEdgeType * pTE = vteArr.back();
					vteArr.pop_back();

					EdgeType * e = createEdgeWithIndex();
					assert(e != NULL);

					int id1 = pTE->key(0);
					VertexType * v1 = m_map_Vertices[id1];
					e->SetVertex1(v1);

					int id2 = pTE->key(1);
					VertexType * v2 = m_map_Vertices[id2];
					e->SetVertex2(v2);

					e->edges()->push_back(pTE);
					pTE->SetEdge(e);

					// find tedges that share the same edge
					// adding them to the correponding edge, and delete it from the array
					TEArray tmp_tedges;
					size_t arrSize = vteArr.size();
					for (size_t arrIdx =0; arrIdx < arrSize; arrIdx++)
					{
						TEPtr pTEMatch = vteArr[arrIdx];
						if (*pTEMatch == *pTE)
						{
							pTEMatch->SetEdge(e);
							tmp_tedges.push_back(pTEMatch);

							e->edges()->push_back(pTEMatch);
							vteArr.erase(arrIdx);
							--arrIdx;
							--arrSize;
							// if ereasing element at arrIdx we do not increase arrIdx
						}

					}

					//// adding those 

					//for (std::list<TEdgeType*>::iterator it = tmp_edges.begin(); it != tmp_edges.end(); it++)
					//{
					//	TEdgeType * pH = *it;
					//	pL->remove(pH);
					//	e->edges()->push_back(pH);
					//}

				}

			}


			for (auto it = mEContainer.begin(); it != mEContainer.end(); it++)
			{
				EdgeType * pE = *it;
				VertexType * v1 = EdgeVertex1(pE);
				VertexType * v2 = EdgeVertex2(pE);
				v1->edges()->push_back(pE);
				v2->edges()->push_back(pE);
			}
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_tet(TetType* pT, int tId, int * v)
		{
			//set the tet->id

			pT->id() = tId;

			//set TVertices of the Tet

			for (int k = 0; k < 4; k++)
			{
				TVertexType * pTV = createTVertex();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				VertexType * pV = m_map_Vertices[v[k]];
				pTV->set_vert(pV);
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
			}

			//set half faces

			int order[4][3] = { { 1, 2, 3 },{ 2, 0, 3 },{ 0, 1, 3 },{ 1, 0, 2 } };

			TVertexType   * pTV[3];
			HalfFaceType * pHF[4];

			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 3; k++)
				{
					pTV[k] = TetTVertex(pT, order[i][k]);
				}
				pT->setHalfFace(_construct_half_face(pTV), i);
				pHF[i] = TetHalfFace(pT, i);
			}

			// connect the four half faces

			for (int i = 0; i < 4; i++)
			{
				pHF[i]->SetTet(pT);
			}

			//Seting the dual half edges

			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[i]);
				pH0 = HalfEdgeNext(pH0);
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[(i + 1) % 3]);
				pH1 = HalfEdgePrev(pH1);

				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				TEdgeType * pTE = createTEdgeWithIndex();
				assert(pTE != NULL);
				pTE->SetTet(pT);
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}

				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				VertexType * v = m_map_Vertices[pTE->key(0)];
				VertexTEdgeList(v).push_back(pTE);
			}

			HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[3]);
			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[2 - i]);
				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				TEdgeType * pTE = createTEdgeWithIndex();
				assert(pTE != NULL);
				//set TEdge->Tet
				pTE->SetTet(pT);
				//set HalfEdge->TEdge
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}
				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				VertexType * v = m_map_Vertices[pTE->key(0)];
				VertexTEdgeList(v).push_back(pTE);

				pH0 = HalfEdgeNext(pH0);
			}
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_construct_tet_orientation(TetType* pT, int tId, int  v[4])
		{
			//orient the tet 
			CPoint A = m_map_Vertices[v[0]]->position();
			CPoint B = m_map_Vertices[v[1]]->position();
			CPoint C = m_map_Vertices[v[2]]->position();
			CPoint D = m_map_Vertices[v[3]]->position();
			CPoint AB = B - A;
			CPoint AC = C - A;
			CPoint AD = D - A;

			double orientation_product = AB * (AC ^ AD);
			if (orientation_product < 0) {
				int temp = v[2];
				v[2] = v[3];
				v[3] = temp;
			}
			//set the tet->id

			pT->id() = tId;

			//set TVertices of the Tet

			for (int k = 0; k < 4; k++)
			{
				TVertexType * pTV = createTVertex();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				VertexType * pV = m_map_Vertices[v[k]];
				pTV->set_vert(pV);
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
			}

			//set half faces

			int order[4][3] = { { 1, 2, 3 }, { 2, 0, 3 }, { 0, 1, 3 }, { 1, 0, 2 } };

			TVertexType   * pTV[3];
			HalfFaceType * pHF[4];

			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 3; k++)
				{
					pTV[k] = TetTVertex(pT, order[i][k]);
				}
				pT->setHalfFace(_construct_half_face(pTV), i);
				pHF[i] = TetHalfFace(pT, i);
			}

			// connect the four half faces

			for (int i = 0; i < 4; i++)
			{
				pHF[i]->SetTet(pT);
			}

			//Seting the dual half edges

			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[i]);
				pH0 = HalfEdgeNext(pH0);
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[(i + 1) % 3]);
				pH1 = HalfEdgePrev(pH1);

				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				TEdgeType * pTE = createTEdgeWithIndex();
				pTE->SetTet(pT);
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}

				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				VertexType * v = m_map_Vertices[pTE->key(0)];
				VertexTEdgeList(v).push_back(pTE);
			}

			HalfEdgeType * pH0 = HalfFaceHalfEdge(pHF[3]);
			for (int i = 0; i < 3; i++)
			{
				HalfEdgeType * pH1 = HalfFaceHalfEdge(pHF[2 - i]);
				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				TEdgeType * pTE = createTEdgeWithIndex();
				//set TEdge->Tet
				pTE->SetTet(pT);
				//set HalfEdge->TEdge
				pH0->SetTEdge(pTE);
				pH1->SetTEdge(pTE);

				if (pH0->source()->id() < pH0->target()->id())
				{
					pTE->SetLeft(pH0);
					pTE->SetRight(pH1);
				}
				else
				{
					pTE->SetLeft(pH1);
					pTE->SetRight(pH0);
				}
				pTE->key(0) = pTE->left()->source()->id();
				pTE->key(1) = pTE->left()->target()->id();

				VertexType * v = m_map_Vertices[pTE->key(0)];
				VertexTEdgeList(v).push_back(pTE);

				pH0 = HalfEdgeNext(pH0);
			}
		};

		//write tet mesh to the file

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write(const char * output)
		{

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error is opening file %s\n", output);
				return;
			}
			_os << m_nVertices << " vertices" << std::endl;
			_os << m_nTets << " tets" << std::endl;

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				CPoint  p = pV->position();
				for (int k = 0; k < 3; k++)
				{
					_os << " " << p[k];
				}
				if (pV->string().size() > 0)
				{
					_os << " " << "{" << pV->string() << "}";
				}
				_os << std::endl;
			}

			for (int i = 0; i < m_nTets; i++)
			{
				TetType * pt = m_map_Tets[i];
				_os << 4;
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pt->tvertex(k)->vert()->id();
				}
				//if( pt->string().size() > 0 )
				//{
				//	_os << " " << "{"<< pt->string() << "}";
				//}
				_os << std::endl;
			}

			for (auto eIter = mEContainer.begin(); eIter != mEContainer.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				if (pE->string().size() > 0)
				{
					_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
					_os << "{" << pE->string() << "}" << std::endl;
				}
			}

			_os.close();
		};

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write_t(const char * output, bool highPrecision)
		{
			//write traits to string, add by Wei Chen, 11/23/2015
			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				pV->_to_string();
			}

			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				TetType * pT = *tIter;
				pT->_to_string();
			}

			for (auto eIter = mEContainer.begin(); eIter != mEContainer.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				pE->_to_string();
			}
			//write traits end

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error while opening file %s\n", output);
				return;
			}

			if (highPrecision) {
				_os << std::setiosflags(std::ios::fixed);
			}

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType * pV = *vIter;
				CPoint p = pV->position();
				_os << "Vertex " << pV->id();
				for (int k = 0; k < 3; k++)
				{
					if (highPrecision) {
						_os << " " << std::setprecision(16) << p[k];
					}
					else {
						_os << " " << p[k];
					}
				}
				//if (pV->string().size() > 0)
				//{
				//	_os << " " << "{" << pV->string() << "}";
				//}
				_os << std::endl;
			}

			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				TetType * pT = *tIter;
				_os << "Tet " << pT->id();
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pT->tvertex(k)->vert()->id();
				}
				//if (pT->string().size() > 0)
				//{
				//	_os << " " << "{" << pT->string() << "}";
				//}
				_os << std::endl;
			}

			for (auto eIter = mEContainer.begin(); eIter != mEContainer.end(); eIter++)
			{
				EdgeType * pE = *eIter;
				//if (pE->string().size() > 0)
				//{
				//	_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
				//	_os << "{" << pE->string() << "}" << std::endl;
				//}
			}

			_os.close();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write_vtk(const char* filename, bool highPrecision)
		{
			reinitializeVIds();

			std::fstream _os(filename, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error while opening file %s\n", filename);
				return;
			}

			if (highPrecision) {
				_os << std::setiosflags(std::ios::fixed);
			}

			_os << "# vtk DataFile Version 2.0\n";
			_os << "Unstructured Grid\n";
			_os << "ASCII\n";
			_os << "DATASET UNSTRUCTURED_GRID\n";
			_os << "POINTS " << mVContainer.size() << " double\n";

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType* pV = *vIter;
				CPoint p = pV->position();
				for (int k = 0; k < 3; k++)
				{
					if (highPrecision) {
						_os << " " << std::setprecision(16) << p[k];
					}
					else {
						_os << " " << p[k];
					}
				}

				_os << std::endl;
			}
			_os << "\n";

			_os << "CELLS " << mTContainer.size() << ' ' << mTContainer.size()*5 << '\n';


			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				TetType* pT = *tIter;
				_os << "4 ";
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pT->tvertex(k)->vert()->id();
				}
				_os << std::endl;
			}
			_os << "\n";

			_os << "CELL_TYPES " << mTContainer.size()  << '\n';
			for (auto tIter = mTContainer.begin(); tIter != mTContainer.end(); tIter++)
			{
				_os << "10\n";
			}

			_os.close();
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::_write_tet_list_to_vtk(const char* filename, std::vector<TPtr> tets, bool highPrecision)
		{
			VPropHandle<int> vNewIdsHdl;
			addVProp(vNewIdsHdl);

			for (auto vIter = mVContainer.begin(); vIter != mVContainer.end(); vIter++)
			{
				VertexType* pV = *vIter;
				gVP(vNewIdsHdl, pV) = -1;
			}

			int currentVIds = 0;
			std::vector<VPtr> appearingVerts;
			for (auto tIter = tets.begin(); tIter != tets.end(); tIter++)
			{
				TPtr pT = *tIter;
				for (int k = 0; k < 4; k++)
				{
					VPtr pV = pT->tvertex(k)->vert();

					int & vId = gVP(vNewIdsHdl, pV);
					if (vId == -1) {
						vId = currentVIds;
						++currentVIds;
						appearingVerts.push_back(pV);
					}

				}
			}

			std::fstream _os(filename, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error while opening file %s\n", filename);
				return;
			}

			if (highPrecision) {
				_os << std::setiosflags(std::ios::fixed);
			}

			_os << "# vtk DataFile Version 2.0\n";
			_os << "Unstructured Grid\n";
			_os << "ASCII\n";
			_os << "DATASET UNSTRUCTURED_GRID\n";
			_os << "POINTS " << appearingVerts.size() << " double\n";

			for (auto vIter = appearingVerts.begin(); vIter != appearingVerts.end(); vIter++)
			{
				VertexType* pV = *vIter;
				CPoint p = pV->position();
				for (int k = 0; k < 3; k++)
				{
					if (highPrecision) {
						_os << " " << std::setprecision(16) << p[k];
					}
					else {
						_os << " " << p[k];
					}
				}

				_os << std::endl;
			}
			_os << "\n";

			_os << "CELLS " << tets.size() << ' ' << tets.size() * 5 << '\n';


			for (auto tIter = tets.begin(); tIter != tets.end(); tIter++)
			{
				TetType* pT = *tIter;
				_os << "4 ";
				for (int k = 0; k < 4; k++)
				{
					_os << " " << gVP(vNewIdsHdl, pT->tvertex(k)->vert());
				}
				_os << std::endl;
			}
			_os << "\n";

			_os << "CELL_TYPES " << tets.size() << '\n';
			for (auto tIter = tets.begin(); tIter != tets.end(); tIter++)
			{
				_os << "10\n";
			}

			_os.close();
		}

		/*------------------------------------------------------------------------------------------------
		Access Vertex data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::vector<EdgeType*>* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexEdgeList(VertexType* pVertex)
		{
			return (std::vector<EdgeType*>*) pVertex->edges();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline CPArray<TEdgeType*, TMESH_ARRAY_PRE_ALLOC_SIZE> & CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexTEdgeList(VertexType* pVertex)
		{
			return gVP(mVTEArrayHandle, pVertex);
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::list<HalfFaceType*>* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexHalfFaceList(VertexType* pVertex)
		{
			return (std::list<HalfFaceType*>*) pVertex->HalfFaces();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::vector<TVertexType*>* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexTVertexList(VertexType* pVertex)
		{
			return (std::vector<TVertexType*>*) pVertex->tvertices();
		};

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::VertexEdge(VertexType* v1, VertexType* v2)
		{
			std::vector<EdgeType*>* vEdgeList = VertexEdgeList(v1);

			for (std::vector<EdgeType*>::iterator titer = (*vEdgeList).begin(); titer != (*vEdgeList).end(); titer++)
			{
				EdgeType* pE = *titer;

				VertexType* w1 = EdgeVertex1(pE);
				VertexType* w2 = EdgeVertex2(pE);

				if (w1 == v1 && w2 == v2)
				{
					return pE;
				}
				if (w1 == v2 && w2 == v1)
				{
					return pE;
				}
			}
			return NULL;
		}
		/*------------------------------------------------------------------------------------------------
		Access TVertex data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexVertex(TVertexType* pTVertex)
		{
			return (VertexType*)pTVertex->vert();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexTet(TVertexType* pTVertex)
		{
			return (TetType*)pTVertex->tet();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexHalfEdge(TVertexType* pTVertex)
		{
			return (HalfEdgeType*)pTVertex->halfedge();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TVertexOppositeHalfFace(TVertexType* pTVertex)
		{
			return (HalfFaceType*)pTVertex->halfedge()->next()->dual()->half_face();
		}
		/*------------------------------------------------------------------------------------------------
		Access TEdge data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeLeftHalfEdge(TEdgeType* pTEdge)
		{
			return (HalfEdgeType*)pTEdge->left();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeRightHalfEdge(TEdgeType* pTEdge)
		{
			return (HalfEdgeType*)pTEdge->right();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeEdge(TEdgeType* pTEdge)
		{
			return (EdgeType*)pTEdge->edge();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeDualTEdge(TEdgeType* pTEdge)
		{
			return (TEdgeType*)pTEdge->dual();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeTet(TEdgeType* pTEdge)
		{
			return (TetType*)pTEdge->tet();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeLeftHalfFace(TEdgeType* pTEdge)
		{
			return HalfEdgeHalfFace(TEdgeLeftHalfEdge(pTEdge));
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TEdgeRightHalfFace(TEdgeType* pTEdge)
		{
			return HalfEdgeHalfFace(TEdgeRightHalfEdge(pTEdge));
		}

		/*------------------------------------------------------------------------------------------------
		Access HalfEdge data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeSource(HalfEdgeType* pHalfEdge)
		{
			return (VertexType*)pHalfEdge->source();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTarget(HalfEdgeType* pHalfEdge)
		{
			return (VertexType*)pHalfEdge->target();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTSource(HalfEdgeType* pHalfEdge)
		{
			return (TVertexType*)pHalfEdge->tSource();
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTTarget(HalfEdgeType* pHalfEdge)
		{
			return (TVertexType*)pHalfEdge->tTarget();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTet(HalfEdgeType* pHalfEdge)
		{
			return (TetType*)HalfEdgeTTarget(pHalfEdge)->tet();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeDual(HalfEdgeType* pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->dual();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeNext(HalfEdgeType* pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->next();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgePrev(HalfEdgeType* pHalfEdge)
		{
			return (HalfEdgeType*)pHalfEdge->prev();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeEdge(HalfEdgeType* pHalfEdge)
		{
			return (EdgeType*)pHalfEdge->tedge()->edge();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeTEdge(HalfEdgeType* pHalfEdge)
		{
			return (TEdgeType*)pHalfEdge->tedge();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeHalfFace(HalfEdgeType* pHalfEdge)
		{
			return (HalfFaceType*)pHalfEdge->half_face();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline CPoint CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfEdgeVec(HalfEdgeType* pHalfEdge)
		{
			return HalfEdgeTarget(pHalfEdge)->position() - HalfEdgeSource(pHalfEdge)->position();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline std::vector<TEdgeType*>* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeTEdgeList(EdgeType* pEdge)
		{
			return (std::vector<TEdgeType*>*) pEdge->edges();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeVertex1(EdgeType* pEdge)
		{
			return (VertexType*)pEdge->vertex1();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeVertex2(EdgeType* pEdge)
		{
			return (VertexType*)pEdge->vertex2();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline double CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeLength(EdgeType* pEdge)
		{
			VertexType* pV1 = EdgeVertex1(pEdge);
			VertexType* pV2 = EdgeVertex2(pEdge);

			return (pV1->position() - pV2->position()).norm();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline double CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::EdgeLengthSquare(EdgeType* pEdge)
		{
			return (EdgeVertex1(pEdge)->position() - EdgeVertex2(pEdge)->position()).norm2();
		}

		/*------------------------------------------------------------------------------------------------
		Access HalfFace data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceHalfEdge(HalfFaceType* pHalfFace)
		{
			return (HalfEdgeType*)pHalfFace->half_edge();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline FaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceFace(HalfFaceType* pHalfFace)
		{
			return (FaceType*)pHalfFace->face();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceTet(HalfFaceType* pHalfFace)
		{
			return (TetType*)pHalfFace->tet();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceDual(HalfFaceType* pHalfFace)
		{
			return (HalfFaceType*)pHalfFace->dual();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceOppositeTVertex(HalfFaceType* pHalfFace)
		{
			HalfEdgeType* pHE = HalfFaceHalfEdge(pHalfFace);
			HalfEdgeType* pHEDualNext = HalfEdgeNext(HalfEdgeDual(pHE));

			return HalfEdgeTTarget(pHEDualNext);
		}
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFace3Points(HalfFaceType* pHF, CPoint* v)
		{
			HalfEdgeType* pHE = HalfFaceHalfEdge(pHF);
			v[0] = HalfEdgeSource(pHE)->position();
			v[1] = HalfEdgeTarget(pHE)->position();
			v[2] = HalfEdgeTarget(HalfEdgeNext(pHE))->position();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline CPoint CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceNormal(HalfFaceType* pHF)
		{
			HalfEdgeType* pHE1 = HalfFaceHalfEdge(pHF);;
			HalfEdgeType* pHE2 = HalfEdgeNext(pHE1);;

			CPoint v1 = HalfEdgeVec(pHE1);
			CPoint v2 = HalfEdgeVec(pHE2);
			CPoint n = v1 ^ v2;
			n /= n.norm();

			return n;
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline CPoint CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::HalfFaceOrientedArea(HalfFaceType* pHF)
		{
			HalfEdgeType* pHE1 = HalfFaceHalfEdge(pHF);;
			HalfEdgeType* pHE2 = HalfEdgeNext(pHE1);;

			CPoint v1 = HalfEdgeVec(pHE1);
			CPoint v2 = HalfEdgeVec(pHE2);
			CPoint n = v1 ^ v2;

			return n;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline bool CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::PointInTet(TPtr pT, const CPoint& p)
		{
			double tetOrientedVol = TetOrientedVolume(pT);
			for (int iHF = 0; iHF < 4; iHF++) {
				HalfFaceType* pHF = TetHalfFace(pT, iHF);
				HalfEdgeType* pHE = HalfFaceHalfEdge(pHF);
				CPoint halfFacePt = HalfEdgeSource(pHE)->position();
				
				CPoint normalD = HalfFaceOrientedArea(pHF);
				double orientedVolume = (p - halfFacePt) * normalD;
				if (orientedVolume > 0 && tetOrientedVol > 0) {
					return false;
				}
				else if (orientedVolume < 0 && tetOrientedVol < 0)
				{
					return false;
				}
			}
			return true;
		}

		/*------------------------------------------------------------------------------------------------
		Access Face data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::FaceLeftHalfFace(FaceType* pFace)
		{
			return (HalfFaceType*)pFace->left();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::FaceRightHalfFace(FaceType* pFace)
		{
			return (HalfFaceType*)pFace->right();
		}

		/*------------------------------------------------------------------------------------------------
		Access Tetrahedron data members
		--------------------------------------------------------------------------------------------------*/
		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetHalfFace(TetType* pT, int j)
		{
			return (HalfFaceType*)pT->half_face(j);
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetTVertex(TetType* pT, int j)
		{
			return (TVertexType*)pT->tvertex(j);
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetVertex(TetType* pT, int j)
		{
			return (VertexType*)pT->tvertex(j)->vert();
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline double CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetOrientedVolume(TetType* pT)
		{
			CPoint A = pT->vertex(0)->position();
			CPoint B = pT->vertex(1)->position();
			CPoint C = pT->vertex(2)->position();
			CPoint D = pT->vertex(3)->position();
			CPoint AB = B - A;
			CPoint AC = C - A;
			CPoint AD = D - A;

			double orientation_product = AB * (AC ^ AD);
			return orientation_product;;
		}

		template <typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline CPoint CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::TetCentroid(TetType* pT)
		{
			CPoint centroid;
			for (int i = 0; i < 4; ++i) {
				centroid += TetVertex(pT, i)->position();
			}
			centroid /= 4;
			return centroid;
		}

		//Operations of new a object
		/*! New a vertex in tmesh */
		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::newVertex()
		{
			size_t index;
			VertexType* pV = mVContainer.newMember(index);
			assert(pV != NULL);

			//for (int i = 0; i < mVProps.size(); i++) {
			//	BasicPropHandle* pProp = mVProps[i];
			//	pProp->initializePropMember(index);
			//}
			pV->index() = index;
			return pV;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createVertexWithId(int id)
		{
			VertexType* pV;
			pV = newVertex();
			m_map_Vertices.insert(VMapPair(id, pV));
			pV->id() = id;
			return pV;;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline VertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createVertexWithIndex()
		{
			VertexType* pV;
			pV = newVertex();
			pV->id() = (int)pV->index();
			m_map_Vertices.insert(VMapPair(pV->id(), pV));
			return pV;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TVertexType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createTVertex()
		{
			size_t index;
			TVertexType* pTV = mTVContainer.newMember(index);
			assert(pTV != NULL);
			pTV->index() = index;

			return pTV;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline FaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createFace()
		{
			size_t index;
			FaceType* pF = mFContainer.newMember(index);
			assert(pF != NULL);
			pF->index() = index;

			return pF;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfFaceType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createHalfFaceWithIndex()
		{
			size_t index;
			HalfFaceType* pHF = mHFContainer.newMember(index);
			assert(pHF != NULL);
			pHF->index() = index;

			return pHF;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline HalfEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createHalfEdgeWithIndex()
		{
			size_t index;
			HalfEdgeType* pHE = mHEContainer.newMember(index);
			assert(pHE != NULL);
			pHE->index() = index;

			return pHE;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline EdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createEdgeWithIndex()
		{
			size_t index;
			EdgeType* pE = mEContainer.newMember(index);
			assert(pE != NULL);
			pE->index() = index;

			return pE;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TEdgeType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createTEdgeWithIndex()
		{
			size_t index;
			TEdgeType* pTE = mTEContainer.newMember(index);
			assert(pTE != NULL);
			pTE->index() = index;

			return pTE;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createTetWithIndex()
		{
			size_t index;
			TetType* pT = mTContainer.newMember(index);
			assert(pT != NULL);
			m_map_Tets.insert(TMapPair(index, pT));
			pT->index() = index;

			return pT;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline TetType* CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::createTetWithId(int id)
		{
			size_t index;
			TetType* pT = mTContainer.newMember(index);
			assert(pT != NULL);
			m_map_Tets.insert(TMapPair(id, pT));
			pT->index() = index;

			return pT;
		}

		template<typename TVertexType, typename VertexType, typename HalfEdgeType, typename TEdgeType, typename EdgeType, typename HalfFaceType, typename FaceType, typename TetType>
		inline void CTMesh<TVertexType, VertexType, HalfEdgeType, TEdgeType, EdgeType, HalfFaceType, FaceType, TetType>::reinitializeVIds()
		{
			int currentId = 0;
			m_map_Vertices.clear();
			for (VPtr pV : mVContainer)
			{
				m_map_Vertices.insert(VMapPair(currentId, pV));
				pV->id() = currentId;
				++currentId;
			}
		}

	};


};
#endif _MESHLIB_BASE_TET_MESH_H