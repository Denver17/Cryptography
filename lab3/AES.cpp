// AES.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;


/**
 * S盒
 */
static const int S[16][16] = 
{   
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

/**
 * 逆S盒
 */
static const int S_1[16][16] = 
{ 
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};


//列混淆左乘矩阵
const unsigned char mix_array[4][4] =
{
	0x02, 0x03, 0x01, 0x01,
	0x01, 0x02, 0x03, 0x01,
	0x01, 0x01, 0x02, 0x03,
	0x03, 0x01, 0x01, 0x02
};

//列混淆左乘逆矩阵
const unsigned char mix_array_1[4][4] =
{
	0x0E, 0x0B, 0x0D, 0x09,
	0x09, 0x0E, 0x0B, 0x0D,
	0x0D, 0x09, 0x0E, 0x0B,
	0x0B, 0x0D, 0x09, 0x0E
};



//G函数中所用轮系数
const unsigned int Rcon[11] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };


//用于输出密文/明文的函数
void print_text(const unsigned char* text, int text_size)
{
	for (int i = 0; i < text_size; i++)
	{
		cout << hex << (int)text[i] << " ";
	}
	cout << endl;
}




//G函数中运用S盒进行的代换操作
void Key_S(unsigned char(*extend_key_array)[44], unsigned int n_col)
{
	for (int i = 0; i < 4; i++)
	{
		//extend_key_array共有8位，我们取高4位表示S盒的行数，取低4位表示S盒的列数
		extend_key_array[i][n_col] = S[(extend_key_array[i][n_col]) >> 4][(extend_key_array[i][n_col]) & 0x0F];
	}
}


//G函数处理过程
void G_Function(unsigned char(*extend_key_array)[44], unsigned int n_col)
{
	//1、将扩展密钥矩阵的n_col-1列复制到n_col列，并将n_col列上的第一行移到最后一行，其余行向上移一行
	for (int i = 0; i < 4; i++)
	{
		extend_key_array[i][n_col] = extend_key_array[(i + 1) % 4][n_col - 1];
	}

	//2、对n_col这一列进行S盒代换(利用Key_S函数)
	Key_S(extend_key_array, n_col);

	//3、将n_col列中的第一行行元素与Rcon数组进行异或运算
	extend_key_array[0][n_col] ^= Rcon[n_col / 4];

}



//该函数得到每一轮的子密钥

void get_keys(const unsigned char(*key_array)[4], unsigned char(*extend_key_array)[44])
{
	//1、将密钥数组放到前四列扩展密钥数组
	for (int i = 0; i < 16; i++)
	{
		//行号取i的低2位，列号取i的高2位
		extend_key_array[i & 0x03][i >> 2] = key_array[i & 0x03][i >> 2];
	}

	//2、计算扩展矩阵的后四十列
	for (int i = 1; i < 11; i++)
	{
		//(1)若列号为4的倍数，那么需要执行G函数，否则就直接将n_col-1复制到n_col上

		//A.列号为4的倍数的情况
		G_Function(extend_key_array, 4 * i);
		
		//异或操作
		for (int k = 0; k < 4; k++)
		{
			//这里我们使用G函数将倍数为4的列号(从1开始)赋值给了下一轮密钥的第一列，因此直接使用这一列
			extend_key_array[k][4 * i] = extend_key_array[k][4 * i] ^ extend_key_array[k][4 * (i - 1)];
		}

		//B.列号不为4的倍数的情况
		for (int j = 1; j < 4; j++)
		{
			//这三列每一列都有4行
			for (int k = 0; k < 4; k++)
			{
				//注意：考虑子密钥k1的情况，k1的w5是由w1与（w0经过和w3(经G函数处理)异或之后得到的），而不是w0本身
				//因此这里使用了4*i+j-1
				extend_key_array[k][4 * i + j] = extend_key_array[k][4 * i + j - 1] ^ extend_key_array[k][4 * (i - 1) + j];
			}
		}

	}

}



