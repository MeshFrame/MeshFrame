#include "MeshViewer.h"

#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include <MeshFrame\core\bmp\RgbImage.h>
#include <MeshFrame\core\Mesh\MeshCoreHeaders.h>

#include <MeshFrame\core\viewer\Arcball.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

#define MESHLIB_VIEWER_EYE_X 0
#define MESHLIB_VIEWER_EYE_Y 0
#define MESHLIB_VIEWER_EYE_Z 3.5

#undef min
#undef max

using namespace::MeshLib;

typedef CBaseMesh<CVertex, CEdge, CFace, CHalfEdge> CMeshGL;
typedef CIterators<CMeshGL> ITGL;
CMeshGL::Ptr pMesh;

VPropHandle<CPointF> vColorHdl, vNormalHdl;
VPropHandle<CPoint2> vUVHdl;
FPropHandle<CPointF> fColorHdl, fNormalHdl;
EPropHandle<CPointF> eColorHdl;

/* window width and height */
int win_width, win_height;
int gButton;
int startx, starty;
GLfloat  faceDefaultColor[3] = { 0.7, 0.7, 0.8 };
GLfloat  edgeDefaultColor[3] = { 0.5, 0.5, 0.1 };
GLfloat  vertexDefaultColor[3] = { 0.8, 0.0, 0.0 };
GLSetting glSetting;
bool selectionMode = false;
GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

//This is the function pointer to the user defined key responding function.
//It will be called after a key was pressed.
//It can be changed by user, by calling CSampleMeshViewer::setUserKeyFunc
void(*custom_user_key_func)(unsigned char key);
void(*custom_idle_func)(void);

/* rotation quaternion and translation vector for the object */
CQrot       ObjRot(0, 0, 1, 0);
CPoint      ObjTrans(0, 0, 0);
//This is a global variant, stores the pointer to mesh to be visualized.
/* texture id and image */
int textureFlag = 2;
GLuint texName = 0;
RgbImage image;
/* arcball object */
CArcball arcball;

/*! setup the object, transform from the world to the object coordinate system */
void setupObject(void) {
	double rot[16];

	glTranslated(ObjTrans[0], ObjTrans[1], ObjTrans[2]);
	ObjRot.convert(rot);
	glMultMatrixd((GLdouble *)rot);
}

/*! the eye is always fixed at world z = +5 */
void setupEye(void) {
	glLoadIdentity();
	gluLookAt(MESHLIB_VIEWER_EYE_X, MESHLIB_VIEWER_EYE_Y, MESHLIB_VIEWER_EYE_Z, 0, 0, 0, 0, 1, 0);
}

