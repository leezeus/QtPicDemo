#pragma once

#include "QImage"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/ml.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
#include "QPainter"

// QImage 转换成 cv::Mat 
cv::Mat QImage2cvMat(QImage image);
// cv::Mat 转化成 QImage
QImage cvMat2QImage(const cv::Mat& mat);
// 生成一个图案 调试用
QImage createImage(int width, int height, int color);


class FrameProcessor
{
public:
	// processing method
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;
};

class FaceDetecter :public FrameProcessor
{
public:
	FaceDetecter(std::string path_);
	
private :
	cv::CascadeClassifier  m_faceClassifier;
	std::string m_static;
	std::vector<cv::Rect> m_detectResults;

public :

	virtual void process(cv::Mat &input, cv::Mat &output);
};