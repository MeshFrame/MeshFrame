#ifndef _MESHVIEWER_H_
#define _MESHBIEWER_H_

#include <MeshLib\core\Mesh\Vertex.h>
#include <MeshLib\core\Geometry\PointF.h>
#include <vector>
#include <array>

//Buffers used to render

extern void init_openGL();
extern void loopEvent();
extern bool windowClose();

namespace MeshLib {

	/*
	* The struct containing the configuration of the viewer.
	*/
	struct GLSetting
	{
		//enum type to store coloring mode of geometric elements.
		//none        : means do not draw this element
		//defaultColor: means use default color to draw this element.
		//userDefined : means to user defined color, to use this, the coressponding geometric element's class must 
		//              be child class of CVertexVisual, CFaceVisual or CEdgeVisual
		enum ColorMode { none, defaultColor, userDefined };
		//Face's coloring mode
		ColorMode faceColorMode = defaultColor;
		//vertex's coloring mode
		ColorMode vertexColorMode = none;
		//edge's coloring mode
		ColorMode edgeColorMode = none;
		enum ShadeFlag { _vertex, _face };
		ShadeFlag shadeFlag = _face;

		bool  customFaceVisibility = false;
		bool  customEdgeVisibility = false;
		bool  customVertexVisibility = false;

		//size of veretex
		double vertexSize = 4.0;
		//size of edge
		double edgeSize = 2.0;
		//whether to show uv using texture map. Requiring giving texture file first.
		bool showUV = false;
	};


	/*
	* The main class of our viwer.
	* It manages the data used in GLViwer. User can use this class to communicate with OpenGL.
	* Easy to use, any mesh can be visualized.
	*/

	class CMeshViewer {
	public:
		/*
		* Construct function, can take one to three paremeters, two with default parementer.
		* \param pM           : pointer to your mesh, it can be pointer to any mesh .
		* \param toComputeN   : whether to compute normal for vertices and faces again
		* \param toNormalizer : wether to normalize your mesh
		*/
		CMeshViewer();
		CMeshViewer(void * pM, bool toComputeN = true, bool toNormalize = false, bool copyFields = true);
		void setMeshPointer(void * pM, bool toComputeN = true, bool toNormalize = false, bool copyFields = true);

		/*
		* You can set your key responding function own here.
		* \param newUserFunc: It is a pointer to a function, whose return type is void and its input parameter is
		*                     a unsigned char, which is the key captured by OpenGL. You can use this to do you own
		*                     key reaction. Note that 's', 'h', 'f', '?' has already been occupied.
		*/
		void setUserKeyFunc(void(*newUserFunc)(unsigned char key));
		void setUserIdleFunc(void(*newIdleFunc)(void));
		/*
		* Show mesh. This function will open a new window. After the window was closed, the function will return to
		* your code, where the show() function was called, without terminating whole function.
		*/
		void show();
		GLSetting& setting() { return *m_glSetting; };
		void setTexture(const char* texturePath);
	private:
		GLSetting * m_glSetting;
		void * m_pM;

		void computeFNormal();
		void computeVNormal();
		void normalizeMesh();
		void copyFNormal();
		void copyVNormal();
		void copyFColor();
		void copyVColor();
		void copyEColor();
		void copyVUV();
	};

}

#endif // !_MESHVIEWER_H_