/*! setup light */
void setupLight() {
	GLfloat lightOnePosition[4] = { 0, 0, 1, 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}

/*! draw axis */
void draw_axis() {
	glLineWidth(2.0);
	//x axis
	glColor3f(1.0, 0.0, 0.0);	//red
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glEnd();

	//y axis
	glColor3f(0.0, 1.0, 0);		//green
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glEnd();

	//z axis
	glColor3f(0.0, 0.0, 1.0);	//blue
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();

	glLineWidth(1.0);
}
void draw_faces() {
	glBegin(GL_TRIANGLES);
	for (auto pF : ITGL::MFIterator(pMesh)) {
		for (auto pV : ITGL::FVIterator(pF)) {
			CPoint pt = pV->point();
			CPointF n;
			switch (glSetting.shadeFlag)
			{
			case GLSetting::_face:
				n = pMesh->gFP(fNormalHdl, pF);
				break;
			case GLSetting::_vertex:
				n = pMesh->gVP(vNormalHdl, pV);
				break;
			}
			glNormal3d(n[0], n[1], n[2]);
			CPointF color;
			switch (glSetting.faceColorMode)
			{
			case GLSetting::ColorMode::defaultColor:
				glColor3fv(faceDefaultColor);
				break;
			case GLSetting::ColorMode::userDefined:
				switch (glSetting.shadeFlag)
				{
				case GLSetting::_face:
					color = pMesh->gFP(fColorHdl, pF);
					glColor3f(color[0], color[1], color[2]);
					break;				 
				case GLSetting::_vertex:
					color = pMesh->gVP(vColorHdl, pV);
					glColor3f(color[0], color[1], color[2]);
					break;
				}
				break;
			default:
				break;
			}
			if (glSetting.showUV) {
				CPoint2 uv = pMesh->gVP(vUVHdl, pV);
				glTexCoord2d(uv[0], uv[1]);
			}
			glVertex3d(pt[0], pt[1], pt[2]);
		}
	}
	glEnd();
}

void draw_edges() {
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(glSetting.edgeSize);
	//glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	for (auto pE : ITGL::MEIterator(pMesh)) {
		glBegin(GL_LINES);
		switch (glSetting.edgeColorMode) {
		case GLSetting::ColorMode::defaultColor:
			glColor3fv(edgeDefaultColor);
			break;
		case GLSetting::ColorMode::userDefined:
			glColor3f(pE->color()[0], pE->color()[1], pE->color()[2]);
			break;
		default:
			break;
		}
		CPoint v1 = CMeshGL::edgeVertex1(pE)->point();
		CPoint v2 = CMeshGL::edgeVertex2(pE)->point();
		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void draw_vertices() {
	glDisable(GL_LIGHTING);
	for (auto pV : ITGL::MVIterator(pMesh)) {
		glPointSize(glSetting.vertexSize);
		glBegin(GL_POINTS);
		switch (glSetting.vertexColorMode) {
		case GLSetting::ColorMode::defaultColor:
			glColor3fv(vertexDefaultColor);
			break;
		case GLSetting::ColorMode::userDefined:
			glColor3f(pV->color()[0], pV->color()[1], pV->color()[2]);
			break;
		default:
			break;
		}
		glVertex3f(pV->point()[0], pV->point()[1], pV->point()[2]);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}


/*! display call back function
*/
void display() {
	/* clear frame buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLight();
	/* transform from the eye coordinate system to the world system */
	setupEye();
	glPushMatrix();
	/* transform from the world to the ojbect coordinate system */
	setupObject();


	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	draw_axis();
	if (glSetting.faceColorMode != GLSetting::none)
		draw_faces();
	if (glSetting.edgeColorMode != GLSetting::none)
		draw_edges();
	if (glSetting.vertexColorMode != GLSetting::none)
		draw_vertices();

	glPopMatrix();
	glutSwapBuffers();
}

/*! Called when a "resize" event is received by the window. */
void reshape(int w, int h) {
	float ar;
	//std::cout << "w:" << w << "\th:" << h << std::endl;
	win_width = w;
	win_height = h;

	ar = (float)(w) / h;
	glViewport(0, 0, w, h);               /* Set Viewport */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// magic imageing commands
	gluPerspective(40.0, /* field of view in degrees */
		ar, /* aspect ratio */
		1.0, /* Z near */
		100.0 /* Z far */);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

/*! helper function to remind the user about commands, hot keys */
void help()
{
	printf("Upper case characters for controling.");
	printf("W  -  Wireframe Display\n");
	printf("F  -  Flat Shading \n");
	printf("S  -  Smooth Shading\n");
	printf("?  -  Help Information\n");
	printf("esc - quit\n");
	printf("We suggest you to define your own controlling keys with lower case charactors.");
}
void default_user_key_func(unsigned char key) {
	return;
}
/*! Keyboard call back function */
void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'F':
		//Flat Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		glSetting.shadeFlag = GLSetting::_face;
		break;
	case 'S':
		//Smooth Shading
		glPolygonMode(GL_FRONT, GL_FILL);
		glSetting.shadeFlag = GLSetting::_vertex;
		break;
	case 'W':
		//Wireframe mode
		glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case '?':
		help();
		break;
	case 'T':
		textureFlag = (textureFlag + 1) % 3;
		switch (textureFlag)
		{
		case 0:
			glDisable(GL_TEXTURE_2D);
			break;
		case 1:
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			break;
		case 2:
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			break;
		}
		break;
	default:
		custom_user_key_func(key);
		break;
	}
	glutPostRedisplay();
}
/*! setup GL states */
void setupGLstate() {
	GLfloat lightOneColor[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat globalAmb[] = { .1, .1, .1, 1 };
	GLfloat lightOnePosition[] = { .0, 0.0, 1.0, 1.0 };

	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.35, 0.53, 0.70, 0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightOneColor);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightOneColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT1, GL_POSITION, lightOnePosition);
}
/*! initialize bitmap image texture */
void initialize_bmp_texture()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//	int ImageWidth  = image.GetNumRows();
	//	int ImageHeight = image.GetNumCols();
	int ImageWidth = image.GetNumCols();
	int ImageHeight = image.GetNumRows();
	GLubyte *pr = (GLubyte *)image.ImageData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0, GL_RGB,
		GL_UNSIGNED_BYTE, pr);

	if (textureFlag == 1)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	else if (textureFlag == 2)
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

