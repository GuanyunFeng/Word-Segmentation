#include "SegCore.h"



SegCore::SegCore()
{
}


SegCore::~SegCore()
{
}

void SegCore::Seg(vector<unsigned short> sentence, Dict *dict, wchar_t *ws) {
	unsigned i, j, k, p, q, next, current, n = 0;
	int count = 0;
	unsigned maxlen = 0, numb = 3;
	char tmpStr[5];
	vector<vector<DAGInfo>> dag;
	unsigned routs[1024][3];
	unsigned rout[3];
	dag.clear();
	dag = dict->tree.SearchDAG(sentence);
	//i是第i个字
	for (i = 0; i < dag.size(); ) {
		n = 0; maxlen = 0; numb = 3;
		//j用于遍历i所有能组成的词
		for (j = 0; j < dag[i].size(); j++) { //进行第一条规则筛选
			memset(rout, 0, 3 * sizeof(unsigned));
			rout[0] = dag[i][j].pos - i + 1;
			current = next = dag[i][j].pos + 1;
			if (current < dag.size())
				for (k = 0; k < dag[current].size(); k++) {
					rout[1] = rout[2] = 0;
					rout[1] = dag[current][k].pos - current + 1;
					next = dag[current][k].pos + 1;
					if (next < dag.size())
						rout[2] = dag[next][dag[next].size() - 1].pos - next + 1;
					if (maxlen <= rout[0] + rout[1] + rout[2]) {
						if (maxlen < rout[0] + rout[1] + rout[2])
							n = 0;
						maxlen = rout[0] + rout[1] + rout[2];
						routs[n][0] = rout[0];
						routs[n][1] = rout[1];
						routs[n][2] = rout[2];
						n++;
					}
				}
			else {
				if (maxlen <= rout[0] + rout[1] + rout[2]) {
					if (maxlen < rout[0] + rout[1] + rout[2])
						n = 0;
					maxlen = rout[0] + rout[1] + rout[2];
					routs[n][0] = rout[0];
					routs[n][1] = rout[1];
					routs[n][2] = rout[2];
					n++;
				}
			}
		}
		if (n == 1)
			goto CONTINUE;
		else {
			numb = 3;
			for (p = 0, q = 0; p < n; p++) {//进行第二条规则筛选
				if (routs[p][2] && routs[p][1] && routs[p][0]) {
					if (numb == 3) {
						routs[q][0] = routs[p][0];
						routs[q][1] = routs[p][1];
						routs[q][2] = routs[p][2];
						q++;
					}
				}
				else if (!routs[p][2] && routs[p][1] && routs[p][0]) {
					if (numb > 2) {
						numb = 2; q = 0;
						routs[q][0] = routs[p][0];
						routs[q][1] = routs[p][1];
						routs[q][2] = routs[p][2];
						q++;
					}
					else if (numb = 2) {
						routs[q][0] = routs[p][0];
						routs[q][1] = routs[p][1];
						routs[q][2] = routs[p][2];
						q++;
					}
				}
				else if (!routs[p][2] && !routs[p][1] && routs[p][0]) {
					if (numb > 1) {
						numb = 1; q = 0;
						routs[q][0] = routs[p][0];
						routs[q][1] = routs[p][1];
						routs[q][2] = routs[p][2];
						q++;
					}
					else if (numb = 1) {
						routs[q][0] = routs[p][0];
						routs[q][1] = routs[p][1];
						routs[q][2] = routs[p][2];
						q++;
					}
				}
				assert(!(!routs[p][2] && !routs[p][1] && !routs[p][0]));
			}
			n = q;
		}
		if (n == 1)
			goto CONTINUE;
		else { ////进行第三条规则筛选
			double average = (double)maxlen / numb;
			double variance = 100, tmp = 0;
			for (p = 0, q = 0; p < n; p++) {
				tmp = (routs[p][0] - average)*(routs[p][0] - average)
					+ (routs[p][1] - average)*(routs[p][1] - average)
					+ (routs[p][2] - average)*(routs[p][2] - average);
				if (tmp < variance) {
					variance = tmp;
					q = 0;
					routs[q][0] = routs[p][0];
					routs[q][1] = routs[p][1];
					routs[q][2] = routs[p][2];
					q++;
				}
				else if (tmp == variance) {
					routs[q][0] = routs[p][0];
					routs[q][1] = routs[p][1];
					routs[q][2] = routs[p][2];
					q++;
				}
			}
			n = q;
		}
		if (n == 1)
			goto CONTINUE;
		else { ////进行第四条规则筛选
			double mp = 0, tmpMp = 0;
			for (p = 0, q = 0; p < n; p++) {
				tmpMp = 0;
				if (routs[p][0] == 1)
					tmpMp += log(dag[i][0].freq);
				if (routs[p][1] == 1)
					tmpMp += log(dag[i + routs[p][0]][0].freq);
				if (routs[p][2] == 1)
					tmpMp += log(dag[i + routs[p][0] + routs[p][1]][0].freq);
				//
				if (tmpMp > mp) {
					mp = tmpMp;
					q = 0;
					routs[q][0] = routs[p][0];
					routs[q][1] = routs[p][1];
					routs[q][2] = routs[p][2];
					q++;
				}
				else if (tmpMp == mp) {
					routs[q][0] = routs[p][0];
					routs[q][1] = routs[p][1];
					routs[q][2] = routs[p][2];
					q++;
				}
			}
			n = q;
		}
	CONTINUE:
		for (p = 0; p < routs[0][0]; p++)
			ws[count++] = sentence[i++];
		ws[count++] = '/';
		ws[count] = '\0';
	}
}

FILE* SegCore::Myfopen(char* path, const char* mode, Encode encoding) {
	return SegCore::Myfopen((const char*)path, mode, encoding);
}

FILE* SegCore::Myfopen(const char *path, const char * mode, Encode encoding) {
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

int SegCore::MyReadLine(FILE* fp, wchar_t* &ws, Encode encoding) {
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
	}
	ws = tmpWStr;
	return 0;
}

vector<vector<unsigned short>> SegCore::MySplit(wchar_t *wstr, vector<wchar_t> &puncList) {
	vector<vector<unsigned short>> wsList;
	wchar_t tmpWStr[1024];
	wsList.clear(); puncList.clear();
	int wslen = wcslen(wstr), i, j;
	for (i = 0; i < wslen; ) {
		for (j = 0; i + j < wslen; j++) {
			if (wstr[i + j] == L'，' || wstr[i + j] == L','
				|| wstr[i + j] == L'。' || wstr[i + j] == L'.'
				|| wstr[i + j] == L'“' || wstr[i + j] == L'”'
				|| wstr[i + j] == L'：' || wstr[i + j] == L':'
				|| wstr[i + j] == L'！' || wstr[i + j] == L'!'
				|| wstr[i + j] == L'？' || wstr[i + j] == L'?'
				|| wstr[i + j] == L'—' || wstr[i + j] == L'-'
				|| wstr[i + j] == L' ' || wstr[i + j] == L'"'
				|| wstr[i + j] == L'；' || wstr[i + j] == L';'
				|| wstr[i + j] == L'（' || wstr[i + j] == L'）'
				|| wstr[i + j] == L'(' || wstr[i + j] == L')')
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