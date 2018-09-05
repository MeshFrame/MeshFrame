#include <MeshFrame/core/Memory/MemoryPool.h>
#include <MeshFrame/core/Memory/MPIterator.h>
#include <MeshFrame/core/viewer/MeshViewer.h>
#include <MeshFrame/core/Mesh/MeshCoreHeaders.h>
#include <string>
#include <vector>
#include <stdio.h>

extern int a;

int a = 0;

#include <ctime>
#include <random>
#define NUM_MEMBERS 2000
#define NUM_TO_D
#define NUM_TO_DELETE 200
#define NUM_REPEAT 10000000
using namespace MeshLib;

std::default_random_engine generator(time(NULL));
std::uniform_int_distribution<int> randInt(0, NUM_TO_DELETE);

class DemoClass {
public:
	DemoClass() :
		str("Demo.")
	{
		i = 100;
	}
	std::string getStr() { return str; };
private:
	int i;
	char str[100];
};

CMeshViewer * pMeshViewer;

void myKeyFunc(unsigned char key) {
	switch (key)
	{
	case 'C':
		if (pMeshViewer->setting().faceColorMode == GLSetting::defaultColor) {
			pMeshViewer->setting().faceColorMode = GLSetting::userDefined;
		}
		else
		{
			pMeshViewer->setting().faceColorMode = GLSetting::defaultColor;
		}
		break;
	case 'E':
		if (pMeshViewer->setting().edgeColorMode == GLSetting::defaultColor) {
			pMeshViewer->setting().edgeColorMode = GLSetting::none;
		}
		else {
			pMeshViewer->setting().edgeColorMode = GLSetting::defaultColor;
		}
		break;
	case 'V':
		if (pMeshViewer->setting().vertexColorMode == GLSetting::defaultColor) {
			pMeshViewer->setting().vertexColorMode = GLSetting::none;
		}
		else {
			pMeshViewer->setting().vertexColorMode = GLSetting::defaultColor;
		}
		break;
	default:
		break;
	}

}

typedef MemoryPool<DemoClass> CClassContainer;
typedef CBaseMesh<CVertexNUVRGB, CEdge, CFace, CHalfEdge> M;

int main(int argc, char ** argv) {

	//if (argc != 2) {
	//	printf("Need Input file.\n");
	//	return -1;
	//}
	//CVertexNUVRGB v;
	//M::VPtr pV = &v;

	M m;
	clock_t time1 = clock();
	m.read_obj(argv[1]);
	time1 = clock() - time1;
	printf("Time consumption in reading mesh: %d\n", time1);

	getchar();

	CMeshViewer viewer;
	//viewer.setting().
	pMeshViewer = &viewer;
	viewer.setMeshPointer(&m, true, true);
	viewer.setUserKeyFunc(myKeyFunc);
	viewer.setTexture(argv[2]);
	printf("Show mesh.\n");
	viewer.show();
	m.write_ply("mesh.ply");
	getchar();
}
