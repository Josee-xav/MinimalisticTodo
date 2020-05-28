#pragma once

#include <QtWidgets/QMainWindow>
#include <ui_TodoApp.h>
#include "json.hpp"
#include <fstream>
#include <QMessageBox>
#include "messageBox.h"
#include <QGraphicsAnchorLayout>
#include <QCheckBox>
#include <QShortcut> 

#include <qdebug.h>

#include "TaskLayout.h"

#define LOAD_JSON_ARG "--loadjson"

class TodoApp : public QMainWindow
{
	Q_OBJECT

public:
	TodoApp(QWidget* parent = Q_NULLPTR);

	// This function overrides the QMainWindow closeEvent function so i can send the user a warning dialog before closing
	void closeEvent(QCloseEvent* event) override;

private slots:
	void reloadTasksFromJson(bool warn = true);
	void saveTasks();
	void addTaskBtn();

	void closeTaskInputArea();

	void deleteTask();
	void setSaved(bool s);
private:
	void removeItemsInLayout(QLayout* layout);

	void addTask(std::string taskText);

	QVBoxLayout* scrollTaskAreaLayout;
	std::vector <std::string> tasksText;
	Ui::TodoAppClass ui;
	bool saved;
};
