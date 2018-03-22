#pragma once
#include "segment.h"

segment::segment(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lineEdit_Dict->setReadOnly(true);
	ui.lineEdit_Input->setReadOnly(true);
	ui.lineEdit_Output->setReadOnly(true);
	ui.textEdit_out->setReadOnly(true);
	ui.dict_text->setReadOnly(true);
	ui.checkBox_3->setCheckState(Qt::Checked);
	ui.checkBox->setCheckState(Qt::Checked);

	connect(ui.DictBrowser, SIGNAL(clicked()), this, SLOT(BrowseDict()));
	connect(ui.InputBrowser, SIGNAL(clicked()), this, SLOT(BrowseFile()));
	connect(ui.OutBrowser, SIGNAL(clicked()), this, SLOT(SaveFile()));
	connect(ui.lineEdit_Dict, SIGNAL(textChanged(const QString &)), this, SLOT(CheckDictEncode()));
	connect(ui.lineEdit_Input, SIGNAL(textChanged(const QString &)), this, SLOT(CheckFileEncode()));
	connect(ui.comboBox_Dict, SIGNAL(currentIndexChanged(int)), this, SLOT(SetDictEncode()));
	connect(ui.comboBox_Input, SIGNAL(currentIndexChanged(int)), this, SLOT(SetFileEncode()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(Run()));
	connect(ui.action_about, SIGNAL(triggered()), this, SLOT(ShowAbout()));
	connect(ui.action_help, SIGNAL(triggered()), this, SLOT(ShowHelp()));
	connect(ui.pushButton_search, SIGNAL(clicked()), this, SLOT(Search()));
	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(Add()));
	connect(ui.pushButton_del, SIGNAL(clicked()), this, SLOT(Del()));
	connect(ui.action_2, SIGNAL(triggered()), this, SLOT(BrowseDict()));
	connect(ui.action_3, SIGNAL(triggered()), this, SLOT(BrowseFile()));
	connect(ui.action_4, SIGNAL(triggered()), this, SLOT(SaveFile()));
	connect(ui.action_6, SIGNAL(triggered()), this, SLOT(LoadDict()));
	connect(ui.action_7, SIGNAL(triggered()), this, SLOT(Run()));
	connect(ui.action_8, SIGNAL(triggered()), this, SLOT(DisplayDict()));
	connect(ui.pushButton_runTest, SIGNAL(clicked()), this, SLOT(RunTest()));
	connect(ui.pushButton_cls, SIGNAL(clicked()), this, SLOT(ClearText()));
	connect(ui.comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(SetDict()));
	connect(ui.radioButton, SIGNAL(clicked()), this, SLOT(SetMM()));
	connect(ui.radioButton_2, SIGNAL(clicked()), this, SLOT(SetMP()));
	connect(ui.radioButton_3, SIGNAL(clicked()), this, SLOT(SetMax()));
	this->dict = new Dict();
	char* curDirectory = (char*)malloc(80 * sizeof(char));
	curDirectory = _getcwd(curDirectory, 80 * sizeof(char));
	strcat(curDirectory, "\\dict");
	wchar_t *ws = Decode::AnsiToUnicode(curDirectory);
	ui.lineEdit_Dict->setText(QString::fromWCharArray(ws));
	setWindowIcon(QIcon(":/images/Resources/1.ico"));
}


segment::~segment() {
	delete this->dict;
}

//slots
void segment::ShowAbout() {
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setWindowTitle(tr(u8"关于软件"));
	box.setText(tr(u8"这是一个简易的中文分词器。\n作者：IS1602冯冠云"));
	box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
	box.exec();
}

void segment::ShowHelp() {
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setWindowTitle(tr(u8"使用帮助"));
	box.setText(tr(u8"抱歉暂时没有使用帮助哦!"));
	box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
	box.exec();
}

void segment::BrowseFile() {
	QString path = QFileDialog::getOpenFileName(this, tr(u8"请选择待分词文本"), ".", tr(u8"文本文档(*.txt);;所有文件(*)"));
	ui.lineEdit_Input->setText(path);
}

void segment::BrowseDict() {
	QString path = QFileDialog::getOpenFileName(this, tr(u8"请选择词典"), ".", tr(u8"所有文件(*)"));
	ui.lineEdit_Dict->setText(path);
}

