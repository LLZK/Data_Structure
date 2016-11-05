#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<iostream>
#include<string>
#include<cassert>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include"Huffman.h"
using namespace std;

typedef long long LongType;
struct CharInfo
{
	unsigned char _ch;
	LongType _count;
	string _code;

	CharInfo(LongType x = 0)
		:_count(x)
		, _ch(0)
	{}

	//���رȽ���������Ա��ڵ��÷º���ʱʹ��
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

	//��0���бȽ�
	bool operator!= (CharInfo x)
	{
		return _count != x._count;
	}
	//���ؼӺţ��Ա��ڹ��츸�ڵ�ʱʹ��
	CharInfo operator+(CharInfo c)
	{
		return CharInfo(_count + c._count);
	}

};

class FileCompress
{
public:
	//ѹ��
	string Compress(string filename)
	{
		FILE *fout = fopen(filename.c_str(), "rb");
		assert(fout);

		//��ʼ��Charinfo�е�ch
		for (int i = 0; i < 256; i++)
		{
			_infos[i]._ch = i;
		}
		//ͳ���ַ����ִ���
	    int ch = fgetc(fout);
		while (ch != EOF)
		{
			_infos[ch]._count++;
			ch = fgetc(fout);
		}


		//���ɹ�������
		CharInfo invalid;//�Ƿ�ֵ
		HuffmanTree<CharInfo> Tree(_infos, 256, invalid);

		//���ɹ���������
		HuffmanTreeNode<CharInfo>* cur = Tree.GetRoot();
		string s;
		PrevRootHuffmanCode(cur, s);

		//�����ļ�
		string ConfigFilename = filename + ".config";
		FILE *finconfig = fopen(ConfigFilename.c_str(), "wb");
		char buff[128];//�����������
		string s1;
		for (int i = 0; i < 256; i++)
		{
			if (_infos[i]._count > 0)
			{
				fputc(_infos[i]._ch, finconfig);
				//s1.push_back(_infos[i]._ch);
				s1 = s1 +',';
				//char* count =
				_itoa((int)_infos[i]._count, buff, 10);
				s1 = s1 + buff;
				s1 += '\n';
				fputs(s1.c_str(), finconfig);
				s1.clear();
				//fputc('\n', finconfig);
			}
		}

		//ѹ��
		fseek(fout, 0, SEEK_SET);
		string RetFilename = filename+".compress";
		FILE* fin = fopen(RetFilename.c_str(), "wb");
		assert(fin);
		int pos = 0;
		unsigned char value = 0;
		int ReadCh = fgetc(fout);
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

		//������һ���ֽڲ���8λ�����Ʋ�0
		if (pos)
		{
			value <<= (8 - pos);
			fputc(value, fin);
		}


		//��һ���㷨
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
		//int flag = 0;//��ǩ��ÿ��ȡ8λ--��д--�������ٶ�
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

		////�����󲻹�8λ--->��0��
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
		fclose(finconfig);
		//�����ļ���
		return RetFilename;
	}

	//bool ReadConfig(char* line, FILE * stream)
	//{
	//	if (fgets(line, 130, stream))
	//	{
	//		return true;
	//	}
	//	return false;
	//}

	bool Readline(FILE* fOut, string& str)
	{
		int ch = fgetc(fOut);
		if (ch == EOF)
		{
			return false;
		}
		else if (ch == '\n')
		{
			str += ch;
			ch = fgetc(fOut);
			while (ch != EOF &&ch != '\n')
			{
				str += ch;
				ch = fgetc(fOut);
			}
			return true;
		}
		while (ch != EOF &&ch != '\n')
		{
			str += ch;
			ch = fgetc(fOut);
		}
		return true;
		
	}
	//��ѹ��
	 string Uncompress(string filename)
	 {
		 size_t index = filename.rfind('.');
		 //���ý�ѹ���ļ���
		 string Uncompressname = filename.substr(0, index) + ".uncompress";

		 
		 //���¹�����������
		 //����Ҫ��ȡ�������ļ����ļ���
		 string ConfigFilename = filename.substr(0, index) + ".config";
		 FILE *foutconfig = fopen(ConfigFilename.c_str(), "rb");
		 //��ȡ�����ļ�
		 string s;
		 while (Readline(foutconfig,s))
		 {
			 unsigned char ch = s[0];
			 _infos[ch]._ch = s[0];
			 const char* str = s.c_str()+2;
			 LongType count = atoi((const char*)str);
			 _infos[ch]._count = count;
			 s.clear();
		 }

		 CharInfo invalid;
		 HuffmanTree<CharInfo> Tree(_infos, 256, invalid);

		 HuffmanTreeNode<CharInfo> *root = Tree.GetRoot();
		 HuffmanTreeNode<CharInfo> *cur = root;

		 //���ַ��Ĵ���
		 LongType sz = root->_weight._count;

		 //�Զ���ʽ��ѹ���ļ�
		 FILE *fout = fopen(filename.c_str(), "rb");

		 //��д��ʽ��һ���µ��ļ�(��ѹ���ļ�)
		 FILE *fin = fopen(Uncompressname.c_str(), "wb");

		 unsigned char ReadCh = fgetc(fout);
		 int pos = 7;
		 while (ReadCh != EOF)
		 {
			 if (pos >= 0)
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
					 break;
				 }
			 }
			 else
			 {
				 ReadCh = fgetc(fout);
				 pos = 7;
			 }
		 }
		 fclose(fout);
		 fclose(fin);
		 fclose(foutconfig);
		 return Uncompressname;
	 }
protected:
	//ǰ���������Huffman����
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
		// ÿ����һ���ͼ�һ��0
		PrevRootHuffmanCode(cur->_left, s + '0');
		// ÿ����һ���ͼ�һ��1
		PrevRootHuffmanCode(cur->_right, s + '1');
		
	}
protected:
	CharInfo _infos[256];
};

void Test1()
{
	int t1 = clock();
	FileCompress f1;
	string ret = f1.Compress("Input.BIG");
	int t2 = clock();
	cout << t2 - t1 << endl;
	//FileCompress Secf1;
	//string Secret = Secf1.Compress(ret.c_str());
	//FileCompress Thrf1;
	//string Thret = Thrf1.Compress(Secret.c_str());
	FileCompress f2;
	string ret2 = f2.Compress("222.png");
	FileCompress fmp4;
	string ret3 = fmp4.Compress("1.mkv");


	//f1.Uncompress(ret);
	//FILE *fout = fopen(ret.c_str(), "r");
	//char ret1 = fgetc(fout);
	//char ret2 = fgetc(fout);
	//char ret3 = fgetc(fout);
}
void Test2()
{
	FileCompress f3;
	f3.Uncompress("Input.BIG.compress");
	FileCompress f4;
	f4.Uncompress("222.png.compress");
	FileCompress fmp4;
	fmp4.Uncompress("1.mkv.compress");
}