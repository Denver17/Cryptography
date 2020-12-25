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
	big_num(const big_num &);												//����ָ���Ĵ��������г�ʼ��
	big_num(const string &);												//����ʮ�������ַ������г�ʼ��
	big_num(const long_t &);												//����ָ����long_t���г�ʼ��
	~big_num();
	

	big_num add(const big_num &);											//�������ӷ�
	big_num sub(const big_num &);											//����������
	big_num mul(const big_num &) const;										//�������˷�
	big_num div(const big_num &);											//����������
	big_num remain(const big_num &);										//������ȡ��
	big_num mod(const big_num &);											//������ȡģ
	big_num div_rem(const big_num &, big_num &);							//������������ȡ��
	big_num pow(const big_num &);											//�������ݳ�
	big_num mod_pow(const big_num &, const big_num &) const;				//��������ģ
	big_num mod_inverse(const big_num &);									//������ʹ����չŷ������㷨��˷���Ԫ

	big_num left_shift(const unsigned);										//��������
	big_num right_shift(const unsigned);									//��������

	int my_compare(const big_num &) const;									//�Ƚ�����
	bool my_equal(const big_num &) const;									//�ж��Ƿ����ָ������
	static big_num to_bignum(const long_t &);								//��ָ������ת��Ϊ������
	string to_string() const;												//��������ת��Ϊʮ�������ַ���
	
	big_num my_abs() const;													//��ȡ�������ľ���ֵ

protected:

	//���������
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
	big_num trim();							//ȥ����λ�����0
	int hex_to_dec(char);					//ʮ�������ַ�ת��Ϊʮ������

public:
	static const int base_bit = 5;			//2^5=32,������ÿλ�洢�Ķ�����λ��
	static const int base_char = 8;			//��ɴ���������λ��Ҫ��ʮ������λ��
	static const int base_int = 32;			//��������һλ��Ӧ�Ķ�����λ��
	static const int base_num = 0xffffffff;	//��ȡ��λ�ĸ���
	static const int base_temp = 0x1f;		//��ȡģ32�������ĸ���
	static const big_num ZERO;				//����������0
	static const big_num ONE;				//����������1
	static const big_num TWO;				//����������2
	static const big_num TEN;				//����������10


private:
	bool is_negative;						//���ڱ���Ƿ�Ϊ����
	vector<base_t> data;					//��λ�洢���ݣ���λ�ں�
	
	class bit {								//ʹ�ô������ܹ����ж����ƴ���
	public:
		bit(const big_num &);				//���ݴ��������г�ʼ��

		size_t size() { return length; }	//���ش�������Ӧ�Ķ���������λ��
		bool at(size_t);					//���ص�iΪ�������Ƿ�Ϊ1

	private:
		vector<base_t> bit_vector;			//���������ݴ洢��ÿ��Ԫ�ض�Ӧ32λ������
		size_t length;						//��������λ��
	};
	friend class RSA;						//����RSAΪ����Ԫ��

};