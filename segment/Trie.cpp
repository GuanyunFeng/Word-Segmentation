#include "Trie.h"



Trie::Trie(){
	this->root = new TrieNode();
}


Trie::~Trie(){
	Trie::DeletNode(this->root);
	//delete this->root;
}

//把一个单词添加到树，vacab是单词以及词频词性信息
bool Trie::InsertVacab(Info info) {
	//把单词添加到Trie树
	TrieNode *pTmp = this->root;
	for (unsigned int i = 0; i < info.vacab.size(); i++) {
		assert(pTmp);
		unordered_map<unsigned short, TrieNode*>::iterator ite = pTmp->childList.find(info.vacab[i]);
		if (ite == pTmp->childList.end()) {
			TrieNode *tmpNode = new TrieNode();
			if (!tmpNode) return false;
			pTmp->childList[info.vacab[i]] = tmpNode;
			pTmp = pTmp->childList[info.vacab[i]] = tmpNode;
			tmpNode = NULL;
		}
		else if (!pTmp->childList[info.vacab[i]]) {
			TrieNode *tmpNode = new TrieNode();
			if (!tmpNode) return false;
			pTmp->childList[info.vacab[i]] = tmpNode;
			pTmp = pTmp->childList[info.vacab[i]] = tmpNode;
			tmpNode = NULL;
		}
		else pTmp = pTmp->childList[info.vacab[i]];

	}
	//if (pTmp->wordTag) return false;
	pTmp->wordTag = true;
	pTmp->freq = info.freq;
	strcpy(pTmp->prop, info.prop);
	this->numb++;
	return true;
}


//把某一个单词从Tire树中删除，vacab是单词
bool Trie::DeletVacab(vector<unsigned short> vacab) {
	TrieNode *pTmp = this->root, *pTmp3 = pTmp;
	unsigned short tmpWord = vacab[0];
	for (unsigned int i = 0; i < vacab.size(); i++) {
		unordered_map<unsigned short, TrieNode*>::iterator it = pTmp->childList.find(vacab[i]);
		if (it == pTmp->childList.end()) return false;
		if (pTmp->wordTag) {
			pTmp3 = pTmp;
			tmpWord = vacab[i];
		}
		pTmp = pTmp->childList[vacab[i]];
	}
	//如果还有子树，只是去除成词标记
	if (!pTmp->childList.empty()) {
		pTmp->wordTag = false;
		pTmp->freq = 0;
	}
	//否则删除上一个可以成词的结点子树
	else 
		Trie::DeletNode(pTmp3->childList[tmpWord]);
	return true;
}


//把结点node以及其所以子树删除
void Trie::DeletNode(TrieNode* &node) {
	if (!node) return;
	/*//可以成词
    if (node->wordTag) {
		node->wordTag = false;
		node->freq = 0;
	}*/
	//叶节点
	if (node->childList.empty()) {
		delete node; 
		node = NULL;
	}
	//非叶节点
	else {
		unordered_map<unsigned short, TrieNode*>::iterator it = node->childList.begin();
		for (; it != node->childList.end(); it++)
			this->DeletNode(it->second);
	}
}


//在加载的Trie树中查找单词str
WordInfo Trie::Search(vector<unsigned short> vacab) {
	Info info;
	TrieNode* pTmp = this->root;
	if (!pTmp) return info; //info.freq == 0可作为是否正确找到单词的判断依据
	for (unsigned int i = 0; i < vacab.size(); i++) {
		unordered_map<unsigned short, TrieNode*>::iterator it = pTmp->childList.find(vacab[i]);
		//如果Trie树中没有该单词
		if (it == pTmp->childList.end()) return info;
		pTmp = pTmp->childList[vacab[i]];
		if (!pTmp) return info;
	}
	//如果True树中有该路径，但不能作为一个单词
	if (!pTmp->wordTag) return info;
	else {
		info.vacab = vacab;
		strcpy(info.prop, pTmp->prop);
		info.freq = pTmp->freq;
		return info;
	}
}

vector<vector<DAGInfo>> Trie::SearchDAG(vector<unsigned short> sentence)
{
	vector<vector<DAGInfo>> dag;
	vector<DAGInfo> tmp;
	DAGInfo tmpInfo;
	dag.clear(); tmp.clear();
	TrieNode* pTmp = this->root;
	assert(pTmp);
	for (unsigned int i = 0; i < sentence.size(); i++) {
		pTmp = this->root;
		tmp.clear();
		for (unsigned j = i; j < sentence.size(); j++) {
			unordered_map<unsigned short, TrieNode*>::iterator it = pTmp->childList.find(sentence[j]);
			if (it == pTmp->childList.end()) break;
			else if (it->second->wordTag) {
				tmpInfo.freq = it->second->freq;
				strcpy(tmpInfo.prop, it->second->prop);
				tmpInfo.pos = j;
				tmp.push_back(tmpInfo);
			}
			pTmp = pTmp->childList[sentence[j]];
		}
		if (tmp.empty()) {
			tmpInfo.pos = i;
			tmpInfo.freq = 1;
			strcpy(tmpInfo.prop, "*");
			tmp.push_back(tmpInfo);
		}
		dag.push_back(tmp);
	}
	return dag;
}

