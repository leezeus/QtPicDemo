#pragma once
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QImage>
#include "glog/logging.h"
#include "glog/log_severity.h"
#include "picture_convert.h"

typedef void(*PROCESSOR)(cv::Mat&, cv::Mat&);

//process interface;
class FrameProcessor 
{
public:
	// processing method
	virtual void process(cv::Mat &input, cv::Mat &output) = 0;
};


class Task : public QThread 
{
	Q_OBJECT

public:
	Task(QObject * parent = Q_NULLPTR);
	virtual ~Task();
	bool setInput(std::string filename);
	bool setInput(int id);
	bool setInput(const std::vector<std::string>& imgs);
	void setDelay(long delay);
	bool taskInit(std::string file_name, long delay);
	void setFrameProcessor(PROCESSOR frameProcessingCallback);
	void setFrameProcessor(FrameProcessor* frameProcessorPtr);
	void callProcess();
	void dontCallProcess();
	bool isOpened();
	bool readNextFrame(cv::Mat& frame);
	long getFrameNumber();
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
	long     m_delay;
	long    m_fnumber;
	std::vector<std::string> m_images;
	std::vector<std::string>::const_iterator m_itImg;
	long    m_frameToStop;
	bool    m_processorCallFlag;
	bool    m_callIt;
	cv::VideoCapture m_capture;
	FrameProcessor *m_frameProcessor;
	PROCESSOR m_frameProcessorFun;
	
	
};
