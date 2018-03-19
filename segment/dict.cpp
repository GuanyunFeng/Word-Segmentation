#include "Dict.h"

//默认字典为当前目录下的"dict.txt"
Dict::Dict(){
	char* curDirectory = (char*)malloc(80 * sizeof(char));
	curDirectory = _getcwd(curDirectory, 80 * sizeof(char));
	strcat(curDirectory, "\\dict");
	this->dictPath = curDirectory;
	free(curDirectory);
	//this->dictEncode = Decode::CheckEncode((char*)(this->dictPath).c_str());
}

Dict::Dict(char *path) {
	this->dictPath = path;
}


Dict::~Dict() {}


//设置字典路径
bool Dict::SetDictPath(char *dictPath) {
	if (dictPath == NULL) return false; //如果路径字符串为空，则返回false
	else {
		this->dictPath = dictPath;
		return true;
	}
}


//从文本中读出一个单词的信息，封装成WordInfo返回
Info Dict::ReadVacab(FILE* fp, Encode encoding) {
	Info info;
	if (encoding == ANSI || encoding == UTF_8 || encoding == UTF_8_BOM) {
		char str1[128], str2[33], str3[5];
		if (fscanf(fp, "%s%s%s", str1, str2, str3) == -1)
			return info;
		string tmpStr = str1;
		info.vacab = Dict::ToUnicode(tmpStr, encoding);
		for (int i = strlen(str2) - 1, j = 1; i >= 0; i--, j *= 10)
			info.freq += (str2[i] - 48) * j;
		strcpy(info.prop, str3);
	}
	else if (encoding == UCS2_BE ) {
		char tmp[20];
		int i = 0, j = 1;
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[0] == 0 && tmp[1] == 0x20) break;
			word high = tmp[0] & 0x00ff;;
			word low = tmp[1] & 0x00ff;;
			info.vacab.push_back((high << 8) | low);
		}
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[0] == 0 && tmp[1] == 0x20) break;
			info.freq += (tmp[1] - 48)*j;
			j *= 10;
		}
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[0] == 0 && tmp[1] == 0x0d) {
				fread(tmp, 1, 2, fp);
				break;
			}
			if (tmp[0] == 0 && tmp[1] == 0x0a) break;
			info.prop[i++] = tmp[1];
		}
		info.prop[i] = '\0';
	}
	else if (encoding == UCS2_LE) {
		char tmp[20];
		int i = 0, j = 1;
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[0] == 0 && tmp[1] == 0x20) break;
			word high = tmp[1] & 0x00ff;;
			word low = tmp[0] & 0x00ff;;
			info.vacab.push_back((high << 8) | low);
		}
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[1] == 0 && tmp[0] == 0x20) break;
			info.freq += (tmp[0] - 48)*j;
			j *= 10;
		}
		while (fread(tmp, 1, 2, fp) > 0) {
			if (tmp[1] == 0 && tmp[0] == 0x0d) {
				fread(tmp, 1, 2, fp);
				break;
			}
			if (tmp[1] == 0 && tmp[0] == 0x0a) break;
			info.prop[i++] = tmp[0];
		}
		info.prop[i] = '\0';
	}
	return info;
}
	

//字典加载到Trie树中
bool Dict::LoadDict(Encode encoding) {
	Info info;
	FILE* fp = fopen(this->dictPath.c_str(), "rb");
	if (!fp) return false;

	//跳过前导符
	if (encoding == UTF_8_BOM)
		fseek(fp, 3L, SEEK_SET);
	else if ((encoding == UCS2_BE) || (encoding == UCS2_LE))
		fseek(fp, 2L, SEEK_SET);

	while ((info = this->ReadVacab(fp, encoding)).freq) {
		if (!this->tree.InsertVacab(info)) return false;
		this->total += info.freq;
	}
	fclose(fp);
	this->loaded = true;
	return true;
}


//添加单词到Trie树中，并做标记。在保存的时候对字典进行写入。
bool Dict::AddVacab(Info info){
	//Trie树中已经存在
	if(!tree.InsertVacab(info)) return false;
	this->added.push_back(info);
	return true;
}


//在Trie树中删除单词，并做标记。在保存的时候对字典进行修改。
bool Dict::DelVacab(string str, Encode encoding) {
	vector<word> v1 = Dict::ToUnicode(str, encoding);
	if (!v1.size()) return false;
	if (!tree.DeletVacab(v1)) return false;
	this->deleted.push_back(str);
	return true;
}


//保存，对字典进行修改。
bool Dict::SaveChange() {
	FILE* fp;
	char str[20];
	vector<int> vec;
	if ((fp = fopen(this->dictPath.c_str(), "a+")) == NULL)
		return false;

	if (this->added.size() > 0) {
		vector<Info>::iterator it = this->added.begin();
		while (it != this->added.end()) {
			strcpy(str, Dict::ToString(it->vacab, this->dictEncode).c_str());
			fputs(str, fp); fputc(' ', fp);
			for (int i = it->freq, j = 10; i; i = i / j, j *= 10)
				vec.push_back(i%j);
			for (int i = 0; i < vec.size(); i++)
				str[i] = (char)(vec[vec.size() - i - 1] + 48);
			str[vec.size()] = '\0';
			fputs(str, fp); fputc(' ', fp);
			strcpy(str, it->prop);
			fputs(str, fp); fputc('\r', fp); fputc('\n', fp);
		}
	}

	if (this->deleted.size() > 0) {
		fseek(fp, 0, SEEK_SET);
		//真麻烦，不想实现了,你踏马没事删个锤子啊删，叫你删，叫你删
	}

	fclose(fp);
	return true;
}


