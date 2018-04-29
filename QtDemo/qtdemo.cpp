#include "qtdemo.h"
#include "QMessageBox"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "picture_convert.h"
#include"QPainter"
#include "QPixmap"
QtDemo::QtDemo(QWidget *parent)
	: QMainWindow(parent)
{
	LOG(INFO) << "QtDemo construct*";
	m_task = new Task();
	connect(m_task, SIGNAL(sendPic(QImage)),this,SLOT(receivePic(QImage)));
	ui.setupUi(this);
}

QtDemo::~QtDemo()
{
	if (m_task)
	{
		delete m_task;
	}
}



void QtDemo::onBtnPicClicked()
{
	cv::Mat img = cv::imread("C://PIC//face.jpg");
	//cv::namedWindow("test");
	//cv::imshow("test", img);
	QImage qimg = cvMat2QImage(img);

	//ui.m_picLable->setPixmap(QPixmap::fromImage(createImage(256, 256,3)));
	//QThread::msleep(5000);
	//ui.m_picLable->setPixmap(QPixmap::fromImage(createImage(256, 256, 9)));

	//ui.m_picLable->repaint();
	m_task->taskInit("C://pic/phei.avi",40);
	m_task->start();
	//ui.m_picLable->setPixmap(QPixmap::fromImage(qimg));
	
}

void QtDemo::onBtnClicked()
{
	ui.m_picLable->setPixmap(QPixmap::fromImage(createImage(256, 256, 9)));
	LOG(INFO) << "button cliced";
}

void QtDemo::receivePic(QImage pic)
{
	qDebug() << "receive 0...";
	ui.m_picLable->repaint();
	ui.m_picLable->setPixmap(QPixmap::fromImage(pic));
}
