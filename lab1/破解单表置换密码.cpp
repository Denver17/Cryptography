// 破解单表置换密码.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<map>
using namespace std;

int main()
{
	double pra = 0;
	int sum = 0;
	double my_sort[30];
    char text[1000] = { '\0' };
	map<int, double>freq;

	cin.getline(text, 1000);

	for (int i = 0; i < 26; i++)
	{
		freq[i] = 0.0;
	}

	for (int i = 0; i < strlen(text); i++)
	{
		if (text[i] >= 65 && text[i] <= 90)
		{
			sum++;
		}
	}

	for (int i = 0; i < strlen(text); i++)
	{
		if (text[i] >= 65 && text[i] <= 90)
		{
			int temp = text[i];
			temp -= 65;
			freq[temp] = freq[temp] + 1;
		}
	}

	for (int i = 0; i < 26; i++)
	{
		int temp = 65 + i;
		char tmp = (char)temp;
		freq[i] = freq[i]/sum;
		pra += freq[i];
		my_sort[i] = freq[i] * 100;
		cout << tmp << "=" << freq[i] * 100 << endl;
		
	}

	for (int i = 0; i < 26; i++)
		for (int j = 0; j < i; j++)
		{
			if (my_sort[i] > my_sort[j])
			{
				double my_temp = my_sort[i];
				my_sort[i] = my_sort[j];
				my_sort[j] = my_temp;
			}
		}

	for (int i = 0; i < 26; i++)
	{
		cout << my_sort[i] << endl;
	}


	cout << "pra=" << pra << endl;

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
