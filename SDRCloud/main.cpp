#include "sdrcloud.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SDRCloud w;
	w.show();
	return a.exec();
}
