#include "Defination.h"
#include "Decode.h"

class MyFunc {
public:
	static FILE * Myfopen(char * path, const char* mode, Encode encoding);
	static FILE * Myfopen(const char *path, const char * mode, Encode encoding);
	static int MyReadLine(FILE* fp, wchar_t* &ws, Encode encoding);
	static vector<vector<word>> MySplit(wchar_t *wstr, vector<wchar_t> &puncList);
};
