/*!
*      \file vertex.h
*      \brief Base vertex Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for vertex
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_VERTEX_H_
#define _TMESHLIB_VERTEX_H_

#include <list>
#include <vector>
#include "../Geometry/Point.h"
#include "../Memory/Array.h"

namespace MeshLib
{
	namespace TMeshLib
	{
		class CTVertex;
		class CHalfEdge;
		class CEdge;
		class CTEdge;
		class CHalfFace;
		class CFace;
		class CTet;

		//typedef CPArray<CEdge*, 12> CEArray;
		//typedef CPArray<CTVertex*, 12> CTVArray;
		
		typedef std::vector<CEdge*> CEArray;
		typedef std::vector<CTVertex*> CTVArray;

		//typedef std::vector<CEdge*> CEArray;
		//typedef std::vector<CTVertex*> CTVArray;

		class CVertex
		{
		public:
			CVertex() { m_iID = 0; m_bIsBoundary = false; };
			~CVertex(){ m_pTVertices.clear(); }

			CPoint& position() { return m_vPosition; };
			const CPoint & position() const { return m_vPosition; };
			int    & id()       { return m_iID; };
			size_t & index()    { return m_index; };
			bool   & boundary()	{ return m_bIsBoundary; };

			CEArray * edges() { return &m_pEdges; };
			CTVArray * tvertices() { return &m_pTVertices; };
			//std::list<CTEdge*>  * tedges(){ return &m_pTEdges; };
			std::list<CHalfFace*>  * HalfFaces(){ return &m_pHFaces; };

			//std::string&        string(){ return m_string; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:

			CPoint m_vPosition;
			int    m_iID;
			size_t    m_index;
			bool   m_bIsBoundary;

			std::list<CHalfFace*>  m_pHFaces;		//temporary HalfFace list, will be empty after loading the whole mesh 
			//std::list<CTEdge*>     m_pTEdges;		//temporary TEdge list, will be empty after loading the whole mesh

			CTVArray   m_pTVertices;	//adjacent TVertecies
			CEArray      m_pEdges;	    //adjacent Edges;

			//std::string m_string;
		};
	};
};

#endif