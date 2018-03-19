#pragma once
#include"Trie.h"
#include"Decode.h"
#include<string>
#include<vector>
#include<iostream>
#include<stdio.h>
#include<direct.h>
#include<windows.h>
#include<assert.h>
using namespace std;

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
	static vector<unsigned short> ToUnicode(string str, Encode encoding);
	static string ToString(vector<unsigned short> vacab, Encode encoding);
	Encode dictEncode = UNKNOWN;

	Trie tree;
private:
	string dictPath;
	vector<string> deleted;
	vector<Info> added;
	bool loaded = false;
	unsigned long total = 0;
};