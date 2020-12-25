#pragma once
#include "big_num.h"
#include <ostream>

using namespace std;

class RSA
{
public:
	RSA(){}
	RSA(const unsigned len) { init(len); }			//利用len初始化对象
	~RSA(){}

	void init(const unsigned);						//初始化，产生公私钥对

	big_num encrypt_by_public(const big_num &);		//公钥加密
	big_num decrypt_by_private(const big_num &);	//私钥解密

	//可用于数字签名
	big_num encrypt_by_private(const big_num &);	//私钥加密
	big_num decrypt_by_public(const big_num &);		//公钥解密

protected:
	friend ostream & operator << (ostream &, const RSA &);	//输出相关数据

private:
	big_num create_odd_num(unsigned);						//生成一个大奇数，参数为其长度
	bool is_prime(const big_num &, const unsigned);			//判断是否为素数
	big_num create_rand_smaller(const big_num &);			//随机生成一个更小的数
	big_num create_prime(unsigned, const unsigned);			//生成一个大素数，参数为其长度
	void create_exponent(const big_num &);					//根据提供的欧拉数生成公钥、私钥指数

public:
	big_num n, e;		//公钥

private:
	big_num d;			//私钥
	big_num p, q;		//大素数p、q
	big_num eul;		//n的欧拉函数


};