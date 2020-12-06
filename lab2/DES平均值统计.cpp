#include <iomanip>
#include <iostream>
#include <bitset>
using namespace std;

bitset<64> key;                // 64位密钥
bitset<48> all_keys[16];         // 存放16轮子密钥


bitset<64> data_change_1[16];	//存放原明文16轮加密的每一轮得到的结果
bitset<64> data_change_2[16];	//存放修改后的明文16轮加密的每一轮得到的结果
bitset<64> data_change_3[16];	//存放原密文16轮解密的每一轮得到的结果
bitset<64> data_change_4[16];	//存放修改后的密文16轮解密的每一轮得到的结果
int tag=0;						//判断数据给1、2或3、4
int avg_tag = 0;
int all_sum_1 = 0;				//统计总数
int all_sum_2 = 0;				//统计总数
double average_1 = 0;			//统计明文平均值
double average_2 = 0;			//统计密文平均值




// 初始置换表
int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2,
			60, 52, 44, 36, 28, 20, 12, 4,
			62, 54, 46, 38, 30, 22, 14, 6,
			64, 56, 48, 40, 32, 24, 16, 8,
			57, 49, 41, 33, 25, 17, 9,  1,
			59, 51, 43, 35, 27, 19, 11, 3,
			61, 53, 45, 37, 29, 21, 13, 5,
			63, 55, 47, 39, 31, 23, 15, 7 };

// 结尾置换表
int IP_1[] = { 40, 8, 48, 16, 56, 24, 64, 32,
			  39, 7, 47, 15, 55, 23, 63, 31,
			  38, 6, 46, 14, 54, 22, 62, 30,
			  37, 5, 45, 13, 53, 21, 61, 29,
			  36, 4, 44, 12, 52, 20, 60, 28,
			  35, 3, 43, 11, 51, 19, 59, 27,
			  34, 2, 42, 10, 50, 18, 58, 26,
			  33, 1, 41,  9, 49, 17, 57, 25 };

/*------------------下面是生成密钥所用表-----------------*/

// 密钥置换表，将64位密钥变成56位
int PC_1[] = { 57, 49, 41, 33, 25, 17, 9,
			   1, 58, 50, 42, 34, 26, 18,
			  10,  2, 59, 51, 43, 35, 27,
			  19, 11,  3, 60, 52, 44, 36,
			  63, 55, 47, 39, 31, 23, 15,
			   7, 62, 54, 46, 38, 30, 22,
			  14,  6, 61, 53, 45, 37, 29,
			  21, 13,  5, 28, 20, 12,  4 };

// 压缩置换，将56位密钥压缩成48位子密钥
int PC_2[] = { 14, 17, 11, 24,  1,  5,
			   3, 28, 15,  6, 21, 10,
			  23, 19, 12,  4, 26,  8,
			  16,  7, 27, 20, 13,  2,
			  41, 52, 31, 37, 47, 55,
			  30, 40, 51, 45, 33, 48,
			  44, 49, 39, 56, 34, 53,
			  46, 42, 50, 36, 29, 32 };

