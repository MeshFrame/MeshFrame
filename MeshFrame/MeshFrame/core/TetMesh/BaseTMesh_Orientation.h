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

#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Parser/StrUtil_fast.h"

#ifndef MAX_LINE 
#define MAX_LINE 2048
#endif

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
		* \tparam CVertex   vertex   class, derived from TMeshLib::CVertex     class
		* \tparam CTVertex  tetrahedron vertex   class, derived from TMeshLib::CTVertex   class
		* \tparam CHalfEdge halfedge class, derived from TMeshLib::CHalfEdge class
		* \tparam CTEdge	tetrahedron edge class, derived from TMeshLib::CTEdge class
		* \tparam CEdge     edge     class, derived from MeshLib::CEdge     class
		* \tparam CFace     face     class, derived from TMeshLib::CFace     class
		* \tparam CHalfFace half face     class, derived from TMeshLib::CHalfFace     class
		* \tparam CTet      tetrahedron class, derived from TMeshLib::CTet class
		*/

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		class CTMesh
		{
		public:
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
			void _load(const char *);
			/*!
				Load tet mesh from a ".t" file
				*/
			void _load_t(const char *);
			/*!
				Write tet mesh to a file
				*/
			void _write(const char *);

			/*!
			Write tet mesh to a .t file
			*/
			void _write_t(const char * filename);

			/*!
				access the list of half faces
				*/
			std::list<CHalfFace*> & half_faces() { return m_pHalfFaces; };
			/*!
				access the list of edges
				*/
			std::list<CEdge*>       & edges() { return m_pEdges; };
			/*!
				access list of faces
				*/
			std::list<CFace*>        & faces() { return m_pFaces; };

			/*!
			access list of vertices
			*/
			std::list<CVertex*> & vertices() { return m_pVertices; };

			/*! number of tets */
			int numTets() { return m_nTets; };

			/*! number of vertices */
			int numVertices() { return m_nVertices; };

			/*! max vertex id*/
			int maxVertexId() { return m_maxVertexId; };

			/*! Access the array of tets */
			std::list<CTet*> & tets() { return m_pTets; };

			/*! access the vertex with ID */
			CVertex * idVertex(int id) { return m_map_Vertices[id]; };

			/*! access the tet with ID */
			CTet      * idTet(int id) { return m_map_Tets[id]; };



			//Access Vertex data members
			/*! Vertex->Edge List */
			std::list<CEdge*> * VertexEdgeList(CVertex * pVertex);
			/*! Vertex->TEdge List */
			std::list<CTEdge*> * VertexTEdgeList(CVertex * pVertex);
			/*! Vertex->HalfFace List */
			std::list<CHalfFace*> * VertexHalfFaceList(CVertex * pVertex);
			/*! Vertex->TVertex List */
			std::list<CTVertex*> * VertexTVertexList(CVertex * pVertex);
			/*! Vertex->Edge */
			CEdge   * VertexEdge(CVertex * v1, CVertex * v2);

			//Access TVertex data memebers
			CVertex * TVertexVertex(CTVertex * pTVertex);
			CTet * TVertexTet(CTVertex * pTVertex);
			CHalfEdge * TVertexHalfEdge(CTVertex * pTVertex);

			//Access TEdge data memebers
			CHalfEdge * TEdgeLeftHalfEdge(CTEdge * pTEdge);
			CHalfEdge * TEdgeRightHalfEdge(CTEdge * pTEdge);
			CEdge *		TEdgeEdge(CTEdge * pTEdge);
			CTEdge *	TEdgeDualTEdge(CTEdge * pTEdge);
			CTet *		TEdgeTet(CTEdge * pTEdge);

			//Access HalfEdge data members
			/*! HalfEdge->source vertex */
			CVertex * HalfEdgeSource(CHalfEdge * pHalfEdge);
			/*! HalfEdge->target vertex */
			CVertex * HalfEdgeTarget(CHalfEdge * pHalfEdge);
			/*! HalfEdge->source tvertex */
			CTVertex * HalfEdgeTSource(CHalfEdge * pHalfEdge);
			/*! HalfEdge->target tvertex */
			CTVertex * HalfEdgeTTarget(CHalfEdge * pHalfEdge);
			/*! HalfEdge->dual halfedge */
			CHalfEdge * HalfEdgeDual(CHalfEdge * pHalfEdge);
			/*! HalfEdge->next HalfEdge */
			CHalfEdge * HalfEdgeNext(CHalfEdge * pHalfEdge);
			/*! HalfEdge->prev HalfEdge */
			CHalfEdge * HalfEdgePrev(CHalfEdge * pHalfEdge);
			/*! HalfEdge->TEdge TEdge */
			CTEdge * HalfEdgeTEdge(CHalfEdge * pHalfEdge);
			/*! HalfEdge->HalfFace */
			CHalfFace * HalfEdgeHalfFace(CHalfEdge * pHalfEdge);

			//Access Edge data members
			/*! TEdge list of the edge */
			std::list<CTEdge*>* EdgeTEdgeList(CEdge * pEdge);
			/*! Edge->Vertex1 */
			CVertex* EdgeVertex1(CEdge * pEdge);
			/*! Edge->Vertex2 */
			CVertex* EdgeVertex2(CEdge * pEdge);
			/*! length of the edge*/
			double EdgeLength(CEdge * pEdge);

			//Access HalfFace data memebers
			/*! HalfFace->HalfEdge */
			CHalfEdge* HalfFaceHalfEdge(CHalfFace * pHalfFace);
			/*! HalfFace->face */
			CFace* HalfFaceFace(CHalfFace * pHalfFace);
			/*! HalfFace->Tet */
			CTet * HalfFaceTet(CHalfFace * pHalfFace);
			/*! HalfFace->dual half face */
			CHalfFace * HalfFaceDual(CHalfFace * pHalfFace);

			//Face
			/*! access the left half face of a face */
			CHalfFace * FaceLeftHalfFace(CFace * pFace);
			/*! access the right half face of a face */
			CHalfFace * FaceRightHalfFace(CFace * pFace);

			//Tetrahedron

			/*! access the j-th half edge of a tet */
			CHalfFace* TetHalfFace(CTet * pT, int j);
			/*! access the j-th tvertex of a tet */
			CTVertex * TetTVertex(CTet * pT, int j);
			/*! access the j-th vertex of a tet */
			CVertex * TetVertex(CTet * pT, int j);

		protected:

			/*!
			construct tetrahedron
			\tparam v array of vertex ids
			\tparam pT retulting tetrahedron
			*/

			void  _construct_tet(CTet* pT, int tID, int * v);
			/*! construct faces */
			void  _construct_faces();
			/*! construct edges */
			void  _construct_edges();
			/*!  construct half face
			\tparam array of vertex pointers
			*/
			CHalfFace*   _construct_half_face(CTVertex **);

			/*! release all the memory allocations */
			void _clear();

		protected:
			/*!
			list of faces
			*/
			std::list<CFace*>        m_pFaces;
			/*!
			list of half faces
			*/
			std::list<CHalfFace*>	 m_pHalfFaces;
			/*!
			list of half edges
			*/
			std::list<CHalfEdge*>	 m_pHalfEdges;
			/*!
			list of edges
			*/
			std::list<CEdge*>        m_pEdges;
			/*!
			list of tetrahedra
			*/
			std::list<CTEdge*>		 m_pTEdges;

			/*!
			 array of vertices
			 */
			std::list<CVertex*>		 m_pVertices;
			//CVertex *				 m_pVertices;

			/*!
			map of CVertex id and pointer
			*/
			std::map<int, CVertex *> m_map_Vertices;

			/*!
			array of tets
			*/
			std::list<CTet*>		 m_pTets;
			//CTet*                    m_pTets;

			std::map<int, CTet*>     m_map_Tets;

			/*! number of vertices */
			int m_nVertices;

			/*! number of tets */
			int m_nTets;

			/*! number of edges */
			int m_nEdges;

			/*! max vertex id */
			int m_maxVertexId;

		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_clear()
		{
			for (std::list<CEdge*>::iterator eit = m_pEdges.begin(); eit != m_pEdges.end(); eit++)
			{
				CEdge * pE = *eit;
				delete pE;
			}

			for (std::list<CTEdge*>::iterator eit = m_pTEdges.begin(); eit != m_pTEdges.end(); eit++)
			{
				CTEdge * pE = *eit;
				delete pE;
			}

			for (std::list<CHalfEdge*>::iterator hit = m_pHalfEdges.begin(); hit != m_pHalfEdges.end(); hit++)
			{
				CHalfEdge * pH = *hit;
				delete pH;
			}


			for (std::list<CFace*>::iterator fit = m_pFaces.begin(); fit != m_pFaces.end(); fit++)
			{
				CFace * pF = *fit;
				delete pF;
			}

			for (std::list<CHalfFace*>::iterator fit = m_pHalfFaces.begin(); fit != m_pHalfFaces.end(); fit++)
			{
				CHalfFace * pF = *fit;
				delete pF;
			}

			m_pVertices.clear();
			m_pTets.clear();
			//delete[] m_pTVertices;
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_load(const char * input)
		{
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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();
				m_nVertices = strutil::parseString<int>(token);
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
			line = strutil::trim(line);
			strutil::Tokenizer stokenizer(line, " \r\n");
			stokenizer.nextToken();
			std::string token = stokenizer.getToken();
			m_nTets = strutil::parseString<int>(token);
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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutil::parseString<float>(token);
				}

				CVertex * v = new CVertex();
				//CVertex & v = m_pVertices[i];
				v->id() = i;
				v->position() = p;

				m_pVertices.push_back(v);
				m_map_Vertices.insert(std::pair<int, CVertex*>(i, v));

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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				//skip the first "4" in the line
				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutil::parseString<int>(token);
				}

				CTet * pT = new CTet();
				m_pTets.push_back(pT);
				m_map_Tets.insert(std::pair<int, CTet *>(id, pT));

				_construct_tet(pT, id, vId);
			}

			_construct_faces();
			_construct_edges();

			is.close();

			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (std::list<CFace*>::iterator fiter = m_pFaces.begin(); fiter != m_pFaces.end(); ++fiter)
			{
				CFace * pF = *fiter;

				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					CHalfFace * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);

					for (int i = 0; i < 3; ++i)
					{
						int vid = pH->key(i);
						CVertex * v = idVertex(vid);
						v->boundary() = true;
					}
				}
			}
		};


		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_load_t(const char * input)
		{
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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Vertex")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}


				stokenizer.nextToken();
				token = stokenizer.getToken();
				int vid = strutil::parseString<int>(token);

				CPoint p;
				for (int k = 0; k < 3; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					p[k] = strutil::parseString<float>(token);
				}

				CVertex * v = new CVertex();
				//CVertex & v = m_pVertices[i];
				v->id() = vid;
				v->position() = p;

				m_pVertices.push_back(v);

				m_map_Vertices.insert(std::pair<int, CVertex *>(vid, v));

				if (!stokenizer.nextToken("\t\r\n")) continue;
				token = stokenizer.getToken();

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
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

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
				int tid = strutil::parseString<int>(token);


				for (int k = 0; k < 4; k++)
				{
					stokenizer.nextToken();
					std::string token = stokenizer.getToken();
					vId[k] = strutil::parseString<int>(token);
				}



				CTet * pT = new CTet();
				m_pTets.push_back(pT);
				m_map_Tets.insert(std::pair<int, CTet *>(tid, pT));

				_construct_tet(pT, tid, vId);

				// read in string
				if (!stokenizer.nextToken("\t\r\n")) continue;
				token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					pT->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}

			_construct_faces();
			_construct_edges();

			for (int id = 0; id < m_nEdges && is.getline(buffer, MAX_LINE); id++)
			{
				std::string line(buffer);
				line = strutil::trim(line);
				strutil::Tokenizer stokenizer(line, " \r\n");

				stokenizer.nextToken();
				std::string token = stokenizer.getToken();

				if (token != "Edge")
				{
					fprintf(stderr, "File Format Error\r\n");
					return;
				}

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id1 = strutil::parseString<int>(token);

				stokenizer.nextToken();
				token = stokenizer.getToken();
				int id2 = strutil::parseString<int>(token);

				CVertex * pV1 = idVertex(id1);
				CVertex * pV2 = idVertex(id2);

				CEdge * pE = VertexEdge(pV1, pV2);

				if (!stokenizer.nextToken("\t\r\n"))
				{
					continue;
				}

				token = stokenizer.getToken();

				int sp = (int)token.find("{");
				int ep = (int)token.find("}");

				if (sp >= 0 && ep >= 0)
				{
					pE->string() = token.substr(sp + 1, ep - sp - 1);
				}
			}

			m_nEdges = (int)m_pEdges.size();

			is.close();

			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				if (pV->id() > m_maxVertexId)
				{
					m_maxVertexId = pV->id();
				}
			}

			// label the boundary for faces and vertices
			for (std::list<CFace*>::iterator fiter = m_pFaces.begin(); fiter != m_pFaces.end(); ++fiter)
			{
				CFace * pF = *fiter;

				if (this->FaceLeftHalfFace(pF) == NULL || this->FaceRightHalfFace(pF) == NULL)
				{
					pF->boundary() = true;
					CHalfFace * pH =
						FaceLeftHalfFace(pF) == NULL ? FaceRightHalfFace(pF) : FaceLeftHalfFace(pF);

					for (int i = 0; i < 3; ++i)
					{
						int vid = pH->key(i);
						CVertex * v = idVertex(vid);
						v->boundary() = true;
					}
				}
			}

			// read in traits
			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				pV->_from_string();
			}

			for (std::list<CTet *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				CTet * pT = *tIter;
				pT->_from_string();
			}

			for (std::list<CEdge*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				CEdge * pE = *eIter;
				pE->_from_string();
			}

		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		CHalfFace* CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_construct_half_face(CTVertex ** pTV)
		{
			CHalfFace * pHF = new CHalfFace;
			assert(pHF != NULL);
			m_pHalfFaces.push_back(pHF);

			CVertex * pV[3];

			for (int i = 0; i < 3; i++)
			{
				pV[i] = TVertexVertex(pTV[i]);
			}

			CHalfEdge * pH[3];
			for (int i = 0; i < 3; i++)
			{
				pH[i] = new CHalfEdge;
				assert(pH[i] != NULL);
				m_pHalfEdges.push_back(pH[i]);

				pH[i]->SetHalfFace(pHF);
				pH[i]->SetSource(pTV[i]);
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

			CVertex * pv = m_map_Vertices[pHF->key(0)];

			pv->HalfFaces()->push_back(pHF);

			return pHF;
		};

		//construct faces
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_construct_faces()
		{
			CVertex * pV = NULL;

			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				pV = *vIter;

				std::list<CHalfFace*> * pL = VertexHalfFaceList(pV);

				while (!pL->empty())
				{
					CHalfFace * pF = pL->front();
					pL->remove(pF);
					CFace * f = new CFace;
					assert(f != NULL);
					m_pFaces.push_back(f);
					f->SetLeft(pF);
					pF->SetFace(f);

					for (std::list<CHalfFace*>::iterator it = pL->begin(); it != pL->end(); it++)
					{
						CHalfFace * pH = *it;

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
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_construct_edges()
		{
			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				std::list<CTEdge*> * pL = VertexTEdgeList(pV);

				while (!pL->empty())
				{
					CTEdge * pTE = pL->front();
					pL->remove(pTE);
					CEdge * e = new CEdge;
					assert(e != NULL);

					int id1 = pTE->key(0);
					CVertex * v1 = m_map_Vertices[id1];
					e->SetVertex1(v1);

					int id2 = pTE->key(1);
					CVertex * v2 = m_map_Vertices[id2];
					e->SetVertex2(v2);

					m_pEdges.push_back(e);

					e->edges()->push_back(pTE);
					pTE->SetEdge(e);

					std::list<CTEdge*> tmp_edges;
					for (std::list<CTEdge*>::iterator it = pL->begin(); it != pL->end(); it++)
					{
						CTEdge * pH = *it;

						if (*pH == *pTE)
						{
							pH->SetEdge(e);
							tmp_edges.push_back(pH);
						}
					}

					for (std::list<CTEdge*>::iterator it = tmp_edges.begin(); it != tmp_edges.end(); it++)
					{
						CTEdge * pH = *it;
						pL->remove(pH);
						e->edges()->push_back(pH);
					}

				}

			}


			for (std::list<CEdge*>::iterator it = m_pEdges.begin(); it != m_pEdges.end(); it++)
			{
				CEdge * pE = *it;
				CVertex * v1 = EdgeVertex1(pE);
				CVertex * v2 = EdgeVertex2(pE);
				v1->edges()->push_back(pE);
				v2->edges()->push_back(pE);
			}
		};


		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_construct_tet(CTet* pT, int tId, int  v[4])
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
				CTVertex * pTV = new CTVertex();
				pT->setTVertex(pTV, k);
				pTV->id() = k;

				CVertex * pV = m_map_Vertices[v[k]];
				pTV->set_vert(pV);
				pV->tvertices()->push_back(pTV);

				pTV->set_tet(pT);
			}

			//set half faces

			int order[4][3] = { { 1, 2, 3 }, { 2, 0, 3 }, { 0, 1, 3 }, { 1, 0, 2 } };

			CTVertex   * pTV[3];
			CHalfFace * pHF[4];

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
				CHalfEdge * pH0 = HalfFaceHalfEdge(pHF[i]);
				pH0 = HalfEdgeNext(pH0);
				CHalfEdge * pH1 = HalfFaceHalfEdge(pHF[(i + 1) % 3]);
				pH1 = HalfEdgePrev(pH1);

				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				CTEdge * pTE = new CTEdge;
				assert(pTE != NULL);
				m_pTEdges.push_back(pTE);
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

				CVertex * v = m_map_Vertices[pTE->key(0)];
				v->tedges()->push_back(pTE);
			}

			CHalfEdge * pH0 = HalfFaceHalfEdge(pHF[3]);
			for (int i = 0; i < 3; i++)
			{
				CHalfEdge * pH1 = HalfFaceHalfEdge(pHF[2 - i]);
				pH0->SetDual(pH1);
				pH1->SetDual(pH0);

				CTEdge * pTE = new CTEdge;
				assert(pTE != NULL);
				//save TEdges to the TEdge list
				m_pTEdges.push_back(pTE);
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

				CVertex * v = m_map_Vertices[pTE->key(0)];
				v->tedges()->push_back(pTE);

				pH0 = HalfEdgeNext(pH0);
			}
		};

		//write tet mesh to the file

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_write(const char * output)
		{

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error is opening file %s\n", output);
				return;
			}
			_os << m_nVertices << " vertices" << std::endl;
			_os << m_nTets << " tets" << std::endl;

			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
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
				CTet * pt = m_map_Tets[i];
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

			for (std::list<CEdge*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				CEdge * pE = *eIter;
				if (pE->string().size() > 0)
				{
					_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
					_os << "{" << pE->string() << "}" << std::endl;
				}
			}

			_os.close();
		};

		template<typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		void CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::_write_t(const char * output)
		{
			//write traits to string, add by Wei Chen, 11/23/2015
			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				pV->_to_string();
			}

			for (std::list<CTet *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				CTet * pT = *tIter;
				pT->_to_string();
			}

			for (std::list<CEdge*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				CEdge * pE = *eIter;
				pE->_to_string();
			}
			//write traits end

			std::fstream _os(output, std::fstream::out);
			if (_os.fail())
			{
				fprintf(stderr, "Error while opening file %s\n", output);
				return;
			}

			for (std::list<CVertex*>::iterator vIter = m_pVertices.begin(); vIter != m_pVertices.end(); vIter++)
			{
				CVertex * pV = *vIter;
				CPoint p = pV->position();
				_os << "Vertex " << pV->id();
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

			for (std::list<CTet *>::iterator tIter = m_pTets.begin(); tIter != m_pTets.end(); tIter++)
			{
				CTet * pT = *tIter;
				_os << "Tet " << pT->id();
				for (int k = 0; k < 4; k++)
				{
					_os << " " << pT->tvertex(k)->vert()->id();
				}
				if (pT->string().size() > 0)
				{
					_os << " " << "{" << pT->string() << "}";
				}
				_os << std::endl;
			}

			for (std::list<CEdge*>::iterator eIter = m_pEdges.begin(); eIter != m_pEdges.end(); eIter++)
			{
				CEdge * pE = *eIter;
				if (pE->string().size() > 0)
				{
					_os << "Edge " << EdgeVertex1(pE)->id() << " " << EdgeVertex2(pE)->id() << " ";
					_os << "{" << pE->string() << "}" << std::endl;
				}
			}

			_os.close();
		}

		/*------------------------------------------------------------------------------------------------
			Access TVertex data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TVertexVertex(CTVertex * pTVertex)
		{
			return (CVertex*)pTVertex->vert();
		};
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTet * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TVertexTet(CTVertex * pTVertex)
		{
			return (CTet*)pTVertex->tet();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TVertexHalfEdge(CTVertex * pTVertex)
		{
			return (CHalfEdge*)pTVertex->halfedge();
		};
		/*------------------------------------------------------------------------------------------------
			Access Vertex data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline std::list<CEdge*> * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::VertexEdgeList(CVertex * pVertex)
		{
			return (std::list<CEdge*>*) pVertex->edges();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline std::list<CTEdge*> * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::VertexTEdgeList(CVertex * pVertex)
		{
			return (std::list<CTEdge*>*) pVertex->tedges();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline std::list<CHalfFace*> * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::VertexHalfFaceList(CVertex * pVertex)
		{
			return (std::list<CHalfFace*>*) pVertex->HalfFaces();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline std::list<CTVertex*> * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::VertexTVertexList(CVertex * pVertex)
		{
			return (std::list<CTVertex*>*) pVertex->tvertices();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::VertexEdge(CVertex * v1, CVertex * v2)
		{
			std::list<CEdge*> * vEdgeList = VertexEdgeList(v1);

			for (std::list<CEdge*>::iterator titer = (*vEdgeList).begin(); titer != (*vEdgeList).end(); titer++)
			{
				CEdge * pE = *titer;

				CVertex * w1 = EdgeVertex1(pE);
				CVertex * w2 = EdgeVertex2(pE);

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
		};

		/*------------------------------------------------------------------------------------------------
			Access TEdge data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TEdgeLeftHalfEdge(CTEdge * pTEdge)
		{
			return (CHalfEdge*)pTEdge->left();
		};
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TEdgeRightHalfEdge(CTEdge * pTEdge)
		{
			return (CHalfEdge*)pTEdge->right();
		};
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TEdgeEdge(CTEdge * pTEdge)
		{
			return (CEdge*)pTEdge->edge();
		};
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TEdgeDualTEdge(CTEdge * pTEdge)
		{
			return (CTEdge*)pTEdge->dual();
		};
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTet * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TEdgeTet(CTEdge * pTEdge)
		{
			return (CTet*)pTEdge->tet();
		};

		/*------------------------------------------------------------------------------------------------
			Access HalfEdge data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeSource(CHalfEdge * pHalfEdge)
		{
			return (CVertex*)pHalfEdge->source();
		}
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeTarget(CHalfEdge * pHalfEdge)
		{
			return (CVertex*)pHalfEdge->target();
		}

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeTSource(CHalfEdge * pHalfEdge)
		{
			return (CTVertex*)pHalfEdge->tSource();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeTTarget(CHalfEdge * pHalfEdge)
		{
			return (CTVertex*)pHalfEdge->tTarget();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeDual(CHalfEdge * pHalfEdge)
		{
			return (CHalfEdge*)pHalfEdge->dual();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeNext(CHalfEdge * pHalfEdge)
		{
			return (CHalfEdge*)pHalfEdge->next();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgePrev(CHalfEdge * pHalfEdge)
		{
			return (CHalfEdge*)pHalfEdge->prev();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeTEdge(CHalfEdge * pHalfEdge)
		{
			return (CTEdge*)pHalfEdge->tedge();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfEdgeHalfFace(CHalfEdge * pHalfEdge)
		{
			return (CHalfFace*)pHalfEdge->half_face();
		}
		/*------------------------------------------------------------------------------------------------
			Access Edge data members
			--------------------------------------------------------------------------------------------------*/

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline std::list<CTEdge*> * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::EdgeTEdgeList(CEdge * pEdge)
		{
			return (std::list<CTEdge*>*) pEdge->edges();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::EdgeVertex1(CEdge * pEdge)
		{
			return (CVertex*)pEdge->vertex1();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline double CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::EdgeLength(CEdge * pEdge)
		{
			CVertex * pV1 = EdgeVertex1(pEdge);
			CVertex * pV2 = EdgeVertex2(pEdge);

			return (pV1->position() - pV2->position()).norm();
		}

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::EdgeVertex2(CEdge * pEdge)
		{
			return (CVertex*)pEdge->vertex2();
		}
		/*------------------------------------------------------------------------------------------------
			Access HalfFace data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfEdge * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfFaceHalfEdge(CHalfFace * pHalfFace)
		{
			return (CHalfEdge*)pHalfFace->half_edge();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfFaceFace(CHalfFace * pHalfFace)
		{
			return (CFace*)pHalfFace->face();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTet * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfFaceTet(CHalfFace * pHalfFace)
		{
			return (CTet*)pHalfFace->tet();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::HalfFaceDual(CHalfFace * pHalfFace)
		{
			return (CHalfFace*)pHalfFace->dual();
		};

		/*------------------------------------------------------------------------------------------------
			Access Face data members
			--------------------------------------------------------------------------------------------------*/
		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::FaceLeftHalfFace(CFace * pFace)
		{
			return (CHalfFace*)pFace->left();
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::FaceRightHalfFace(CFace * pFace)
		{
			return (CHalfFace*)pFace->right();
		};


		/*------------------------------------------------------------------------------------------------
			Access Tetrahedron data members
			--------------------------------------------------------------------------------------------------*/

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CHalfFace * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TetHalfFace(CTet * pT, int j)
		{
			return (CHalfFace*)pT->half_face(j);
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CTVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TetTVertex(CTet * pT, int j)
		{
			return (CTVertex*)pT->tvertex(j);
		};

		template <typename CTVertex, typename CVertex, typename CHalfEdge, typename CTEdge, typename CEdge, typename CHalfFace, typename CFace, typename CTet>
		inline CVertex * CTMesh<CTVertex, CVertex, CHalfEdge, CTEdge, CEdge, CHalfFace, CFace, CTet>::TetVertex(CTet * pT, int j)
		{
			return (CVertex*)pT->tvertex(j)->vert();
		};

	};
};
#endif _MESHLIB_BASE_TET_MESH_H