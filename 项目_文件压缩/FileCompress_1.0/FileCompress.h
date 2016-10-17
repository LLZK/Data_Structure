#define _CRT_SECURE_NO_WARNINGS 1


#pragma once
#include<iostream>
#include<string>
#include<cassert>
#include<vector>
#include"Huffman.h"
using namespace std;

typedef long long LongType;

struct CharInfo
{
	char _ch;
	LongType _count;
	string _code;
	
	CharInfo(LongType x = 0)
		:_count(x)
		, _ch(0)
	{}

	//重载比较运算符，以便在调用仿函数时使用
	bool operator>(CharInfo c)
	{
		return _count > c._count;
	}
	bool operator<(CharInfo c)
	{
		return _count < c._count;
	}
	bool operator==(CharInfo c)
	{
		return _count == c._count;
	}

	//与0进行比较
	bool operator!= (CharInfo x)
	{
		return _count != x._count;
	}
	//重载加号，以便在构造父节点时使用
	CharInfo operator+(CharInfo c)
	{
		return CharInfo(_count + c._count);
	}

};

class FileCompress
{
public:
	//压缩
	string Compress(string filename)
	{
		FILE *fout = fopen(filename.c_str(), "r");
		assert(fout);

		//初始化Charinfo中的ch
		for (int i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
		}
		//统计字符出现次数
		char ch = fgetc(fout);
		while (ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fout);
		}


		//生成哈夫曼树
		CharInfo invalid;
		HuffmanTree<CharInfo> Tree(_infos, 256, invalid);

		//生成哈夫曼编码
		HuffmanTreeNode<CharInfo>* cur = Tree.GetRoot();
		string s;
		PrevRootHuffmanCode(cur, s);



		//压缩
		fseek(fout, 0, SEEK_SET);
		string RetFilename = filename+".compress";
		FILE* fin = fopen(RetFilename.c_str(), "w");
		assert(fin);
		int pos = 0;
		char value = 0;
		char ReadCh = fgetc(fout);
		while (ReadCh != EOF)
		{
			for (size_t i = 0; i < _infos[ReadCh]._code.size(); i++)
			{
				value <<= 1;
				if (_infos[ReadCh]._code[i] == '1')
				{
					value |= 1;
				}
				else
				{
					value |= 0;
				}
				++pos;

				if (pos == 8)
				{
					fputc(value, fin);
					pos = 0;
					value = 0;
				}
			}
			ReadCh = fgetc(fout);
		}

		//如果最后一个字节不够8位，左移补0
		if (pos)
		{
			value <<= (8 - pos);
			fputc(value, fin);
		}


		//另一种算法
		//string HuffmanCode;
		//while (ReadCh != EOF)
		//{
		//	size_t i = 0;
		//	while (i < _infos[ReadCh]._code.size())
		//	{
		//		HuffmanCode.push_back(_infos[ReadCh]._code[i]);
		//		i++;
		//	}
		//	ReadCh = fgetc(fout);
		//}

		//char c = '0';
		//size_t i = 0;
		//int flag = 0;//标签，每读取8位--》写--》重新再读
		//while (i < HuffmanCode.size())
		//{
		//	c = c * 2 + HuffmanCode[i]-'0';
		//	flag++;
		//	if (flag == 8)
		//	{
		//		fputc(c, fin);
		//		flag = 0;
		//		c ='0';
		//	}
		//	i++;
		//}

		////如果最后不够8位--->补0；
		//if (flag > 0 && flag < 8)
		//{
		//	for (int i = 0; i < 8 - flag; i++)
		//	{
		//		c = c * 2 + 0;
		//	}
		//	fputc(c, fin);
		//}
		fclose(fin);
		fclose(fout);
		//返回文件名
		return RetFilename;
	}

	//解压缩
	 string Uncompress(string filename)
	 {

		 //重新构建哈夫曼树
		 CharInfo invalid;
		 HuffmanTree<CharInfo> Tree(_infos, 256, invalid);

		 HuffmanTreeNode<CharInfo> *root = Tree.GetRoot();
		 HuffmanTreeNode<CharInfo> *cur = root;

		 //总字符的次数
		 LongType sz = root->_weight._count;

		 //以读方式打开压缩文件
		 FILE *fout = fopen(filename.c_str(), "r");

		 //以写方式打开一个新的文件
		 string Uncompressname = filename + '1';
		 FILE *fin = fopen(Uncompressname.c_str(), "w");

		 char ReadCh = fgetc(fout);
		 int pos = 7;
		 while (ReadCh != EOF)
		 {
			 while (pos >= 0)
			 {
				 if (((ReadCh >> pos) & 1) == 1)
				 {
					 cur = cur->_right;
				 }
				 else
				 {
					 cur = cur->_left;
				 }
				 --pos;
				 if (cur->_left == NULL && cur->_right == NULL&& sz > 0)
				 {
					 fputc(cur->_weight._ch, fin);
					 cur = root;
					 --sz;
				 }
				 if (sz <= 0)
				 {
					 return Uncompressname;
				 }
			 }
			 ReadCh = fgetc(fout);
			 pos = 7;
		 }
		 fclose(fout);
		 fclose(fin);
		 return Uncompressname;
	 }
protected:
	//前序遍历生成Huffman编码
	void PrevRootHuffmanCode(HuffmanTreeNode<CharInfo> * root, string s)
	{
		HuffmanTreeNode<CharInfo>* cur = root;
		if (cur == NULL)
		{
			return;
		}
		if (cur->_left == NULL && cur->_right == NULL)
		{
			_infos[cur->_weight._ch]._code = s;
			return;
		}
		// 每向左一步就加一个0
		PrevRootHuffmanCode(cur->_left, s + '0');
		// 每向右一步就加一个1
		PrevRootHuffmanCode(cur->_right, s + '1');
		
	}
protected:
	CharInfo _infos[256];
};

void Test()
{
	FileCompress f1;
	string ret = f1.Compress("222.png");
	f1.Uncompress(ret);
	//FILE *fout = fopen(ret.c_str(), "r");
	//char ret1 = fgetc(fout);
	//char ret2 = fgetc(fout);
	//char ret3 = fgetc(fout);
}