void segment::SaveFile() {
	QString name;
	if (!ui.lineEdit_Input->text().isEmpty()) {
		QStringList List = ui.lineEdit_Input->text().split('\\');
		name = List[List.size() - 1] + "_out";
	}
	else  name = tr("out");
	QString path = QFileDialog::getSaveFileName(this, tr(u8"选择输出文件路径"), name, tr(u8"文本文档 (*.txt)"));
	ui.lineEdit_Output->setText(path);
}

void segment::CheckDictEncode() {
	if (ui.lineEdit_Dict->text().isEmpty()) return;
	Encode encoding = UNKNOWN;
	if (ui.comboBox_Dict->currentIndex() == 0) {
		encoding = Decode::CheckEncode(ui.lineEdit_Dict->text().toLatin1().data());
		if (encoding == UNICODE_BIG_ENDIAN) {
			this->dict_encode = UCS2_BE;
			ui.label_dict->setText(tr("UCS2_BE"));
		}
		else if (encoding == UNICODE_LITTLE_ENDIAN) {
			this->dict_encode = UCS2_LE;
			ui.label_dict->setText(tr("UCS2_LE"));
		}
		else if (encoding == UTF_8_BOM) {
			this->dict_encode = UTF_8_BOM;
			ui.label_dict->setText(tr("UTF_8_BOM"));
		}
		else if (encoding == UTF_8) {
			this->dict_encode = UTF_8;
			ui.label_dict->setText(tr("UTF_8_NO_BOM"));
		}
		else if (encoding == ANSI) {
			this->dict_encode = ANSI;
			ui.label_dict->setText(tr("ANSI"));
		}
		else {
			QMessageBox box(QMessageBox::Warning, u8"无法检测到编码!", u8"无法自动检测出字典的编码，\n请手动选择编码方式");
			box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
			box.exec();
		}
	}
	else {
		encoding = Decode::CheckEncode(ui.lineEdit_Dict->text().toLatin1().data());
		if ((encoding != UNKNOWN) && (encoding != ui.comboBox_Dict->currentIndex() - 1)) {
			QMessageBox box(QMessageBox::Warning, u8"警告!", u8"您选择的编码与系统检测到的编码不匹配，\n是否使用系统检测到的编码？");
			box.addButton(tr(u8"是"), QMessageBox::AcceptRole);
			box.addButton(tr(u8"否"), QMessageBox::RejectRole);
			int ret = box.exec();
			if (ret == QMessageBox::AcceptRole) {
				ui.comboBox_Dict->setCurrentIndex(0);
				segment::CheckDictEncode();
			}
			else {
				if (ui.comboBox_Dict->currentIndex() == 2) {
					this->dict_encode = UCS2_BE;
					ui.label_dict->setText(tr("UCS2_BE"));
				}
				else if (ui.comboBox_Dict->currentIndex() == 3) {
					this->dict_encode = UCS2_LE;
					ui.label_dict->setText(tr("UCS2_LE"));
				}
				else if (ui.comboBox_Dict->currentIndex() == 5) {
					this->dict_encode = UTF_8_BOM;
					ui.label_dict->setText(tr("UTF_8_BOM"));
				}
				else if (ui.comboBox_Dict->currentIndex() == 4) {
					this->dict_encode = UTF_8;
					ui.label_dict->setText(tr("UTF_8_NO_BOM"));
				}
				else if (ui.comboBox_Dict->currentIndex() == 1) {
					this->dict_encode = ANSI;
					ui.label_dict->setText(tr("ANSI"));
				}
			}
		}
	}
}

