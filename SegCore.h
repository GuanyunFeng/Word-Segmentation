#pragma once
#include<vector>
#include "Decode.h"
#include "Trie.h"
#include "dict.h"
#include"Defination.h"

class SegCore
{
public:
	SegCore();
	~SegCore();
	static void Seg(vector<unsigned short> sentence,Dict *dict, wchar_t* ws);

	static FILE * Myfopen(char * path, const char* mode, Encode encoding);
	static FILE * Myfopen(const char *path, const char * mode, Encode encoding);
	static int MyReadLine(FILE* fp, wchar_t* &ws, Encode encoding);
	static vector<vector<word>> MySplit(wchar_t *wstr, vector<wchar_t> &puncList);

};

