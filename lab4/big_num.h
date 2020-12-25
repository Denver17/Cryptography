#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <vector>
using namespace std;

class big_num
{
public:

	typedef long long long_t;
	typedef unsigned base_t;
	big_num() : is_negative(false) { data.push_back(0); }
	big_num(const big_num &);												//利用指定的大整数进行初始化
	big_num(const string &);												//利用十六进制字符串进行初始化
	big_num(const long_t &);												//利用指定的long_t进行初始化
	~big_num();
	

	big_num add(const big_num &);											//大整数加法
	big_num sub(const big_num &);											//大整数减法
	big_num mul(const big_num &) const;										//大整数乘法
	big_num div(const big_num &);											//大整数除法
	big_num remain(const big_num &);										//大整数取余
	big_num mod(const big_num &);											//大整数取模
	big_num div_rem(const big_num &, big_num &);							//大整数整除并取余
	big_num pow(const big_num &);											//大整数幂乘
	big_num mod_pow(const big_num &, const big_num &) const;				//大整数幂模
	big_num mod_inverse(const big_num &);									//大整数使用扩展欧几里得算法求乘法逆元

	big_num left_shift(const unsigned);										//左移运算
	big_num right_shift(const unsigned);									//右移运算

	int my_compare(const big_num &) const;									//比较运算
	bool my_equal(const big_num &) const;									//判断是否等于指定的数
	static big_num to_bignum(const long_t &);								//将指定的数转化为大整数
	string to_string() const;												//将大整数转化为十六进制字符串
	
	big_num my_abs() const;													//获取大整数的绝对值

protected:

	//运算符重载
	friend big_num operator + (const big_num &, const big_num &);
	friend big_num operator - (const big_num &, const big_num &);
	friend big_num operator * (const big_num &, const big_num &);
	friend big_num operator / (const big_num &, const big_num &);
	friend big_num operator % (const big_num &, const big_num &);
	friend bool operator < (const big_num &, const big_num &);
	friend bool operator > (const big_num &, const big_num &);
	friend bool operator == (const big_num &, const big_num &);
	friend bool operator <= (const big_num &, const big_num &);
	friend bool operator >= (const big_num &, const big_num &);
	friend bool operator != (const big_num &, const big_num &);



	friend big_num operator + (const big_num &, const long_t &);
	friend big_num operator - (const big_num &, const long_t &);
	friend big_num operator * (const big_num &, const long_t &);
	friend big_num operator / (const big_num &, const long_t &);
	friend big_num operator % (const big_num &, const long_t &);
	friend bool operator < (const big_num &, const long_t &);
	friend bool operator > (const big_num &, const long_t &);
	friend bool operator == (const big_num &, const long_t &);
	friend bool operator <= (const big_num &, const long_t &);
	friend bool operator >= (const big_num &, const long_t &);
	friend bool operator != (const big_num &, const long_t &);

	friend ostream & operator << (ostream &, const big_num &);
	big_num operator = (const string & str) { return (*this) = big_num(str); }
	big_num operator = (const long_t & num) { return (*this) = big_num(num); }


private:
	big_num trim();							//去掉高位多余的0
	int hex_to_dec(char);					//十六进制字符转化为十进制数

public:
	static const int base_bit = 5;			//2^5=32,大整数每位存储的二进制位数
	static const int base_char = 8;			//组成大整数的移位需要的十六进制位数
	static const int base_int = 32;			//大整数的一位对应的二进制位数
	static const int base_num = 0xffffffff;	//截取低位的辅助
	static const int base_temp = 0x1f;		//截取模32的余数的辅助
	static const big_num ZERO;				//大整数常量0
	static const big_num ONE;				//大整数常量1
	static const big_num TWO;				//大整数常量2
	static const big_num TEN;				//大整数常量10


private:
	bool is_negative;						//用于标记是否为负数
	vector<base_t> data;					//按位存储数据，高位在后方
	
	class bit {								//使得大整数能够进行二进制处理
	public:
		bit(const big_num &);				//根据大整数进行初始化

		size_t size() { return length; }	//返回大整数对应的二进制数的位数
		bool at(size_t);					//返回第i为二进制是否为1

	private:
		vector<base_t> bit_vector;			//二进制数据存储，每个元素对应32位二进制
		size_t length;						//二进制总位数
	};
	friend class RSA;						//定义RSA为其友元类

};