void segment::CheckFileEncode() {
	if (ui.lineEdit_Input->text().isEmpty()) return;
	Encode encoding = UNKNOWN;
	if (ui.comboBox_Input->currentIndex() == 0) {
		encoding = Decode::CheckEncode(ui.lineEdit_Input->text().toLatin1().data());
		if (encoding == UNICODE_BIG_ENDIAN) {
			this->in_encode = UCS2_BE;
			ui.label_input->setText(tr("UCS2_BE"));
		}
		else if (encoding == UNICODE_LITTLE_ENDIAN) {
			this->in_encode = UCS2_LE;
			ui.label_input->setText(tr("UCS2_LE"));
		}
		else if (encoding == UTF_8_BOM) {
			this->in_encode = UTF_8_BOM;
			ui.label_input->setText(tr("UTF_8_BOM"));
		}
		else if (encoding == UTF_8) {
			this->in_encode = UTF_8;
			ui.label_input->setText(tr("UTF_8_NO_BOM"));
		}
		else if (encoding == ANSI) {
			this->in_encode = ANSI;
			ui.label_input->setText(tr("ANSI"));
		}
		else {
			QMessageBox box(QMessageBox::Warning, u8"无法检测到编码!", u8"无法自动检测出字典的编码，\n请手动选择编码方式");
			box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
			box.exec();
		}
	}
	else {
		encoding = Decode::CheckEncode(ui.lineEdit_Input->text().toLatin1().data());
		if ((encoding != UNKNOWN)&&(encoding != ui.comboBox_Input->currentIndex() - 1)) {
			QMessageBox box(QMessageBox::Warning, u8"警告!", u8"您选择的编码与系统检测到的编码不匹配，\n是否使用系统检测到的编码？");
			box.addButton(tr(u8"是"), QMessageBox::AcceptRole);
			box.addButton(tr(u8"否"), QMessageBox::RejectRole);
			int ret = box.exec();
			if (ret == QMessageBox::AcceptRole) {
				ui.comboBox_Input->setCurrentIndex(0);
				segment::CheckFileEncode();
			}
			else {
				if (ui.comboBox_Input->currentIndex() == 2) {
					this->in_encode = UCS2_BE;
					ui.label_input->setText(tr("UCS2_BE"));
				}
				else if (ui.comboBox_Input->currentIndex() == 3) {
					this->in_encode = UCS2_LE;
					ui.label_input->setText(tr("UCS2_LE"));
				}
				else if (ui.comboBox_Input->currentIndex() == 5) {
					this->in_encode = UTF_8_BOM;
					ui.label_input->setText(tr("UTF_8_BOM"));
				}
				else if (ui.comboBox_Input->currentIndex() == 4) {
					this->in_encode = UTF_8;
					ui.label_input->setText(tr("UTF_8_NO_BOM"));
				}
				else if (ui.comboBox_Input->currentIndex() == 1) {
					this->in_encode = ANSI;
					ui.label_input->setText(tr("ANSI"));
				}
			}
		}
	}
}

void segment::SetDictEncode() {
	this->dict->SetUnloaded();
	if (ui.comboBox_Dict->currentText() == tr("AUTO"))
		segment::CheckDictEncode();
	else if (ui.comboBox_Dict->currentText() == tr("ANSI")) {
		this->dict_encode = ANSI;
		ui.label_dict->setText(tr("ANSI"));
	}
	else if (ui.comboBox_Dict->currentText() == tr("UCS2_LE")) {
		this->dict_encode = UCS2_LE;
		ui.label_dict->setText(tr("UCS2_LE"));
	}
	else if (ui.comboBox_Dict->currentText() == tr("UCS2_BE")) {
		this->dict_encode = UCS2_BE;
		ui.label_dict->setText(tr("UCS2_BE"));
	}
	else if (ui.comboBox_Dict->currentText() == tr("UTF-8")) {
		this->dict_encode = UTF_8;
		ui.label_dict->setText(tr("UTF-8"));
	}
	else if (ui.comboBox_Dict->currentText() == tr("UTF-8-BOM")) {
		this->dict_encode = UTF_8_BOM;
		ui.label_dict->setText(tr("UTF-8-BOM"));
	}
	else return;
}

void segment::SetFileEncode() {
	if (ui.comboBox_Input->currentText() == tr("AUTO"))
		segment::CheckFileEncode();
	else if (ui.comboBox_Input->currentText() == tr("ANSI")) {
		this->in_encode = ANSI;
		ui.label_input->setText(tr("ANSI"));
	}
	else if (ui.comboBox_Input->currentText() == tr("UCS2_LE")) {
		this->in_encode = UCS2_LE;
		ui.label_input->setText(tr("UCS2_LE"));
	}
	else if (ui.comboBox_Input->currentText() == tr("UCS2_BE")) {
		this->in_encode = UCS2_BE;
		ui.label_input->setText(tr("UCS2_BE"));
	}
	else if (ui.comboBox_Input->currentText() == tr("UTF-8")) {
		this->in_encode = UTF_8;
		ui.label_input->setText(tr("UTF-8"));
	}
	else if (ui.comboBox_Input->currentText() == tr("UTF-8-BOM")) {
		this->in_encode = UTF_8_BOM;
		ui.label_input->setText(tr("UTF-8-BOM"));
	}
	else return;
}

