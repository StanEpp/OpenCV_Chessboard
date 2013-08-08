#pragma warning(disable: 4996)
#include "opencv2/opencv.hpp"
#include <algorithm>
#include "boost/bind.hpp"


namespace myCV{
/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////CAMERACALIBRATION///////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/

	void CameraCalibration::Initialisation(){

		if(!_objectPointsSet){

			std::vector<cv::Point3f> tmp;
			//Compute the the three dimensional world-coordinates
			for(float h = 0.0f; h < _board_h; h+=1.0f){
				for (float w = 0.0f; w < _board_w; w+=1.0f){
					tmp.push_back(cv::Point3f(h, w, 0.0f));
				}
			}
			//Put all the world-coordinates for each frame in a vector
			for(int n = 0; n < _nImages; ++n){
				_objectPoints.push_back(tmp);
			}

		}

		GrabFrames();
	}

	void CameraCalibration::GrabFrames(){

		std::vector<cv::Point2f> corners;
		cv::Size board_sz(_board_w, _board_h);
		cv::Size winSize(11, 11);
		cv::Size zeroZone(-1, -1);
		int cornerCount = 0, index = 0, frames = 0;
		bool found = false;

		cv::Mat img, imgText, grayImg;

		img.create((int)_capture.get(CV_CAP_PROP_FRAME_HEIGHT), (int)_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
		imgText.create((int)_capture.get(CV_CAP_PROP_FRAME_HEIGHT), (int)_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
		grayImg.create((int)_capture.get(CV_CAP_PROP_FRAME_HEIGHT), (int)_capture.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC1);

		_capture >> img;
		img.copyTo(imgText);
		
		//Init stringstream for the image counter text
		std::ostringstream ostr;
		
		cv::namedWindow("Grab_Frames");

		while(frames != _nImages){
			ostr.str("");
			ostr << frames <<"/" <<_nImages;

			//Find Chessboardcorners on the original Image "img"
			found = cv::findChessboardCorners(img, board_sz, corners ,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

			//Draw the visual output on the scribble Image "imgText"
			cv::drawChessboardCorners(imgText, board_sz, cv::Mat(corners), found);
			cv::putText(imgText, ostr.str().c_str(), cv::Point(50,50),cv::FONT_HERSHEY_COMPLEX|cv::FONT_ITALIC, 1.0, cv::Scalar(0,0,0));
			cv::imshow("Grab_Frames", imgText);

			//Picture has been chosen by the user
			if(cvWaitKey(10) == 0x0D && found){
				cv::cvtColor(img, grayImg, CV_BGR2GRAY);

				cv::cornerSubPix(grayImg, corners, winSize, zeroZone, cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

				_imagePoints.push_back(corners);

				frames++;
			}

			//Take the next Frame and copy it to the visual output frame "imgText"
			_capture >> img;
			img.copyTo(imgText);
		}

		//Compute matrices
		cv::calibrateCamera(_objectPoints, _imagePoints, img.size(), _intrinsicsMatrix, _distortionCoeffs, _rvecs, _tvecs);
		_initialisation = true;
		cv::destroyWindow("Grab_Frames");

	}

	void CameraCalibration::setObjectPoints(std::vector<std::vector<cv::Point3f>> l_objectPoints){

		_objectPoints = l_objectPoints;
		_objectPointsSet = true;
	}

	cv::Mat CameraCalibration::getIntrinsicsMatrix() const{

		if(_initialisation){
			return _intrinsicsMatrix;
		} else {
			throw Error("Initialisation of camera has not been done yet!");
		}
	}

	cv::Mat CameraCalibration::getDistortionCoeffs() const{

		if(_initialisation){
			return _distortionCoeffs;
		} else {
			throw Error("Initialisation of camera has not been done yet!");
		}
	}

	std::vector<cv::Mat> CameraCalibration::getRotationVectors(){

		if(_initialisation){
			return _rvecs;
		} else {
			throw Error("Initialisation of camera has not been done yet!");
		}
	}

	std::vector<cv::Mat> CameraCalibration::getTranslationVectors(){
		
		if(_initialisation){
			return _tvecs;
		} else {
			throw Error("Initialisation of camera has not been done yet!");
		}
	}