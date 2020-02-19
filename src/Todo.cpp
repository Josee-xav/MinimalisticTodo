#include "Todo.h"

Todo::Todo(QWidget* parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	QObject::connect(ui.addText, SIGNAL(clicked()), this, SLOT(addTaskBtn()));
	QObject::connect(ui.reloadTasks, SIGNAL(clicked()), this, SLOT(reloadTasksFromJson()));
	QObject::connect(ui.upbtn, SIGNAL(clicked()), this, SLOT(closeTaskInputArea()));

	scrollTaskAreaLayout = new QVBoxLayout(this);
	scrollTaskAreaLayout->setAlignment(Qt::AlignTop); // makes sure that when adding widgets it places them on the top rather than the center(default)

	ui.scrollAreaWidgetContents->setLayout(scrollTaskAreaLayout); // adds the vertical layout to the scrolls area

	if (QCoreApplication::arguments().count() > 1) {
		if (QCoreApplication::arguments().at(1).contains(LOAD_JSON_ARG))
			reloadTasksFromJson(false); // adding false to the argument makes sure that a message box doesnt come up asking if the user is sure.
	}
}

void Todo::addTaskBtn()
{
	QString taskText = ui.todoText->toPlainText();

	if (!taskText.isEmpty()) {
		addTask(taskText.toStdString());
	}
}

void Todo::removeItemsInLayout(QLayout* layout)
{
	QLayoutItem* item;
	while ((item = scrollTaskAreaLayout->takeAt(0))) {
		if (item->layout()) {
			removeItemsInLayout(item->layout());
			delete item->layout();
		}
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}

}

void Todo::addTask(std::string taskText)
{
	TaskLayout* task = new TaskLayout(taskText.c_str());

	connect(task, SIGNAL(sendDelete()), this, SLOT(deleteTask()));

	scrollTaskAreaLayout->addWidget(task);

	tasksText.push_back(taskText); // adds the task text to a vector so we can use it to save the text

	ui.todoText->setPlainText("");
}


void Todo::reloadTasksFromJson(bool warn)
{
	if (warn) {
		if (QMessageBox::No == styledQuestionMessageBox("Reload Tasks Confirmation", "Press yes if you want to reload the saved tasks. This will remove any unsaved tasks.", "background-color: rgb(24, 24, 24);\ncolor: white;",
			QMessageBox::Yes | QMessageBox::No))
		{
			return;
		}
	}

	std::ifstream jsonFile("Taskss.json");
	// checks if ifstream can do its job
	if (!jsonFile) {
		QMessageBox::Ok == styledQuestionMessageBox("Error", "Couldnt find Taskss.json file", "background-color: rgb(24, 24, 24);\ncolor: white;", QMessageBox::Ok);
		return;
	}

	// checks if theirs any previous tasks to remove because the message box did say that will remove em
	if (scrollTaskAreaLayout->count() >= 0)
		removeItemsInLayout(scrollTaskAreaLayout); // remove all of the previous tasks

	nlohmann::json parsedJson = nlohmann::json::parse(jsonFile);

	tasksText.clear(); // clears the whole vector.
	for (int i = 0; i < parsedJson["Tasks"].size(); i++) {
		addTask(parsedJson["Tasks"].at(i));
	}
}

void Todo::closeEvent(QCloseEvent* event)
{
	if (tasksText.size() > 0) {
		if (QMessageBox::Yes == styledQuestionMessageBox("MTodo | Save tasks?", "Do you want to save your tasks before leaving?\nThis will override any other tasks that are not on ur tasklist right now", "background-color: rgb(24, 24, 24);\ncolor: white;",
			QMessageBox::Yes | QMessageBox::No))
		{
			saveTasks(); // saves the task
		}
	}
	event->accept();
}

void Todo::saveTasks()
{
	nlohmann::json j;
	auto json = nlohmann::json::array();

	for (int i = 0; i < tasksText.size(); i++)
		json.push_back(tasksText.at(i));

	j["Tasks"] = json;

	std::ofstream file("Taskss.json", std::ios::trunc);
	if (!file)
	{
		// Print an error and exit
		if (QMessageBox::Ok == styledQuestionMessageBox("MTodo | Couldnt save", "Wasnt able to write to Taskss.json", "background-color: rgb(24, 24, 24);\ncolor: white;", QMessageBox::Ok))
			exit(1);
	}

	file << j;
}

void Todo::closeTaskInputArea()
{
	if (ui.upbtn->text() == "^") {
		ui.todoText->setVisible(false);
		ui.addText->setVisible(false);
		ui.reloadTasks->setVisible(false);
		ui.upbtn->setText("<");
	}
	else {
		ui.todoText->setVisible(true);
		ui.addText->setVisible(true);
		ui.reloadTasks->setVisible(true);
		ui.upbtn->setText("^");
	}
}

void Todo::deleteTask()
{
	TaskLayout* senderTask = static_cast<TaskLayout*>(sender());

	/// if the user presses "No" then dont delete the task then it stops the function
	if (QMessageBox::No == styledQuestionMessageBox("Deleting Task", "Are you sure that you want to delete this task? Press yes if you want to delete or press no.", "background-color: rgb(24, 24, 24);\ncolor: white;",
		QMessageBox::Yes | QMessageBox::No)) {
		senderTask->getCheckBox()->setChecked(false);
		return;
	}

	if (senderTask != nullptr) {
		//qDebug() << "deleting layout";
		scrollTaskAreaLayout->removeWidget(senderTask);
		senderTask->deleteLater();
	}
#ifdef DEBUG
	else {
		qDebug() << "problem the task didnt get deleted";
	}
#endif // DEBUG
}