void projectRay(int x_cursor, int y_cursor, CPoint &org, CPoint &drt) {

	GLfloat winX, winY;

	// obtain the Z position (not world coordinates but in range 0 - 1)
	GLfloat z_cursor;
	GLdouble posX, posY, posZ;
	winX = (float)x_cursor;
	winY = (float)viewport[3] - (float)y_cursor;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);

	// obtain the world coordinates
	bool bResult = gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &posX, &posY, &posZ);
	drt[0] = posX; drt[1] = posY; drt[2] = posZ;
	bResult = gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &posX, &posY, &posZ);
	org[0] = posX; org[1] = posY; org[2] = posZ;

	drt = drt - org;
}

// Determine whether a ray intersect with a triangle
// Parameters
// orig: origin of the ray
// dir: direction of the ray
// v0, v1, v2: vertices of triangle
// t(out): weight of the intersection for the ray
// u(out), v(out): barycentric coordinate of intersection

bool intersectTriangle(const CPoint& orig, const CPoint& dir,
	CPoint& v0, CPoint& v1, CPoint& v2,
	float* t, float* u, float* v)
{
	// E1
	CPoint E1 = v1 - v0;

	// E2
	CPoint E2 = v2 - v0;

	// P
	CPoint P = dir ^ E2;

	// determinant
	float det = E1 * P;

	// keep det > 0, modify T accordingly
	CPoint T;
	if (det >0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0001f)
		return false;

	// Calculate u and make sure u <= 1
	*u = T * P;
	if (*u < 0.0f || *u > det)
		return false;

	// Q
	CPoint Q = T ^ E1;

	// Calculate v and make sure u + v <= 1
	*v = dir * Q;
	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = E2 * Q;

	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}
/*! mouse click call back function */
void  mouseClick(int button, int state, int x, int y) {
	/* set up an arcball around the Eye's center
	switch y coordinates to right handed system  */

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && selectionMode) {
		CPoint drt, org;
		// build ray
		projectRay(x, y, drt, org);


		int viewport[4];
		// get matrixs and viewport:


		//for (CMeshGL::FPtr pF : pMesh->faces())
		//{
		//	float insertX, insertY, insertZ;
		//
		//	CMeshGL::VPtr pVs[3];
		//	int i = 0;
		//	for (CMeshGL::VPtr pV : ITGL::FVIterator(pF))
		//	{
		//		pVs[i] = pV;
		//		++i;
		//	}
		//	if (intersectTriangle(org, drt, pVs[0]->point(), pVs[1]->point(), pVs[2]->point(), &insertX, &insertY, &insertZ)) {
		//		pF->color().r = 255;
		//		pF->color().g = 0;
		//		pF->color().b = 0;
		//
		//		printf("Insert with face: %d\n", pF->id());
		//	}
		//}


		selectionMode = false;
	}
	else
	{
		gButton = GLUT_LEFT_BUTTON;
		arcball = CArcball(win_width, win_height, x - win_width / 2, win_height - y - win_height / 2);
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_MIDDLE_BUTTON;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		startx = x;
		starty = y;
		gButton = GLUT_RIGHT_BUTTON;
	}
	return;
}