// 每轮左移的位数
int shiftBits[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

/*------------------下面是密码函数 f 所用表-----------------*/

// 扩展置换表，将 32位 扩展至 48位
int E[] = { 32,  1,  2,  3,  4,  5,
			4,  5,  6,  7,  8,  9,
			8,  9, 10, 11, 12, 13,
		   12, 13, 14, 15, 16, 17,
		   16, 17, 18, 19, 20, 21,
		   20, 21, 22, 23, 24, 25,
		   24, 25, 26, 27, 28, 29,
		   28, 29, 30, 31, 32,  1 };

// S盒，每个S盒是4x16的置换表，6位 -> 4位
int S_BOX[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};

// P置换，32位 -> 32位
int P[] = { 16,  7, 20, 21,
		   29, 12, 28, 17,
			1, 15, 23, 26,
			5, 18, 31, 10,
			2,  8, 24, 14,
		   32, 27,  3,  9,
		   19, 13, 30,  6,
		   22, 11,  4, 25 };

/**********************************************************************/
/*                                                                    */
/*                            下面是DES算法实现                       */
/*                                                                    */
/**********************************************************************/



//加密函数F(R,K)，将轮输入中的右半部分R(32位)与48位轮密钥K作为输入，输出32位数据
bitset<32> F(bitset<32> R, bitset<48> K)
{
	//1、首先对轮输入进行E表扩展/置换

	bitset<48> expand_E;

	for (int i = 0; i < 48; ++i)
		expand_E[i] = R[E[i] - 1];



	//2、密钥与扩展后的轮输入进行异或操作

	expand_E = expand_E ^ K;




	//3、S盒代换

	bitset<32> Sub_S;

	int S_Box_Row;

	int S_Box_Clo;

	for (int i = 0; i < 8; i++)
	{
		//得到某个S盒中对应的行号
		S_Box_Row = expand_E[i * 6 + 5] + expand_E[i * 6] * 2;
		//得到某个S盒中对应的列号
		S_Box_Clo = expand_E[i * 6 + 4] * 1 + expand_E[i * 6 + 3] * 2 + expand_E[i * 6 + 2] * 4 + expand_E[i * 6 + 1] * 8;

		//将从S盒中得到的数转化为bitset
		bitset<4> S_i(S_BOX[i][S_Box_Row][S_Box_Clo]);

		//将结果保存
		Sub_S[i * 4] = S_i[3];
		Sub_S[i * 4 + 1] = S_i[2];
		Sub_S[i * 4 + 2] = S_i[1];
		Sub_S[i * 4 + 3] = S_i[0];
	}




	//4、P置换

	bitset<32> output_P;

	output_P = Sub_S;

	for (int i = 0; i < 32; i++)
		Sub_S[i] = output_P[P[i] - 1];



	return Sub_S;
}


//左循环移位函数
bitset<28> left_shift(bitset<28>left_key, int shift_round)
{
	bitset<28> tmp_key;
	for (int i = 0; i < 28; i++)
	{
		tmp_key[i] = left_key[(i + shift_round) % 28];
	}
	return tmp_key;
}




//生成密钥函数(共有十六个密钥)
void get_keys()
{
	//除去校验位后得到的密钥
	bitset<56> final_key;

	//进行置换选择后的密钥
	bitset<56> start_key;

	//左28位密钥与右28位密钥
	bitset<28> left_key;
	bitset<28> right_key;

	//进行置换选择2后的密钥(变为了48位)
	bitset<48> output_key;

	bitset<56> temp_key;


	//1、64位变为56位并进行置换选择1
	for (int i = 0; i < 56; ++i)
		start_key[i] = key[PC_1[i] - 1];



	//round为轮数
	for (int round = 0; round < 16; round++)
	{
		//将密钥拆分为两部分
		for (int i = 0; i < 28; i++)
		{
			left_key[i] = start_key[i];
		}
		for (int i = 0; i < 28; i++)
		{
			right_key[i] = start_key[i + 28];
		}

		//2、左循环移位

		//左28位左循环移位
		left_key = left_shift(left_key, shiftBits[round]);
		//右28位左循环移位
		right_key = left_shift(right_key, shiftBits[round]);


		//3、置换选择2


		for (int i = 0; i < 28; i++)
		{
			start_key[i] = left_key[i];
		}
		for (int i = 28; i < 56; i++)
		{
			start_key[i] = right_key[i - 28];
		}


		for (int i = 0; i < 48; i++)
		{
			output_key[i] = start_key[PC_2[i] - 1];
		}

		all_keys[round] = output_key;

	}

}








//加密函数
bitset<64> encrypt(bitset<64>& plain_text)
{
	//初始置换得到的明文
	bitset<64> output_IP;

	//明文左32比特与右32比特
	bitset<32> plain_left;
	bitset<32> plain_right;

	//下一轮的明文的左32比特与右32比特
	bitset<32> plain_left_next;
	bitset<32> plain_right_next;

	//左右交换并合并后得到的数据
	bitset<64> swap_lr;

	//逆初始置换后得到的密文
	bitset<64> cipher_text;

	//1、初始置换

	for (int i = 0; i < 64; i++)
	{
		output_IP[i] = plain_text[IP[i] - 1];
	}

	//将置换后的明文分为左右两部分
	//注意左右对换
	for (int i = 0; i < 32; i++)
	{
		plain_left[i] = output_IP[i];
	}
	for (int i = 0; i < 32; i++)
	{
		plain_right[i] = output_IP[i + 32];
	}


	//2、轮加密

	for (int round = 0; round < 16; round++)
	{

		//这一轮的明文的右32比特作为下一轮的明文左32比特
		plain_left_next = plain_right;


		//这一轮的明文的右32比特经过加密函数F加密后与这一
		//轮的明文的左32比特进行异或，得到下一轮的明文的
		//右32比特

		plain_right_next = plain_left ^ F(plain_right, all_keys[round]);



		//准备下一轮循环
		plain_left = plain_left_next;
		plain_right = plain_right_next;



		//存放原明文结果
		if (tag == 1)
		{
			for (int j = 0; j < 32; j++)
			{
				data_change_1[round][j] = plain_left[j];
			}
			for (int j = 0; j < 32; j++)
			{
				data_change_1[round][j + 32] = plain_right[j];
			}
		}

		//存放修改后的明文的结果
		if (tag == 2)
		{
			for (int j = 0; j < 32; j++)
			{
				data_change_2[round][j] = plain_left[j];
			}
			for (int j = 0; j < 32; j++)
			{
				data_change_2[round][j + 32] = plain_right[j];
			}
		}
		
	}


	//3、左右交换并合并


	for (int i = 0; i < 32; i++)
	{
		//右侧数据放到左侧
		swap_lr[i] = plain_right[i];
	}

	//cout << " ";

	for (int i = 32; i < 64; i++)
	{
		//左侧数据放到右侧
		swap_lr[i] = plain_left[i - 32];
	}



	//4、逆初始置换

	for (int i = 0; i < 64; i++)
	{
		cipher_text[i] = swap_lr[IP_1[i] - 1];
	}



	return cipher_text;

}




//解密函数
//和加密函数大体类似，值得注意的是在F函数等地方需要修改
bitset<64> decrypt(bitset<64>& cipher_text)
{
	//初始置换得到的明文
	bitset<64> output_IP;

	//明文左32比特与右32比特
	bitset<32> cipher_left;
	bitset<32> cipher_right;

	//下一轮的明文的左32比特与右32比特
	bitset<32> cipher_left_next;
	bitset<32> cipher_right_next;

	//左右交换并合并后得到的数据
	bitset<64> swap_lr;

	//逆初始置换后得到的密文
	bitset<64> plain_text;

	//1、初始置换

	for (int i = 0; i < 64; i++)
	{
		output_IP[i] = cipher_text[IP[i] - 1];
	}

	//将置换后的明文分为左右两部分
	for (int i = 0; i < 32; i++)
	{
		cipher_left[i] = output_IP[i];
	}
	for (int i = 0; i < 32; i++)
	{
		cipher_right[i] = output_IP[i + 32];
	}


	//2、轮加密

	for (int round = 0; round < 16; round++)
	{

		//这一轮的明文的右32比特作为下一轮的明文左32比特
		cipher_left_next = cipher_right;


		//这一轮的明文的右32比特经过加密函数F加密后与这一
		//轮的明文的左32比特进行异或，得到下一轮的明文的
		//右32比特

		cipher_right_next = cipher_left ^ F(cipher_right, all_keys[15 - round]);


		//准备下一轮循环
		cipher_left = cipher_left_next;
		cipher_right = cipher_right_next;




		//存放原密文结果
		if (tag == 3)
		{
			for (int j = 0; j < 32; j++)
			{
				data_change_3[round][j] = cipher_left[j];
			}
			for (int j = 0; j < 32; j++)
			{
				data_change_3[round][j + 32] = cipher_right[j];
			}
		}

		//存放修改后的密文的结果
		if (tag == 4)
		{
			for (int j = 0; j < 32; j++)
			{
				data_change_4[round][j] = cipher_left[j];
			}
			for (int j = 0; j < 32; j++)
			{
				data_change_4[round][j + 32] = cipher_right[j];
			}
		}





	}


	//3、左右交换并合并

	for (int i = 0; i < 32; i++)
	{
		//右侧数据放到左侧
		swap_lr[i] = cipher_right[i];
	}

	for (int i = 32; i < 64; i++)
	{
		//左侧数据放到右侧
		swap_lr[i] = cipher_left[i - 32];
	}


	//4、逆初始置换

	for (int i = 0; i < 64; i++)
	{
		plain_text[i] = swap_lr[IP_1[i] - 1];
	}

	return plain_text;

}



bitset<64> charToBitset(const char s[8])
{
	bitset<64> bits;
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			bits[i * 8 + j] = ((s[i] >> j) & 1);
	return bits;
}


//将数组中的值转为bitset
bitset<64> in_to_bit(int* my_int)
{
	bitset<64> result_key;
	for (int i = 0; i < 8; i++)
	{
		int temp = my_int[i];
		bitset<8> bitHex(temp);
		for (int j = 0; j < 8; j++)
		{
			result_key[i * 8 + j] = bitHex[7 - j];
		}
	}
	return result_key;
}


//将bitset的值转为int
int bit_to_int(bitset<64> my_bitset)
{


	bitset<64> my_turn;
	for (int i = 0; i < 64; i++)
	{
		my_turn[i] = my_bitset[63 - i];
	}



	for (int i = 0; i < 8; i++)
	{
		bitset<8> my_bit_tmp;
		//bitset<8> my_bit_turn;
		int temp = 0;
		//int turn = 0;
		for (int j = 0; j < 8; j++)
		{
			my_bit_tmp[j] = my_bitset[i * 8 + j];
		}
		

		temp += my_bit_tmp[7] + my_bit_tmp[6] * 2 + my_bit_tmp[5] * 4 + my_bit_tmp[4] * 8 +
			my_bit_tmp[3] * 16 + my_bit_tmp[2] * 32 + my_bit_tmp[1] * 64 + my_bit_tmp[0] * 128;
		

		cout << setiosflags(ios::uppercase) << hex << temp << " ";
	}
	cout << endl;
	return 0;
}



void data_compare(int round,bitset<64> data_1,bitset<64> data_2)
{
	int sum = 0;
	for (int i = 0; i < 64; i++)
	{
		if (data_1[i] != data_2[i])
		{
			sum++;
		}
	}
	cout << "第" << dec << round + 1 << "轮相差比特为：" << sum << endl;

	if (avg_tag == 1)
	{
		all_sum_1 += sum;
	}
	if (avg_tag == 2)
	{
		all_sum_2 += sum;
	}
}





int main() {


	bitset<64> plain_text;
	bitset<64> cipher_text;
	bitset<64> plain_text_2;
	bitset<64> cipher_text_2;
	bitset<64> plain_text_3;
	bitset<64> cipher_text_3;


	int my_key[] = { 0x10, 0x31, 0x6E, 0x02, 0x8C, 0x8F, 0x3B, 0x4A };
	int my_txt[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	//将数组内容转为bitset格式
	key = in_to_bit(my_key);
	plain_text = in_to_bit(my_txt);


	get_keys();   // 生成16个子密钥



	//加密操作
	tag = 1;
	cipher_text = encrypt(plain_text);

	cout << "原明文加密结果：" << endl;
	bit_to_int(cipher_text);
	cout << endl;



	//解密操作
	tag = 3;
	plain_text = decrypt(cipher_text);

	cout << "原密文解密结果：" << endl;
	bit_to_int(plain_text);
	cout << endl;


	//改变后的明文
	int my_txt_2[] = { 0x00, 0x00, 0x00, 0x5A, 0x00, 0x00, 0x00, 0x00 };


	//对改变的明文加密
	plain_text_2 = in_to_bit(my_txt_2);

	tag = 2;

	cipher_text_2 = encrypt(plain_text_2);

	cout << "改变明文后的加密结果：" << endl;
	bit_to_int(cipher_text_2);
	cout << endl;
	

	avg_tag = 1;
	for (int i = 0; i < 16; i++)
	{
		data_compare(i,data_change_1[i], data_change_2[i]);
	}

	average_1 = all_sum_1 * 1.0 / 16;
	cout << "平均值为：" << average_1 << endl;



	//改变后的密文
	int my_txt_3[] = { 0x82, 0xDC, 0xBA, 0xFB, 0xDE, 0xAB, 0x3B, 0x02 };


	//对改变的密文解密
	cipher_text_3 = in_to_bit(my_txt_3);

	tag = 4;

	plain_text_3 = decrypt(cipher_text_3);

	cout << endl;

	cout << "改变密文后的解密结果：" << endl;
	bit_to_int(cipher_text_3);
	cout << endl;


	avg_tag = 2;
	for (int i = 0; i < 16; i++)
	{
		data_compare(i, data_change_1[i], data_change_2[i]);
	}
	
	average_2 = all_sum_2 * 1.0 / 16;
	cout << "平均值为：" << average_2 << endl;

	


	return 0;

}
