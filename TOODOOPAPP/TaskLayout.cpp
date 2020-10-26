#include "TaskLayout.h"
#include <QMainWindow>

TaskLayout::TaskLayout(QString taskText)
{
	horLayout = new QHBoxLayout(this);
	horLayout->setSpacing(0);
	horLayout->setContentsMargins(0, 0, 0, 0);

	textBox = new QPlainTextEdit(this);
	checkBox = new QCheckBox(this);

	// sends a click() to the signal so it can emit.
	connect(checkBox, &QAbstractButton::clicked, this, &TaskLayout::sendDelete);

	createTask(taskText);

	// sets the layout to the class
	TaskLayout::setLayout(horLayout);
}

TaskLayout::~TaskLayout()
{
}

QCheckBox* TaskLayout::getCheckBox()
{
	return checkBox;
}

void TaskLayout::createTask(QString text)
{
	textBox->setReadOnly(true);
	textBox->setPlainText(text);
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	textBox->setSizePolicy(sizePolicy);
	textBox->setMaximumSize(QSize(16777215, 50));
	textBox->setStyleSheet(QString::fromUtf8(
		"QPlainTextEdit{\n"
		"border: 1.5px solid rgb(55, 55, 55);\n"
		"background-color: rgb(255, 255, 255);\n"
		"width: 5px;\n"
		"border-radius: 8px;\n"
		"}\n"

		"QScrollBar:vertical{"
		"		border: 1px solid #999999;"
		"		background:white;"
		"		width:3px;"
		"		margin: 0px 0px 0px 0px;"
		"}"
		"QScrollBar::handle:vertical{"
		"	background: black;"
		"	min - height: 0px;"
		"}"
		"	QScrollBar::add - line : vertical{"
		"		background: black;"
		"		height: 0px;"
		"		subcontrol - position: bottom;"
		"		subcontrol - origin: margin;"
		"}"
		"	QScrollBar::sub - line:vertical{"
		"		background: black;"
		"		subcontrol - position: top;"
		"		subcontrol - origin: margin;"
		"}"
	));

	checkBox->setObjectName(QString::fromUtf8("checkBox"));
	checkBox->setText("");

	horLayout->addWidget(textBox);
	horLayout->addWidget(checkBox);
}
