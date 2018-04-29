#pragma once

#include "QImage"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "QPainter"

// QImage ת���� cv::Mat 
cv::Mat QImage2cvMat(QImage image);
// cv::Mat ת���� QImage
QImage cvMat2QImage(const cv::Mat& mat);
// ����һ��ͼ�� ������
QImage createImage(int width, int height, int color);