void segment::Run() {
	if (ui.lineEdit_Dict->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"尚未选择要使用的字典，\n请先选择字典文件!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		segment::BrowseDict();
	}
	if (ui.lineEdit_Input->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"尚未选择要分词的文件，\n请先选择待分词的文件!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		segment::BrowseFile();
	}
	if (ui.lineEdit_Output->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"请选择输出文件保存路径!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		segment::SaveFile();
	}
	segment::LoadDict();
 	QFile fileIn(ui.lineEdit_Input->text());
	wchar_t *ws = NULL;
	FILE *dictfile = SegCore::Myfopen(ui.lineEdit_Dict->text().toLatin1().data(), "rb", this->dict_encode);
	int i = 0;
	while (SegCore::MyReadLine(dictfile, ws, this->dict_encode) != -1) {
		QString str = QString::fromWCharArray(ws);
		ui.dict_text->append(str);
		i++;
		if (i > 1000) break;
	}
	if (!fileIn.exists()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"文件不存在!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		return;
	}
	QFile fileOut(ui.lineEdit_Output->text());
	segment::ReadFile();
	return;
}

void segment::Search() {
	wchar_t *ws = (wchar_t*)malloc(20*sizeof(wchar_t));
	ui.search_word->text().toWCharArray(ws);
	int len = ui.search_word->text().size();
	ws[len] = '\0';
	WordInfo info = this->dict->tree.Search(Decode::UnicoToVec(ws));
	if (info.freq == 0) {
		ui.label_20->setText(u8"不存在");
		ui.label_21->setText("0");
		ui.label_22->setText("*");
		if (ui.del_word->text().isEmpty()) {
			QMessageBox box;
			box.setIcon(QMessageBox::Information);
			box.setWindowTitle(tr(u8"查询失败"));
			box.setText(tr(u8"未查询到该单词"));
			box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
			box.exec();
		}
	}
	else {
		ui.label_20->setText(u8"存在");
		ui.label_21->setText(QString::number(info.freq));
		ui.label_22->setText(QString::fromLatin1(info.prop));
	}
	free(ws);
}

void segment::Add() {
	WordInfo info;
	if (ui.add_word->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"添加失败"));
		box.setText(tr(u8"先填写要添加的单词!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
	}
	wchar_t *ws = (wchar_t*)malloc(20 * sizeof(wchar_t));
	ui.add_word->text().toWCharArray(ws);
	int len = ui.add_word->text().size();
	ws[len] = '\0';
	info.vacab = Decode::UnicoToVec(ws);
	free(ws);
	if (ui.add_freq->text().isEmpty())
		info.freq = 1;
	else
		info.freq = ui.add_freq->text().toInt();
	if(ui.add_prop->text().isEmpty())
		strcpy(info.prop, "*");
	else
		strcpy(info.prop, ui.add_prop->text().toLatin1().data());
	this->dict->tree.InsertVacab(info);
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setWindowTitle(tr(u8"添加成功"));
	box.setText(tr(u8"是否将修改写入字典文件?"));
	box.addButton(tr(u8"修改"), QMessageBox::AcceptRole);
	box.addButton(tr(u8"不修改"), QMessageBox::RejectRole);
	int ret = box.exec();
	if (ret == QMessageBox::AcceptRole) {
		FILE* fp = fopen(ui.lineEdit_Dict->text().toLatin1().data(), "ab");
		QString infoStr = ui.add_word->text() + " "
			+ ui.add_freq->text() + " "
			+ ui.add_prop->text();
		int localLen = ui.add_word->text().size();
		localLen += ui.add_prop->text().size();
		localLen += ui.add_freq->text().size();
		localLen += 2;
		wchar_t *wStr = (wchar_t*)malloc(50 * sizeof(wchar_t));
		infoStr.toWCharArray(wStr);
		wStr[localLen] = '\0';
		if (this->dict_encode == ANSI) {
			char *str = Decode::UnicodeToAnsi(wStr);
			fwrite(str, 1, strlen(str), fp);
			strcpy(str, "\r\n");
			fwrite(str, 1, 2, fp);
		}
		else if (this->dict_encode == UTF_8 || this->dict_encode == UTF_8_BOM) {
			char *str = Decode::UnicodeToUTF8(wStr);
			fwrite(str, 1, strlen(str), fp);
			strcpy(str, "\r\n");
			fwrite(str, 1, 2, fp);
			free(str);
		}
		else if (this->dict_encode == UCS2_LE) {
			fwrite(wStr, 2, wcslen(wStr), fp);
			wcscpy(wStr, L"\r\n");
			fwrite(wStr, 2, 2, fp);
		}
		else if (this->dict_encode == UCS2_LE) {
			unsigned i = 0, j = 0;
			char tmpStr[100];
			for (i = 0, j = 0; i < wcslen(wStr); i++) {
				tmpStr[j++] = (char)((wStr[i]>>8)&0x00ff);
				tmpStr[j++] = (char)(wStr[i] & 0x00ff);
			}
			fwrite(tmpStr, 1, j - 1, fp);
			wcscpy(wStr, L"\r\n");
			fwrite(wStr, 2, 2, fp);
		}
		fclose(fp);
	}
	ui.add_freq->clear();
	ui.add_prop->clear();
	ui.add_word->clear();
}

