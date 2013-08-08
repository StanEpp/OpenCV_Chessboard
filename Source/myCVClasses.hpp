#ifndef _MYCVCLASSES_
#define _MYCVCLASSES_

#include <vector>
#include "opencv2/opencv.hpp"

namespace myCV{

/****************************************************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////CAMERACALIBRATION///////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/****************************************************************************************************************************/


	class CameraCalibration{
	private:
		cv::VideoCapture _capture;
		int _board_w, _board_h, _board_n;
		int _nImages;
		std::vector<std::vector<cv::Point2f> > _imagePoints;
		std::vector<std::vector<cv::Point3f> > _objectPoints;
		std::vector<cv::Mat> _rvecs, _tvecs;
		cv::Mat _intrinsicsMatrix, _distortionCoeffs;
		bool _objectPointsSet, _initialisation;

	public:
		CameraCalibration(cv::VideoCapture& l_capture, int l_board_w, int l_board_h, int l_nImages) : 
			 _board_w(l_board_w), _board_h(l_board_h), _nImages(l_nImages), _board_n(l_board_w * l_board_h),
			 _objectPointsSet(false), _initialisation(false), _capture(l_capture),
			 _imagePoints(std::vector<std::vector<cv::Point2f>>()), _objectPoints(std::vector<std::vector<cv::Point3f>>()), 
			 _intrinsicsMatrix(cv::Mat_<float>()), _distortionCoeffs(cv::Mat_<float>()),
			 _rvecs(std::vector<cv::Mat>()), _tvecs(std::vector<cv::Mat>()){
						
				 _intrinsicsMatrix.create(3, 3, CV_32FC1);
				 _intrinsicsMatrix.ptr<float>(0)[0] = 1.0f;
				 _intrinsicsMatrix.ptr<float>(1)[1] = 1.0f;
				 _distortionCoeffs.create(5, 1, CV_32FC1);

			 }

		  ~CameraCalibration(){}

		  void		Initialisation();
		  void		setObjectPoints(std::vector<std::vector<cv::Point3f>> l_objectPoints);
		  cv::Mat	getIntrinsicsMatrix() const;
		  cv::Mat	getDistortionCoeffs() const;
		  std::vector<cv::Mat> getRotationVectors();
		  std::vector<cv::Mat> getTranslationVectors();

	private:
		void GrabFrames();

	};
	
#endif

