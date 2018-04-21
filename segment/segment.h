#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_segment.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <QTextCodec>
#include<stdio.h>
#include<assert.h>
#include "HelpWidget.h"
#include "Dict.h"
#include "SegCore.h"
#include "Decode.h"
using namespace std;


class segment : public QMainWindow
{
	Q_OBJECT

public:
	segment(QWidget *parent = Q_NULLPTR);
	~segment();
	void ReadFile();

	public slots:
	void ShowAbout();
	void ShowHelp();
	//
	void BrowseFile();
	void BrowseDict();
	void SaveFile();
	//
	void CheckDictEncode();
	void CheckFileEncode();
	void SetDictEncode();
	void SetFileEncode();
	//
	void Search();
	void Add();
	void Del();
	//
	void DisplayDict();
	void LoadDict();
	void SetDict();
	//
	void Run();
	//
	void RunTest();
	void ClearText();
	//
	void SetMM() { 
		this->SegMode = 0;
	}
	void SetMP() {
		this->SegMode = 1;
	}
	void SetMax() {
		this->SegMode = 2;
	}
	
private:
	Ui::segmentClass ui;
	Dict *dict;
	Encode dict_encode = UNKNOWN;
	Encode in_encode = UNKNOWN;
	int SegMode = 0;
};