//该函数将输入的明文与密钥转化为矩阵形式
void input_to_array(const unsigned char *input,unsigned char (*array)[4])
{
	for (int i = 0; i < 16; i++)
	{
		array[i & 0x03][i >> 2] = input[i];
	}
}


//该函数进行轮密钥加，将明文数组与密钥扩展数组进行异或操作
//最后一个参数是最小的列号
//这是由于需要进行多次轮密钥相加，而每一次使用的扩展密钥数组中不同的列数开始，因此需要一个列号来标识
void round_key_add(unsigned char(*plain_array)[4], unsigned char(*extend_key_array)[44],unsigned int min_col)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//注意extend_key_array中min_col的运用
			plain_array[i][j] ^= extend_key_array[i][j + min_col];
		}
	}
}


//A.字节代换层函数
void plain_S_Sub(unsigned char *plain_array)
{
	for (int i = 0; i < 16; i++)
	{
		//plain_array[i]中的高4位用来表示S盒中的行号，低4位用来表示S盒中的列号
		plain_array[i] = S[plain_array[i] >> 4][plain_array[i] & 0x0F];
	}
}

//B.行左位移函数
void left_shift_row(unsigned int *plain_array)
{
	//对于明文对应的矩阵，第一行不需要移位

	//第二行左移位8bit
	plain_array[1] = (plain_array[1] >> 8) | (plain_array[1] << 24);//这里我们将第二行右移8bit，同时将第二行左移24bit，再进行按位或，得到所需效果

	//第三行左移位16bit
	plain_array[2] = (plain_array[2] >> 16) | (plain_array[2] << 16);

	//第四行左移位24bit
	plain_array[3] = (plain_array[3] >> 24) | (plain_array[3] << 8);

}


//C.列混淆层函数
//该层需要用到的伽罗瓦乘法运算函数
char Gal_Mul(unsigned char L_num, unsigned char R_num)
{
	unsigned char result = 0;

	while (L_num)
	{
		if (L_num & 0x01)//若L_num的最低位是1，那么我们就将result与R_num异或，并将结果赋给result
		{
			result ^= R_num;
		}

		//将L_num右移一位
		L_num = L_num >> 1;

		//如果R_num最高位为1
		if (R_num & 0x80)
		{
			//左移一位相当于乘二
			R_num = R_num << 1;		//虽然这里会丢失最高位，但是无需关注

			R_num ^= 0x1B;	//计算伽罗瓦域内除法Num_R = Num_R / (x^8(刚好丢失最高位) + x^4 + x^3 + x^1 + 1)
		}
		else
		{
			//左移一位相当于乘二
			R_num = R_num << 1;
		}
	}

	return result;

}


//列混淆函数
void mix_col(unsigned char(*plain_array)[4])
{
	unsigned char temp_array[4][4];

	//对temp数组初始化
	memcpy(temp_array, plain_array, 16);


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//这里是对我们要加密的矩阵以及列混淆矩阵进行矩阵的乘法运算
			//但是这里用到的乘法是伽罗瓦域内的乘法运算，因此我们自定义一个函数来实现它
			//这里的加法相当于异或操作
			plain_array[i][j] =
				Gal_Mul(mix_array[i][0], temp_array[0][j]) ^
				Gal_Mul(mix_array[i][1], temp_array[1][j]) ^
				Gal_Mul(mix_array[i][2], temp_array[2][j]) ^
				Gal_Mul(mix_array[i][3], temp_array[3][j]);
		}
	}

}



//该函数将得到的密文矩阵转化为密文形式输出

void array_to_output(unsigned char(*array)[4], unsigned char *cipher_text)
{
	for (int i = 0; i < 16; i++)
	{
		cipher_text[i] = array[i & 0x03][i >> 2];
	}
}




