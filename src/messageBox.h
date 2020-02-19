#pragma once
#include <string>
#include <QMessageBox>
#include <qdebug.h>

// just a styled messageBox
int styledQuestionMessageBox(std::string title, std::string description, std::string styleSheets, QMessageBox::StandardButtons buttons);
