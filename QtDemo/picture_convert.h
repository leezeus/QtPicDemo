#pragma once

#include "QImage"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "QPainter"

// QImage 转换成 cv::Mat 
cv::Mat QImage2cvMat(QImage image);
// cv::Mat 转化成 QImage
QImage cvMat2QImage(const cv::Mat& mat);
// 生成一个图案 调试用
QImage createImage(int width, int height, int color);