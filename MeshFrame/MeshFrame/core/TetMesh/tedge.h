/*!
*      \file tedge.h
*      \brief Base tedge Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tedge
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_TEDGE_H_
#define _TMESHLIB_TEDGE_H_

#include <list>
#include "halfedge.h"
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
		* \brief CTEdge, base class for tedge
		*/
		class CTEdge //Edge inside a tetrahedron
		{
		public:

			CTEdge(){ m_pLeft = NULL; m_pRight = NULL; };
			~CTEdge(){};

			CHalfEdge * left() { return m_pLeft; };
			CHalfEdge * right(){ return m_pRight; };
			CEdge     * edge() { return m_pEdge; };
			CTet      * tet()  { return  m_pTet; };
			CTEdge    * dual() { return  m_pLeft->prev()->dual()->next()->tedge(); } //tedge in the same tet, against the current one


			void SetLeft(CHalfEdge * pHe) { m_pLeft = pHe; };
			void SetRight(CHalfEdge* pHe) { m_pRight = pHe; };
			void SetEdge(CEdge * pE) { m_pEdge = pE; };
			void SetTet(CTet * pT)   { m_pTet = pT; };

			bool operator==(const CTEdge & e)
			{
				for (int i = 0; i < 2; i++)
				{
					if (m_key[i] != e.m_key[i]) return false;
				}
				return true;
			}

			int & key(int k) { return m_key[k]; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:

			CHalfEdge    * m_pLeft;
			CHalfEdge    * m_pRight;
			CEdge        * m_pEdge;
			CTet         * m_pTet;
			int            m_key[2];

		};
	};
};

#endif