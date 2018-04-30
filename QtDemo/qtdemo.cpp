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
	m_frameProcesser = new FaceDetecter("..//config//haarcascade_fullbody.xml");
	connect(m_task, SIGNAL(sendPic(QImage)),this,SLOT(receivePic(QImage)));
	ui.setupUi(this);
}

QtDemo::~QtDemo()
{
	if (m_task)
	{
		delete m_task;
	}
	if (m_frameProcesser)
	{
		delete m_frameProcesser;
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
	
	//m_task->taskInit("C://pic/run.mp4",0,m_frameProcesser);
	std::vector<std::string> file_names;
	file_names.push_back("C://PIC//pedstrain1.jpg");
	file_names.push_back("C://PIC//pedstrain2.jpg");
	file_names.push_back("C://PIC//pedstrain3.jpg");
	file_names.push_back("C://PIC//pedstrain4.jpg");
	m_task->taskInit(file_names, 2000, m_frameProcesser);
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
