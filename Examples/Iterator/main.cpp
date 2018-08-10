#include <MeshFrame/core/Mesh/BaseMesh.h>
#include <MeshFrame/core/Mesh/Vertex.h>
#include <MeshFrame/core/Mesh/HalfEdge.h>
#include <MeshFrame/core/Mesh/Edge.h>
#include <MeshFrame/core/Mesh/Face.h>
#include <MeshFrame/core/Mesh/Types.h>
#include <MeshFrame/core/Mesh/Iterators2.h>

#include <ctime>
#include <stdio.h>

using namespace MeshLib;

using std::cout;
using std::endl;

typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> M;
typedef CIterators<M> It;

#define REPEAT_TIMES 1000

//std::recursive_mutex lock;

int main(int argc, char ** argv) {

	M mesh;
	//A sample from MeshLab
	clock_t time1 = clock();
	mesh.read_ply("./Laurana50k.ply");
	time1 = clock() - time1;
	printf("Time consumption in loading: %d\n", time1);
	getchar();

	//Usage of MVIterator, to iterate all vertices in mesh
	for (M::VPtr pV : It::MVIterator(&mesh))
	{
		printf("pV->id(): %d, pV->point(): %f %f %f\n", pV->id(), pV->point()[0], pV->point()[1], pV->point()[2]);

		//Usage of VFIterator, to iterate all faces around vertex
		for (M::FPtr pF : It::VFIterator(pV))
		{
			printf("\tpF->id: %d : ", pF->id());
			//Usage of FVIterator, to iterate all vertex around face
			for (M::VPtr pVF : It::FVIterator(pF))
			{
				printf("%d, ", pVF->id());
			}
			printf("\n");
		}
	}

	getchar();
}
	