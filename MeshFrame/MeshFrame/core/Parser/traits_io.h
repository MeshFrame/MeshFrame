/*! \file Traits_IO.h
 *  \brief  input/output the traits
 *  \author David Gu
 *  \date   documented on 6/23/2011
 *
 */

#ifndef _TRAITS_IO_H_
#define _TRAITS_IO_H_

#include <map>
#include <vector>

#include "Mesh/BaseMesh.h"
#include "Mesh/Vertex.h"
#include "Mesh/HalfEdge.h"
#include "Mesh/Edge.h"
#include "Mesh/Face.h"
#include "mesh/iterators.h"
#include "mesh/boundary.h"
#include "Parser/parser.h"

#define VERTEX_RGB     (0x01<<0)
#define VERTEX_UV      (0x01<<1)
#define VERTEX_Z       (0x01<<2)
#define VERTEX_MU      (0x01<<3)
#define VERTEX_FATHER  (0x01<<4)
#define VERTEX_LAMBDA  (0x01<<5)
#define VERTEX_NORMAL  (0x01<<6)
#define VERTEX_U       (0x01<<7)
#define EDGE_LENGTH    (0x01<<8)
#define EDGE_SHARP     (0x01<<9)
#define EDGE_DU		   (0x01<<10)
#define EDGE_DUV       (0x01<<11)

#define FACE_RGB       (0x01<<16)
#define FACE_NORMAL    (0x01<<17)
#define FACE_U         (0x01<<18)

namespace MeshLib
{

template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_target_k( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "k" )
		  {
			  std::string line = strutil::trim( token->m_value, "()");
			  pV->target_k() = strutil::parseString<double>( line );	
		  }
		}
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _write_vertex_uv( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CPoint2 uv = pV->uv();

		CParser parser( pV->string() );
		parser._removeToken( "uv" );
		
		parser._toString( pV->string() );
		
		std::stringstream iss;
		
		iss << "uv=(" << uv[0] << " " << uv[1] << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};

template<typename M>
void _write_vertex_huv( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CPoint2 uv = pV->huv();

		CParser parser( pV->string() );
		parser._removeToken( "uv" );
		
		parser._toString( pV->string() );
		
		std::stringstream iss;
		
		iss << "uv=(" << uv[0] << " " << uv[1] << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_uv( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "uv" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pV->uv() = uv;
		  }
		}
	}
};

template<typename M>
void __read_vertex_uv( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "uv" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pV->uv() = uv;
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_z( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "z" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pV->z() = std::complex<double>( uv[0], uv[1] );
		  }
		}
	}
};



template<typename M>
void _read_vertex_huv( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "uv" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pV->huv() = uv;
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_father( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "father" )
		  {
			  std::string line = strutil::trim( token->m_value, "()");
			  pV->father() = strutil::parseString<int>( line );	
		  }
		}
	}
};

template<typename M>
void _read_vertex_father_trait( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "father" )
		  {
			  std::string line = strutil::trim( token->m_value, "()");
			  pV->father() = strutil::parseString<int>( line );	
		  }
		}
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_mu( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "mu" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pV->mu() = std::complex<double>( uv[0], uv[1] );
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_normal( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "normal" )
		  {
			  CPoint normal;
			  token->m_value >> normal;
			  pV->normal() = normal;
		  }
		}
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _read_vertex_rgb( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "rgb" )
		  {
			  CPoint rgb;
			  token->m_value >> rgb;
			  pV->rgb() = rgb;
		  }
		}
	}
};

template<typename M>
void _read_vertex_rgb_trait( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CParser parser( pV->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "rgb" )
		  {
			  CPoint rgb;
			  token->m_value >> rgb;
			  pV->rgb() = rgb;
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_edge_length( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		E * pE = *eiter;

		CParser parser( pE->string() );

		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "l" )
		  {
			 std::string line = strutil::trim( token->m_value, "()");
			 pE->length() = strutil::parseString<double>(line) ;		
		  }
		}
	}
};

