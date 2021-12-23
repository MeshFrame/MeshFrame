/*!
*      \file Vertex.h
*      \brief Base class of vertex
*	   \Version 2.0
*	   \Update 31/05/2018
*/

#ifndef  _MESHLIB_VERTEX_H_
#define  _MESHLIB_VERTEX_H_

#include <stdlib.h>
#include "../Geometry/Point.h"
#include "../Geometry/Point2.h"
#include "../Memory/Array.h"
#include "HalfEdge.h"
#include "Color.h"

namespace MeshLib{

  class CHalfEdge;
  
  /*!
  \brief CVertex class, which is the base class of all kinds of vertex classes
  */

  class CVertex
  {
  public:
	  typedef CPArray<CHalfEdge*, 8> CHEArray;

	  /*!
	  CVertex constructor
	  */
      CVertex(){ m_halfedge = NULL; m_boundary = false; };
	  /*!
	  CVertex destructor 
	  */
    ~CVertex(){};

	/*! The point of the vertex
	*/
    CPoint & point()    { return m_point;};
	/*! The normal of the vertex
	*/
    //CPoint & normal()   { return m_normal; };
	/*! The texutre coordinates of the vertex
	*/
	//CPoint2 & uv()       { return m_uv; };

	/*! The most counter clockwise outgoing halfedge of the vertex .
	*/
    CHalfEdge * most_ccw_out_halfedge();
	/*! The most clockwise outgoing halfedge of the vertex .
	*/
    CHalfEdge * most_clw_out_halfedge();
	/*! The most counter clockwise incoming halfedge of the vertex. 
	*/
    CHalfEdge * most_ccw_in_halfedge();
	/*! The most clockwise incoming halfedge of the vertex. 
	*/
    CHalfEdge * most_clw_in_halfedge();

	/*! One incoming halfedge of the vertex .
	*/
    CHalfEdge * & halfedge() { return m_halfedge; };
	/*! Vertex index.
	*/
	size_t & index() { return m_index; };
	/*! Vertex id. 
	*/
    int  & id() { return m_id; };
	/*! Whether the vertex is on the boundary. 
	*/
    bool & boundary() { return m_boundary;};

	/*! Read traits from the string, this function is called by Basemesh.h
	*   And should NEVER be varified by user, because some defualt reading process should be done before
	*   the user doing his own reading procedure.
	*   If you want to have your own reading procedure, change _from_string();
	*/
	void _from_string_default(const char * str) {};
	/*! Convert vertex traits to string, whose Maximum length is MAX_TRAIT_STRING_SIZE.
	*   this function is called by Basemesh.h.  And should NEVER be varified by user,
	*   because some defualt writting process should be done before the user doing his own writting procedure.
	*   If you want to have your own reading procedure, change _to_string();
	*/
	void _to_string_default(char * str) {};
	/*!	Adjacent edges, temporarily used for loading the mesh
	 */

	 /*! Convert vertex traits to string, whose Maximum length is MAX_TRAIT_STRING_SIZE defined in IOFuncDef.h.
	 */
	void _to_string(char * str) {};
	/*! Read traits from the string, should be overrided if extra loading behavior is needed.
	*/
	void _from_string(const char * str) {};
	/*! Vertex index.
	*/

	CHEArray & outHEs() { return m_outHEs; };

	//ColorUnion & getColor() { return m_color; };

	virtual bool hasUV() { return false; };
	virtual CPoint2 & uv() {
		static CPoint2 _uv;
		printf("Vertex does not have uv!\n");
		assert(false);
		system("pause");
		return _uv;
	};
	virtual bool hasNormal() { return false; };
	virtual CPoint & normal() {
		static CPoint _normal;
		printf("Vertex does not have normal!\n");
		assert(false);
		system("pause");
		return _normal;
	};
	virtual bool hasColor() { return false; };
	virtual ColorUnion & color() {
		static ColorUnion _color;
		printf("Vertex does not have color!\n");
		assert(false);
		system("pause");
		return _color;
	};
	
