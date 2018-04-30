#include "task.hpp"
#include "Qdebug"

Task::Task(QObject * parent) : QThread(parent) {
	
	m_runFlag = true;
	m_fnumber = 0;
	m_processorCallFlag = false;
	m_callIt = false;

	LOG(INFO) << "Task constructed";
}

Task::~Task() 
{
	LOG(INFO) << "Task Deconstructed ";
	m_mutex.lock();
	m_runFlag = true;
	m_mutex.unlock();
	wait();
}

bool Task::taskInit(std::string file_name, long delay, FrameProcessor* frameProcessorPtr)
{
	setInput(file_name);
	setDelay(delay);
	setFrameProcessor(frameProcessorPtr);
	LOG(INFO) << "Task init success...";
	return true;
}

bool Task::taskInit(std::vector<std::string> file_names, long delay, FrameProcessor* frameProcessorPtr)
{
	setInput(file_names);
	setDelay(delay);
	setFrameProcessor(frameProcessorPtr);
	LOG(INFO) << "Task init success...";
	return true;
}

bool Task::isOpened()
{
	return m_capture.isOpened() || !m_images.empty();
}


bool Task::readNextFrame(cv::Mat& frame) {

	if (m_images.size() == 0)
		return m_capture.read(frame);
	else 
	{

		if (m_itImg != m_images.end())
		{

			frame = cv::imread(*m_itImg);
			m_itImg++;
			return frame.data != 0;
		}
		return false;
	}
}


// return the frame number of the next frame
long Task::getFrameNumber() 
{

	if (m_images.size() == 0) 
	{

		// get info of from the capture device
		long f = static_cast<long>(m_capture.get(CV_CAP_PROP_POS_FRAMES));
		return f;

	}
	else 
	{ // if input is vector of images

		return static_cast<long>(m_itImg - m_images.begin());
	}
}

void Task::run() 
{
	
	// current frame
	cv::Mat frame;
	// output frame
	cv::Mat output;
	if (!isOpened())
	{
		return;
	}

	while (m_runFlag)
	{
		// read next frame if any
		if (!readNextFrame(frame))
			break;
		// calling the process function or method
		if (m_callIt)
		{
			// process the frame
			if (m_frameProcessorFun)
			{
				m_frameProcessorFun(frame, output);
			}	
			else if (m_frameProcessor)
			{ 
				m_frameProcessor->process(frame, output);
			}	
			// increment frame number
			m_fnumber++;

		}
		else {

			output = frame;
		}

		emit sendPic(cvMat2QImage(output));
		// introduce a delay
		if (m_delay >= 0)
		{
			msleep(m_delay);
		}
		// check if we should stop
		if (m_frameToStop >= 0 && getFrameNumber() == m_frameToStop)
		{
			return;
		}
			
	}


/*
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
*/

}

void Task::stopPlay()
{

}

// set the name of the video file
bool Task::setInput(std::string filename) {

	// In case a resource was already 
	// associated with the VideoCapture instance
	m_capture.release();
	m_images.clear();

	// Open the video file
	return m_capture.open(filename);
}

// set the camera ID
bool Task::setInput(int id) 
{
	m_capture.release();
	m_images.clear();

	// Open the video file
	return m_capture.open(id);
}

// set the vector of input images
bool Task::setInput(const std::vector<std::string>& imgs)
{
	m_capture.release();
	// the input will be this vector of images
	m_images = imgs;
	m_itImg = m_images.begin();
	return true;
}

void Task::setDelay(long delay)
{
	m_delay = delay;
}

void Task::setFrameProcessor(PROCESSOR frameProcessingCallback)
{
	// invalidate frame processor class instance
	m_frameProcessor = NULL;
	// this is the frame processor function that will be called
	m_frameProcessorFun = frameProcessingCallback;
	callProcess();
}

// set the instance of the class that implements the FrameProcessor interface
void Task::setFrameProcessor(FrameProcessor* frameProcessorPtr)
{

	// invalidate callback function
	m_frameProcessorFun = NULL;
	// this is the frame processor instance that will be called
	m_frameProcessor = frameProcessorPtr;
	callProcess();
}

void Task::callProcess()
{

	m_callIt = true;
}

// do not call process callback
void Task::dontCallProcess() 
{

	m_callIt = false;
}