template<typename M>
void _read_edge_length_trait( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		M::CEdge * pE = *eiter;

		CParser parser( pE->string() );

		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "l" )
		  {
			 std::string line = strutil::trim( token->m_value, "()");
			 pE->length() = strutil::parseString<double>(line) ;		
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _read_edge_sharp( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		E * pE = *eiter;

		CParser parser( pE->string() );
		pE->sharp() = false;

		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "sharp" )
		  {
			  pE->sharp() = true;
		  }
		}
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _write_vertex_z( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		parser._removeToken( "z" );

		parser._toString( pV->string() );

		std::stringstream iss;

		iss << "z=(" << pV->z().real() << " " << pV->z().imag() << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _write_vertex_mu( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CParser parser( pV->string() );
		parser._removeToken( "mu" );

		parser._toString( pV->string() );

		std::stringstream iss;

		iss << "mu=(" << pV->mu().real() << " " << pV->mu().imag() << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};

template<typename M, typename V, typename E, typename F, typename H>
void _write_vertex_u( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CPoint rgb = pV->rgb();

		CParser parser( pV->string() );
		parser._removeToken( "u" );
		parser._toString( pV->string() );
		CPoint u = pV->u();
		std::stringstream iss;
		iss << "u=(" << u[0] << " " << u[1] << " " << u[2] << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _write_vertex_rgb( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		V * pV = *viter;
		CPoint rgb = pV->rgb();

		CParser parser( pV->string() );
		parser._removeToken( "rgb" );
		
		parser._toString( pV->string() );
		
		std::stringstream iss;
		
		iss << "rgb=(" << rgb[0] << " " << rgb[1] << " " << rgb[2] << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};

template<typename M>
void _write_vertex_rgb_trait( M * pMesh )
{
	for( M::MeshVertexIterator viter( pMesh ); !viter.end(); viter ++ )
	{
		M::CVertex * pV = *viter;
		CPoint rgb = pV->rgb();

		CParser parser( pV->string() );
		parser._removeToken( "rgb" );
		
		parser._toString( pV->string() );
		
		std::stringstream iss;
		
		iss << "rgb=(" << rgb[0] << " " << rgb[1] << " " << rgb[2] << ")";

		if( pV->string().size() > 0 )
		{
		  pV->string() += " ";
		}
		pV->string() += iss.str();
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _write_edge_sharp( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		E * pE = *eiter;
		CParser parser( pE->string() );
		parser._removeToken( "sharp" );
		parser._toString( pE->string() );
		
		std::string line;
		std::stringstream iss(line);
		if( pE->sharp() )
		{
			iss << "sharp";
		}
		if( pE->string().length() > 0 )
		{
			pE->string() += " ";
		}
		pE->string() += iss.str();

	};
};




template<typename M, typename V, typename E, typename F, typename H>
void _write_edge_du( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		E * pE = *eiter;
		CParser parser( pE->string() );
		parser._removeToken( "du" );

		parser._toString( pE->string() );

		std::stringstream iss;

		iss << "du=(" << pE->du() << ")";

		if( pE->string().size() > 0 )
		{
		  pE->string() += " ";
		}
		pE->string() += iss.str();
	}
};


template<typename M, typename V, typename E, typename F, typename H>
void _input_traits( M * pMesh )
{
	if( M->m_input_traits |= VERTEX_UV )
	{
		_read_vertex_uv<M,V,E,F,H>( pMesh );
	}

	if( M->m_input_traits |= VERTEX_NORMAL )
	{
		_read_vertex_normal<M,V,E,F,H>( pMesh );
	}

	if( M->m_input_traits |= VERTEX_RGB )
	{
		_read_vertex_rgb( pMesh );
	}

	if( M->m_input_traits |= EDGE_LENGTH )
	{
		_read_edge_length( pMesh );
	}

	if( M->m_input_traits |= EDGE_SHARP )
	{
		_read_edge_sharp( pMesh );
	}

};

template<typename M, typename V, typename E, typename F, typename H>
void _output_traits( M * pMesh )
{
	if( M->m_m_output_traits |= VERTEX_UV )
	{
		_write_vertex_uv<M,V,E,F,H>( pMesh );
	}

	if( M->m_m_output_traits |= VERTEX_MU )
	{
		_write_vertex_mu<M,V,E,F,H>( pMesh );
	}

	if( M->m_m_output_traits |= VERTEX_RGB )
	{
		_write_vertex_rgb<M,V,E,F,H>( pMesh );
	}

	if( M->m_m_output_traits |= VERTEX_U )
	{
		_write_vertex_u<M,V,E,F,H>( pMesh );
	}

	if( M->m_m_output_traits |= EDGE_DU )
	{
		_write_edge_du<M,V,E,F,H>( pMesh );
	}

	if( M->m_m_output_traits |= EDGE_SHARP )
	{
		_write_edge_sharp<M,V,E,F,H>( pMesh );
	}
};


template<typename M>
void _write_edge_length_trait( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		M::CEdge * pE = *eiter;
		CParser parser( pE->string() );
		parser._removeToken( "l" );
		parser._toString( pE->string() );

		std::stringstream iss;
		iss << "l=("<< std::setprecision(12) << pE->length() << ")";
		if( pE->string().size() > 0 )
		{
			pE->string() += " ";
		}
		pE->string() += iss.str();
	}
};


template<typename M>
void _write_edge_sharp_trait( M * pMesh )
{
	for( M::MeshEdgeIterator eiter( pMesh ); !eiter.end(); eiter ++ )
	{
		M::CEdge * pE = *eiter;
		CParser parser( pE->string() );
		parser._removeToken( "sharp" );
		parser._toString( pE->string() );
		
		std::string line;
		std::stringstream iss(line);
		if( pE->sharp() )
		{
			iss << "sharp";
		}
		if( pE->string().length() > 0 )
		{
			pE->string() += " ";
		}
		pE->string() += iss.str();

	};
};


template<typename M>
void _read_face_mu( M * pMesh )
{
	for( M::MeshFaceIterator fiter( pMesh ); !fiter.end(); fiter ++ )
	{
		M::CFace * pF = *fiter;
		CParser parser( pF->string() );
		
		for( std::list<CToken*>::iterator iter = parser.tokens().begin() ; iter != parser.tokens().end(); ++ iter )
		{
		  CToken * token = *iter;
		  if( token->m_key == "mu" )
		  {
			  CPoint2 uv;
			  token->m_value >> uv;
			  pF->mu() = std::complex<double>( uv[0], uv[1] );
		  }
		}
	}
};

template<typename M>
void _write_face_mu( M * pMesh )
{
	for( M::MeshFaceIterator fiter( pMesh ); !fiter.end(); fiter ++ )
	{
		M::CFace * pF = *fiter;
		CParser parser( pF->string() );
		parser._removeToken( "mu" );

		parser._toString( pF->string() );

		std::stringstream iss;

		iss << "mu=(" << pF->mu().real() << " " << pF->mu().imag() << ")";

		if( pF->string().size() > 0 )
		{
		  pF->string() += " ";
		}
		pF->string() += iss.str();
	}
};

}
#endif  _TRAITS_IO_H_