/*! mouse motion call back function */
void mouseMove(int x, int y)
{
	CPoint trans;
	CQrot       rot;

	/* rotation, call arcball */
	if (gButton == GLUT_LEFT_BUTTON)
	{
		rot = arcball.update(x - win_width / 2, win_height - y - win_height / 2);
		ObjRot = rot * ObjRot;
		glutPostRedisplay();
	}

	/*xy translation */
	if (gButton == GLUT_MIDDLE_BUTTON)
	{
		double scale = 10. / win_height;
		trans = CPoint(scale*(x - startx), scale*(starty - y), 0);
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

	/* zoom in and out */
	if (gButton == GLUT_RIGHT_BUTTON) {
		double scale = 10. / win_height;
		trans = CPoint(0, 0, scale*(starty - y));
		startx = x;
		starty = y;
		ObjTrans = ObjTrans + trans;
		glutPostRedisplay();
	}

}

void _Idle_func() {
	custom_idle_func();
	display();
}
void default_idle_func() {};
void init_openGL()
{
	/* glut stuff */
	int argcG = 1;
	char* s = "none";
	char** argvG = &s;

	glutInit(&argcG, argvG);                /* Initialize GLUT */
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mesh Viewer");	  /* Create window with given title */
	glViewport(0, 0, 800, 600);

	glutDisplayFunc(display);             /* Set-up callback functions */
	glutIdleFunc(_Idle_func);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyBoard);

	setupGLstate();
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	initialize_bmp_texture();
	glutMainLoop();                       /* Start GLUT event-processing loop */
}

void loopEvent()
{
	glutMainLoopEvent();
}

bool windowClose()
{
	return !glutGetWindow();
}


MeshLib::CMeshViewer::CMeshViewer()
{
	m_glSetting = &glSetting;
	custom_user_key_func = default_user_key_func;
	custom_idle_func = default_idle_func;
}

MeshLib::CMeshViewer::CMeshViewer(void * pNewM, bool toComputeN, bool toNormalize, bool copyFields) {
	setMeshPointer(pNewM, toComputeN, toNormalize, copyFields);
}

void MeshLib::CMeshViewer::setMeshPointer(void * pNewM, bool toComputeN, bool toNormalize, bool copyFields)
{
	m_pM = pNewM;
	pMesh = (CMeshGL::Ptr)pNewM;
	//VPropHandle<CPointF>
	//VPropHandle<CPoint2>
	//FPropHandle<CPointF>
	//EPropHandle<CPointF>

	std::swap(vColorHdl, VPropHandle<CPointF>());;
	std::swap(vNormalHdl, VPropHandle<CPointF>());;
	std::swap(vUVHdl, VPropHandle<CPoint2>());;

	std::swap(fColorHdl, FPropHandle<CPointF>());;
	std::swap(fNormalHdl, FPropHandle<CPointF>());;

	std::swap(eColorHdl, EPropHandle<CPointF>());;

	pMesh->addVProp(vColorHdl);
	pMesh->addVProp(vNormalHdl);
	pMesh->addVProp(vUVHdl);

	pMesh->addFProp(fColorHdl);
	pMesh->addFProp(fNormalHdl);

	pMesh->addEProp(eColorHdl);

	if (toComputeN || !pMesh->faces().front().hasNormal()) {
		computeFNormal();
	}
	else
	{
		copyFNormal();
	}

	if (toComputeN || !pMesh->vertices().front().hasNormal()) {
		computeVNormal();
	}
	else
	{
		copyVNormal();
	}

	if (copyFields && pMesh->vertices().front().hasUV()) {
		copyVUV();
	}

	if (copyFields && pMesh->vertices().front().hasColor()) {
		copyVColor();
	}

	if (copyFields && pMesh->faces().front().hasColor()) {
		copyFColor();
	}

	//if (copyFields && pMesh->edges().front().hasColor()) {
	//	copyEColor();
	//}
	

	if (toNormalize)
		normalizeMesh();
}

