#pragma once
#include <string>
#include "RSA.h"
class encrypt_decrypt
{
public:
	encrypt_decrypt() {}
	~encrypt_decrypt() {}

	void menu();		//��ӡ��Ϣ
	bool encrypt();		//����
	bool decrypt();		//����
	void print();		//��ӡRSA�����Ϣ
	void reset();		//����RSA�����Ϣ

protected:
	void load(int);		//���ݸ���λ������RSA����
	bool is_legal(const string &);	//�ж������ַ����Ƿ�Ϸ�

private:
	RSA rsa;
};