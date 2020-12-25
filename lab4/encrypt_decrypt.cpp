#include "pch.h"
#include <iostream>
#include <ctime>
#include "encrypt_decrypt.h"

/**
 * ��������:�˵���ʾ
 */
void encrypt_decrypt::menu() {
	std::cout << "**********Welcome to use RSA encoder**********" << std::endl;
	std::cout << "               e: encrypt ����               " << std::endl;
	std::cout << "               d: decrypt ����               " << std::endl;
	std::cout << "               p: print   ��ʾ               " << std::endl;
	std::cout << "               r: reset   ����               " << std::endl;
	std::cout << "               q: quit    �˳�               " << std::endl;
	std::cout << "input your choice:" << std::endl;

}

/**
 * ��������:��������
 */
bool encrypt_decrypt::encrypt() {
	std::string str;
	std::cout << "����16��������:" << std::endl;
	std::cout << ">";
	std::cin >> str;// ��������
	if (!std::cin || !is_legal(str))
		return false;
	big_num m(str);
	clock_t start = clock();
	big_num c = rsa.encrypt_by_public(m);
	clock_t finish = clock();

	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "��ʱ: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;
	std::cout << "����: " << m << std::endl;
	std::cout << "����: " << c << std::endl;
	return true;

}

/**
 * ��������:��������
 */
bool encrypt_decrypt::decrypt() {
	std::string str;
	std::cout << "����16��������:" << std::endl;
	std::cout << ">";
	std::cin >> str;// ��������
	if (!std::cin || !is_legal(str))
		return false;
	big_num c(str);
	clock_t start = clock();
	big_num m = rsa.decrypt_by_private(c);
	clock_t finish = clock();

	std::cout << std::fixed;
	std::cout.precision(3);
	//std::cout << "��ʱ: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;
	std::cout << "����: " << c << std::endl;
	std::cout << "����: " << m << std::endl;
	return true;

}

/**
 * ��������:���RSA�����Ϣ
 */
void encrypt_decrypt::print() {
	std::cout << rsa << std::endl;

}

/**
 * ��������:����RSA�����Ϣ
 */
void encrypt_decrypt::reset() {
	std::cout << "������Կ����: ";
	int len;
	std::cin >> len;
	load(len >> 1);
}

/**
 * ��������:���ݸ���λ��len����rsa
 */
void encrypt_decrypt::load(int len) {
	std::cout << "��ʼ��..." << std::endl;
	clock_t start = clock();
	rsa.init(len);    // ��ʼ��
	clock_t finish = clock();
	std::cout << "��ʼ�����." << std::endl;
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "��ʱ: " << (double)(finish - start) / CLOCKS_PER_SEC << "s." << std::endl;

}

/**
 * ��������:�ж������ַ���str�Ƿ�Ϸ�
 * ��������:str����������ַ���
 */
bool encrypt_decrypt::is_legal(const std::string & str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!isalnum(*it))    // ������ĸ��������
			return false;
		if (isalpha(*it)) {
			char ch = tolower(*it);
			if (ch > 'f')    // ����ʮ�������ַ�'f'
				return false;

		}

	}
	return true;

}