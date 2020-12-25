
#include "pch.h"
#include "MD5.h"

using namespace std;

//定义压缩函数每轮循环左移的位数
//每一轮循环左移16次，这16次按照重复的4个数字选取左移位数
//如第一轮为
//7 12 17 22
//7 12 17 22
//7 12 17 22
//7 12 17 22

#define S_11 7
#define S_12 12
#define S_13 17
#define S_14 22

#define S_21 5
#define S_22 9
#define S_23 14
#define S_24 20

#define S_31 4
#define S_32 11
#define S_33 16
#define S_34 23

#define S_41 6
#define S_42 10
#define S_43 15
#define S_44 21




//MD5分组处理过程中所用的函数
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))



//左移函数
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))




//大致分为4轮处理，每一轮使用以下4个的一种，重复使用16次
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + ac; \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}


const byte MD5::PADDING[64] = { 0x80 };
const char MD5::HEX[16] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
};


//默认构造函数
MD5::MD5()
{
	reset();
}


//使用输入的数组进行构造
MD5::MD5(const void* input, size_t length)
{
	reset();
	update(input, length);
}


//使用string进行构造
MD5::MD5(const string& str)
{
	reset();
	update(str);
}


const byte* MD5::digest()
{
	if (!_finished)
	{
		_finished = true;
		final();
	}
	return _digest;
}



//重置MD5
void MD5::reset()
{
	_finished = false;

	_count[0] = _count[1] = 0;
	
	_state[0] = 0x67452301;
	_state[1] = 0xefcdab89;
	_state[2] = 0x98badcfe;
	_state[3] = 0x10325476;
}


//使用新输入的数组进行处理
void MD5::update(const void* input, size_t length)
{
	update((const byte*)input, length);
}

//使用新输入的string进行处理
void MD5::update(const string& str)
{
	update((const byte*)str.c_str(), str.length());
}


//MD5对数据的处理以块为单位进行，
//每输入一个块，其结果（中间结果）就发生变化
//因此使用该函数不断更新过程
void MD5::update(const byte* input, size_t length) {

	uint32 i, index, partLen;

	_finished = false;

	//统计消息长度，index单位为字节，count单位为比特（故count右移3位），计算需要按模64进行
	index = (uint32)((_count[0] >> 3) & 0x3f);

	//length单位为字节，故左移3位，表示比特长度
	//小于表明
	if ((_count[0] += ((uint32)length << 3)) < ((uint32)length << 3)) {
		++_count[1];
	}
	_count[1] += ((uint32)length >> 29);	//count[0]获取了后29bit，最前面的3bit放置在count[1]中

	partLen = 64 - index;	//


	if (length >= partLen) {

		memcpy(&_buffer[index], input, partLen);
		transform(_buffer);

		for (i = partLen; i + 63 < length; i += 64) {
			transform(&input[i]);
		}
		index = 0;

	}
	else {
		i = 0;
	}

	//将结果保存到数组中
	memcpy(&_buffer[index], &input[i], length - i);
}


