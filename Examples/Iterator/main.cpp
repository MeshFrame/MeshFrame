#include <algorithm>
#include <iostream>
#include <MeshLib2/core/Mesh/BaseMesh.h>
#include <MeshLib2/core/Mesh/Vertex.h>
#include <MeshLib2/core/Mesh/HalfEdge.h>
#include <MeshLib2/core/Mesh/Edge.h>
#include <MeshLib2/core/Mesh/Face.h>
#include <MeshLib2/core/Mesh/Types.h>
#include <MeshLib2/core/Mesh/Iterators.h>
#include <ctime>
#include <stdio.h>
#define _CRTDBG_MAP_ALLOC   //并非绝对需要该语句，但如果有该语句，打印出来的是文件名和行数等更加直观的信息
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>

using namespace MeshLib;

using std::cout;
using std::endl;

typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> M;
typedef CIterators<M> It;

#define REPEAT_TIMES 1000

//std::recursive_mutex lock;

int main(int argc, char ** argv) {

	
	for (size_t i = 1; i < 5; i++)
	{
		printf("Load: %s\n", argv[i]);
		M m;
		CPoint p;
		m.read_obj(argv[i]);
		clock_t time1 = clock();
		for (size_t n = 0; n < REPEAT_TIMES; n++)
		{
#pragma omp parallel for num_threads(4)
			for (int i = 0; i < m.vertices().size(); ++i)
			{
				M::VPtr pV = m.vertices().getPointer(i);

				pV->point() += CPoint(0.00001, 0.00001, 0.00001);
				//for (M::FPtr pE : It::VFIterator(pV))
				//{
				//	pV->point() += CPoint(0.00001, 0.00001, 0.00001);
				//}
				p += pV->point();
			}
		}
		
		time1 = clock() - time1;

		printf("%f %f %f\n", p[0],p[1], p[2]);
		printf("Time consumption: \n%d\n", time1);
	}

	getchar();
}
	