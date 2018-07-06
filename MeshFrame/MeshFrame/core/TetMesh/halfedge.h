/*!
*      \file halfedge.h
*      \brief Base tvertex Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tvertex
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_HALFEDGE_H_
#define _TMESHLIB_HALFEDGE_H_

#include <list>
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"

namespace MeshLib
{
	namespace TMeshLib
	{

		class CVertex;
		class CTVertex;
		class CHalfEdge;
		class CEdge;
		class CHalfFace;
		class CFace;
		class CTet;

		/*!
		* \brief CHalfEdge, base class for Half edge
		*/
		class CHalfEdge
		{
		public:
			CHalfEdge()
			{
				m_pSource = NULL;
				m_pTarget = NULL;
				m_pDual = NULL;
				m_pNext = NULL;
				m_pPrev = NULL;
				m_pTEdge = NULL;
				m_pHalfFace = NULL;
			};

			~CHalfEdge(){};

			CVertex* source() { return m_pSource->vert(); };
			CVertex* target()  { return m_pTarget->vert(); };

			CTVertex * tSource() { return m_pSource; };
			CTVertex * tTarget()  { return m_pTarget; };

			CHalfEdge* dual() { return m_pDual; };
			CHalfEdge* next() { return m_pNext; };
			CHalfEdge* prev() { return m_pPrev; };
			CTEdge   * tedge(){ return m_pTEdge; };
			CHalfFace* half_face() { return m_pHalfFace; };

			void SetSource(CTVertex* pVert){ m_pSource = pVert; };
			void SetTarget(CTVertex* pVert){ m_pTarget = pVert; };
			void SetDual(CHalfEdge* pHe){ m_pDual = pHe; };
			void SetNext(CHalfEdge* pHe){ m_pNext = pHe; };
			void SetPrev(CHalfEdge* pHe){ m_pPrev = pHe; };
			void SetTEdge(CTEdge* pTe){ m_pTEdge = pTe; };
			void SetHalfFace(CHalfFace *pF){ m_pHalfFace = pF; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:

			CTVertex   *m_pSource;
			CTVertex   *m_pTarget;
			CHalfEdge  *m_pDual;
			CHalfEdge  *m_pNext;
			CHalfEdge  *m_pPrev;
			CTEdge     *m_pTEdge;
			CHalfFace  *m_pHalfFace;

		};
	};
};

#endif