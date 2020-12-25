// RSA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <time.h>
#include "RSA.h"

using namespace std;


void RSA::init(const unsigned len)
{
	srand((unsigned)time(NULL));
	//产生大素数p与q
	p = create_prime(len, 15);	//出错概率为(1/4)^15
	q = create_prime(len, 15);

	cout << "p=" << p << endl;
	cout << "q=" << q << endl;

	n = p * q;

	cout << "n=" << n << endl;

	eul = (p - 1)*(q - 1);

	create_exponent(eul);
}


//使用公钥进行加密
big_num RSA::encrypt_by_public(const big_num & m)
{
	return m.mod_pow(e, n);
}


//使用私钥进行解密
big_num RSA::decrypt_by_private(const big_num & c)
{
	return c.mod_pow(d, n);
}



//使用私钥进行加密
big_num RSA::encrypt_by_private(const big_num & m)
{
	return decrypt_by_private(m);
}


//使用公钥进行解密
big_num RSA::decrypt_by_public(const big_num & c)
{
	return encrypt_by_public(c);
}


//输出RSA相关数据
ostream & operator << (ostream & out, const RSA & rsa)
{
	out << "n: " << rsa.n << "\n";
	out << "p: " << rsa.p << "\n";
	out << "q: " << rsa.q << "\n";
	out << "e: " << rsa.e << "\n";
	out << "d: " << rsa.d << "\n";
	return out;
}



//生成一个长度为len的奇数
//我们采取二进制，len表示二进制数的长度
big_num RSA::create_odd_num(unsigned len)
{
	static const char hex_table[] = { '0', '1', '2', '3', '4', '5', '6', '7',
									  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	
	len >>= 2;	//十六进制数据每位占4位二进制
	if (len)
	{
		ostringstream oss;		//注意头文件sstream
		for (size_t i = 0; i < len - 1; i++)
		{
			oss << hex_table[rand() % 16];
		}
		oss << hex_table[1];

		//cout << string(oss.str()) << endl;
		//big_num num;

		return big_num(oss.str());
	}
	return big_num("F");
};





//判断一个数是否为素数，使用米勒拉宾大素数检测算法
//num代表需要判断的数，k代表测试次数
bool RSA::is_prime(const big_num & num, const unsigned k)
{
	assert(num != big_num::ZERO);	//若num为0，则报错
	if (num == big_num::ONE)		//1不为素数
		return false;	
	if (num == big_num::TWO)		//2是素数
		return true;

	big_num t = num - 1;
	big_num::bit b(t);				//采用二进制进行处理
	if (b.at(0) == 1)				//减去1之后为奇数，说明原数为偶数，不为素数
		return false;

	//num-1 = 2^s*d
	size_t s = 0;					//用于统计二进制末尾有多少个0
	big_num d(t);
	for (size_t i = 0; i < b.size(); i++)
	{
		if (!b.at(i))
		{
			s++;
			d = d.right_shift(1);
		}
		else
			break;
	}

	for (size_t i = 0; i < k; i++)	//测试次数为k
	{
		
		big_num a = create_rand_smaller(num);	//生成一个介于1至num-1之间的随机数
		big_num x = a.mod_pow(d, num);			//若a^(num-1)=1(mod num)，

		if (x == big_num::ONE)					//这次测试是素数
			continue;
		bool ok = true;
		
		//二次检测
		for (size_t j = 0; j < s&&ok; j++)
		{
			if (x == t)							//若a^(num-1)=num-1(mod num);
				ok = false;
			x = x.mul(x).mod(num);
		}
		if (ok)
			return false;
	}
	return true;	//测试通过，极有可能为素数

}



//随机生成一个比val小的数
//val代表比较的那个数
big_num RSA::create_rand_smaller(const big_num & val)
{
	big_num::base_t t = 0;
	do
	{
		t = rand();
	} while (t == 0);//生成非0随机数

	big_num mod(t);
	big_num num = mod % val;	//生成的数不能超过val
	if (num == big_num::ZERO)	//若生成的数正好能整除val，那么我们对num取val-1
		num = val - big_num::ONE;
	return num;
}



//生成一个二进制长度为len的大素数
//k代表素数检测的次数
big_num RSA::create_prime(unsigned len, const unsigned k)
{
	assert(k > 0);	//检测次数必须大于0
	big_num num = create_odd_num(len);	//先生成一个奇数
	while (!is_prime(num, k))	//米勒拉宾素性检测
	{
		num = num.add(big_num::TWO);	//若该奇数不为素数，我们检测其下一个素数
	}
	return num;
}



//有提供的欧拉数生成公钥、私钥指数
void RSA::create_exponent(const big_num & eul)
{
	e = 65537;
	d = e.mod_inverse(eul);
}


