//加密函数
void encrypt(const unsigned char *plain_text, const unsigned char *key,unsigned char *cipher_text)
{
	unsigned char plain_array[4][4];		//明文的4*4矩阵128Bit
	unsigned char key_array[4][4];			//密钥扩展前的4*4矩阵128Bit
	unsigned char extend_key_array[4][44];	//存储密钥扩展后的矩阵4*44

	//变量初始化
	memset(plain_array, 0, 16);
	memset(key_array, 0, 16);
	memset(extend_key_array, 0, 176);

	//将输入的明文与密钥转化为矩阵形式
	//明文转化为矩阵形式
	input_to_array(plain_text, plain_array);
	//密钥转化为矩阵形式
	input_to_array(key, key_array);

	//得到每一轮的子密钥，第一个参数为初始密钥，第二个参数为扩展后的各个子密钥
	get_keys(key_array, extend_key_array);

	//开始加密过程
	//1、轮密钥加
	round_key_add(plain_array, extend_key_array, 0);//第一次进行轮密钥加，因此最小行号设为0


	//2、进入前9轮(共10轮)循环
	for (int i = 1; i < 10; i++)
	{
		//每轮循环均有4步：
		//A、字节代换层
		plain_S_Sub((unsigned char *)plain_array);

		//B、行位移层
		left_shift_row((unsigned int *)plain_array);

		//C、列混淆层
		mix_col(plain_array);

		//D、密钥相加层
		round_key_add(plain_array, extend_key_array, 4 * i);//每一轮使用的子密钥均不同，即4*i

	}

	//3、最后一轮，不需要列混淆

	plain_S_Sub((unsigned char *)plain_array);

	left_shift_row((unsigned int *)plain_array);

	round_key_add(plain_array, extend_key_array, 4 * 10);


	//将得到的密文由矩阵转化为需要的形式

	array_to_output(plain_array, cipher_text);


}


//以下为解密函数及其所需的函数


//AES的解密过程需要进行右移而不是左移
//行右位移函数
void right_shift_row(unsigned int *cipher_array)
{
	//对于明文对应的矩阵，第一行不需要移位

	//第二行左移位8bit
	cipher_array[1] = (cipher_array[1] << 8) | (cipher_array[1] >> 24);//这里我们将第二行右移8bit，同时将第二行左移24bit，再进行按位或，得到所需效果

	//第三行左移位16bit
	cipher_array[2] = (cipher_array[2] << 16) | (cipher_array[2] >> 16);

	//第四行左移位24bit
	cipher_array[3] = (cipher_array[3] << 24) | (cipher_array[3] >> 8);

}



//密文矩阵需要逆S盒进行逆字节代换
void cipher_S_Sub(unsigned char *cipher_array)
{
	for (int i = 0; i < 16; i++)
	{
		//plain_array[i]中的高4位用来表示逆S盒中的行号，低4位用来表示逆S盒中的列号
		//这里注意要用逆S盒，即S_1
		cipher_array[i] = S_1[cipher_array[i] >> 4][cipher_array[i] & 0x0F];
	}
}


//列混淆也需要逆向
void re_mix_col(unsigned char(*cipher_array)[4])
{
	unsigned char temp_array[4][4];

	//对temp数组初始化
	memcpy(temp_array, cipher_array, 16);


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//这里是对我们要加密的矩阵以及列混淆矩阵进行矩阵的乘法运算
			//但是这里用到的乘法是伽罗瓦域内的乘法运算，因此我们自定义一个函数来实现它
			//这里的加法相当于异或操作
			//注意这里需要使用逆向列混淆矩阵，即mix_array_1
			cipher_array[i][j] =
				Gal_Mul(mix_array_1[i][0], temp_array[0][j]) ^
				Gal_Mul(mix_array_1[i][1], temp_array[1][j]) ^
				Gal_Mul(mix_array_1[i][2], temp_array[2][j]) ^
				Gal_Mul(mix_array_1[i][3], temp_array[3][j]);
		}
	}

}



