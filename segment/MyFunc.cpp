#include "MyFunc.h"

FILE* MyFunc::Myfopen(char* path, const char* mode, Encode encoding) {
	return MyFunc::Myfopen((const char*)path, mode, encoding);
}

FILE* MyFunc::Myfopen(const char *path, const char * mode, Encode encoding) {
	if (!strcmp(mode, "w") || !strcmp(mode, "wb")) {
		FILE *fp = fopen(path, mode);
		assert(fp);
		if (encoding == UTF_8_BOM) {
			unsigned char tmp[4];
			tmp[0] = 0xef; tmp[1] = 0xbb; tmp[2] = 0xbf;
			fwrite(tmp, 3, 1, fp);
		}
		else if (encoding == UCS2_BE) {
			unsigned char tmp[4];
			tmp[0] = 0xfe; tmp[1] = 0xff;
			fwrite(tmp, 2, 1, fp);
		}
		else if (encoding == UCS2_LE) {
			unsigned char tmp[4];
			tmp[0] = 0xff; tmp[1] = 0xfe;
			fwrite(tmp, 2, 1, fp);
		}
		return fp;
	}
	else if (!strcmp(mode, "r") || !strcmp(mode, "rb")) {
		FILE* fp = fopen(path, mode);
		assert(fp);
		if (encoding == UTF_8_BOM)
			fseek(fp, 3, SEEK_SET);
		else if ((encoding == UCS2_BE) || (encoding == UCS2_LE))
			fseek(fp, 2, SEEK_SET);
		return fp;
	}
	else {
		FILE* fp = fopen(path, mode);
		assert(fp);
		return fp;
	}
}

int MyFunc::MyReadLine(FILE* fp, wchar_t* &ws, Encode encoding) {
	char tmp[5];
	wchar_t tmp3, tmp4;
	wchar_t *tmpWStr = NULL;
	int i = 0, state;
	if (encoding == UCS2_LE) {
		tmpWStr = (wchar_t*)malloc(1024 * sizeof(wchar_t));
		while ((state = fread(tmp, 1, 2, fp)) != 0) {
			tmp4 = tmp[0] & 0x00ff;
			tmp3 = tmp[1] & 0x00ff;
			tmpWStr[i] = (tmp3 << 8) | tmp4;
			if (tmpWStr[i] == '\r') {
				fread(tmp, 1, 2, fp);
				tmpWStr[i] = '\0';
				break;
			}
			i++;
			tmpWStr[i] = '\0';
		}
		if (state == 0) {
			tmpWStr[i] = '\0';
			ws = tmpWStr;
			return -1;
		}
		ws = tmpWStr;
		return 0;
	}
	else if (encoding == UCS2_BE) {
		tmpWStr = (wchar_t*)malloc(1024 * sizeof(wchar_t));
		while ((state = fread(tmp, 1, 2, fp)) != 0) {
			tmp4 = tmp[0] & 0x00ff;
			tmp3 = tmp[1] & 0x00ff;
			tmpWStr[i] = (tmp4 << 8) | tmp3;
			if (tmpWStr[i] == '\r') {
				fread(tmp, 1, 2, fp);
				tmpWStr[i] = '\0';
				break;
			}
			i++;
			tmpWStr[i] = '\0';
		}
		if (state == 0) {
			tmpWStr[i] = '\0';
			ws = tmpWStr;
			return -1;
		}
		ws = tmpWStr;
		return 0;
	}
	else if ((encoding == UTF_8) || (encoding == UTF_8_BOM) || (encoding == ANSI)) {
		char tmpStr[20480];
		while ((state = fread(tmp, 1, 1, fp)) != 0) {
			tmpStr[i] = tmp[0];
			if (tmp[0] == '\r') {
				fread(tmp, 1, 1, fp);
				assert(tmp[0] == '\n');
				tmpStr[i] = '\0';
				if (encoding == ANSI)
					tmpWStr = Decode::AnsiToUnicode(tmpStr);
				else
					tmpWStr = Decode::UTF8ToUnicode(tmpStr);
				break;
			}
			else if (tmp[0] == '\n') {
				tmpStr[i] = '\0';
				if (encoding == ANSI)
					tmpWStr = Decode::AnsiToUnicode(tmpStr);
				else
					tmpWStr = Decode::UTF8ToUnicode(tmpStr);
				break;
			}
			i++;
			tmpStr[i] = '\0';
		}
		if (state == 0) {
			tmpStr[i] = '\0';
			if (encoding == ANSI)
				tmpWStr = Decode::AnsiToUnicode(tmpStr);
			else
				tmpWStr = Decode::UTF8ToUnicode(tmpStr);
			ws = tmpWStr;
			return -1;
		}
		ws = tmpWStr;
		return 0;
	}
}

vector<vector<word>> MyFunc::MySplit(wchar_t *wstr, vector<wchar_t> &puncList) {
	vector<vector<word>> wsList;
	wchar_t tmpWStr[1024];
	wsList.clear(); puncList.clear();
	int wslen = wcslen(wstr), i, j;
	for (i = 0; i < wslen; ) {
		for (j = 0; i + j < wslen; j++) {
			if (wstr[i + j] == L'£¬' || wstr[i + j] == L','
				|| wstr[i + j] == L'¡£' || wstr[i + j] == L'.'
				|| wstr[i + j] == L'¡°' || wstr[i + j] == L'¡±'
				|| wstr[i + j] == L'£º' || wstr[i + j] == L':'
				|| wstr[i + j] == L'£¡' || wstr[i + j] == L'!'
				|| wstr[i + j] == L'£¿' || wstr[i + j] == L'?'
				|| wstr[i + j] == L'¡ª' || wstr[i + j] == L'-'
				|| wstr[i + j] == L' '  || wstr[i + j] == L'"'
				|| wstr[i + j] == L'£»' || wstr[i + j] == L';'
				|| wstr[i + j] == L'£¨' || wstr[i + j] == L'£©'
				|| wstr[i + j] == L'('  || wstr[i + j] == L')')
			{
				tmpWStr[j] = '\0';
				wsList.push_back(Decode::UnicoToVec(tmpWStr));
				puncList.push_back(wstr[i + j]);
				memset(tmpWStr, 0, 2 * 1024);
				break;
			}
			else
				tmpWStr[j] = wstr[i + j];
		}
		if (i + j == wslen) {
			tmpWStr[j] = '\0';
			wsList.push_back(Decode::UnicoToVec(tmpWStr));
			break;
		}
		i += j + 1;
	}
	return wsList;
}