void segment::Del() {
	if (ui.del_word->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"先填写要添加的单词!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
	}
	wchar_t *ws = (wchar_t*)malloc(20 * sizeof(wchar_t));
	ui.del_word->text().toWCharArray(ws);
	int len = ui.del_word->text().size();
	ws[len] = '\0';
	this->dict->tree.DeletVacab(Decode::UnicoToVec(ws));
	free(ws);
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setWindowTitle(tr(u8"删除成功"));
	box.setText(tr(u8"已经成功删除该词。"));
	box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
	box.exec();
	ui.del_word->clear();
}

void segment::DisplayDict() {
	QMessageBox box;
	box.setIcon(QMessageBox::Information);
	box.setWindowTitle(tr(u8"警告!"));
	box.setText(tr(u8"编译器环境下文件太大会卡死的!"));
	box.addButton(tr(u8"继续"), QMessageBox::AcceptRole);
	box.addButton(tr(u8"取消"), QMessageBox::RejectRole);
	int ret = box.exec();
	if (ret == QMessageBox::RejectRole) return;
	this->dict->LoadDict(this->dict_encode);
	if (!this->dict->IsLoaded()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"尚未加载字典，\n请加载字典文件!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		this->dict->LoadDict(this->dict_encode);
	}
	if (ui.lineEdit_Dict->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"尚未选择要使用的字典，\n请先选择字典文件!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		segment::BrowseDict();
	}
	wchar_t *ws = NULL;
	FILE *dictfile = SegCore::Myfopen(ui.lineEdit_Dict->text().toLatin1().data(), "rb", this->dict_encode);
	if (!dictfile) return;
	int i = 0;
	while (SegCore::MyReadLine(dictfile, ws, this->dict_encode) != -1) {
		QString str = QString::fromWCharArray(ws);
		ui.dict_text->append(str);
		i++;
		if (i > this->dict->tree.numb) break;
	}
	ui.label_11->setText("当前使用字典");
}

void segment::LoadDict() {
	if (this->dict->IsLoaded()) return;
	if (ui.lineEdit_Dict->text().isEmpty()) {
		QMessageBox box;
		box.setIcon(QMessageBox::Information);
		box.setWindowTitle(tr(u8"警告!"));
		box.setText(tr(u8"尚未选择要使用的字典，\n请先选择字典文件!"));
		box.addButton(tr(u8"确定"), QMessageBox::AcceptRole);
		box.exec();
		segment::BrowseDict();
	}
	this->dict->tree.DeletNode(this->dict->tree.root);
	this->dict->tree.root = new TrieNode();
	this->dict->tree.numb = 0;
	this->dict->SetDictPath(ui.lineEdit_Dict->text().toLatin1().data());
	this->dict->dictEncode = this->dict_encode;
	this->dict->LoadDict(dict->dictEncode);
}

void segment::RunTest() {
	segment::LoadDict();
	ui.textEdit_out->clear();
	wchar_t ws[20480];
	QString tmpStr = ui.textEdit_input->toPlainText();
	tmpStr.toWCharArray(ws);
	vector<wchar_t> puncs;
	vector<vector<unsigned short>> wsList = SegCore::MySplit(ws, puncs);
	for (unsigned i = 0; i < wsList.size();i++) {
		if(this->SegMode == 0)
			SegCore::MMSeg(wsList[i], this->dict, ws);
		else if(this->SegMode ==1)
			SegCore::MPSeg(wsList[i], this->dict, ws);
		else
			SegCore::MaxSeg(wsList[i], this->dict, ws);
		tmpStr = QString::fromWCharArray(ws);
		ui.textEdit_out->append(tmpStr);
	}
}

void segment::ClearText() {
	ui.textEdit_input->clear();
	ui.textEdit_out->clear();
}

