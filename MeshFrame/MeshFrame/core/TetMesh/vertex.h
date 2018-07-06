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
#include "../Geometry/Point.h"

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

		class CVertex
		{
		public:
			CVertex() { m_iID = 0; m_bIsBoundary = false; };
			~CVertex(){ m_pTVertices.clear(); }

			CPoint & position() { return m_vPosition; };
			int    & id()       { return m_iID; };
			bool   & boundary()	{ return m_bIsBoundary; };

			std::list<CEdge*>   * edges() { return &m_pEdges; };
			std::list<CTEdge*>  * tedges(){ return &m_pTEdges; };

			std::list<CHalfFace*>  * HalfFaces(){ return &m_pHFaces; };
			std::list<CTVertex*> * tvertices() { return &m_pTVertices; };

			std::string&        string(){ return m_string; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:

			CPoint m_vPosition;
			int    m_iID;
			bool   m_bIsBoundary;

			std::list<CHalfFace*>  m_pHFaces;		//temporary HalfFace list, will be empty after loading the whole mesh 
			std::list<CTEdge*>     m_pTEdges;		//temporary TEdge list, will be empty after loading the whole mesh

			std::list<CTVertex*>   m_pTVertices;	//adjacent TVertecies
			std::list<CEdge*>      m_pEdges;	    //adjacent Edges;

			std::string m_string;
		};
	};
};

#endif