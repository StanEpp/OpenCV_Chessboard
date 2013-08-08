#include "OpenGL.h"

namespace myOpenGL{
/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////ID-Management/////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/

	std::vector<int> Object::_IDs;

	int Object::getAvailableID(){
		
		std::vector<int>::iterator i = Object::_IDs.begin();
		int counter = 0;

		for(; i != Object::_IDs.end(); ++i){
			if((*i) == counter){
				counter++;
			} else {
				Object::_IDs.insert(i, counter);
				return counter;
			}
		}

		Object::_IDs.push_back(counter);
		return counter;

	}

	void Object::deleteID(int l_ID){
		std::vector<int>::iterator i = Object::_IDs.begin();

		for(; i != Object::_IDs.end(); ++i){
			if((*i) == l_ID){
				Object::_IDs.erase(i);
				break;
			}
		}

	}

	std::vector<int> Object::getIDList(){
		return Object::_IDs;
	}

/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////Object//////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/


	void Object::drawObject(){

		glPushMatrix();

			_Translate();
			_Rotate();

			std::vector<_point3D>::iterator i = _Points->begin();
			glBegin(this->mode);
				for(;i != _Points->end(); ++i){
					glColor3f((*i).rgb.r, (*i).rgb.g, (*i).rgb.b);
					glVertex3f((*i).x , (*i).y, (*i).z);
				}
			glEnd();

		glPopMatrix();

	}

	void Object::setXRot(float l_xRot){
		_xRot = l_xRot;
	}

	void Object::setYRot(float l_yRot){
		_yRot = l_yRot;
	}

	void Object::setZRot(float l_zRot){
		_zRot = l_zRot;
	}

	void Object::setRot(float l_theta, float l_xAxis, float l_yAxis, float l_zAxis){
		_theta = l_theta;
		_xAxis = l_xAxis;
		_yAxis = l_yAxis;
		_zAxis = l_zAxis;
	}

	void Object::setXTrans(float l_xTrans){
		_xTrans = l_xTrans;
	}

	void Object::setYTrans(float l_yTrans){
		_yTrans = l_yTrans;
	}

	void Object::setZTrans(float l_zTrans){
		_zTrans = l_zTrans;
	}

	float Object::getXRot() const{
		return _xRot;
	}

	float Object::getYRot() const{
		return _yRot;
	}

	float Object::getZRot() const{
		return _zRot;
	}

	float Object::getXTrans() const{
		return _xTrans;
	}

	float Object::getYTrans() const{
		return _yTrans;
	}

	float Object::getZTrans() const{
		return _zTrans;
	}

	int  Object::getID() const{
		return _ID;
	}

	int Object::getNumberOfVertex() const{
		return _Points->size();
	}

	void Object::insertNewVertex(float l_x, float l_y, float l_z){
		_point3D v;
		v.x = l_x; v.y = l_y; v.z = l_z;
		v.rgb.r = 0.0f; v.rgb.g = 0.0f; v.rgb.b = 0.0f;
		_Points->push_back(v);
	}

	void Object::insertNewVertex(float l_x, float l_y, float l_z, float l_r, float l_g, float l_b){
		_point3D v;
		v.x = l_x; v.y = l_y; v.z = l_z;
		v.rgb.r = l_r; v.rgb.g = l_g; v.rgb.b = l_b;
		_Points->push_back(v);
	}

	void Object::deleteAllVertex(){
		_Points->clear();
	}

