#pragma once
#include"ui_HelpWidget.h"
#include<QWidget>

class HelpWidget: public QWidget
{
	Q_OBJECT

public:
	HelpWidget(QWidget *parent = 0);
	~HelpWidget();

private:
	Ui::HelpWidget ui;
};

