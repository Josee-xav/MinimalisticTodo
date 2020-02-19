#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMainWindow>
#include "json.hpp"
#include <fstream>
#include <QMessageBox>
#include "messageBox.h"
#include <QGraphicsAnchorLayout>
#include <QCheckBox>
#include <qdebug.h>
#include <QShortcut> 
#include "ui_Todo.h"

#include "TaskLayout.h"
#include <QVBoxLayout>

#define LOAD_JSON_ARG "--loadjson"

class Todo : public QMainWindow
{
	Q_OBJECT

public:
	Todo(QWidget* parent = Q_NULLPTR);

	// This function overrides the QMainWindow closeEvent function so i can send the user a warning dialog before closing
	void closeEvent(QCloseEvent* event) override;

private slots:
	void reloadTasksFromJson(bool warn = true);
	void addTaskBtn();

	// the function that closes the task input area so it looks cleaner
	void closeTaskInputArea();

	// removes the task. By getting the sender tasklayout and then removing that tasklayout from the scrollarea
	void deleteTask();
	void saveTasks();
private:
	void addTask(std::string taskText);

	void removeItemsInLayout(QLayout* layout);
	void createShortcuts();

	QVBoxLayout* scrollTaskAreaLayout;
	std::vector <std::string> tasksText;
	Ui::TodoClass ui;
};
