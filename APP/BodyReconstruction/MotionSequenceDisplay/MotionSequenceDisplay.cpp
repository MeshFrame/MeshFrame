#include <MeshFrame/core/Mesh/MeshCoreHeaders.h>
#include <MeshFrame/core/viewer/MeshViewer.h>
#include <AC/AC.h>
#include <AC/Def.h>
#include <AC/IO_AC.h>
#include <ctime>

typedef MeshLib::CBaseMesh<MeshLib::CVertexUV, MeshLib::CEdge, MeshLib::CFace, MeshLib::CHalfEdge> M;
typedef MeshLib::CIterators<M> It;

MeshLib::CMeshViewer * pViewer;
AC::VecStr filesObj;
AC::VecStr filesBmp;
M * pMesh = NULL;
clock_t waitTimeStart;
MeshLib::CPoint centroid;

bool startPlay = true;
bool hasTexture = false;
bool pause = false;
bool singleStep = false;
bool showFrameId = false;


float scale;
int frameId = 0;

int frameRate = 25;
int minFrameRate = 1;
int maxFrameRate = 200;
int totalWaitTimeThres = 1000 / frameRate;

void getMeshScale(M * pM, MeshLib::CPoint & c, float & s) {
	MeshLib::CPoint center(0, 0, 0);
	MeshLib::CPoint min(0, 0, 0);
	MeshLib::CPoint max(0, 0, 0);

	for (auto pV : It::MVIterator((M::Ptr)pM)) {
		MeshLib::CPoint v = pV->point();
		center += v;
		for (int i = 0; i < 3; ++i) {
			min[i] = v[i] < min[i] ? v[i] : min[i];
			max[i] = v[i] > max[i] ? v[i] : max[i];
		}
	}
	center = center / pM->vertices().size();
	centroid = center;
	MeshLib::CPoint scl = max - min;
	s = (scl[0] + scl[1] + scl[2]) / 3;
	if (s == 0.0)
	{
		s = 1.0;
	}
}


void normalizeMesh(M * pM, MeshLib::CPoint c, float s) {
	for (auto pV : It::MVIterator(pM)) {
		pV->point() = (pV->point() - c) / s;
	}
}


void changeToNextFrame() {
	int size = filesObj.size();
	delete pMesh;
	pMesh = new M;
	pMesh->read_obj(filesObj[frameId].c_str(), false);
	normalizeMesh(pMesh, centroid, scale);
	pViewer->setMeshPointer(pMesh, true, false, true);
	if (hasTexture)
	{
		pViewer->setTexture(filesBmp[frameId].c_str());
	}
	if (showFrameId)
	{
		printf("Frame: %d Name: %s\n", frameId, filesObj[frameId].c_str());
	}
	++frameId;
	if (frameId >= size)
	{
		frameId = 0;
	}
}

void changeToLastFrame() {
	int size = filesObj.size();
	delete pMesh;
	pMesh = new M;
	pMesh->read_obj(filesObj[frameId].c_str(), false);
	normalizeMesh(pMesh, centroid, scale);
	pViewer->setMeshPointer(pMesh, true, false, true);
	if (hasTexture)
	{
		pViewer->setTexture(filesBmp[frameId].c_str());
	}
	if (showFrameId)
	{
		printf("Frame: %d Name: %s\n", frameId, filesObj[frameId].c_str());
	}
	--frameId;
	if (frameId < 0)
	{
		frameId = size - 1;
	}
}


void sequenceDisplayKeyFunction(unsigned char key) {
	switch (key)
	{
	case 'P':
	case 'p':
		pause = !pause;
		singleStep = !singleStep;
		if (singleStep)
		{
			showFrameId = true;
		}
		else {
			showFrameId = false;
		}
		break;
	case 'N':
	case 'n':
		if (singleStep)
		{
			changeToNextFrame();
		}
		break;
	case 'B':
	case 'b':
		if (singleStep)
		{
			changeToLastFrame();
		}
		break;
	case '+':
	case '=':
		if (frameRate < maxFrameRate)
		{
			++frameRate;
			totalWaitTimeThres = 1000 / frameRate;
			printf("Change frame rate to %d FPS.\n", frameRate);
		}
		break;
	case '-':
	case '_':
		if (frameRate > minFrameRate)
		{
			--frameRate;
			totalWaitTimeThres = 1000 / frameRate;
			printf("Change frame rate to %d FPS.\n", frameRate);
		}
	default:
		break;
	}
}


void switchFaceIdleFunc() {
	int totalWaitTime = ((float)(clock() - waitTimeStart) * 1000) / CLOCKS_PER_SEC;
	if (pause || totalWaitTime < totalWaitTimeThres)
	{
		return;
	}
	else {
		changeToNextFrame();

		
		waitTimeStart = clock();
	}
}


int main(int argc, char ** argv) {
	if (argc < 2) {
		printf("Need 1 input file: mesh.\n");
		return -1;
	}

	AC::IO::getFilesWithExt(argv[1], "obj", filesObj);
	if (filesObj.size() == 0)
	{
		AC::IO::getFilesWithExt(argv[1], "txt", filesObj);
	}
	AC::IO::getFilesWithExt(argv[1], "bmp", filesBmp);

	M m;
	m.read_obj(filesObj.front().c_str(), false);
	waitTimeStart = clock();
	getMeshScale(&m, centroid, scale);
	normalizeMesh(&m, centroid, scale);

	MeshLib::CMeshViewer viewer;
	pViewer = &viewer;
	viewer.setting().vertexColorMode = MeshLib::GLSetting::defaultColor;
	viewer.setting().vertexSize = 4;
	viewer.setMeshPointer(&m, true, false, true);
	if (hasTexture)
	{
		viewer.setTexture(filesBmp.front().c_str());
	}
	viewer.setUserIdleFunc(switchFaceIdleFunc);
	viewer.setUserKeyFunc(sequenceDisplayKeyFunction);
	viewer.show();
	delete pMesh;
}