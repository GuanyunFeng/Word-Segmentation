#pragma once
#include"Defination.h"
#include<vector>
#include<windows.h>
#include<assert.h>

//±àÂë
typedef int Encode;
#define UNKNOWN -1
#define ANSI 0
#define UNICODE_BIG_ENDIAN 1
#define UCS2_BE 1
#define UNICODE_LITTLE_ENDIAN 2
#define UCS2_LE 2
#define UTF_8 3
#define UTF_8_NO_BOM 3
#define UTF_8_BOM 4

class Decode
{
public:
	static wchar_t* AnsiToUnicode(const char *);
	static wchar_t* AnsiToUnicode(char *);
	static char* UnicodeToAnsi(const wchar_t *);
	static char* UnicodeToAnsi(wchar_t *);
	static wchar_t* UTF8ToUnicode(const char *);
	static wchar_t* UTF8ToUnicode(char *);
	static char* UnicodeToUTF8(const wchar_t *);
	static char* UnicodeToUTF8(wchar_t *);
	static char* UTF8ToAnsi(const char *);
	static char* UTF8ToAnsi(char *);
	static char* AnsiToUTF8(const char *);
	static char* AnsiToUTF8(char *);
	static char* WideToMilty(const wchar_t *);
	static char* WideToMilty(wchar_t *);
	static wchar_t* MiltyToWide(const char *);
	static wchar_t* MiltyToWide(char *);
	static vector<unsigned short> UnicoToVec(const wchar_t *);
	static vector<unsigned short> UnicoToVec(wchar_t *);
	static wchar_t* VecToUnico(vector<unsigned short>);
	static vector<unsigned short> AnsiToVec(const char *);
	static vector<unsigned short> AnsiToVec(char *);
	static char* VecToAnsi(vector<unsigned short>);
	static vector<unsigned short> UTF8ToVec(const char *);
	static vector<unsigned short> UTF8ToVec(char *);
	static char* VecToUTF8(vector<unsigned short>);
	static Encode CheckEncode(const char *);
	static Encode CheckEncode(char *);
};