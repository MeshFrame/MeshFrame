/*!
*      \file Iterators.h
*      \brief Iterators for accessing geometric objects on a mesh
*
*/
 
#ifndef  _ITERATORS_H_
#define  _ITERATORS_H_
#include <iterator>
#include <algorithm>
#include <list>
#include <vector>

#include "HalfEdge.h"
#include "BaseMesh.h"

namespace MeshLib{

//v -> v

/*!
	\brief VertexVertexIterator, transverse all the neighboring vertices of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexVertexIterator
{
public:
	/*!
		VertexVertexIterator constructor
		\param v the current vertex
	*/
	VertexVertexIterator( VertexType *  v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge();
	};

	/*!
		VertexVertexIterator destructor
	*/
	~VertexVertexIterator(){};

	/*!
		VertexVertexIterator prefix operator ++, goes to the next neighboring vertex CCWly
	*/

	void operator++() //prefix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};


	/*!
		VertexVertexIterator postfix operator ++, goes to the next neighboring vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};

	/*!
		The neighboring vertex, pointed by the current iterator
	*/

	 VertexType * value() 
	 { 
		 if( !m_vertex->boundary() )
		 {
			 return (VertexType*)m_halfedge->target(); 
		 }

		 if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
		 {
			 return (VertexType*)m_halfedge->target();
		 }

		 if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
		 {
			 return (VertexType*)m_halfedge->source();
		 }
		 return NULL;
	 };

	/*!
		The neighboring vertex, pointed by the current iterator
	*/
	 VertexType * operator*() { return value(); };

	/*!
		Indicate whether all the neighboring vertices have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };

	/*!
		Reset the iterator.
	*/
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*!
		Current vertex
	*/
	VertexType *   m_vertex;
	/*!	
		Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};


//v -> edge
/*!
	\brief VertexEdgeIterator, transverse all the neighboring edges of a vertex ccwly.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexEdgeIterator
{
public:
	/*!
		VertexEdgeIterator constructor
		\param v the current vertex
	*/
	VertexEdgeIterator( VertexType *  v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge();
	};

	/*!
		VertexVertexIterator destructor
	*/
	~VertexEdgeIterator(){};
/*!
		VertexVertexIterator prefix operator ++, goes to the next neighboring vertex CCWly
	*/

	void operator++() //prefix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};
	/*!
		VertexVertexIterator postfix operator ++, goes to the next neighboring vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL ); 
		
		if( !m_vertex->boundary() )
		{
			if( m_halfedge != (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() )
			{
				m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
			}
			else
			{
				m_halfedge = NULL;
			}
			return;
		}

		if( m_vertex->boundary() )
		{
			if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_in_halfedge() )
			{
				m_halfedge = NULL;
				return;
			}

			HalfEdgeType * he = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();

			if( he == NULL )
			{
				m_halfedge = (HalfEdgeType*)m_vertex->most_ccw_in_halfedge();
			}
			else
			{
				m_halfedge = he;
			}
		}

		return;
	};
	/*!
		The neighboring edge, pointed by the current iterator
	*/

	 EdgeType * value() 
	 { 
		 assert( m_halfedge != NULL );
		 return (EdgeType*)m_halfedge->edge();
	 };

	/*!
		The neighboring edge, pointed by the current iterator
	*/
	 EdgeType * operator*() { return value(); };
	/*!
		Indicate whether all the neighboring edges have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };
	/*!
		Reset the VerexEdgeIterator.
	*/
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*! current vertex 
	*/
	VertexType *   m_vertex;
	/*! current halfedge
	*/
	HalfEdgeType * m_halfedge;
};



// v->face
/*!
	\brief VertexFaceIterator, transverse all the neighboring faces of a vertex ccwly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class VertexFaceIterator
{
public:
	/*!
		VertexFaceIterator constructor
		\param v the current vertex
	*/
	VertexFaceIterator( VertexType * & v )
	{ 
		m_vertex = v; 
		m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); 
	};
	/*!
		VertexFaceIterator destructor
	*/
	~VertexFaceIterator(){};
	/*!
		VertexFaceIterator prefix operator ++, goes to the next neighboring face CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );  

		if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() ) 
		{
			m_halfedge = NULL;
			return;
		}
		m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
	};
	/*!
		VertexFaceIterator prefix operator ++, goes to the next neighboring face CCWly
	*/

	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );  

		if( m_halfedge == (HalfEdgeType*)m_vertex->most_ccw_out_halfedge() ) 
		{
			m_halfedge = NULL;
			return;
		}
		m_halfedge = (HalfEdgeType*)m_halfedge->ccw_rotate_about_source();
	};
	/*!
		The neighboring face, pointed by the current iterator
	*/
	FaceType * value() { return (FaceType*) m_halfedge->face(); };
	/*!
		The neighboring face, pointed by the current iterator
	*/
	 FaceType * operator*() { return value(); };
	/*!
		Indicate whether all the neighboring faces have been accessed.
	*/
	 bool end(){ return m_halfedge == NULL; };
	 /*!
	 Reset the VertexFaceIterator
	 */
	 void reset()	{ m_halfedge = (HalfEdgeType*)m_vertex->most_clw_out_halfedge(); };

