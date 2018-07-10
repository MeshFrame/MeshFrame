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

#define REPEAT_TIMES 10000
#define _CRTDBG_MAP_ALLOC   //并非绝对需要该语句，但如果有该语句，打印出来的是文件名和行数等更加直观的信息
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define NUM_PROPS 1000

using namespace MeshLib;

using std::cout;
using std::endl;

class MyVertex : public CVertex
{
public:
	//int a[NUM_PROPS];
	int a;

protected:
};

typedef CBaseMesh<MyVertex, CEdge, CFaceRGB, CHalfEdge> M;
typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> BasicM;
typedef CIterators<M> It;

//std::recursive_mutex lock;

int main(int argc, char ** argv) {
	if (argc != 2) {
		printf("Please give a input .m file.\n");
		return 0;
	}

	//omp_set_num_threads(4);
	
	M  m;  
	m.read_ply(argv[1]);

	printf("Size of delete maks: %d\n", m.vertices().getDeleteMask().size());

	VPropHandle<CPoint> vNormalHandle;
	VPropHandle<int> vIdxHandle;
	FPropHandle<CPoint> fNormalHandle;
	m.addVProp(vIdxHandle);
	m.addVProp(vNormalHandle);
	m.addFProp(fNormalHandle);


	VPropHandle<int> vIntHandles[NUM_PROPS];
	for (size_t i = 0; i < NUM_PROPS; i++)
	{
		m.addVProp(vIntHandles[i]);
	}
	int k = 0;

	//m.removeVProp(vNameHandle);
	m.removeFProp(fNormalHandle);
	m.removeVProp(vNormalHandle);
	m.removeVProp(vIdxHandle);

	clock_t time2 = clock();
	for (size_t i = 0; i < REPEAT_TIMES; i++)
	{
		for (M::VPtr pV : m.vertices()) {
			int & idx = pV->a;
			idx = k++;
		}
	}
	time2 = clock() - time2;

	clock_t time1 = clock();
	for (size_t i = 0; i < REPEAT_TIMES; i++)
	{
		for (M::VPtr pV : m.vertices()) {
			int & idx = m.getVProp(vIntHandles[NUM_PROPS -1], pV);
			idx = k++;
		}
	}
	time1 = clock() - time1;
	printf("Time used in acessing Dynamic prop: %d\n", time1);

	k = 0;

	printf("Time used in acessing member prop: %d\n", time2);
	//
	//getchar();
	//
	//for (M::FPtr pF : It::MFIterator(&m))
	//{
	//	m.gFP(fNormalHandle, pF) = CPoint(1,2,3);
	//}
	////deleted Prop
	//m.removeVProp(vNormalHandle);
	//
	////prop is avaliable even after cast of m's type
	//BasicM::Ptr pM = (BasicM::Ptr)&m;
	//for (BasicM::VPtr pV : pM->vertices()) {
	//	//CPoint& p = m.getVProp(vNormalHandle, pV);
	//	//p = M::vertexNormal(pV);
	//
	//	std::string & str = pM->gVP(vNameHandle, pV);
	//	std::cout << str << " - " << pM->getVProp(vIdxHandle, pV) << "\n";
	//	//::cout << p << "\n";
	//}
	//m.write_m("out.m");
	//delete pM;


	
	getchar();
}
	