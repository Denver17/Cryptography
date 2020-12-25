#pragma once
#include <string>
#include "RSA.h"
class encrypt_decrypt
{
public:
	encrypt_decrypt() {}
	~encrypt_decrypt() {}

	void menu();		//打印信息
	bool encrypt();		//加密
	bool decrypt();		//解密
	void print();		//打印RSA相关信息
	void reset();		//重置RSA相关信息

protected:
	void load(int);		//根据给定位数加载RSA对象
	bool is_legal(const string &);	//判断输入字符串是否合法

private:
	RSA rsa;
};