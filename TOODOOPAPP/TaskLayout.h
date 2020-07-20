#pragma once
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qplaintextedit.h>
#include <qobject.h>
#include "json.hpp"
#include <fstream>
#include <QMessageBox>
#include "messageBox.h"
#include <QGraphicsAnchorLayout>
#include <QCheckBox>
#include <qdebug.h>
#include <QShortcut> 

class TaskLayout : public QWidget {
	Q_OBJECT
public:
	TaskLayout(QString taskText);

	~TaskLayout();

	QCheckBox* getCheckBox();

	QPlainTextEdit* textBox;
Q_SIGNALS:
	void sendDelete();

private:
	std::string text;
	void createTask(QString text);

	QCheckBox* checkBox;
	QHBoxLayout* horLayout;
};
