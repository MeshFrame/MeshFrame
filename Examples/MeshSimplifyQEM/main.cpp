#define  _CRT_SECURE_NO_WARNINGS
#include "mesh.h"

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
int main(int argc, char** argv)
{
    clock_t timeReadm = clock();
    DynamicMeshM mesh;
	mesh.read_obj(argv[1]);


	CMeshViewer viewer1;
	pMeshViewer = &viewer1;
	viewer1.setMeshPointer(&mesh, true, true);
	viewer1.setUserKeyFunc(myKeyFunc);
	printf("Show mesh.\n");
	viewer1.show();

    mesh.simplify(atoi(argv[3]));

    timeReadm = clock() - timeReadm;
    std::cout << "time consumption in readm is" << timeReadm*1.0 / (CLOCKS_PER_SEC) << "\n";

    mesh.write_obj(argv[2]);

    CMeshViewer viewer;
    pMeshViewer = &viewer;
    viewer.setMeshPointer(&mesh, true, true);
    viewer.setUserKeyFunc(myKeyFunc);
    printf("Show mesh.\n");
    viewer.show();

    system("pause");
    return 0;
}