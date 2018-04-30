#include "picture_convert.h"
#include "glog/logging.h"
#include "glog/log_severity.h"
cv::Mat QImage2cvMat(QImage image)
{
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.bits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.bits(), image.bytesPerLine());
		break;
	}
	return mat;
}

QImage cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		//image.setNumColors(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

QImage createImage(int width, int height, int color)
{
	QImage image(width, height, QImage::Format_RGB16);
	QPainter painter;
	QPen pen;
	pen.setStyle(Qt::NoPen);
	QBrush brush(Qt::blue);


	painter.begin(&image);
	painter.fillRect(image.rect(), brush);
	if (color == 3)
	{
		brush.setColor(Qt::gray);
	}
	else
	{
		brush.setColor(Qt::yellow);
	}
	painter.setPen(pen);
	painter.setBrush(brush);

	static const QPointF points1[3] = {
		QPointF(4, 4),
		QPointF(7, 4),
		QPointF(5.5, 1)
	};

	static const QPointF points2[3] = {
		QPointF(1, 4),
		QPointF(7, 4),
		QPointF(10, 10)
	};

	static const QPointF points3[3] = {
		QPointF(4, 4),
		QPointF(10, 4),
		QPointF(1, 10)
	};

	painter.setWindow(0, 0, 10, 10);

	int x = 0;
	int y = 0;
	int starWidth = image.width() / 3;
	int starHeight = image.height() / 3;

	QRect rect(x, y, starWidth, starHeight);

	for (int i = 0; i < 9; ++i) {

		painter.setViewport(rect);
		painter.drawPolygon(points1, 3);
		painter.drawPolygon(points2, 3);
		painter.drawPolygon(points3, 3);

		if (i % 3 == 2) {
			y = y + starHeight;
			rect.moveTop(y);

			x = 0;
			rect.moveLeft(x);

		}
		else {
			x = x + starWidth;
			rect.moveLeft(x);
		}
	}

	painter.end();
	return image;
}

FaceDetecter::FaceDetecter(std::string path_)
{
	//"..//config//haarcascade_fullbody.xml"
	if (!m_faceClassifier.load(path_)) {
		LOG(INFO) << "Load haarcascade_mcs_eye.xml failed!";
	}
}

void FaceDetecter::process(cv::Mat &input, cv::Mat &output)
{
	cv::Mat image_gray;
	//转为灰度图
	cv::cvtColor(input, image_gray, CV_BGR2GRAY);
	//直方图均衡化，增加对比度方便处理
	equalizeHist(image_gray, image_gray);
	//检测
	m_faceClassifier.detectMultiScale(image_gray, m_detectResults, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 60));

	input.copyTo(output);
	//画出检测到的位置
	for (size_t eyeIdx = 0; eyeIdx < m_detectResults.size(); eyeIdx++)
	{
		rectangle(output, m_detectResults[eyeIdx], cv::Scalar(0, 0, 255));
	}
}