void MeshLib::CMeshViewer::computeFNormal()
{
	for (auto pF : ITGL::MFIterator(pMesh))
	{
		CPointF & fNProp = pMesh->gFP(fNormalHdl, pF);
		CPoint fN = CMeshGL::faceNormal(pF);
		fNProp[0] = (float)fN[0];
		fNProp[1] = (float)fN[1];
		fNProp[2] = (float)fN[2];
	}
}

void MeshLib::CMeshViewer::computeVNormal()
{
	for (auto pV : ITGL::MVIterator(pMesh))
	{
		CPointF vN(0, 0, 0);
		for (auto pF : ITGL::VFIterator(pV)) {
			vN += pMesh->gFP(fNormalHdl, pF) * CMeshGL::faceArea(pF);
		}
		pMesh->gVP(vNormalHdl, pV) = vN / vN.norm();
	}
}

void MeshLib::CMeshViewer::normalizeMesh()
{
	CPoint center(0, 0, 0);
	CPoint min(0, 0, 0);
	CPoint max(0, 0, 0);
	for (auto pV : ITGL::MVIterator((CMeshGL::Ptr)m_pM)) {
		CPoint v = pV->point();
		center += v;
		for (int i = 0; i < 3; ++i) {
			min[i] = v[i] < min[i] ? v[i] : min[i];
			max[i] = v[i] > max[i] ? v[i] : max[i];
		}
	}
	center = center / ((CMeshGL::Ptr)m_pM)->vertices().size();
	CPoint scale = max - min;
	double l = (scale[0] + scale[1] + scale[2]) / 3;
	if (l == 0.0)
	{
		l = 1.0;
	}
	for (auto pV : ITGL::MVIterator((CMeshGL::Ptr)m_pM)) {
		pV->point() = (pV->point() - center) / l;
	}
}

void MeshLib::CMeshViewer::copyFNormal()
{
	for (auto pF : ITGL::MFIterator(pMesh))
	{
		CPointF & fNProp = pMesh->gFP(fNormalHdl, pF);
		CPoint fN = CMeshGL::faceNormal(pF);
		fNProp[0] = (float)fN[0];
		fNProp[1] = (float)fN[1];
		fNProp[2] = (float)fN[2];
	}
}

void MeshLib::CMeshViewer::copyVNormal()
{
	for (auto pV : ITGL::MVIterator(pMesh))
	{
		CPoint & vN = pV->normal();
		CPointF & vNProp = pMesh->gVP(vNormalHdl, pV);
		vNProp[0] = (float)vN[0];
		vNProp[1] = (float)vN[1];
		vNProp[2] = (float)vN[2];
	}
}

void MeshLib::CMeshViewer::copyFColor()
{

}

void MeshLib::CMeshViewer::copyVColor()
{
	for (auto pV : ITGL::MVIterator(pMesh)) {
		CPointF & vColorProp = pMesh->gVP(vColorHdl, pV);
		vColorProp[0] = pV->color().r;
		vColorProp[1] = pV->color().g;
		vColorProp[2] = pV->color().b;
	}
}

void MeshLib::CMeshViewer::copyEColor()
{
}

void MeshLib::CMeshViewer::copyVUV()
{
	for (auto pV : ITGL::MVIterator(pMesh))
	{
		CPoint2 & vUV = pV->uv();
		CPoint2 & vUVProp = pMesh->gVP(vUVHdl, pV);
		vUVProp[0] = (float)vUV[0];
		vUVProp[1] = (float)vUV[1];
	}
}

void MeshLib::CMeshViewer::setUserKeyFunc(void(*newUserFunc)(unsigned char key))
{
	custom_user_key_func = newUserFunc;
}
void MeshLib::CMeshViewer::setUserIdleFunc(void(*newIdleFunc)(void))
{
	custom_idle_func = newIdleFunc;
}

void MeshLib::CMeshViewer::show()
{
	init_openGL();
}
void MeshLib::CMeshViewer::setTexture(const char* texturePath)
{
	image.Reset();
	glDeleteTextures(1, &texName);
	image.LoadBmpFile(texturePath);
	glSetting.showUV = true;
	initialize_bmp_texture();
}
