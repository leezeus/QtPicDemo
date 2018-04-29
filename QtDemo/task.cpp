#include "task.hpp"
#include "Qdebug"

Task::Task(QObject * parent) : QThread(parent) {
	
	m_runFlag = false;
	LOG(INFO) << "Task constructed ";
	
}

Task::~Task() 
{
	LOG(INFO) << "Task Deconstructed ";
	m_mutex.lock();
	m_runFlag = true;
	m_mutex.unlock();
	wait();
}

void Task::run() 
{
	
	qDebug() << "running....";
	for (int i = 0; i < 100;i++)
    {
		if (i % 2 == 0)
		{
			emit sendPic(createImage(256, 256, 3));
			qDebug() << "sent 0...";
			LOG(INFO) << "send signal 0";
		}
		else
		{
			emit sendPic(createImage(256, 256, 9));
			qDebug() << "sent 1...";
			LOG(INFO) << "send signal 1";
		}

		msleep(500);
    }
	


}

void Task::stopPlay()
{

}
