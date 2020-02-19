#include "Todo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Todo w;
	w.show();
	return a.exec();
}