//解密函数
void decrypt(const unsigned char *cipher_text, const unsigned char *key, unsigned char *plain_text)
{
	unsigned char cipher_array[4][4];		//密文的4*4矩阵128Bit
	unsigned char key_array[4][4];			//密钥扩展前的4*4矩阵128Bit
	unsigned char extend_key_array[4][44];	//存储密钥扩展后的矩阵4*44

	//变量初始化
	memset(cipher_array, 0, 16);
	memset(key_array, 0, 16);
	memset(extend_key_array, 0, 176);

	//将输入的密文与密钥转化为矩阵形式
	//密文转化为矩阵形式
	input_to_array(cipher_text, cipher_array);
	//密钥转化为矩阵形式
	input_to_array(key, key_array);

	//得到每一轮的子密钥，第一个参数为初始密钥，第二个参数为扩展后的各个子密钥
	get_keys(key_array, extend_key_array);



	//开始解密过程
	//1、解密先进行加密的最后一轮，不需要列混淆

	round_key_add(cipher_array, extend_key_array, 4 * 10);

	right_shift_row((unsigned int *)cipher_array);

	cipher_S_Sub((unsigned char *)cipher_array);


	//2、进入前9轮(共10轮)循环
	for (int i = 9; i > 0; i--)
	{
		//每轮循环均有4步：
		//D、密钥相加层
		round_key_add(cipher_array, extend_key_array, 4 * i);//每一轮使用的子密钥均不同，即4*i

		//C、逆向列混淆层
		re_mix_col(cipher_array);

		//B、逆向行位移层
		right_shift_row((unsigned int *)cipher_array);

		//A、字节代换层
		cipher_S_Sub((unsigned char *)cipher_array);
		
	}

	//3、轮密钥加
	round_key_add(cipher_array, extend_key_array, 0);//第一次进行轮密钥加，因此最小行号设为0


	//将得到的明文由矩阵转化为需要的形式

	array_to_output(cipher_array, plain_text);
}






int main()
{
	//unsigned int plain[] = { 0x00, 0x01, 0x00, 0x01, 0x01, 0xa1, 0x98, 0xaf, 0xda, 0x78, 0x17, 0x34, 0x86, 0x15, 0x35, 0x66 };
	//unsigned int my_key[] = { 0x00, 0x01, 0x20, 0x01, 0x71, 0x01, 0x98, 0xae, 0xda, 0x79, 0x17, 0x14, 0x60, 0x15, 0x35, 0x94 };


	unsigned char plain_text[] = { 0x00, 0x01, 0x00, 0x01, 0x01, 0xa1, 0x98, 0xaf, 0xda, 0x78, 0x17, 0x34, 0x86, 0x15, 0x35, 0x66 };
	unsigned char key[] = { 0x00, 0x01, 0x20, 0x01, 0x71, 0x01, 0x98, 0xae, 0xda, 0x79, 0x17, 0x14, 0x60, 0x15, 0x35, 0x94 };
	unsigned char cipher_text[16];
	unsigned char de_cipher_text[17];


	//AES加密
	encrypt(plain_text, key, cipher_text);

	//密文输出
	cout << "密文1为：" << endl;
	print_text(cipher_text, 16);

	//AES解密
	decrypt(cipher_text, key, de_cipher_text);

	//明文输出
	cout << "解密后的明文1为：" << endl;
	print_text(de_cipher_text, 16);


	//第二个测试样例
	unsigned char plain_text_2[] = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30,0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	unsigned char key_2[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	unsigned char cipher_text_2[17];
	unsigned char de_cipher_text_2[17];

	//加密函数
	encrypt(plain_text_2, key_2, cipher_text_2);

	//密文输出
	cout << "密文2为：" << endl;
	print_text(cipher_text_2, 16);

	//AES解密
	decrypt(cipher_text_2, key, de_cipher_text_2);

	//明文输出
	cout << "解密后的明文2为：" << endl;
	print_text(plain_text_2, 16);

	return 0;
}




// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
