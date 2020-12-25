#pragma once
#include "big_num.h"
#include <ostream>

using namespace std;

class RSA
{
public:
	RSA(){}
	RSA(const unsigned len) { init(len); }			//����len��ʼ������
	~RSA(){}

	void init(const unsigned);						//��ʼ����������˽Կ��

	big_num encrypt_by_public(const big_num &);		//��Կ����
	big_num decrypt_by_private(const big_num &);	//˽Կ����

	//����������ǩ��
	big_num encrypt_by_private(const big_num &);	//˽Կ����
	big_num decrypt_by_public(const big_num &);		//��Կ����

protected:
	friend ostream & operator << (ostream &, const RSA &);	//����������

private:
	big_num create_odd_num(unsigned);						//����һ��������������Ϊ�䳤��
	bool is_prime(const big_num &, const unsigned);			//�ж��Ƿ�Ϊ����
	big_num create_rand_smaller(const big_num &);			//�������һ����С����
	big_num create_prime(unsigned, const unsigned);			//����һ��������������Ϊ�䳤��
	void create_exponent(const big_num &);					//�����ṩ��ŷ�������ɹ�Կ��˽Կָ��

public:
	big_num n, e;		//��Կ

private:
	big_num d;			//˽Կ
	big_num p, q;		//������p��q
	big_num eul;		//n��ŷ������


};