/*!
*      \file tvertex.h
*      \brief Base tvertex Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tvertex
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_TVERTEX_H_
#define _TMESHLIB_TVERTEX_H_

#include <list>
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"

namespace MeshLib
{
	namespace TMeshLib
	{

		class CVertex;
		class CHalfEdge;
		class CEdge;
		class CHalfFace;
		class CFace;
		class CTet;


		/*!
		* \brief CTVertex, base class for Tetrahedron vertex
		*/
		class CTVertex
		{
		public:
			CTVertex(){ m_pVertex = NULL; m_pTet = NULL; m_pHalfedge = NULL; };
			~CTVertex(){};

			int & id() { return m_iID; };

			CVertex         * vert(){ return m_pVertex; };
			CTet            *  tet() { return m_pTet; };
			CHalfEdge       * halfedge() { return m_pHalfedge; };

			void set_vert(CVertex * pV) { m_pVertex = pV; };
			void set_tet(CTet        * pT) { m_pTet = pT; };
			void set_halfedge(CHalfEdge * pH) { m_pHalfedge = pH; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:
			//vertex ID
			int            m_iID;
			CVertex      * m_pVertex;
			CTet         * m_pTet;
			//outgoing, halfedge start from this TVertex
			CHalfEdge *    m_pHalfedge;
		};

	};
};

#endif