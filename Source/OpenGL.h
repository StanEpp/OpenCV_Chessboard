#ifndef _MYOPENGL_
#define _MYOPENGL_

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <vector>
#include "boost/shared_ptr.hpp"


namespace myOpenGL{
	class Object;
/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////Draw////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/
	class Draw{
	private:
		boost::shared_ptr<std::vector<Object*> >	_Objects;
		
		float _xRot, _yRot;
		float _xTrans, _yTrans, _zTrans;

	public:
		Draw() : _xRot(0), _yRot(0), _xTrans(0), _yTrans(0), _zTrans(-15.0f), _Objects(new std::vector<Object*>) {}

		int drawWorld();
		void pushObject(Object* l_obj);
		void removeObject(Object* l_obj);
		void removeObject(int l_ID);
		void clear();

		void setXRot(float l_xRot);
		void setYRot(float l_yRot);

		void setXTrans(float l_xTrans);
		void setYTrans(float l_yTrans);
		void setZTrans(float l_zTrans);

		float getXRot() const;
		float getYRot() const;

		float getXTrans() const;
		float getYTrans() const;
		float getZTrans() const;

		int getNumberOfObjects() const;


	private:
		void _Rotate();
		void _Translate();
	};

/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////Object//////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/
	class Object{

		friend class Draw;

	private:

		struct _point3D {float x; float y; float z; struct RGB {float r; float g; float b;}rgb;};
		boost::shared_ptr<std::vector<_point3D>>	_Points;
		
		float _xRot, _yRot, _zRot;
		float _theta, _xAxis, _yAxis, _zAxis;
		float _xTrans, _yTrans, _zTrans;
		int   _ID;

		std::vector<Draw*> _drawReferences;

		GLenum	mode;

		///////////////////////////ID-Management////////////////////////////////
		/**/private:														////
		/**/static std::vector<int> _IDs;									////
		/**/static void				deleteID(int l_ID);						////
		/**/static int				getAvailableID();						////
		/**/public:															////
		/**/static std::vector<int>	getIDList();							////
		////////////////////////////////////////////////////////////////////////

	public:
		Object(GLenum mode) : _xRot(0.0f), _yRot(0.0f), _zRot(0.0f), _xTrans(0.0f), _yTrans(0.0f), _zTrans(0.0f),
							  _theta(0.0f), _xAxis(0.0f), _yAxis(0.0f), _zAxis(0.0f), _ID(Object::getAvailableID()),
							  mode(mode), _Points(new std::vector<_point3D>), _drawReferences(std::vector<Draw*>()){}

		~Object(){
			Object::deleteID(_ID);
			
			std::vector<Draw*>::iterator i = _drawReferences.begin();
			for(; i != _drawReferences.end(); ++i){
				(*i)->removeObject(_ID);
			}

		}


		void drawObject();
		void pushVertex(float* l_v);

		void setXRot(float l_xRot);
		void setYRot(float l_yRot);
		void setZRot(float l_zRot);
		void setRot(float l_theta, float l_xAxis, float l_yAxis, float l_zAxis);
		
		void setXTrans(float l_xTrans);
		void setYTrans(float l_yTrans);
		void setZTrans(float l_zTrans);

		float getXRot() const;
		float getYRot() const;
		float getZRot() const;

		float getXTrans() const;
		float getYTrans() const;
		float getZTrans() const;

		void setColor(float l_R, float l_G, float l_B);
		void insertNewVertex(float l_x, float l_y, float l_z);
		void insertNewVertex(float l_x, float l_y, float l_z, float l_r, float l_g, float l_b);
		void deleteAllVertex();
		int  getNumberOfVertex() const;

		bool operator==(Object l_obj);

		int getID() const;

	private:
		void _Rotate();
		void _Translate();
		void _setDrawReference(Draw* l_draw);
	};

/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////OpenGlWindow////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/

	class OpenGLWindow{
	private:

		static DWORD	s_tlsIndex;

		HDC			hDC;			// Private GDI Device Context
		HGLRC		hRC;			// Permanent Rendering Context
		HWND		hWnd;			// Holds Our Window Handle
		HINSTANCE	hInstance;		// Holds The Instance Of The Application
		GLsizei		width, height, bits;
		bool		keys[256];
		LPCWSTR		title; 
		Draw*		draw;

	public:

		OpenGLWindow(int width, int height, int Colordepth, LPCWSTR Windowtitle, Draw* draw) :
						hDC(NULL), hRC(NULL), hWnd(NULL), width(width), height(height), draw(draw),
						title(Windowtitle), bits(Colordepth){
							s_tlsIndex = ::TlsAlloc();
							for(int i = 0; i < 256; i++){
								keys[i] = false;
							}
						}

		int  mouseX, mouseXU, mouseY, mouseYU;
		int DrawGLScene();
		bool getKeyStatus(int Key);
		void setKeyStatus(int Key, bool Status);
		int getMouseX();
		int getMouseY();
		BOOL CreateGLWindow();
		GLvoid KillGLWindow();
		HDC  getHDC();
		HWND getHWND();
		HINSTANCE getHINSTANCE();
		HGLRC getHRC();

	private:
		GLvoid ReSizeGLScene();
		int InitGL();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	};

}

#endif

