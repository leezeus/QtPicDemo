#pragma once
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QImage>
#include "glog/logging.h"
#include "glog/log_severity.h"
#include "picture_convert.h"

class Task : public QThread 
{
	Q_OBJECT

public:
	Task(QObject * parent = Q_NULLPTR);
	virtual ~Task();
	void taskInit();
protected:
	void run() Q_DECL_OVERRIDE;
signals:
	void sendPic(QImage img);
	public slots:
	void stopPlay();
private:
	QImage  m_image;
	QMutex  m_mutex;
	QString m_fileName;
	bool    m_runFlag;
	cv::VideoCapture m_capture;
	
	
};
