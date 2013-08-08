// calib.cpp
// Calling convention:
// calib board_w board_h number_of_views
//
// Hit ‘p’ to pause/unpause, ESC to quit
//
#pragma warning(disable: 4996)
#include "opencv2/opencv.hpp"
#include "OpenGL.h"
#include "myCVClasses.hpp"

using namespace myOpenGL;

LPWSTR charToWideChar(char* str);
void createBox(Object* obj);					
void createChessboard(Object* obj, float a);

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{

	MSG msg;
	bool done = false;

	Draw* draw = new Draw();					//"Draw" class takes all 3D objects and draws them on screen
	Object* Box = new Object(GL_LINES);			//The box object
	Object* Chessboard = new Object(GL_QUADS);	//The chessboard object
	createBox(Box);								//Create the box
	float a = 0.2f;								//The widht/height of each square of the chessboard object
	createChessboard(Chessboard, a);			//Create the chessboard object
	//Push both 3D objects into the draw class, so that the get drawn
	draw->pushObject(Box);					
	draw->pushObject(Chessboard);

	//The size of the chessboard which should be searched during the camera calibration plus
	//the number of images which should be taken
	int board_h = 6, board_w = 8, nImages = 2;
	bool found = false;								//Will be used to determine wether a chessboard has been 
													//found in the camera frame or not
	
	cv::VideoCapture capture(0);									//create an object with access to the camera
	myCV::CameraCalibration camCalib(capture, 8, 6, nImages);		//create an object which handels the camera calibration
	cv::Mat image(480,640,CV_32FC3), grayImg;							
	cv::Mat R(3,1,CV_64F), T(3,1,CV_64F);							//Declaration of rotation and translation Vector
	std::vector<cv::Point2f> corners;								//Vector which will contain the corner coordinates for each camera frame
	std::vector<cv::Point3f> _3DPoints;								//Vector that contains the 3D coordinates for each chessboard corner

	//Creating the OpenGL window
	OpenGLWindow* window = new OpenGLWindow(640,
										    480,
											IPL_DEPTH_8U,
											charToWideChar("3D Window"),
											draw);

	//Initialising the 3D-Points for the chessboard
	float rot = 0.0f;
	cv::Point3f _3DPoint;
	float y = (((board_h-1.0f)/2.0f)*a)+(a/2.0f);
	float x = 0.0f;
	for(int h = 0; h < board_h; h++, y+=a){
		x = (((board_w-2.0f)/2.0f)*(-a))-(a/2.0f);
		for(int w = 0; w < board_w; w++, x+=a){
			_3DPoint.x = x;
			_3DPoint.y = y;
			_3DPoint.z = 0.0f;
			_3DPoints.push_back(_3DPoint);
		}
	}
	
	//Camera calibration
	try{
		camCalib.Initialisation();
	} catch(cv::Exception &e){
		MessageBoxA(NULL, e.what(), "Cameracalibration", MB_ICONQUESTION | MB_YESNO);
	}

	capture >> image;
	
	corners.clear();

	cv::Mat M = camCalib.getIntrinsicsMatrix();
	cv::Mat D = camCalib.getDistortionCoeffs();

	if (!window->CreateGLWindow())
	{
		return 0;									// Quit If Window Was Not Created
	}
	
	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((!window->DrawGLScene()) || window->getKeyStatus(VK_ESCAPE))	// Active?  Was There A Quit Received?
			{
				done=true;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				//Search for a chessboard in the camera frame and save his corners
				found = cv::findChessboardCorners(image, cv::Size(8,6), corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
				if(found){ //A chessboard has been found 
					cv::cvtColor(image, grayImg, CV_BGR2GRAY);
					cv::cornerSubPix(grayImg, corners, cv::Size(11, 11), cv::Size(-1,-1), cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
					cv::drawChessboardCorners(image, cv::Size(board_w, board_h), cv::Mat(corners), found);	//Draw the chessboard corners on the camera frame 
					
					cv::solvePnP(cv::Mat(_3DPoints), cv::Mat(corners), M, D, R, T);		//Calculate the Rotation and Translation vector
					double theta = 0;

					//Calculate the angle of the rotation
					theta = sqrt((R.at<double>(0,0)*R.at<double>(0,0))+
									  (R.at<double>(1,0)*R.at<double>(1,0))+
									  (R.at<double>(2,0)*R.at<double>(2,0)));

					//Translate the 3D chessboard
					Chessboard->setXTrans((-T.at<double>(0,0)));
					Chessboard->setYTrans((-T.at<double>(1,0)-1.0));
					Chessboard->setZTrans((-T.at<double>(2,0)+5.0));

					//Rotate the 3D chessboard about the given vector for "theta" degrees.
					Chessboard->setRot((theta*180.0f)/3.14159f, R.at<double>(0,0), R.at<double>(1,0), R.at<double>(2,0));
				}
				cv::imshow("Calibrate", image);
				corners.clear();

				capture >> image;
				SwapBuffers(window->getHDC());	//Swap Buffers
				if(cv::waitKey(1) == 27){
					return 0;
				}
			}
		}
	}

	window->KillGLWindow();
	return (msg.wParam);							// Exit The Program
}


LPWSTR charToWideChar(char* str){
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* w_str = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str, -1, w_str, len);
	return w_str;
}

void createBox(Object* obj){

		obj->insertNewVertex(2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(2.5f, 2.5f, -5.0f);
		obj->insertNewVertex(2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, -5.0f);
		obj->insertNewVertex(2.5f, 2.5f, -5.0f);
		obj->insertNewVertex(2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(2.5f, 2.5f, -5.0f);
		obj->insertNewVertex(2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, -2.5f, -5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, 5.0f);
		obj->insertNewVertex(-2.5f, 2.5f, -5.0f);

}

void createChessboard(Object* obj, float a){
	float w = 0.0f; //width 
	float h = 0.0f; //height 

	for (float w = -3.0f; w < 4.0f; w++){
		for (float h = -2.0f; h < 3.0f; h++){
		
			if((int)h%2 == 0){
				if((int)w%2 != 0){
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)+(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)+(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)-(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)-(a/2.0f), 0.0f);
				} else {
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)+(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)+(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)-(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)-(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
				}
			} else {
				if((int)w%2 == 0){
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)+(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)+(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)-(a/2.0f), 0.0f);
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)-(a/2.0f), 0.0f);
				} else {
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)+(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)+(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)+(a/2.0f), (h*a)-(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
					obj->insertNewVertex((w*a)-(a/2.0f), (h*a)-(a/2.0f), 0.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
	}
}
