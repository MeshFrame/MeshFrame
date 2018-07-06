#pragma once
#include "..\..\..\MeshLib\core\Mesh\MeshHeaders.h"
#include "..\..\..\MeshLib\core\Geometry\Point.h"
#include "..\..\..\MeshLib\core\Geometry\BaryCoordinates3D.h"
#include "Edge_Denoising.h"
#include "Vetex_Denoising.h"
namespace MeshLib
{
	class CVertex_Denoising;
	class CFace_Denoising;
	class CEdge_Denoising;
	typedef MeshDenoising<CVertex_Denoising, CEdge_Denoising, CFace_Denoising, CHalfEdge> CMeshDenoising;
	class CFace_Denoising : public CFace
	{
	public:
		CFace_Denoising() : m_normal(0, 0, 0), m_area(0, 0, 0) {
			for (CMeshDenoising::FaceVertexIterator FVIter(this); !FVIter.end(); ++FVIter) {
				CVertex_Denoising *pV = *FVIter;

			}
		};
		~CFace_Denoising() {};

		CPoint & normal() { return m_normal; };

	private:
		CPoint m_normal;
		CPoint m_area;
	};
	
}