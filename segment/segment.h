#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_segment.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <QTextCodec>
#include <qdebug.h>
#include "Defination.h"
#include "Dict.h"
#include "MyFunc.h"
#include "Decode.h"


class segment : public QMainWindow
{
	Q_OBJECT

public:
	segment(QWidget *parent = Q_NULLPTR);
	~segment();
	void ReadFile();
	void Cut(vector<word> sentence, FILE* Outfile);
	QString CutTest(vector<word> sentence);

	public slots:
	void ShowAbout();
	void ShowHelp();
	void BrowseFile();
	void BrowseDict();
	void SaveFile();
	void CheckDictEncode();
	void CheckFileEncode();
	void SetDictEncode();
	void SetFileEncode();
	void SetOutputEncode();
	void Run();
	void Search();
	void Add();
	void Del();
	void DisplayDict();
	void LoadDict();
	void RunTest();
	void ClearText();
	
private:
	Ui::segmentClass ui;
	Dict *dict;
	Encode dict_encode = UNKNOWN;
	Encode in_encode = UNKNOWN;
	Encode out_encode = UCS2_LE;
	vector<vector<unsigned>> dag;
	vector<unsigned> rout;
};