//
void segment::ReadFile() {
	FILE *Infile = SegCore::Myfopen(ui.lineEdit_Input->text().toLatin1().data(), "rb", this->in_encode);
	FILE *Outfile = SegCore::Myfopen(ui.lineEdit_Output->text().toLatin1().data(), "wb", UCS2_LE);
	wchar_t *line = NULL, ws[1024];
	char tmpStr[5];
	vector<wchar_t> puncs;
	vector<unsigned short> tmpWs;
	tmpWs.clear();
	while (SegCore::MyReadLine(Infile, line, this->in_encode) != -1) {
		vector<vector<unsigned short>> wsList = SegCore::MySplit(line, puncs);
		if (wsList.size() > 0) {
			if (!tmpWs.empty())
				wsList[0].insert(wsList[0].begin(), tmpWs.begin(), tmpWs.end());
			if (puncs.size() < wsList.size()) {
				for (unsigned i = 0; i < wsList.size() - 1; i++) {
					//segment::Cut(wsList[i], Outfile);
					SegCore::MMSeg(wsList[i], this->dict, ws);
					fwrite(ws, 2, wcslen(ws), Outfile);
					tmpStr[0] = (char)(puncs[i] & 0x00ff);
					tmpStr[1] = (char)((puncs[i] >> 8) & 0x00ff);
					fwrite(tmpStr, 1, 2, Outfile);
					tmpStr[0] = 0x0a; tmpStr[1] = 0;
					fwrite(tmpStr, 1, 2, Outfile);
					tmpStr[0] = 0x0d; tmpStr[1] = 0;
					fwrite(tmpStr, 1, 2, Outfile);
				}
				tmpWs = wsList[wsList.size() - 1];
			}
			else {
				for (unsigned i = 0; i < wsList.size() - 1; i++) {
					SegCore::MMSeg(wsList[i], this->dict, ws);
					fwrite(ws, 2, wcslen(ws), Outfile);
					tmpStr[0] = (char)(puncs[i] & 0x00ff);
					tmpStr[1] = (char)((puncs[i] >> 8) & 0x00ff);
					fwrite(tmpStr, 1, 2, Outfile);
					tmpStr[0] = 0x0a; tmpStr[1] = 0;
					fwrite(tmpStr, 1, 2, Outfile);
					tmpStr[0] = 0x0d; tmpStr[1] = 0;
					fwrite(tmpStr, 1, 2, Outfile);
				}
				tmpWs.clear();
			}
		}
		tmpStr[0] = 0x0a; tmpStr[1] = 0;
		fwrite(tmpStr, 1, 2, Outfile);
		tmpStr[0] = 0x0d; tmpStr[1] = 0;
		fwrite(tmpStr, 1, 2, Outfile);
	}
	fclose(Infile);
	fclose(Outfile);
	return;
}

void segment::SetDict() {
	if (ui.comboBox_3->currentIndex() == 0) {
		char* curDirectory = (char*)malloc(80 * sizeof(char));
		curDirectory = _getcwd(curDirectory, 80 * sizeof(char));
		strcat(curDirectory, "\\dict");
		wchar_t *ws = Decode::AnsiToUnicode(curDirectory);
		ui.lineEdit_Dict->setText(QString::fromWCharArray(ws));
	}
	else if (ui.comboBox_3->currentIndex() == 1) {
		char* curDirectory = (char*)malloc(80 * sizeof(char));
		curDirectory = _getcwd(curDirectory, 80 * sizeof(char));
		strcat(curDirectory, "\\dict.small");
		wchar_t *ws = Decode::AnsiToUnicode(curDirectory);
		ui.lineEdit_Dict->setText(QString::fromWCharArray(ws));
	}
	else {
		char* curDirectory = (char*)malloc(80 * sizeof(char));
		curDirectory = _getcwd(curDirectory, 80 * sizeof(char));
		strcat(curDirectory, "\\dict.big");
		wchar_t *ws = Decode::AnsiToUnicode(curDirectory);
		ui.lineEdit_Dict->setText(QString::fromWCharArray(ws));
	}
}


		{
			qreal sizef = font.pointSizeF();
			font.setPointSizeF(sizef* fontscale);
		}
		else
		{
			font.setPixelSize(fontsize * fontscale * 90 / 72); // 设置字体大小为像素大小，而不是以磅为单位,
		}
	}
	else
	{
		font.setPixelSize(fontsize * fontscale);
	}

	ui.label->setFont(font);
}