private:
	/*!
	current vertex
	*/
	VertexType *   m_vertex;
	/*!
	current halfedge
	*/
	HalfEdgeType * m_halfedge;
};

// f -> halfedge
/*!
	\brief FaceHalfedgeIterator, transverse all the halfedges of a face CCWly.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceHalfedgeIterator
{
public:
	/*!
		FaceHalfedgeIterator constructor
		\param f the current face
	*/
	FaceHalfedgeIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};
	/*!
		FaceHalfedgeIterator destructor
	*/
	~FaceHalfedgeIterator(){};
	/*!
		VertexVertexIterator prefix operator ++, goes to the next halfedge CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		VertexVertexIterator prefix operator ++, goes to the next halfedge CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		The halfedge, pointed by the current iterator
	*/
	HalfEdgeType * value() { return m_halfedge; };
	/*!
		The halfedge, pointed by the current iterator
	*/
	HalfEdgeType * operator*() { return value(); };

	/*!
		Indicate whether all the halfedges have been accessed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*!
		current face
	*/
	FaceType *        m_face;
	/*!
		current halfedge
	*/
	HalfEdgeType * m_halfedge;
};


// f -> edge
/*!
	\brief FaceEdgeIterator, transverse all the edges of a face CCWly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceEdgeIterator
{
public:
	/*!
		FaceEdgeIterator constructor
		\param f the current face
	*/	
	FaceEdgeIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};

	/*!
		FaceEdgeIterator destructor
	*/	
	~FaceEdgeIterator(){};
	/*!
		FaceEdgeIterator prefix operator ++, goes to the next edge CCWly
	*/
	void operator++()	//prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		FaceEdgeIterator prefix operator ++, goes to the next edge CCWly
	*/
	void operator++(int)	//postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}
	/*!
		The edge, pointed by the current iterator
	*/
	EdgeType * value() { return (EdgeType*) m_halfedge->edge(); };
	/*!
		The edge, pointed by the current iterator
	*/
	EdgeType * operator*() { return value(); };
	/*!
		Indicate whether all the edges have been transversed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*! Current face. */
	FaceType  *       m_face;
	/*! Current halfedge. */
	HalfEdgeType * m_halfedge;
};


// f -> vertex
/*!
	\brief FaceVertexIterator, transverse all the vertices of a face CCWly.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class FaceVertexIterator
{
public:
	/*!
		FaceVertexIterator constructor
		\param f the current face
	*/
	FaceVertexIterator( FaceType * f )
	{ 
		m_face = f; 
		m_halfedge = (HalfEdgeType*)f->halfedge(); 
	};
	/*!
		FaceVertexIterator destructor
	*/
	
	~FaceVertexIterator(){};
	/*!
		FaceVertexIterator prefix operator ++, goes to the next vertex CCWly
	*/
	void operator++() //prefix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}

	/*!
		FaceVertexIterator prefix operator ++, goes to the next vertex CCWly
	*/
	void operator++(int) //postfix
	{
		assert( m_halfedge != NULL );
		m_halfedge = (HalfEdgeType*)m_halfedge->he_next();

		if( m_halfedge == (HalfEdgeType*)m_face->halfedge() )
		{
			 m_halfedge = NULL;
			return;
		};
	}
	/*!
		The vertex, pointed by the current iterator
	*/
	VertexType * value() { return (VertexType*) m_halfedge->target(); };
	/*!
		The vertex, pointed by the current iterator
	*/
	VertexType * operator*() { return value(); };
	/*!
		Indicate whether all the vertices have been accessed.
	*/
	bool end(){ return m_halfedge == NULL; };

private:
	/*!	Current face.
	*/
	FaceType         * m_face;
	/*!	Current halfedge.
	*/
	HalfEdgeType * m_halfedge;
};


// mesh->v
/*!
	\brief MeshVertexIterator, transverse all the vertices in the mesh.
*/

template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshVertexIterator
{
public:
	/*!
	MeshVertexIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshVertexIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh ):
		_iter(pMesh->vertices().begin())
	{
		m_pMesh = pMesh;
		
	}
	/*!
	The vertex, pointed by the current iterator
	*/
	VertexType * value() { return *_iter; };
	/*!
	The vertex, pointed by the current iterator
	*/
		
	VertexType * operator*(){ return value(); };
	/*!
		MeshVertexIterator prefix operator ++, goes to the next vertex 
	*/	
	void operator++()	 { ++ _iter; }; //prefix
	/*!
		MeshVertexIterator prefix operator ++, goes to the next vertex 
	*/	
	void operator++(int) { ++ _iter; }; //postfix
	/*!
		Indicate whether all the vertices have been accessed.
	*/
	bool end() { return _iter == m_pMesh->vertices().end(); }
	
private:
	/*!
		Current mesh.
	*/
	CBaseMesh<VertexType, EdgeType, FaceType, HalfEdgeType> * m_pMesh;
	MPIterator<VertexType> _iter;

};

