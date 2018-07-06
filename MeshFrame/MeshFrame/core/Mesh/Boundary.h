/*!
*      \file Boundary.h
*      \brief Trace boundary loops
*
*/


#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <deque>

#include "../Mesh/BaseMesh.h"
#include "../Mesh/Iterators.h"

namespace MeshLib
{
/*!
	\brief CLoopSegment Boundary loop  segment class.
	\tparam CVertex Vertex type
	\tparam CEdge   Edge   type
	\tparam CFace   Face   type
	\tparam CHalfEdge HalfEdge type
*/

template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
class CLoopSegment
{
public:
	/*!
		Constructor of the CLoopSegment
		\param pMesh  pointer to the current mesh
		\param pH halfedge on the boundary loop
	*/
	CLoopSegment(CBaseMesh<CVertex,CEdge,CFace,CHalfEdge>* pMesh, std::vector<CHalfEdge*> & pHs )
	{
		m_pMesh = pMesh;

		for( size_t i = 0; i < pHs.size(); i ++ )
		{
			m_halfedges.push_back( pHs[i] );
		}
	}
	 /*!
		Destructor of CLoop.
	 */
	~CLoopSegment()
	{
		m_halfedges.clear();
	};

	/*!
	The vector of haledges on the current boundary loop segment.
	*/
	std::vector<CHalfEdge*>  & halfedges()
	{
		return m_halfedges;
	}
	/*!
		Starting vertex
	*/
	CVertex * start()
	{
		if( m_halfedges.size() == 0 ) return NULL;
		CHalfEdge * he = m_halfedges[0];
		return m_pMesh->halfedgeSource( he );
	}
	/*!
		ending vertex
	*/
	CVertex * end()
	{
		if( m_halfedges.size() == 0 ) return NULL;
		size_t n = m_halfedges.size();
		CHalfEdge * he = m_halfedges[n-1];
		return m_pMesh->halfedgeTarget( he );
	}

protected:
	/*!
		The mesh pointer
	*/
	CBaseMesh<CVertex,CEdge,CFace,CHalfEdge>		* m_pMesh;

	/*!
		The vector of consecutive halfedges along the boundary loop.
	*/
	std::vector<CHalfEdge*>							  m_halfedges;

};

/*!
	\brief CLoop Boundary loop  class.
	\tparam CVertex Vertex type
	\tparam CEdge   Edge   type
	\tparam CFace   Face   type
	\tparam CHalfEdge HalfEdge type
*/

template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
class CLoop
{
	typedef CLoopSegment<CVertex,CEdge, CFace, CHalfEdge> TSegment;

public:
	/*!
		Constructor of the CLoop
		\param pMesh  pointer to the current mesh
		\param pH halfedge on the boundary loop
	*/
	 CLoop( CBaseMesh<CVertex, CEdge,CFace, CHalfEdge> * pMesh, CHalfEdge * pH );
	/*!
		Constructor of the CLoop
		\param pMesh  pointer to the current mesh
	*/
	 CLoop( CBaseMesh<CVertex, CEdge,CFace, CHalfEdge> * pMesh ) { m_pMesh = pMesh; m_length = 0; m_pHalfedge = NULL; };
	 /*!
		Destructor of CLoop.
	 */
	~CLoop();

	/*!
	The list of haledges on the current boundary loop.
	*/
	std::list<CHalfEdge*>  & halfedges()
	{
		return m_halfedges;
	}

