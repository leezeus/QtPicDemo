#ifndef QTDEMO_H
#define QTDEMO_H

#include <QtWidgets/QMainWindow>
#include "ui_qtdemo.h"
#include "glog/logging.h"
#include "glog/log_severity.h"
#include "task.hpp"
#include "QDebug"
class QtDemo : public QMainWindow
{
	Q_OBJECT

public:
	QtDemo(QWidget *parent = 0);
	~QtDemo();
private:
	Ui::QtDemoClass ui;
	private slots:
	void onBtnPicClicked();
	void onBtnClicked();
	void receivePic(QImage pic);
private:
	Task * m_task;
};

#endif // QTDEMO_H
