// 单表置换密码.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include"pch.h"
#include<iostream>
#include<stdio.h>
#include<vector>
using namespace std;

char *global_text;
int len_text;

void encrypt(char *plaintext)
{
	//char plaintext[1000] = { '\0' };
	char ciphertext[1000] = { '\0' };
	vector<char>key;
	key.push_back('H');
	key.push_back('K');
	key.push_back('W');
	key.push_back('T');
	key.push_back('X');
	key.push_back('Y');
	key.push_back('S');
	key.push_back('G');
	key.push_back('B');
	key.push_back('P');
	key.push_back('Q');
	key.push_back('E');
	key.push_back('J');
	key.push_back('A');
	key.push_back('Z');
	key.push_back('M');
	key.push_back('L');
	key.push_back('N');
	key.push_back('O');
	key.push_back('F');
	key.push_back('C');
	key.push_back('I');
	key.push_back('D');
	key.push_back('V');
	key.push_back('U');
	key.push_back('R');

	for (unsigned int i = 0; i < strlen(plaintext); i++)
	{
		if (plaintext[i] >= 97 && plaintext[i] <= 122)
		{
			int temp = plaintext[i];
			temp -= 32;
			plaintext[i] = (char)temp;
		}
		if (plaintext[i] >= 65 && plaintext[i] <= 90)
		{
			int tmp = plaintext[i];
			tmp -= 65;
			ciphertext[i] = key[tmp];
		}
		else
		{
			ciphertext[i] = plaintext[i];
		}
	}

	cout << "密文为：" << endl;

	for (unsigned int i = 0; i < strlen(plaintext); i++)
	{
		cout << ciphertext[i];
	}

	cout << endl;

	global_text = ciphertext;

}

char find(char cipher)
{
	vector<char>key;
	key.push_back('H');
	key.push_back('K');
	key.push_back('W');
	key.push_back('T');
	key.push_back('X');
	key.push_back('Y');
	key.push_back('S');
	key.push_back('G');
	key.push_back('B');
	key.push_back('P');
	key.push_back('Q');
	key.push_back('E');
	key.push_back('J');
	key.push_back('A');
	key.push_back('Z');
	key.push_back('M');
	key.push_back('L');
	key.push_back('N');
	key.push_back('O');
	key.push_back('F');
	key.push_back('C');
	key.push_back('I');
	key.push_back('D');
	key.push_back('V');
	key.push_back('U');
	key.push_back('R');
	for (int i = 0; i <= 25; i++)
	{
		if (cipher == key[i])
		{
			int tmp = i;
			tmp += 65;
			cipher = (char)tmp;
			break;
		}
	}
	return cipher;
}

void decrypt(char *ciphertext)
{
	//char *ciphertext = global_text;

	cout << "明文为：" << endl;

	for (unsigned int i = 0; i < len_text; i++)
	{
		ciphertext[i] = find(ciphertext[i]);
		cout << ciphertext[i];
		/*
		else
		{
			ciphertext[i] = global_text[i];
		}
		*/
	}

	//cout << "************" << strlen(global_text) << endl;

	/*for (unsigned int i = 0; i < strlen(global_text); i++)
	{
		cout << ciphertext[i];
	}*/

	cout << endl;
}

int main()
{
	char plaintext[1000] = { '\0' };
	char ciphertext[1000] = { '\0' };
	
	cout << "请输入明文：";

	cin.getline(plaintext, 1000);

	len_text = strlen(plaintext);

	encrypt(plaintext);

	//cout << strlen(global_text) << endl;

	memcpy(ciphertext, global_text,sizeof(ciphertext));

	decrypt(ciphertext);

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