	/*!
		The length of the current boundary loop.
	*/
	double length()
	{
		return m_length;
	}
	/*!
	 *  Output to a file
	 */
	void write( const char * file );
	/*!
	 *  Input from a file
	 */
	void read( const char * file );
	/*!
		The vector of segments on this loop
	*/
	std::vector<TSegment*>  & segments()
	{
		return m_segments;
	}
	/*!
		divide the loop to segments
		\param markers, the array of markers, the first marker is the starting one
	*/
	void divide( std::vector<CVertex*> & markers );

protected:
	/*!
		Pointer to the current mesh.
	*/
	CBaseMesh<CVertex,CEdge,CFace,CHalfEdge>		* m_pMesh;
	/*! The length of the current boundary loop.
	*/
	double											  m_length;
	/*!
		Starting halfedge of the current boundary loop.
	*/
	CHalfEdge										* m_pHalfedge;
	/*!
		List of consecutive halfedges along the boundary loop.
	*/
	std::list<CHalfEdge*>							  m_halfedges;
	/*!
		Vector of segments
	*/
	std::vector<TSegment*>							  m_segments;
};

/*!
	\brief CBoundary Boundary  class.
	\tparam CVertex Vertex type
	\tparam CEdge   Edge   type
	\tparam CFace   Face   type
	\tparam CHalfEdge HalfEdge type
*/

template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
class CBoundary
{
	typedef CLoop<CVertex,CEdge, CFace, CHalfEdge> TLoop;

public:
	/*!
	CBoundary constructor
	\param pMesh pointer to the current mesh
	*/
	CBoundary( CBaseMesh<CVertex,CEdge,CFace,CHalfEdge> * pMesh );
	/*!
	CBoundary destructor
	*/
	~CBoundary();
	/*!
	The list of boundary loops.
	*/
	std::vector<TLoop*> & loops()
	{
		return m_loops;
	}

protected:
	/*!
		Pointer to the current mesh.
	*/
	CBaseMesh<CVertex,CEdge,CFace,CHalfEdge> * m_pMesh;
	/*!
		List of boundary loops.
	*/
	typename std::vector<TLoop*> m_loops;
	/*!
		Bubble sort the loops
		\param loops the vector of loops
	*/
	void _bubble_sort( std::vector<CLoop<CVertex, CEdge, CFace, CHalfEdge>*> & loops);
};

/*!
	CLoop constructure, trace the boundary loop, starting from the halfedge pH.
	\param pMesh pointer to the current mesh
	\param pH  halfedge on the current boundary loop
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
CLoop<CVertex, CEdge, CFace, CHalfEdge>::CLoop( CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> * pMesh, CHalfEdge * pH )
{
	m_pMesh     = pMesh;
	m_pHalfedge = pH;

	m_length = 0;
	CHalfEdge * he = pH;
	//trace the boundary loop
	do{
		CVertex * v = (CVertex*)he->target();
		he = m_pMesh->vertexMostClwOutHalfEdge( v );
		m_halfedges.push_back( he );
    m_length += m_pMesh->edgeLength( (CEdge*)he->edge() );
	}while( he != m_pHalfedge );
}

/*!
CLoop destructor, clean up the list of halfedges in the loop
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
CLoop<CVertex, CEdge, CFace, CHalfEdge>::~CLoop()
{
	m_halfedges.clear();

	for( size_t i = 0; i < m_segments.size(); i ++ )
	{
		delete m_segments[i];
	}
	m_segments.clear();
}


/*!
	_bubble_sort
	bubble sort  a vector of boundary loop objects, according to their lengths
	\param loops vector of loops
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CBoundary<CVertex, CEdge, CFace, CHalfEdge>::_bubble_sort( std::vector<CLoop<CVertex, CEdge, CFace, CHalfEdge>*> & loops)
{
      int i, j, flag = 1;    // set flag to 1 to start first pass
      CLoop<CVertex,CEdge,CFace,CHalfEdge> * temp;             // holding variable
      int numLength = (int)loops.size( );
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
               if (loops[j+1]->length() > loops[j]->length() )      // ascending order simply changes to <
              {
                    temp = loops[j];								// swap elements
                    loops[j] = loops[j+1];
                    loops[j+1] = temp;
                    flag = 1;										// indicates that a swap occurred.
               }
          }
     }
}

/*!
	CBoundary constructor
	\param pMesh the current mesh
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
CBoundary<CVertex, CEdge, CFace, CHalfEdge>::CBoundary( CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> * pMesh )
{
	m_pMesh = pMesh;
	//collect all boundary halfedges
	std::set<CHalfEdge*> boundary_hes;
	for( MeshEdgeIterator<CVertex, CEdge, CFace, CHalfEdge> eiter( m_pMesh); !eiter.end(); eiter ++ )
	{
		CEdge * e = *eiter;
		if( !m_pMesh->isBoundary(e) ) continue;

		CHalfEdge * he = m_pMesh->edgeHalfedge( e, 0);
		boundary_hes.insert( he );
	}
	//trace all the boundary loops
	while( !boundary_hes.empty() )
	{
		//get the first boundary halfedge
        typename std::set<CHalfEdge*>::iterator siter = boundary_hes.begin();
		CHalfEdge * he = *siter;
		//trace along this boundary halfedge
		CLoop<CVertex, CEdge, CFace, CHalfEdge> * pL = new CLoop<CVertex, CEdge, CFace, CHalfEdge>( m_pMesh, he );
		assert(pL);
		m_loops.push_back( pL );
		//remove all the boundary halfedges, which are in the same boundary loop as the head, from the halfedge list
		for( typename std::list<CHalfEdge*>::iterator hiter = pL->halfedges().begin();
			hiter != pL->halfedges().end(); hiter ++ )
		{
			CHalfEdge * he = *hiter;
			siter = boundary_hes.find( he );
			if( siter == boundary_hes.end() ) continue;
			boundary_hes.erase( siter );
		}
	}

	//std::sort( vlps.begin(), vlps.end(), loop_compare<CVertex,CFace,CEdge,CHalfEdge> );
	_bubble_sort( m_loops );
}

/*!	CBoundary destructor, delete all boundary loop objects.
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
CBoundary<CVertex, CEdge, CFace, CHalfEdge>::~CBoundary()
{
	for( int i = 0; i < (int)m_loops.size(); i ++ )
	{
		CLoop<CVertex, CEdge, CFace, CHalfEdge> * pL = m_loops[i];
		delete pL;
	}
};

/*!
	Output the loop to a file
	\param file_name the name of the file
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CLoop<CVertex, CEdge, CFace, CHalfEdge>::write( const char * file_name )
{
	std::ofstream myfile;
	myfile.open (file_name);
	for( typename std::list<CHalfEdge*>::iterator hiter = m_halfedges.begin(); hiter != m_halfedges.end(); hiter ++ )
	{
		CHalfEdge * pH = *hiter;
		CVertex * pV = m_pMesh->halfedgeSource(pH);
		CVertex * pW = m_pMesh->halfedgeTarget(pH);
		myfile << pV->id() << " " << pW->id() << std::endl;
	}
	myfile.close();
};

/*!
	Output the loop to a file
	\param file_name the name of the file
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CLoop<CVertex, CEdge, CFace, CHalfEdge>::read( const char * file_name )
{
	std::ifstream myfile;
	myfile.open (file_name);

	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			int source, target;
			myfile >> source >> target;
			CVertex * pS = m_pMesh->idVertex( source );
			CVertex * pT = m_pMesh->idVertex( target );
			CEdge   * pE = m_pMesh->vertexEdge( pS, pT );
			CHalfEdge * pH = m_pMesh->edgeHalfedge(pE,0);
			m_halfedges.push_back( pH );
		}
		myfile.close();
	}

};


/*!
	Divide the loop to segments
	\param markers
*/
template<typename CVertex, typename CEdge, typename CFace, typename CHalfEdge>
void CLoop<CVertex, CEdge, CFace, CHalfEdge>::divide( std::vector<CVertex*> & markers )
{
	std::deque<CHalfEdge*> queue;
	for( typename std::list<CHalfEdge*>::iterator hiter = m_halfedges.begin(); hiter != m_halfedges.end(); hiter ++ )
	{
		CHalfEdge * ph = *hiter;
		queue.push_back( ph );
	}

	size_t n = 0;
	while( true )
	{
		CHalfEdge * ph = queue.front();
		if( m_pMesh->halfedgeSource( ph ) == markers[0] ) break;
		queue.pop_front();
		queue.push_back( ph );
		n ++;
		if( n > queue.size() ) break;
	}

	if( n > queue.size() )
	{
		std::cerr << "CLoop::divide: can not find the starting vertex " << std::endl;
		return;
	}

	for( size_t i = 0; i < markers.size(); i ++ )
	{
		std::vector<CHalfEdge*> hes;
		CHalfEdge * ph = queue.front();
		queue.pop_front();
		assert( m_pMesh->halfedgeSource( ph ) == markers[i] );
		hes.push_back( ph );

		while( m_pMesh->halfedgeTarget(ph ) != markers[(i+1)%markers.size()] )
		{
			if( queue.empty())
			{
				std::cerr << "CLoop::divide error" << std::endl;
				return;
			}
			ph = queue.front();
			queue.pop_front();
			hes.push_back( ph );
		}

		TSegment * pS = new TSegment( m_pMesh, hes );
		assert( pS != NULL );
		m_segments.push_back( pS );
	}
};


}
#endif