  protected:

	size_t m_index;
    /*! Vertex ID. 
	*/
    int    m_id ;
    /*! Vertex position point. 
	*/
    CPoint m_point;
	/*! Normal at the vertex. 
	*/
    //CPoint m_normal;
	/*! Texture coordinates of the vertex. 
	*/
	//CPoint2 m_uv;

	//ColorUnion m_color;
	/*! The most CCW incoming halfedge of the vertex.
	*/
    CHalfEdge *     m_halfedge;
	/*! Indicating if the vertex is on the boundary. 
	*/
    bool            m_boundary = false;
	/*! The string of the vertex, which stores the traits information. 
	*/
	//char			m_string[MAX_TRAIT_STRING_SIZE];
	/*! List of adjacent edges, such that current vertex is the end vertex of the edge with smaller id
	 */
	CHEArray m_outHEs;

  }; //class CVertex


/*! \brief The most counter clockwise incoming halfedge of the vertex
 *  \return the most CCW in halfedge
*/
inline CHalfEdge *  CVertex::most_ccw_in_halfedge()  
{ 
	//for interior vertex
	if( !m_boundary )
	{
		return m_halfedge; //current half edge is the most ccw in halfedge 
	}

	//for boundary vertex
	CHalfEdge * he = m_halfedge->ccw_rotate_about_target();
	CHalfEdge * pStartHE = he;
	//rotate to the most ccw in halfedge
	size_t numHEs = m_outHEs.size();
	int numHEIterated = 0;
	while( he != NULL )
	{
		if (numHEIterated >= numHEs) {
			break;
		}

		m_halfedge = he;
		he = m_halfedge->ccw_rotate_about_target();
		
		++numHEIterated;
	}
	// the halfedge of the vertex becomes the most ccw in halfedge
	return m_halfedge;
};

//most clockwise in halfedge

inline CHalfEdge *  CVertex::most_clw_in_halfedge()  
{ 
	//for interior vertex 
	if( !m_boundary )
	{
		return most_ccw_in_halfedge()->ccw_rotate_about_target(); //the most ccw in halfedge rotate ccwly once to get the most clw in halfedge
	}
	//for boundary vertex
	CHalfEdge * he = m_halfedge->clw_rotate_about_target();
	CHalfEdge * pStartHE = he;
	//rotate to the most clw in halfedge
	while( he != NULL )
	{
		m_halfedge = he;
		he = m_halfedge->clw_rotate_about_target();
		if (he == pStartHE) {
			break;
		}
	}

	return m_halfedge;
};

//most counter clockwise out halfedge

inline CHalfEdge *  CVertex::most_ccw_out_halfedge()  
{ 
	//for interior vertex
	if( !m_boundary )
	{
		return most_ccw_in_halfedge()->he_sym(); //most ccw out halfedge is the dual of the most ccw in halfedge
	}

	//for boundary vertex
	CHalfEdge * he = m_halfedge->he_next();
	//get the out halfedge which is the next halfedge of the most ccw in halfedge
	CHalfEdge * ne = he->ccw_rotate_about_source();
	//rotate ccwly around the source vertex
	while( ne != NULL )
	{
		he = ne;
		ne = he->ccw_rotate_about_source();
	}

	return he;
};

//most clockwise out halfedge

inline CHalfEdge * CVertex::most_clw_out_halfedge()  
{ 
	//for interior vertex
	if( !m_boundary )
	{
		return most_ccw_out_halfedge()->ccw_rotate_about_source();  //most ccw out halfedge rotate ccwly once about the source
	}
	//get one out halfedge
	CHalfEdge * he = m_halfedge->he_next();
	//rotate the out halfedge clwly about the source
	CHalfEdge * ne = he->clw_rotate_about_source();
	
	while( ne != NULL )
	{
		he = ne;
		ne = he->clw_rotate_about_source();
	}

	return he;
};



}//name space MeshLib

#endif //_MESHLIB_VERTEX_H_defined