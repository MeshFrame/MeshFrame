#pragma once
#include "..\..\..\MeshLib\core\Mesh\MeshHeaders.h"
#include "..\..\..\MeshLib\core\Geometry\Point.h"
#include "Denoising.h"
#include "Face_Denoising.h"
#include "Edge_Denoising.h"
namespace MeshLib
{
	class CVertex_Denoising;
	class CFace_Denoising;
	class CEdge_Denoising;
	typedef MeshDenoising<CVertex_Denoising, CEdge_Denoising, CFace_Denoising, CHalfEdge> CMeshDenoising;

	class CVertex_Denoising : public CVertex
	{
	public:
		CVertex_Denoising() {};
		~CVertex_Denoising() {};
		CPoint & normal() {return m_normal;};
		CPoint & gradient() { return m_gradient;};
		CPoint elasticDirection(CMeshDenoising *pMesh);
		void gradientDescent(double step);
	private:
		CPoint m_normal;
		CPoint m_gradient;
	};
	CPoint CVertex_Denoising::elasticDirection(CMeshDenoising *pMesh){
		CPoint direction(0, 0, 0);
		for (CMeshDenoising::VertexInHalfedgeIterator VIHeIter(pMesh, this); !VIHeIter.end(); ++VIHeIter )
		{
			CHalfEdge * pIHe= *VIHeIter;
			CPoint elasticEdge = pIHe->source()->point() - pIHe->target()->point();
			elasticEdge = elasticEdge / (elasticEdge.norm() * elasticEdge.norm());
			direction += elasticEdge;
		}
		direction /= direction.norm();
		return direction;
	}
	void CVertex_Denoising::gradientDescent(double step)
	{
		this->point() += step*gradient();
	}
}