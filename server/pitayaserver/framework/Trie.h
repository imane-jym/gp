/*
 * Trie.h
 *
 *  Created on: 2011-12-9
 *      Author: yq
 */

#ifndef TRIE_H_
#define TRIE_H_

#include <iostream>
#include <string>
#include <memory.h>

const int branchNum = 256; //声明常量

struct Trie_node
{
	bool isStr; //记录此处是否构成一个串。
	Trie_node *next[branchNum];//指向各个子树的指针
	Trie_node():isStr(false)
	{
		memset(next,0,sizeof(next));
	}
};

class Trie
{
public:
	Trie();
	~Trie();
	void insert(const char* word);
	bool search(char* word);
private:
	void deleteTrie(Trie_node *root);
private:
	Trie_node* root;
};

Trie::Trie()
{
	root = new Trie_node();
}
Trie::~Trie()
{
	deleteTrie(root);
}
void Trie::insert(const char* word)
{
	Trie_node *location = root;
	while(*word)
	{
		if(location->next[(unsigned char)(*word)] == NULL)//不存在则建立
		{
			Trie_node *tmp = new Trie_node();
			location->next[(unsigned char)(*word)] = tmp;
		}
		location = location->next[(unsigned char)(*word)]; //每插入一步，相当于有一个新串经过，指针要向下移动
		word++;
	}
	location->isStr = true; //到达尾部,标记一个串
}

bool Trie::search(char *word)
{
	Trie_node *location = root;
	while(*word && location)
	{
		location = location->next[(unsigned char)(*word)];
		word++;
	}
	return(location!=NULL && location->isStr);
}

void Trie::deleteTrie(Trie_node *root)
{
	for(int i = 0; i < branchNum; i++)
	{
		if(root->next[i] != NULL)
		{
			deleteTrie(root->next[i]);
		}
	}
	delete root;
}


#endif /* TRIE_H_ */
