#pragma once

#include <string>
#include <fstream>

using namespace std;


typedef unsigned char byte;		//char��Ϊ8bit
typedef unsigned int uint32;	//int��Ϊ32bit


//����MD5��
class MD5 
{
public:
	MD5();
	MD5(const void* input, size_t length);
	MD5(const string& str);
	void update(const void* input, size_t length);
	void update(const string& str);
	const byte* digest();
	string to_string();
	void reset();

private:
	void update(const byte* input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32* input, byte* output, size_t length);
	void decode(const byte* input, uint32* output, size_t length);
	string bytes_to_string(const byte* input, size_t length);

	MD5(const MD5&);
	MD5& operator = (const MD5&);

private:
	uint32 _state[4];				//�ĸ�32bit�Ĵ���������
	uint32 _count[2];				//��Ϣ�����ǰ�ĳ��ȣ�С�˴洢����64bit
	byte _buffer[64];				//��������
	byte _digest[16];				//ժҪ�������������̶�Ϊ128����
	bool _finished;					//�ж������Ƿ����

	static const byte PADDING[64];
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };

};