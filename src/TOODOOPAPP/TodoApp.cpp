#include "TodoApp.h"

TodoApp::TodoApp(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setSaved(true);
	QObject::connect(ui.addTextBtn, SIGNAL(clicked()), this, SLOT(addTaskBtn()), Qt::QueuedConnection);
	QObject::connect(ui.saveTasks, SIGNAL(clicked()), this, SLOT(saveTasks()), Qt::QueuedConnection);
	QObject::connect(ui.reloadTasksBtn, SIGNAL(clicked()), this, SLOT(reloadTasksFromJson()), Qt::QueuedConnection);
	QObject::connect(ui.upbtn, SIGNAL(clicked()), this, SLOT(closeTaskInputArea()), Qt::QueuedConnection);

	scrollTaskAreaLayout = new QVBoxLayout(this);
	scrollTaskAreaLayout->setAlignment(Qt::AlignTop); // makes sure that when adding widgets it places them on the top rather than the center(default)

	ui.scrollAreaWidgetContents->setLayout(scrollTaskAreaLayout); // adds the vertical layout to the scrolls area

	if(QCoreApplication::arguments().count() > 1) {
		if(QCoreApplication::arguments().at(1).contains(LOAD_JSON_ARG))
			reloadTasksFromJson(false);
	}
}


void TodoApp::addTaskBtn()
{
	QString taskText = ui.todoText->toPlainText();

	if(!taskText.isEmpty()) {
		addTask(taskText.toStdString());
		setSaved(false);
	}
}

void TodoApp::removeItemsInLayout(QLayout* layout)
{
	QLayoutItem* item;
	while((item = scrollTaskAreaLayout->takeAt(0))) {
		if(item->layout()) {
			removeItemsInLayout(item->layout());
			delete item->layout();
		}
		if(item->widget()) {
			delete item->widget();
		}
		delete item;
	}

}

void TodoApp::addTask(std::string taskText)
{
	TaskLayout* task = new TaskLayout(taskText.c_str());

	connect(task, SIGNAL(sendDelete()), this, SLOT(deleteTask()));

	scrollTaskAreaLayout->addWidget(task);

	tasksText.push_back(taskText); // adds the task text to a vector so we can use it to save the text

	ui.todoText->setPlainText("");
}


void TodoApp::reloadTasksFromJson(bool warn)
{
	if(warn) {
		if(QMessageBox::No == styledQuestionMessageBox("Reload Tasks Confirmation", "Press yes if you want to reload the saved tasks. This will remove the unsaved tasks. Press no if you dont want to.", "background-color: rgb(24, 24, 24);\ncolor: white;",
			QMessageBox::Yes | QMessageBox::No))
		{
			return;
		}
	}

	std::ifstream jsonFile("Taskss.json");
	// checks if ifstream can do its job
	if(!jsonFile) {
		styledQuestionMessageBox("Error", "Couldnt find Taskss.json file", "background-color: rgb(24, 24, 24);\ncolor: white;", QMessageBox::Ok);
		return;
	}

	if(scrollTaskAreaLayout->count() >= 1)
		removeItemsInLayout(scrollTaskAreaLayout); // remove all of the previous tasks


	nlohmann::json parsedJson = nlohmann::json::parse(jsonFile);

	tasksText.clear(); // clears the whole arrays
	for(int i = 0; i < parsedJson["Tasks"].size(); i++) {
		addTask(parsedJson["Tasks"].at(i));
	}
}

void TodoApp::closeEvent(QCloseEvent* event)
{
	if(tasksText.size() > 0 && !saved) {
		if(QMessageBox::Yes == styledQuestionMessageBox("MTodo | Warning", "Did you save your changes??", "background-color: rgb(24, 24, 24);\ncolor: white;",
			QMessageBox::Yes | QMessageBox::No))
			event->accept();
		else
			event->ignore();

	}
	else
		event->accept();
}

void TodoApp::saveTasks()
{
	if(tasksText.size() > 0) {
		nlohmann::json j;
		auto json = nlohmann::json::array();
		for(int i = 0; i < tasksText.size(); i++)
			json.push_back(tasksText.at(i));


		j["Tasks"] = json;

		std::ofstream file("Taskss.json", std::ios::trunc);
		file << j;

		styledQuestionMessageBox("MTodo | Saved", "Saved tasks", "background-color: rgb(24, 24, 24);\ncolor: white;", QMessageBox::Ok);
	}
}


void TodoApp::closeTaskInputArea()
{
	if(ui.upbtn->text() == "^") {
		ui.todoText->setVisible(false);
		ui.addTextBtn->setVisible(false);
		ui.reloadTasksBtn->setVisible(false);
		ui.saveTasks->setVisible(false);
		ui.upbtn->setText("<");
	}
	else {
		ui.todoText->setVisible(true);
		ui.addTextBtn->setVisible(true);
		ui.reloadTasksBtn->setVisible(true);
		ui.saveTasks->setVisible(true);
		ui.upbtn->setText("^");
	}
}

void TodoApp::deleteTask()
{
	TaskLayout* senderTask = static_cast<TaskLayout*>(sender());

	/// if the user presses "No" then dont delete the task then it stops the function
	if(QMessageBox::No == styledQuestionMessageBox("Deleting Task", "Are you sure that you want to delete this task? Press yes if you want to delete or press no.", "background-color: rgb(24, 24, 24);\ncolor: white;",
		QMessageBox::Yes | QMessageBox::No)) {
		senderTask->getCheckBox()->setChecked(false);
		return;
	}

	if(senderTask != nullptr) {
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

void TodoApp::setSaved(bool s)
{
	saved = s;
	ui.saveTasks->setEnabled(!saved);
}
