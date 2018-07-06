/*!
*      \file face.h
*      \brief Base Face Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for Face
*	   \author David Gu
*      \date 10/01/2011
*
*/

#ifndef _TMESHLIB_FACE_H_
#define _TMESHLIB_FACE_H_

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
		* \brief CFace, base class for Face
		*/
		class CFace
		{
		public:
			CFace()
			{
				m_pLeft = NULL;
				m_pRight = NULL;
				m_bIsBoundary = false;
			};

			~CFace(){};

			CHalfFace * left() { return m_pLeft; };
			CHalfFace * right(){ return m_pRight; };

			void SetLeft(CHalfFace * pHf) { m_pLeft = pHf; };
			void SetRight(CHalfFace * pHf) { m_pRight = pHf; };
			bool & boundary(){ return m_bIsBoundary; };

			virtual void _from_string() { };
			virtual void _to_string() { };

		protected:

			CHalfFace * m_pLeft;
			CHalfFace * m_pRight;
			bool		m_bIsBoundary;
		};
	};
};

#endif