// mesh->f
/*!
	\brief MeshFaceIterator, transverse all the faces in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshFaceIterator
{
public:
	/*!
	MeshFaceIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshFaceIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh ):
		_iter(pMesh->faces().begin())
	{
      m_pMesh = pMesh;
	}
	/*!
	The face, pointed by the current iterator
	*/
	FaceType * value() { return *_iter; };
	/*!
	The face, pointed by the current iterator
	*/
	FaceType * operator*(){ return value(); };

	/*!
		MeshFaceIterator prefix operator ++, goes to the next vertex 
	*/
	void operator++() { ++ _iter; }; //prefix
	/*!
		MeshFaceIterator postfix operator ++, goes to the next vertex 
	*/
	void operator++(int) { ++ _iter; }; //postfix
	/*!
		Indicate whether all the faces have been accessed.
	*/
	bool end() { return _iter == m_pMesh->faces().end(); }

private:
	/*! Current mesh.
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*! Current face memorypool iterator.
	*/
	MPIterator<FaceType> _iter;

};

//Mesh->e
/*!
	\brief MeshEdgeIterator, transverse all the edges in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshEdgeIterator
{
public:
	/*!
	MeshEdgeIterator constructor, 
	\param pMesh the current mesh
	*/	
	MeshEdgeIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh ):
		_iter(pMesh->edges().begin())
	{
		m_pMesh = pMesh;
	}
	/*!
	The edge, pointed by the current iterator
	*/	
	EdgeType * value() { return *_iter; };
	/*!
	The edge, pointed by the current iterator
	*/	
	EdgeType * operator*(){ return value(); };
	/*!
		MeshEdgeIterator prefix operator ++, goes to the next edge
	*/	
	void operator++() { ++ _iter; }; //prefix
	/*!
		MeshEdgeIterator postfix operator ++, goes to the next edge
	*/	
	void operator++(int) {_iter++; }; //postfix
	/*!
		Indicate whether all the edges have been accessed.
	*/	
	bool end() { return _iter == m_pMesh->edges().end(); }


private:
	/*!
	current mesh
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * m_pMesh;
	/*!
	current edge memorypool iterator
	*/
	MPIterator<EdgeType> _iter;
};

// Mesh->he
/*!
	\brief MeshHalfEdgeIterator, transverse all the halfedges in the mesh.
*/
template<typename VertexType, typename EdgeType, typename FaceType, typename HalfEdgeType>
class MeshHalfEdgeIterator
{
public:
	/*!
	MeshHalfedgeIterator constructor, 
	\param pMesh the current mesh
	*/
	MeshHalfEdgeIterator( CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> * pMesh ):
		_iter(pMesh->edges().begin())
	{
     m_pMesh = pMesh;
     m_id = 0;
	}
	/*!
	The halfedge, pointed by the current iterator
	*/	
	HalfEdgeType * value() { EdgeType * e = *_iter; return (HalfEdgeType*)e->halfedge(m_id); };
	/*!
	The halfedge, pointed by the current iterator
	*/	
	HalfEdgeType * operator*(){ return value(); };
	/*!
		MeshVertexIterator prefix operator ++, goes to the next halfedge 
	*/
	void operator++() //prefix
	{ 
		++m_id;

		switch( m_id )
		{
		case 1:
			{
				EdgeType * e = *_iter;
				if( e->halfedge(m_id) == NULL )
				{
					m_id = 0;
					++ _iter;
				}
			}
			break;
		case 2:
			m_id = 0;
			++_iter;
			break;
		}
	};
	/*!
		MeshVertexIterator postfix operator ++, goes to the next vertex 
	*/
	void operator++(int) //postfix
	{ 
		++m_id;

		switch( m_id )
		{
		case 1:
			{
				EdgeType * e = *_iter;
				if( e->halfedge(m_id) == NULL )
				{
					m_id = 0;
					++ _iter;
				}
			}
			break;
		case 2:
			m_id = 0;
			++_iter;
			break;
		}
	};
	/*!
	Indicate whether all the halfedges have been accessed
	*/
	bool end() { return _iter == m_pMesh->edges().end(); }
	

private:
	/*!
		Current halfedge
	*/
	HalfEdgeType * m_he;
	/*!
		Current mesh
	*/
	CBaseMesh<VertexType,EdgeType,FaceType,HalfEdgeType> *	 m_pMesh;
	/*!
		Current edge memorypool iterator
	*/
	MPIterator<EdgeType> _iter;
	int  m_id;
};


} //Iterators

#endif
