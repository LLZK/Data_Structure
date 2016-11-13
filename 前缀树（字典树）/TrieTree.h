#pragma once 

#include<iostream>
using namespace std;

enum ISEXIST
{
	TRUE,
	FALSE
};

struct TrieNode
{
	size_t _count;
	TrieNode* _Next[26];
	ISEXIST _exist;

	TrieNode()
		:_count(0)
		, _exist(FALSE)
	{
		for (int i = 0; i < 26; i++)
		{
			_Next[i] = NULL;
		}
	}
};


class TrieTree
{
	typedef TrieNode Node;
public:
	TrieTree()
	{
		_root = new Node();
	}
	bool Insert(const string& str)
	{
		Node* root = _root;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (root->_Next[str[i] - 97] == NULL)
			{
				root->_Next[str[i] - 97] = new Node();
				root->_Next[str[i] - 97]->_count++;
			}
			else
			{
				root->_Next[str[i] - 97]->_count++;
			}

			if (i == str.size() - 1)
			{
				root->_Next[str[i] - 97]->_exist = TRUE;
			}

			root = root->_Next[str[i] - 97];
		}
		
		return true;
	}

	bool Test(const string & str)
	{
		Node* root = _root;
		int i = 0;
		for ( i = 0; i < str.size(); ++i)
		{
			if (root->_Next[str[i] - 97] == NULL)
			{
				return false;
			}
			root = root->_Next[str[i] - 97];
		}
		if (root->_Next[str[i - 1] - 97]->_exist == FALSE)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool Remove(const string& str)
	{
		if (Test(str) == false)
		{
			return false;
		}

		Node* root = _root;
		int i = 0;
		for (i = 0; i < str.size(); ++i)
		{
			if (i == str.size() - 1)
			{
				root->_Next[str[i] - 97]->_exist = FALSE;
			}
			if (root->_Next[str[i] - 97]->_count)
			{
				Node* del = root->_Next[str[i] - 97];
				root = root->_Next[str[i] - 97];
				delete del;
				del = NULL;
			}
			else
			{
				root->_Next[str[i] - 97]->_count--;
				root = root->_Next[str[i] - 97];
			}
		}
		return true;
	}
protected:
	Node* _root;
};


void TestTrieTree()
{
	TrieTree tree;
	tree.Insert(string("abcas"));
}
