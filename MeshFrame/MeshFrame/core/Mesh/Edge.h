#ifndef _MESHLIB_EDGE_H_
#define _MESHLIB_EDGE_H_
/*!
*      \file Edge.h
*      \brief Base class of edge
*
*	   \Version 2.0
*	   \Update 12/15/2017
*/

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "HalfEdge.h"

#define STRINGSIZE 256

namespace MeshLib{

class CHalfEdge;
class CVertex;

/*!
\brief CEdge class, which is the base class of all kinds of edge classes
*/
class CEdge
{
public:
	/*!
		CEdge constructor, set both halfedge pointers to be NULL.
	*/
	CEdge(){};
	/*!
		CEdge destructor.
	*/
	~CEdge(){};
	/*!
		Edge index.
	*/
	size_t & index() { return m_index; };
	/*!
		Edge ID
	 */
	int & id() { return m_id; };

	/*!
		The halfedge attached to the current edge
		\param id either 0 or 1
		\return the halfedge[id]
	*/
	CHalfEdge * halfedge(int id) { 
		switch (id)
		{
		case 0:
			return m_halfedge;
			break;
		case 1:
			if (m_halfedge != NULL)
			{
				return m_halfedge->he_sym();
			}
			else {
				return NULL;
			}
			break;
		default:
			break;
		}
	};
	CHalfEdge * & halfedge() { return m_halfedge; };
	/*!	
		whether the edge is on the boundary.
	*/
	bool		  boundary() { return (m_halfedge->he_sym() == NULL); };
	/*!
		The dual halfedge to the input halfedge
		\param he halfedge attached to the current edge
		\return the other halfedge attached to the current edge
	*/
	CHalfEdge * & other( CHalfEdge * he ) { return he->he_sym(); };
    /*!
		The string of the current edge.
	*/
	//char * string() { return m_string; };
	/*!
		Read the traits from the string.
	*/
	virtual void _from_string(const char * str) {};
	/*!
		Save the traits to the string, whose Maximum length is MAX_TRAIT_LINE in BaseMesh.h.
	*/
	virtual void _to_string(char * str) {};

	virtual bool hasColor() {
		return false;
	}
	virtual ColorUnion & color() {
		printf("Face does not have normal!\n");
		assert(false);
		system("pause");
		return ColorUnion();
	}
protected:

	/*!
		Pointer to the two halfedges attached to the current edge.
	*/
	CHalfEdge      * m_halfedge = NULL;
	/*!
		The string associated to the current edge.
	*/
	// char			m_string[STRINGSIZE];
	/*! 
		Edge index.
	*/
	size_t m_index;
	/*!
		Edge ID
	 */
	int				 m_id;
};



}//name space MeshLib

#endif //_MESHLIB_EDGE_H_ defined