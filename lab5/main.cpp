// MD5.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "md5.h"
#include <iostream>
#include <bitset>

using namespace std;

void PrintMD5(const string& str, MD5& md5) {
	cout << "MD5(\"" << str << "\") = " << md5.to_string() << endl;
}

bitset<128> str_to_bit(MD5& md5)
{
	bitset<128> res;
	string binary = md5.to_string();
	for (int i = 0; i < 16; i++)
	{
		string b_temp = binary.substr(i * 2, 2);
		bitset<8> temp(stoi(b_temp, nullptr, 16));
		for (int j = 0; j < 8; j++)
		{
			res[i * 8 + j] = temp[j];
		}
		//cout << temp << endl;
	}
	//cout << res << endl;
	return res;
}


int comp(bitset<128> comp_1, bitset<128> comp_2)
{
	int sum = 0;
	for (int i = 0; i < 128; i++)
	{
		if (comp_1[i] != comp_2[i])
		{
			sum++;
		}
	}
	cout << "总共改变的位数为：" << sum << endl;
	return sum;
}

int main() {

	int change_sum[8];			//存储改变的位数
	double change_avg = 0;			//存储改变的位数的平均值

	bitset<128> comp_1;
	bitset<128> comp_2;

	MD5 md5;
	md5.update("");
	PrintMD5("", md5);
	comp_1 = str_to_bit(md5);	//将string转为bit


	md5.reset();
	md5.update("x");
	PrintMD5("x", md5);
	comp_2 = str_to_bit(md5);


	change_sum[0] = comp(comp_1, comp_2);	//对二者进行比较




	md5.reset();
	md5.update("a");
	PrintMD5("a", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("2a");
	PrintMD5("2a", md5);
	comp_2 = str_to_bit(md5);

	change_sum[1] = comp(comp_1, comp_2);
	




	md5.reset();
	md5.update("abc");
	PrintMD5("abc", md5);
	comp_1 = str_to_bit(md5);


	md5.reset();
	md5.update("jabc");
	PrintMD5("jabc", md5);
	comp_2 = str_to_bit(md5);

	change_sum[2] = comp(comp_1, comp_2);





	md5.reset();
	md5.update("message digest");
	PrintMD5("message digest", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("message, digest");
	PrintMD5("message, digest", md5);
	comp_2 = str_to_bit(md5);

	change_sum[3] = comp(comp_1, comp_2);




	md5.reset();
	md5.update("abcdefghijklmnopqrstuvwxyz");
	PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("abcdefghijklmno pqrstuvwxyz");
	PrintMD5("abcdefghijklmno pqrstuvwxyz", md5);
	comp_2 = str_to_bit(md5);

	change_sum[4] = comp(comp_1, comp_2);





	md5.reset();
	md5.update("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	PrintMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("ABCDEFGHIJKLMNO'PQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	PrintMD5("ABCDEFGHIJKLMNO'PQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", md5);
	comp_2 = str_to_bit(md5);

	change_sum[5] = comp(comp_1, comp_2);






	md5.reset();
	md5.update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	PrintMD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("12345678901234567890a123456789012345678901234567890123456789012345678901234567890");
	PrintMD5("12345678901234567890a123456789012345678901234567890123456789012345678901234567890", md5);
	comp_2 = str_to_bit(md5);

	change_sum[6] = comp(comp_1, comp_2);




	md5.reset();
	md5.update("1813540");
	PrintMD5("1813540", md5);
	comp_1 = str_to_bit(md5);

	md5.reset();
	md5.update("1,813540");
	PrintMD5("1,813540", md5);
	comp_2 = str_to_bit(md5);

	change_sum[7] = comp(comp_1, comp_2);



	for (int i = 0; i < 8; i++)
	{
		change_avg += change_sum[i];
	}

	cout << "平均改变位数：" << change_avg / 8 << endl;



	return 0;
}
