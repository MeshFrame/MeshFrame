#ifndef _MESHLIB_HALFEDGE_H_
#define _MESHLIB_HALFEDGE_H_
/*!
*      \file HalfEdge.h
*      \brief Base class of HalfEdge
*	   \Version 2.0
*	   \Update 31/05/2018
*/

#include  <assert.h>
#include <math.h>

#define STRINGSIZE 256

namespace MeshLib{

class CVertex;
class CEdge;
class CFace;

/*!
*	\brief CHalfEdge Base class of all kinds of halfedges.
*/
class CHalfEdge
{
public:

	/*!	Constructor, initialize all pointers to be NULL.
	*/
	CHalfEdge(){};
	/*!	Destructure.
	*/
	~CHalfEdge(){};

	/*! Index, current halfedge's index*/
	size_t & index() { return m_index; };
	/*! Pointer to the edge attaching to the current halfedge. */
	CEdge       *   &  edge()    { return m_edge;   };
	/*! Target vertex of the current halfedge. */
	CVertex     *   &  vertex()  { return m_vertex; };
	/*! Target vertex of the current halfedge. */
	CVertex     *   &  target()  { return m_vertex; };
	/*! Source vertex of the current halfedge. */
	CVertex     *   &  source()  { return m_prev->vertex();};
	/*! Previous halfedge of the current halfedge. */
	CHalfEdge *  &  he_prev() { return m_prev;};
	/*! Next halfedge of the current halfedge. */
	CHalfEdge *  &  he_next() { return m_next;};
	/*! The dual halfedge of the current halfedge. */
	CHalfEdge * & he_sym()  { return m_sym; };
	/*! The face, to which the current halfedge attach. */
	CFace     * & face()    { return m_face;};
	/*! Rotate the halfedge about the target vertex ccwly. 
		\return if the current halfedge is the most ccw in halfedge of its target vertex, which is on boundary, return NULL. 	
	*/
	CHalfEdge *   ccw_rotate_about_target();
	/*! Rotate the halfedge about the target vertex clwly. 
		\return if the current halfedge is the most clw in halfedge of its target vertex, which is on boundary, return NULL. 	
	*/
	CHalfEdge *   clw_rotate_about_target();
	/*! Rotate the halfedge about the source vertex ccwly. 
		\return if the current halfedge is the most ccw out halfedge of its source vertex, which is on boundary, return NULL. 
	*/
	CHalfEdge *   ccw_rotate_about_source();
	/*! Rotate the halfedge about the source vertex ccwly. 
		\return if the current halfedge is the most clw out halfedge of its source vertex, which is on boundary, return NULL. 
	*/
	CHalfEdge *   clw_rotate_about_source();
	/*! String of the current halfedge. */
	//char * string() { return m_string; };
	/*! Convert the traits to string, whose Maximum length is MAX_TRAIT_LINE in BaseMesh.h. */
	void _to_string(char * str)   {};
	/*! Read traits from string. */
	void _from_string(char * str) {};

protected:
	/*! Index, current halfedge's index*/
	size_t			  m_index = 0;
	/*! Edge, current halfedge attached to. */
	CEdge       *     m_edge = NULL;
	/*! Face, current halfedge attached to. */
	CFace       *     m_face = NULL;
	/*! Target vertex of the current halfedge. */
	CVertex     *     m_vertex = NULL;		//target vertex
	/*! Previous halfedge of the current halfedge, in the same face. */
	CHalfEdge	*	  m_prev = NULL;
	/*! Next halfedge of the current halfedge, in the same face. */
	CHalfEdge	*     m_next = NULL;
	/*! sym halfedge of the current halfedge, in the different face. */
	CHalfEdge	*     m_sym = NULL;
	/*! The string of the current halfedge. */
	//char			  m_string[STRINGSIZE];
};

//roate the halfedge about its target vertex CCWly

inline CHalfEdge * CHalfEdge::ccw_rotate_about_target()
{
	CHalfEdge * he_dual = he_sym();
	if( he_dual == NULL ) return NULL;

	return he_dual->he_prev();
};

//roate the halfedge about its target vertex CLWly

inline CHalfEdge * CHalfEdge::clw_rotate_about_target()
{
	CHalfEdge * he = he_next()->he_sym();
	return he;
};

//roate the halfedge about its source vertex CCWly


inline CHalfEdge * CHalfEdge::ccw_rotate_about_source()
{

	CHalfEdge * he = he_prev()->he_sym();
	return he;
};

//roate the halfedge about its source vertex CLWly

inline CHalfEdge * CHalfEdge::clw_rotate_about_source()
{
	CHalfEdge * he = he_sym();
	if( he == NULL ) return NULL;
	return he->he_next();
};

}//namespace MeshLib

#endif //_MESHLIB_HALFEDGE_H_ defined