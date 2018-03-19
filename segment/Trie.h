#pragma once
#include<vector>
#include<unordered_map>
#include<stdio.h>
#include<assert.h>
using namespace std;

struct TrieNode {
	unordered_map <unsigned short, TrieNode* > childList;
	bool  wordTag;  //是否可以以该结点作为单词结尾
	int freq;       //词频
	char prop[5];     //词性
	TrieNode() { childList.clear(); wordTag = false; freq = 0; }
};

typedef struct WordInfo {
	vector< unsigned short> vacab;      //单词
	char prop[5];                 //词性
	unsigned int freq = 0;        //词频
	WordInfo() { int freq = 0; vacab.clear(); };
}Info;                         //用于保存从字典中提取的信息

struct DAGInfo {
	int pos;
	int freq;       //词频
	char prop[5];     //词性
};


class Trie
{
public:
	Trie();
	~Trie();
public:
	bool InsertVacab(Info word);
	bool DeletVacab(vector<unsigned short> vacab);
	void DeletNode(TrieNode* &node);
	WordInfo Search(vector<unsigned short> vacab);
	vector<vector<DAGInfo> > SearchDAG(vector<unsigned short> sentence);
	TrieNode * Getroot() { return this->root; }
	int GetNum() { return this->numb; }

	TrieNode * root = NULL;
	int numb = 0;
};