	void Object::_Rotate(){
		glRotatef(_xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(_yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(_zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(_theta, _xAxis, _yAxis, _zAxis);
	}

	void Object::_Translate(){
		glTranslatef(_xTrans, _yTrans, _zTrans);
	}

	void Object::_setDrawReference(Draw* l_draw){
		_drawReferences.push_back(l_draw);
	}

	bool Object::operator ==(Object l_obj){
		if(_ID == l_obj.getID()){
			return true;
		} else {
			return false;
		}
	}


/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////Draw////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/

	int Draw::drawWorld(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

		glLoadIdentity();	

		_Translate();
		_Rotate();

		std::vector<Object*>::iterator i = _Objects->begin();
		
		for(; i != _Objects->end(); ++i){
			if((*i) != NULL){
				(*i)->drawObject();
			}
		}
		
		return true;										// Keep Going
	}

	void Draw::setXRot(float l_xRot){
		_xRot = l_xRot;
	}

	void Draw::setYRot(float l_yRot){
		_yRot = l_yRot;
	}

	void Draw::setXTrans(float l_xTrans){
		_xTrans = l_xTrans;
	}

	void Draw::setYTrans(float l_yTrans){
		_yTrans = l_yTrans;
	}

	void Draw::setZTrans(float l_zTrans){
		_zTrans = l_zTrans;
	}

	float Draw::getXRot() const{
		return _xRot;
	}

	float Draw::getYRot() const{
		return _yRot;
	}

	float Draw::getXTrans() const{
		return _xTrans;
	}

	float Draw::getYTrans() const{ 
		return _yTrans;
	}

	float Draw::getZTrans() const{
		return _zTrans;
	}

	int Draw::getNumberOfObjects() const{
		return _Objects->size();
	}

	void Draw::_Rotate(){
		glRotatef(_xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(_yRot, 0.0f, 1.0f, 0.0f);
	}

	void Draw::_Translate(){
		glTranslatef(_xTrans, _yTrans, _zTrans);
	}

	void Draw::pushObject(Object* l_obj){
		l_obj->_setDrawReference(this);
		_Objects->push_back(l_obj);
	}

	void Draw::removeObject(Object* l_obj){
		std::vector<Object*>::iterator i = _Objects->begin();
		for(;i != _Objects->end(); i++){
			if((*l_obj) == (**i)){
				_Objects->erase(i);
				break;
			}
		}
	}

	void Draw::removeObject(int l_ID){
		std::vector<Object*>::iterator i = _Objects->begin();
		for(;i != _Objects->end(); i++){
			if(l_ID == (*i)->getID()){
				_Objects->erase(i);
				break;
			}
		}
	}

	void Draw::clear(){
		_Objects->clear();
	}

/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////OpenGLWindow////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/


	DWORD	OpenGLWindow::s_tlsIndex = 0;

	GLvoid OpenGLWindow::ReSizeGLScene()		// Resize And Initialize The GL Window
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
		
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

	int OpenGLWindow::InitGL()										// All Setup For OpenGL Goes Here
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.3725f, 0.6196f, 0.65f, 0.5f);		// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		return TRUE;										// Initialization Went OK
	}

	GLvoid OpenGLWindow::KillGLWindow(GLvoid)				// Properly Kill The Window
	{

		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,(LPCWSTR)"Release Of DC And RC Failed.",(LPCWSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,(LPCWSTR)"Release Rendering Context Failed.",(LPCWSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			hRC=NULL;										// Set RC To NULL
		}

		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,(LPCWSTR)"Release Device Context Failed.",(LPCWSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hDC=NULL;										// Set DC To NULL
		}

		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,(LPCWSTR)"Could Not Release hWnd.",(LPCWSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hWnd=NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass((LPCWSTR)"OpenGL",hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,(LPCWSTR)"Could Not Unregister Class.",(LPCWSTR)"SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hInstance=NULL;									// Set hInstance To NULL
		}
	}

	BOOL OpenGLWindow::CreateGLWindow()
	{
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
		
		hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= &OpenGLWindow::WndProc;				// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= hInstance;							// Set The Instance
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= (LPCWSTR)"OpenGL";	// Set The Class Name

		::TlsSetValue(s_tlsIndex, this);

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			MessageBox(NULL,(LPCWSTR)"Failed To Register The Window Class.", (LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											// Return FALSE
		}
		
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									(LPCWSTR)"OpenGL",					// Class Name
									title,								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Window Creation Error.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		DeleteMenu(GetSystemMenu(hWnd, false), SC_CLOSE, MF_BYCOMMAND); //Deactivate Closing by x-Button

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Can't Create A GL Device Context.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Can't Find A Suitable PixelFormat.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Can't Set The PixelFormat.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Can't Create A GL Rendering Context.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Can't Activate The GL Rendering Context.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		ShowWindow(hWnd,SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);									// Sets Keyboard Focus To The Window
		ReSizeGLScene();					// Set Up Our Perspective GL Screen

		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,(LPCWSTR)"Initialization Failed.",(LPCWSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		return TRUE;									// Success
	}

	LRESULT CALLBACK OpenGLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

		OpenGLWindow* This = reinterpret_cast<OpenGLWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (!This){
			This = static_cast<OpenGLWindow*>(::TlsGetValue(s_tlsIndex));
			::TlsSetValue(s_tlsIndex, 0);
			if(!::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(This))){

			}
			This->hWnd = hWnd;
		}
		const LRESULT ret = This->MainProc(hWnd, uMsg, wParam, lParam);

		if (uMsg == WM_NCDESTROY){
		  This->hWnd = 0;
		}

		return ret;
	}

	LRESULT CALLBACK OpenGLWindow::MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		
		static bool lButtonDown = false;
		static float xRot = 0.0f;
		static float yRot = 0.0f;
		float z = 0.0f;

		switch (uMsg)									// Check For Windows Messages
		{

			case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;									// Exit
			}

			case WM_CLOSE:								// Did We Receive A Close Message?
			{
				PostQuitMessage(0);						// Send A Quit Message	
			}break;

			case WM_KEYDOWN:							// Is A Key Being Held Down?
			{
				keys[wParam] = true;					// If So, Mark It As TRUE
			}break;

			case WM_KEYUP:								// Has A Key Been Released?
			{
				keys[wParam] = false;					// If So, Mark It As FALSE
			}break;

			case WM_LBUTTONUP:
			{
				if(lButtonDown){
					lButtonDown = false;
				}
			}break;

			case WM_LBUTTONDOWN:
			{
				if(!lButtonDown){
					this->mouseX = LOWORD(lParam);
					this->mouseY = HIWORD(lParam);
					lButtonDown = true;
					xRot = this->draw->getXRot();
					yRot = this->draw->getYRot();
				}
			}break;

			case WM_MOUSEMOVE:
			{
				if(lButtonDown){
					this->mouseXU = LOWORD(lParam);
					this->mouseYU = HIWORD(lParam);
					this->draw->setXRot(xRot+(mouseYU-mouseY));
					this->draw->setYRot(yRot+(mouseXU-mouseX));
					
				}

			}break;

			case WM_MOUSEWHEEL:
			{
				//Divide by 480.0f to get the Value 0.25f, because 
				// the Macro always return 120 or -120
				this->draw->setZTrans(this->draw->getZTrans()+(GET_WHEEL_DELTA_WPARAM(wParam)/480.0f));
			}break;

		}

		if(keys[87] == true){
				this->draw->setYTrans(this->draw->getYTrans()+0.125f);
		}
		if(keys[83] == true){
				this->draw->setYTrans(this->draw->getYTrans()-0.125f);
		}
		if(keys[65] == true){
				this->draw->setXTrans(this->draw->getXTrans()-0.125f);
		}
		if(keys[68] == true){
				this->draw->setXTrans(this->draw->getXTrans()+0.125f);
		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}

	bool OpenGLWindow::getKeyStatus(int Key){
		return keys[Key];
	}

	void OpenGLWindow::setKeyStatus(int Key, bool Status){
		keys[Key] = Status;
	}

	int OpenGLWindow::DrawGLScene(){
		return this->draw->drawWorld();
	}

	HDC OpenGLWindow::getHDC(){
		return this->hDC;
	}

	HWND OpenGLWindow::getHWND(){
		return this->hWnd;
	}

	HINSTANCE OpenGLWindow::getHINSTANCE(){
		return this->hInstance;
	}

	HGLRC OpenGLWindow::getHRC(){
		return this->hRC;
	}

	int OpenGLWindow::getMouseX(){
		return this->mouseX;
	}

	int OpenGLWindow::getMouseY(){
		return this->mouseY;
	}

}