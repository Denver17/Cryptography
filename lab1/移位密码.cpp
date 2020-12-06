// 移位密码.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
//#include<vector>
//#include<string>
//#include<stdio.h>
using namespace std;

char *global_text;

void encrpty(char *plaintext, int key)
{
	for (int i = 0; i < strlen(plaintext); i++)
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
			tmp += key;
			tmp -= 65;
			tmp = tmp % 26;
			tmp += 65;
			plaintext[i] = char(tmp);
		}
	}

	cout << "密文为：" << endl;

	for (int i = 0; i < strlen(plaintext); i++)
	{
		cout << plaintext[i];
	}

	cout << endl;

	global_text = plaintext;
}

void decrpty(char *ciphertext, int key)
{
	for (int i = 0; i < strlen(ciphertext); i++)
	{
		if (ciphertext[i] >= 65 && ciphertext[i] <= 90)
		{
			int tmp = ciphertext[i];
			tmp -= 65;
			tmp += (26 - key);
			tmp = tmp % 26;
			tmp += 65;
			ciphertext[i] = char(tmp);
		}
	}
	cout << "明文为：" << endl;

	for (int i = 0; i < strlen(ciphertext); i++)
	{
		cout << ciphertext[i];
	}
	cout << endl;
}


int main()
{
	char plaintext[1000] = { '\0' };
	char ciphertext[1000] = { '\0' };
	
	cout << "请输入明文：" << endl;
	cin.getline(plaintext,1000);
	int key;
	cout << "请输入移位数：" << endl;
	cin >> key;
	
	encrpty(plaintext, key);

	memcpy(ciphertext, global_text, sizeof(ciphertext));

	decrpty(ciphertext, key);

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