//查找单词，返回Info信息。未找到则Info.freq=0。
Info Dict::SearchDict(string str, Encode encoding){
	vector<word> vacab = Dict::ToUnicode(str, encoding);
	return this->tree.Search(vacab);
}


//设置字典解码方式
bool Dict::SetEncode(Encode encoding){
	if (encoding<0 || encoding>4)
		return false;
	this->dictEncode = encoding;
	return true;
}

//把字符串转成Unicode编码，存储在vector中返回
vector<word> Dict::ToUnicode(string str, Encode encoding) {
	vector<word> tmpUnico;
	tmpUnico.clear();

	//字符串为UTF-8编码
	if (encoding == UTF_8 || encoding == UTF_8_BOM) {
		for (unsigned i = 0; i < str.size(); ) {
			//第一个字节小于127(即字节以0开头),为ASCII字符
			if (!(str[i] & 0x80)) tmpUnico.push_back(str[i++]);
			//第一个字节形式为0x110*****,那么UTF8编码占2字节
			else if ((byte)str[i] < 0xDF && i + 1 < str.size()) {
				word tmpL = (str[i] & 0x1F); tmpL = tmpL << 6;
				word tmpR = str[i + 1] & 0x3F;
				tmpUnico.push_back(tmpL | tmpR);
				i += 2;
			}
			//第一个字节形式为0x1110****,那么UTF8编码占3个字节
			else if ((byte)str[i] < 0xEF && i + 2 < str.size()) {
				word tmpL = (str[i] & 0x0F); tmpL = tmpL << 12;
				word tmpM = (str[i + 1] & 0x3F); tmpM = tmpM << 6;
				word tmpR = str[i + 2] & 0x3F;
				tmpUnico.push_back(tmpL | tmpM | tmpR);
				i += 3;
			}
			//不属于Unicode跳过相应字节(属于UCS-4 编码，重新规范后，这些字节值无法出现在合法 UTF-8序列中)
			else if ((byte)str[i] <= 0xF7)	i += 4;
			else if ((byte)str[i] <= 0xFB) i += 5;
			else if ((byte)str[i] <= 0xFD) i += 6;
			else return tmpUnico; //exit(-1);
		}
	}

	//字符串为Unicode_little_endian编码
	if (encoding == UNICODE_LITTLE_ENDIAN) {
		for (unsigned i = 0; i < str.size(); i += 2) {
			word high = str[i + 1];
			high = (high & 0x00FF) << 8;
			word low = str[i];
			low = low & 0x00FF;
			tmpUnico.push_back(high | low);
		}
	}

	//字符串为Unicode_big_endian编码
	if (encoding == UNICODE_BIG_ENDIAN) {
		for (unsigned i = 0; i < str.size(); i += 2) {
			word high = str[i];
			high = (high & 0x00FF) << 8;
			word low = str[i + 1];
			low = low & 0x00FF;
			tmpUnico.push_back(high | low);
		}
	}

	//ANSI编码。
	if (encoding == ANSI)
		tmpUnico = Decode::AnsiToVec(str.c_str());

	return tmpUnico;
}


//把vector中存储的词转换成相应编码的字符串返回
string Dict::ToString(vector<word> vacab, Encode encoding){
	string str;
	char tmpStr[20];

	//UTF-8编码
	if (encoding == UTF_8 || encoding == UTF_8_BOM) {
		unsigned j = 0;
		for (unsigned i = 0; i < vacab.size(); i++) {
			if (vacab[i] <= 0x0080) tmpStr[j++] = (char)(vacab[i] & 0x7F);
			else if (vacab[i] <= 0x0800) {
				tmpStr[j] = (char)((vacab[i] >> 6) | 0xC0);
				tmpStr[j + 1] = (char)((vacab[i] & 0x3F) | 0x80);
				j += 2;
			}
			else {
				tmpStr[j] = (char)((vacab[i] >> 12) | 0xE0);
				tmpStr[j + 1] = (char)(((vacab[i] >> 6) & 0x3F) | 0x80);
				tmpStr[j + 2] = (char)((vacab[i] & 0x3F) | 0x80);
				j += 3;
			}
		}
		tmpStr[j] = '\0';
		str = tmpStr;
	}

	//Unicode_little_endian编码
	else if (encoding == UNICODE_LITTLE_ENDIAN) {
		for (int i = 0; i < vacab.size(); i++) {
			tmpStr[2 * i] = (char)(vacab[i] & 0x00FF);
			tmpStr[2 * i + 1] = (char)(vacab[i] >> 6);
		}
		tmpStr[2 * vacab.size()] = '\0';
		str = tmpStr;
	}

	//Unicode_big_endian编码
	else if (encoding == UNICODE_BIG_ENDIAN) {
		for (int i = 0; i < vacab.size(); i++) {
			tmpStr[2 * i] = (char)(vacab[i] >> 6);
			tmpStr[2 * i + 1] = (char)(vacab[i] & 0x00FF);
		}
		tmpStr[2 * vacab.size()] = '\0';
		str = tmpStr;
	}

	//ANSI
	else if (encoding == ANSI)
		str = Decode::VecToAnsi(vacab);

	else;
	return str;
}