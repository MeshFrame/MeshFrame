/*!
*      \file tet.h
*      \brief Base tet Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for tet
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_TET_H_
#define _TMESHLIB_TET_H_

#include <list>
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
		* \brief CTet, base class for Tet
		*/
		class CTet
		{
		public:

			CTet()	{ m_iID = 0; };
			~CTet(){};

			CHalfFace * half_face(int j) { return m_pHalfFace[j]; };
			CTVertex *  tvertex(int j) { return m_pTVertex[j]; };
			CVertex * vertex(int j) { return m_pTVertex[j]->vert(); };
			void setTVertex(CTVertex * pTV, int j) { m_pTVertex[j] = pTV; };
			void setHalfFace(CHalfFace * pHF, int j) { m_pHalfFace[j] = pHF; };

			int  & id() { return m_iID; };

			virtual void _from_string() { };
			virtual void _to_string() { };

			std::string string() { return m_string; };

		protected:

			CHalfFace  * m_pHalfFace[4];
			CTVertex   * m_pTVertex[4];
			int          m_iID;

			std::string m_string;
		};
	};
};

#endif