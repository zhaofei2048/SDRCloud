#include "sdrcloud.h"
#include <QtWidgets/QApplication>
#include "qcustomplot.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SDRCloud w;

	w.show();
	w.myTimer->start(1);
	w.test();
	return a.exec();
}