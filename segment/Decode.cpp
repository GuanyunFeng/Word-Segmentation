#include "Decode.h"

wchar_t * Decode::AnsiToUnicode(char *str) {
	return Decode::AnsiToUnicode((const char*)str);
}

wchar_t * Decode::AnsiToUnicode(const char *str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	assert(len);
	wchar_t *tmpStr = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	assert(tmpStr);
	memset(tmpStr, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str, -1, tmpStr, strlen(str));
	return tmpStr;
}

char * Decode::UnicodeToAnsi(wchar_t *str) {
	return Decode::UnicodeToAnsi((const wchar_t *)str);
}

char * Decode::UnicodeToAnsi(const wchar_t *str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	assert(len);
	char* tmpStr = (char *)malloc((len + 1) * sizeof(char));
	assert(tmpStr);
	memset(tmpStr, 0, sizeof(char) * (len + 1));
	WideCharToMultiByte(CP_ACP, 0, str, -1, tmpStr, len, NULL, NULL);
	return tmpStr;
}

wchar_t * Decode::UTF8ToUnicode(char *str) {
	return Decode::UTF8ToUnicode((const char*)str);
}

wchar_t * Decode::UTF8ToUnicode(const char *str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	assert(len);
	wchar_t * tmpStr = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
	assert(tmpStr);
	memset(tmpStr, 0, (len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)tmpStr, len);
	return tmpStr;
}

char * Decode::UnicodeToUTF8(wchar_t *str) {
	return Decode::UnicodeToUTF8((const wchar_t *)str);
}

char * Decode::UnicodeToUTF8(const wchar_t *str)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	assert(len);
	char *tmpStr = (char*)malloc((len + 1) * sizeof(char));
	assert(tmpStr);
	memset(tmpStr, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, tmpStr, len, NULL, NULL);
	return tmpStr;
}

char * Decode::UTF8ToAnsi(char *str)
{
	return Decode::UnicodeToAnsi(Decode::UTF8ToUnicode(str));
}

char * Decode::UTF8ToAnsi(const char *str)
{
	return Decode::UnicodeToAnsi(Decode::UTF8ToUnicode(str));
}

char * Decode::AnsiToUTF8(char *str)
{
	return Decode::UnicodeToUTF8(Decode::AnsiToUnicode(str));
}

char * Decode::AnsiToUTF8(const char *str)
{
	return Decode::UnicodeToUTF8(Decode::AnsiToUnicode(str));
}

char * Decode::WideToMilty(wchar_t *wstr)
{
	return Decode::WideToMilty((const wchar_t *)wstr);
}

char * Decode::WideToMilty(const wchar_t *wstr)
{
	int len = wcstombs(NULL, wstr, 0);
	assert(len);
	char *tmpStr = (char *)malloc(sizeof(char)*(len + 1));
	memset(tmpStr, 0, (len + 1) * sizeof(char));
	len = wcstombs(tmpStr, wstr, len + 1);
	return tmpStr;
}

wchar_t * Decode::MiltyToWide(char *str) {
	return Decode::MiltyToWide((const char*)str);
}

wchar_t * Decode::MiltyToWide(const char *str)
{
	int len = mbstowcs(NULL, str, 0);
	assert(len);
	wchar_t* tmpStr = (wchar_t *)malloc(sizeof(wchar_t)*(len + 1));
	memset(tmpStr, 0, (len + 1) * sizeof(wchar_t));
	len = mbstowcs(tmpStr, str, len + 1);
	return tmpStr;
}

vector<unsigned short> Decode::UnicoToVec(wchar_t *wstr)
{
	return Decode::UnicoToVec((const wchar_t*)wstr);
}

vector<unsigned short> Decode::UnicoToVec(const wchar_t *wstr)
{
	vector<unsigned short> vec;
	vec.clear();
	for (int i = 0; i < wcslen(wstr); i++)
		vec.push_back(wstr[i]);
	return vec;
}

wchar_t * Decode::VecToUnico(vector<unsigned short> vec)
{
	wchar_t *wstr = (wchar_t*)malloc((vec.size() + 1) * sizeof(wchar_t));
	assert(wstr);
	for (int i = 0; i < vec.size(); i++)
		wstr[i] = vec[i];
	wstr[vec.size()] = '\0';
	return wstr;
}

vector<unsigned short> Decode::AnsiToVec(char *str)
{
	return Decode::UnicoToVec(Decode::AnsiToUnicode(str));
}

vector<unsigned short> Decode::AnsiToVec(const char *str)
{
	return Decode::UnicoToVec(Decode::AnsiToUnicode(str));
}

char * Decode::VecToAnsi(vector<unsigned short> vec)
{
	return Decode::UnicodeToAnsi(Decode::VecToUnico(vec));
}

vector<unsigned short> Decode::UTF8ToVec(char *str)
{
	return Decode::UnicoToVec(Decode::UTF8ToUnicode(str));
}

vector<unsigned short> Decode::UTF8ToVec(const char *str)
{
	return Decode::UnicoToVec(Decode::UTF8ToUnicode(str));
}

char * Decode::VecToUTF8(vector<unsigned short> vec)
{
	return Decode::UnicodeToUTF8(Decode::VecToUnico(vec));
}

Encode Decode::CheckEncode(char * path) {
	return Decode::CheckEncode((const char*)path);
}

Encode Decode::CheckEncode(const char * path) {
	FILE* fp = fopen(path, "rb");
	Encode encoding = UNKNOWN;
	if (!fp) return encoding;
	char* ch = (char*)malloc(5 * sizeof(char));
	fgets(ch, 3, fp);
	//Unicode
	if ((ch[0] == (char)0xFF) && (ch[1] == (char)0xFE))
		encoding = UNICODE_LITTLE_ENDIAN;
	//Unicode_big_endian
	else if ((ch[0] == (char)0xFE) && (ch[1] == (char)0xFF))
		encoding = UNICODE_BIG_ENDIAN;
	//UTF-8 有bom
	else if ((ch[0] == (char)0xEF) && (ch[1] == (char)0xBB)) {
		fgets(ch, 2, fp);
		if (ch[0] == (char)0xBF) encoding = UTF_8_BOM;
	}
	//UTF-8无bom,或者ANSI
	else {
		fseek(fp, 0, SEEK_SET);
		encoding = UTF_8;
		while ((ch[0] = fgetc(fp))!= EOF) {
			if ((unsigned char)ch[0] < 0x80) continue;
			else if ((unsigned char)ch[0] < 0xC0) {
				encoding = ANSI;
				break;
			}
			else if ((unsigned char)ch[0] < 0xE0) {
				if ((ch[0] = fgetc(fp)) != EOF) {
					if (((unsigned char)ch[0] & 0xC0) != 0x80) {
						encoding = ANSI;
						break;
					}
				}
				else break;
			}
			else if ((unsigned char)ch[0] < 0xF0) {
				if (((ch[0] = fgetc(fp)) != EOF && (ch[1] = fgetc(fp)) != EOF)) {
					if (((unsigned char)ch[0] & 0xC0) != 0x80|| ((unsigned char)ch[1] & 0xC0) != 0x80) {
						encoding = ANSI;
						break;
					}
				}
				else break;
			}
			else {
				encoding = ANSI;
				break;
			}
		}
	}
	free(ch);
	fclose(fp);
	return encoding;
}