//MD5的最后处理，得到摘要
void MD5::final() {

	byte bits[8];
	uint32 oldState[4];
	uint32 oldCount[2];
	uint32 index, padLen;

	//保存现有的count与state
	memcpy(oldState, _state, 16);
	memcpy(oldCount, _count, 8);

	//保存比特数
	encode(_count, bits, 8);

	//填充
	index = (uint32)((_count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	update(PADDING, padLen);

	//附加长度
	update(bits, 8);

	//保存state
	encode(_state, _digest, 16);

	//保存现有的state与count
	memcpy(_state, oldState, 16);
	memcpy(_count, oldCount, 8);
}




//MD5以块为基本单位进行处理
void MD5::transform(const byte block[64]) {

	uint32 a = _state[0], b = _state[1], c = _state[2], d = _state[3], x[16];

	decode(block, x, 64);

	/* Round 1 */
	FF(a, b, c, d, x[0], S_11, 0xd76aa478); /* 1 */
	FF(d, a, b, c, x[1], S_12, 0xe8c7b756); /* 2 */
	FF(c, d, a, b, x[2], S_13, 0x242070db); /* 3 */
	FF(b, c, d, a, x[3], S_14, 0xc1bdceee); /* 4 */
	FF(a, b, c, d, x[4], S_11, 0xf57c0faf); /* 5 */
	FF(d, a, b, c, x[5], S_12, 0x4787c62a); /* 6 */
	FF(c, d, a, b, x[6], S_13, 0xa8304613); /* 7 */
	FF(b, c, d, a, x[7], S_14, 0xfd469501); /* 8 */
	FF(a, b, c, d, x[8], S_11, 0x698098d8); /* 9 */
	FF(d, a, b, c, x[9], S_12, 0x8b44f7af); /* 10 */
	FF(c, d, a, b, x[10], S_13, 0xffff5bb1); /* 11 */
	FF(b, c, d, a, x[11], S_14, 0x895cd7be); /* 12 */
	FF(a, b, c, d, x[12], S_11, 0x6b901122); /* 13 */
	FF(d, a, b, c, x[13], S_12, 0xfd987193); /* 14 */
	FF(c, d, a, b, x[14], S_13, 0xa679438e); /* 15 */
	FF(b, c, d, a, x[15], S_14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG(a, b, c, d, x[1], S_21, 0xf61e2562); /* 17 */
	GG(d, a, b, c, x[6], S_22, 0xc040b340); /* 18 */
	GG(c, d, a, b, x[11], S_23, 0x265e5a51); /* 19 */
	GG(b, c, d, a, x[0], S_24, 0xe9b6c7aa); /* 20 */
	GG(a, b, c, d, x[5], S_21, 0xd62f105d); /* 21 */
	GG(d, a, b, c, x[10], S_22, 0x2441453); /* 22 */
	GG(c, d, a, b, x[15], S_23, 0xd8a1e681); /* 23 */
	GG(b, c, d, a, x[4], S_24, 0xe7d3fbc8); /* 24 */
	GG(a, b, c, d, x[9], S_21, 0x21e1cde6); /* 25 */
	GG(d, a, b, c, x[14], S_22, 0xc33707d6); /* 26 */
	GG(c, d, a, b, x[3], S_23, 0xf4d50d87); /* 27 */
	GG(b, c, d, a, x[8], S_24, 0x455a14ed); /* 28 */
	GG(a, b, c, d, x[13], S_21, 0xa9e3e905); /* 29 */
	GG(d, a, b, c, x[2], S_22, 0xfcefa3f8); /* 30 */
	GG(c, d, a, b, x[7], S_23, 0x676f02d9); /* 31 */
	GG(b, c, d, a, x[12], S_24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH(a, b, c, d, x[5], S_31, 0xfffa3942); /* 33 */
	HH(d, a, b, c, x[8], S_32, 0x8771f681); /* 34 */
	HH(c, d, a, b, x[11], S_33, 0x6d9d6122); /* 35 */
	HH(b, c, d, a, x[14], S_34, 0xfde5380c); /* 36 */
	HH(a, b, c, d, x[1], S_31, 0xa4beea44); /* 37 */
	HH(d, a, b, c, x[4], S_32, 0x4bdecfa9); /* 38 */
	HH(c, d, a, b, x[7], S_33, 0xf6bb4b60); /* 39 */
	HH(b, c, d, a, x[10], S_34, 0xbebfbc70); /* 40 */
	HH(a, b, c, d, x[13], S_31, 0x289b7ec6); /* 41 */
	HH(d, a, b, c, x[0], S_32, 0xeaa127fa); /* 42 */
	HH(c, d, a, b, x[3], S_33, 0xd4ef3085); /* 43 */
	HH(b, c, d, a, x[6], S_34, 0x4881d05); /* 44 */
	HH(a, b, c, d, x[9], S_31, 0xd9d4d039); /* 45 */
	HH(d, a, b, c, x[12], S_32, 0xe6db99e5); /* 46 */
	HH(c, d, a, b, x[15], S_33, 0x1fa27cf8); /* 47 */
	HH(b, c, d, a, x[2], S_34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II(a, b, c, d, x[0], S_41, 0xf4292244); /* 49 */
	II(d, a, b, c, x[7], S_42, 0x432aff97); /* 50 */
	II(c, d, a, b, x[14], S_43, 0xab9423a7); /* 51 */
	II(b, c, d, a, x[5], S_44, 0xfc93a039); /* 52 */
	II(a, b, c, d, x[12], S_41, 0x655b59c3); /* 53 */
	II(d, a, b, c, x[3], S_42, 0x8f0ccc92); /* 54 */
	II(c, d, a, b, x[10], S_43, 0xffeff47d); /* 55 */
	II(b, c, d, a, x[1], S_44, 0x85845dd1); /* 56 */
	II(a, b, c, d, x[8], S_41, 0x6fa87e4f); /* 57 */
	II(d, a, b, c, x[15], S_42, 0xfe2ce6e0); /* 58 */
	II(c, d, a, b, x[6], S_43, 0xa3014314); /* 59 */
	II(b, c, d, a, x[13], S_44, 0x4e0811a1); /* 60 */
	II(a, b, c, d, x[4], S_41, 0xf7537e82); /* 61 */
	II(d, a, b, c, x[11], S_42, 0xbd3af235); /* 62 */
	II(c, d, a, b, x[2], S_43, 0x2ad7d2bb); /* 63 */
	II(b, c, d, a, x[9], S_44, 0xeb86d391); /* 64 */

	_state[0] += a;
	_state[1] += b;
	_state[2] += c;
	_state[3] += d;
}


//512比特的数据映射为128比特
void MD5::encode(const uint32* input, byte* output, size_t length) {

	for (size_t i = 0, j = 0; j < length; ++i, j += 4) {
		output[j] = (byte)(input[i] & 0xff);
		output[j + 1] = (byte)((input[i] >> 8) & 0xff);
		output[j + 2] = (byte)((input[i] >> 16) & 0xff);
		output[j + 3] = (byte)((input[i] >> 24) & 0xff);
	}
}

//128比特的数据映射为512比特
void MD5::decode(const byte* input, uint32* output, size_t length) {

	for (size_t i = 0, j = 0; j < length; ++i, j += 4) {
		output[i] = ((uint32)input[j]) | (((uint32)input[j + 1]) << 8) |
			(((uint32)input[j + 2]) << 16) | (((uint32)input[j + 3]) << 24);
	}
}


//将结果转为16进制string
string MD5::bytes_to_string(const byte* input, size_t length) {

	string str;
	str.reserve(length << 1);
	for (size_t i = 0; i < length; ++i) {
		int t = input[i];
		int a = t / 16;
		int b = t % 16;
		str.append(1, HEX[a]);
		str.append(1, HEX[b]);
	}
	return str;
}



//将摘要保存为string
string MD5::to_string() {
	return bytes_to_string(digest(), 16);
}






