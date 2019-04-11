#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sdrcloud.h"

class SDRCloud : public QMainWindow
{
	Q_OBJECT

public:
	SDRCloud(QWidget *parent = Q_NULLPTR);

private:
	Ui::SDRCloudClass ui;
};
