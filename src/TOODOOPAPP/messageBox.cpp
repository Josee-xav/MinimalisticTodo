#include "messageBox.h"


int styledQuestionMessageBox(std::string title, std::string description, std::string styleSheets, QMessageBox::StandardButtons buttons)
{
	QMessageBox msgBox(QMessageBox::NoIcon, title.c_str(), description.c_str());
	msgBox.setStyleSheet(styleSheets.c_str());
	msgBox.setStandardButtons(buttons);
	int ret = msgBox.exec();
	msgBox.show();

	return msgBox.result();
}
