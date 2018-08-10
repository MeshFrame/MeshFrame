#ifndef _MESHLIB_FACE_H_
#define _MESHLIB_FACE_H_
/*!
*      \file Face.h
*      \brief Base class of face
*
*	   \Version 2.0
*	   \Update 31/05/2017
*/
#include <assert.h>
#include "../Geometry/Point.h"
#include "Color.h"

#define STRINGSIZE 256

namespace MeshLib{


class CHalfEdge;

/*!
	\brief CFace base class of all kinds of face classes
*/
class CFace
{
public:
	/*!	
	CFace constructor
	*/
	CFace(){ m_halfedge = NULL; };
	/*!
	CFace destructor
	*/
	~CFace(){};
	/*!
		One of the halfedges attaching to the current face.
	*/
	CHalfEdge    *		& halfedge() { return m_halfedge; };
	/*!
		index of the current face
	*/
	size_t & index() { return m_index; };
	/*!
		The reference to the current face id
	*/
	int		            & id()          { return m_id;      };
	/*!
		The value of the current face id.
	*/
	const int             id() const { return m_id;      };

	void                  _from_string_default(const char * str) {};
	void                  _to_string_default(char * str) {};
	/*!
		The string of the current face.
	*/
	//char * 				  string()	   { return m_string; };
	/*!
		Convert face traits to the string, whose Maximum length is MAX_TRAIT_LINE in BaseMesh.h..
	*/
	virtual void                  _to_string(char * str)   {};
	/*!
		read face traits from the string.
	*/
	virtual void                  _from_string(const char * str) {};

	virtual bool hasNormal() {
		return false;
	}
	virtual CPoint & normal() {
		static CPoint _normal;
		printf("Face does not have normal!\n");
		assert(false);
		system("pause");
		return _normal;
	}

	virtual bool hasColor() {
		return false;
	}
	virtual ColorUnion & color() {
		printf("Face does not have normal!\n");
		static ColorUnion _color;
		assert(false);
		system("pause");
		return _color;
	}
protected:
	/*!
		index of the current face
	*/
	size_t			   m_index;
	/*!
		id of the current face
	*/
	int			       m_id;
	/*!
		One halfedge  attaching to the current face.
	*/
	CHalfEdge        * m_halfedge;
	/*!
		String of the current face.
	*/
	//char			m_string[STRINGSIZE];
};


}//name space MeshLib

#endif //_MESHLIB_FACE_H_ defined