#include "pch.h"
#include <iostream>
#include <ctime>
#include "encrypt_decrypt.h"

/**
 * 函数功能:菜单显示
 */
void encrypt_decrypt::menu() {
	std::cout << "**********Welcome to use RSA encoder**********" << std::endl;
	std::cout << "               e: encrypt 加密               " << std::endl;
	std::cout << "               d: decrypt 解密               " << std::endl;
	std::cout << "               p: print   显示               " << std::endl;
	std::cout << "               r: reset   重置               " << std::endl;
	std::cout << "               q: quit    退出               " << std::endl;
	std::cout << "input your choice:" << std::endl;

}

/**
 * 函数功能:加密运算
 */
bool encrypt_decrypt::encrypt() {
	std::string str;
	std::cout << "输入16进制数据:" << std::endl;
	std::cout << ">";
	std::cin >> str;// 输入明文
	if (!std::cin || !is_legal(str))
		return false;
	big_num m(str);
	clock_t start = clock();
	big_num c = rsa.encrypt_by_public(m);
	clock_t finish = clock();

	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "用时: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;
	std::cout << "明文: " << m << std::endl;
	std::cout << "密文: " << c << std::endl;
	return true;

}

/**
 * 函数功能:解密运算
 */
bool encrypt_decrypt::decrypt() {
	std::string str;
	std::cout << "输入16进制数据:" << std::endl;
	std::cout << ">";
	std::cin >> str;// 输入密文
	if (!std::cin || !is_legal(str))
		return false;
	big_num c(str);
	clock_t start = clock();
	big_num m = rsa.decrypt_by_private(c);
	clock_t finish = clock();

	std::cout << std::fixed;
	std::cout.precision(3);
	//std::cout << "用时: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;
	std::cout << "密文: " << c << std::endl;
	std::cout << "明文: " << m << std::endl;
	return true;

}

/**
 * 函数功能:输出RSA相关信息
 */
void encrypt_decrypt::print() {
	std::cout << rsa << std::endl;

}

/**
 * 函数功能:重置RSA相关信息
 */
void encrypt_decrypt::reset() {
	std::cout << "输入密钥长度: ";
	int len;
	std::cin >> len;
	load(len >> 1);
}

/**
 * 函数功能:根据给定位数len加载rsa
 */
void encrypt_decrypt::load(int len) {
	std::cout << "初始化..." << std::endl;
	clock_t start = clock();
	rsa.init(len);    // 初始化
	clock_t finish = clock();
	std::cout << "初始化完成." << std::endl;
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "用时: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;

}

/**
 * 函数功能:判断输入字符串str是否合法
 * 参数含义:str代表输入的字符串
 */
bool encrypt_decrypt::is_legal(const std::string & str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!isalnum(*it))    // 不是字母或者数字
			return false;
		if (isalpha(*it)) {
			char ch = tolower(*it);
			if (ch > 'f')    // 超过十六进制字符'f'
				return false;

		}

	}
	return true;

}