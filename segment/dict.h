#pragma once
#include"Defination.h"
#include"Trie.h"
#include"Decode.h"

class Dict{
public:
	Dict();
	Dict(char *path);
	~Dict();
	bool SetDictPath(char *dictPath = NULL);
	Info ReadVacab(FILE* fp, Encode encoding);
	bool LoadDict(Encode encoding);
	bool AddVacab(Info info);
	bool DelVacab(string str, Encode encoding);
	bool SaveChange();
	Info SearchDict(string str, Encode encoding);
	bool SetEncode(Encode encding);
	bool IsLoaded() { return this->loaded; }
	void SetUnloaded() { this->loaded = false; }
	static vector<word> ToUnicode(string str, Encode encoding);
	static string ToString(vector<word> vacab, Encode encoding);
	Encode dictEncode = UNKNOWN;

	Trie tree;
private:
	string dictPath;
	vector<string> deleted;
	vector<Info> added;
	bool loaded = false;
	unsigned long total = 0;
};