#pragma once
#include "..\..\core\Mesh\Vertex.h"
#include "..\..\core\Mesh\Edge.h"
#include "..\..\core\Mesh\Face.h"
#include "..\..\core\Mesh\HalfEdge.h"
#include "..\..\core\Mesh\BaseMesh.h"
#include "..\..\core\Mesh\boundary.h"
#include "..\..\core\Mesh\iterators.h"

#include <random>

namespace MeshLib
{
	template<typename V, typename E, typename F, typename H>
	class MeshDenoising : public CBaseMesh<V, E, F, H>
	{
	public:
		typedef CBoundary<V, E, F, H>					CBoundary;
		typedef CLoop<V, E, F, H>						CLoop;

		typedef MeshVertexIterator<V, E, F, H>			MeshVertexIterator;
		typedef MeshEdgeIterator<V, E, F, H>			MeshEdgeIterator;
		typedef MeshFaceIterator<V, E, F, H>			MeshFaceIterator;
		typedef MeshHalfEdgeIterator<V, E, F, H>		MeshHalfEdgeIterator;

		typedef VertexVertexIterator<V, E, F, H>		VertexVertexIterator;
		typedef VertexEdgeIterator<V, E, F, H>			VertexEdgeIterator;
		typedef VertexFaceIterator<V, E, F, H>			VertexFaceIterator;
		typedef VertexInHalfedgeIterator<V, E, F, H>	VertexInHalfedgeIterator;
		typedef VertexOutHalfedgeIterator<V, E, F, H>	VertexOutHalfedgeIterator;

		typedef FaceVertexIterator<V, E, F, H>			FaceVertexIterator;
		typedef FaceEdgeIterator<V, E, F, H>			FaceEdgeIterator;
		typedef FaceHalfedgeIterator<V, E, F, H>		FaceHalfedgeIterator;

		enum NoiseType {
			Guassian,
			Uniform
		};
		enum NoiseScale {
			Global,
			Density
		};

		void computeNormals();
		void addNoise(NoiseType type, NoiseScale scale, double level);
		double getAverageEdgeLength();
		void normalizeMesh();
		void addNoiseUniform(double noiseLevel);
		void addNoiseGuassian(double noiseLevel);
		void elasticDescent(int iterationNum, double step);
	};
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::computeNormals()
	{
		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * v = *viter;
			CPoint n(0, 0, 0);
			for (VertexFaceIterator vfiter(v); !vfiter.end(); ++vfiter)
			{
				CFace_Denoising * pF = *vfiter;

				CPoint p[3];
				CHalfEdge * he = pF->halfedge();
				for (int k = 0; k < 3; k++)
				{
					p[k] = he->target()->point();
					he = he->he_next();
				}

				CPoint fn = (p[1] - p[0]) ^ (p[2] - p[0]);
				pF->normal() = fn / fn.norm();
				n += fn;
			}

			n = n / n.norm();
			v->normal() = n;
		}
	}
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::addNoise(NoiseType type, NoiseScale scale, double level)
	{
		double noiseLevel;
		switch (scale)
		{
		case Global:
			normalizeMesh();
			noiseLevel = level;
			break;
		case Density:
			noiseLevel = level * getAverageEdgeLength();
			break;
		default:
			noiseLevel = 0;
			break;
		}
		switch (type)
		{
		case Guassian:
			addNoiseGuassian(noiseLevel);
			break;
		case Uniform:
			addNoiseUniform(noiseLevel);
			break;
		default:
			break;
		}
	}
	template<typename V, typename E, typename F, typename H>
	double MeshDenoising<V, E, F, H>::getAverageEdgeLength()
	{
		double averageLength = 0.0;
		CPoint edgeVec;
		int n = 0;
		for (MeshEdgeIterator MEIter(this); !MEIter.end(); ++MEIter) {
			++n;
			E *pE = *MEIter;
			edgeVec = pE->halfedge(0)->target()->point() - pE->halfedge(0)->source()->point();
			averageLength += edgeVec.norm();
		}
		return averageLength/n;
	}
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::normalizeMesh()
	{
		CPoint s(0, 0, 0);
		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * v = *viter;
			s = s + v->point();
		}
		s = s / this->numVertices();

		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * v = *viter;
			CPoint p = v->point();
			p = p - s;
			v->point() = p;
		}

		double d = 0;
		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * v = *viter;
			CPoint p = v->point();
			for (int k = 0; k < 3; k++)
			{
				d = (d > fabs(p[k])) ? d : fabs(p[k]);
			}
		}

		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * v = *viter;
			CPoint p = v->point();
			p = p / d;
			v->point() = p;
		}
	}
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::addNoiseUniform(double noiseLevel)
	{
		std::default_random_engine generator(time(NULL));
		std::uniform_real_distribution<double> uniformNoise(-noiseLevel, noiseLevel);

		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * pV = *viter;
			CPoint normal = pV->normal();
			pV->point() += pV->point() + uniformNoise(generator)*normal;
		}
	}
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::addNoiseGuassian(double noiseLevel)
	{
		std::default_random_engine generator(time(NULL));
		std::normal_distribution<double> guassianNoise(0, noiseLevel);

		for (MeshVertexIterator viter(this); !viter.end(); ++viter)
		{
			V * pV = *viter;
			CPoint normal = pV->normal();
			pV->point() += pV->point() + guassianNoise(generator)*normal;
		}
	}
	template<typename V, typename E, typename F, typename H>
	void MeshDenoising<V, E, F, H>::elasticDescent(int iterationNum, double step)
	{
		for (int i = 0; i < iterationNum; ++i) {
			for (MeshVertexIterator MVIter(this); !MVIter.end(); ++MVIter) {
				V *pV = *MVIter;
				pV->gradient() = pV->elasticDirection(this);
			}
			for (MeshVertexIterator MVIter(this); !MVIter.end(); ++MVIter) {
				V *pV = *MVIter;
				pV->gradientDescent(step);
			}
		}
	}
}