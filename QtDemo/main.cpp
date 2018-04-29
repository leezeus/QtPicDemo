#include "qtdemo.h"
#include <QtWidgets/QApplication>
#include "glog/logging.h"
#include "glog/log_severity.h"
#include "demodefine.h"
#include "config.h"


static int init_log(const char *path)
{
	if (path == NULL || strlen(path) > MAX_PATH_LEN)
	{
		printf("log path is invalid!\n");
		return -1;
	}
	FLAGS_log_dir = path;
	google::InitGoogleLogging("QtDemo");
	google::SetLogDestination(google::GLOG_INFO, path);
	google::SetStderrLogging(google::GLOG_ERROR);
	google::SetLogFilenameExtension("log");
	FLAGS_colorlogtostderr = true;  // Set log color
	FLAGS_logbufsecs = 0;           // Set log output speed(s)
	FLAGS_max_log_size = 1024;      // Set max log file size
	FLAGS_stop_logging_if_full_disk = true;  // If disk is full
	return 0;
}

static void uinit_log()
{
	google::ShutdownGoogleLogging();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	init_log("..//log//");
	Config::get_instance()->load_config("..//config//config.xml");
	LOG(INFO) << "read config success*";
	QtDemo w;
	w.show();

	uinit_log();
	return a.exec();
}
