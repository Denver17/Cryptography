
#include "pch.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include "big_num.h"

//静态常量赋值
const big_num big_num::ZERO = big_num(0);
const big_num big_num::ONE = big_num(1);
const big_num big_num::TWO = big_num(2);
const big_num big_num::TEN = big_num(10);


big_num::~big_num()
{

}



//根据给定的大整数构造一个新的大整数,val为给定的大整数
big_num::big_num(const big_num & val)
{
	*this = val;
}


/**
 * 函数功能:根据给定的十六进制字符串数据构造一个大整数
 * 参数含义:str代表给定的数据
 */
big_num::big_num(const std::string & str) : is_negative(false) {
	std::string t(str);
	if (t.size() && t.at(0) == '-') {
		if (t.size() > 1)
			is_negative = true;
		t = t.substr(1);

	}
	int cnt = (8 - (t.size() % 8)) % 8;// 数的长度不是8的倍数,补足0
	std::string temp;

	for (int i = 0; i < cnt; ++i)
		temp.push_back('0');

	t = temp + t;

	for (size_t i = 0; i < t.size(); i += base_char) {
		base_t sum = 0;
		for (int j = 0; j < base_char; ++j) {    // 8位十六进制组成大整数的一位
			char ch = t[i + j];
			int num = hex_to_dec(ch);
			sum = ((sum << 4) | (num));

		}
		data.push_back(sum);

	}
	reverse(data.begin(), data.end());// 高位在后
	*this = trim();// 去除高位的0
}

/**
 * 函数功能:根据给定的long_t类型数据构造一个大整数
 * 参数含义:num代表给定的数据
 */
big_num::big_num(const long_t & num) : is_negative(false) {
	long_t t = num;
	if (t < 0) {
		is_negative = true;
		t = -t;

	}
	do {
		base_t temp = (t&base_num);    // 每次截取低32位
		data.push_back(temp);
		t >>= base_int;

	} while (t);

}








//大整数加法运算
big_num big_num::add(const big_num & val)
{
	big_num num(*this);
	if (num.is_negative == val.is_negative)	//若二者同号
	{
		int len = val.data.size() - num.data.size();

		while ((len--) > 0)	//被加数位数少，在其高位补0
		{
			num.data.push_back(0);
		}
		int carry = 0;	//进位
		for (size_t i = 0; i < val.data.size(); i++)
		{
			base_t temp = num.data[i];
			num.data[i] += val.data[i] + carry;
			// 无符号数相加,超出取其余数
			// 进位:一种是有无进位都超出,一种是有进位才超出(比如十进制相加,9+9+1,得9,9+0+0,得9)
			carry = (temp > num.data[i] ? 1 : (temp > (temp + val.data[i]) ? 1 : 0));
		}

		for (size_t i = val.data.size(); i < num.data.size() && carry != 0; ++i) {// 还有进位
			            base_t temp = num.data[i];
			            num.data[i] += carry;
			             carry = temp > num.data[i];
		}
		if (carry)	// 还有进位
			num.data.push_back(carry);
	}
	else	//若二者异号
	{
		big_num a = my_abs();
		big_num b = val.my_abs();
		int flag = a.my_compare(b);	//二者绝对值比较大小
		if (flag == -1)	//val的绝对值更大，符号置为val的
		{
			num = b.sub(a);
			num.is_negative = val.is_negative;
		}
		else if (flag == 0)	//二者绝对值相等
		{
			num = ZERO;
		}
		else //自身的绝对值更大，符号使用本身的
		{
			num = a.sub(b);
			num.is_negative = is_negative;
		}
	}

	return num;
}



//大整数减法运算
big_num big_num::sub(const big_num & val)
{
	big_num num(*this);
	big_num a = my_abs();
	big_num b = val.my_abs();
	if (num.is_negative == val.is_negative)	//二者同号
	{
		int flag = a.my_compare(b);
		if (flag == 1)
		{
			int borrow = 0;    // 借位
			// 大数减小数
			for (size_t i = 0; i < val.data.size(); ++i) {
				base_t temp = num.data[i];
				num.data[i] -= val.data[i] + borrow;
				// 借位:一种是有无借位都超出,另一种是有借位才超出(比如十进制相减,9-0-0,得9,9-9-1,得9)
				borrow = temp < num.data[i] ? 1 : (temp - borrow < val.data[i] ? 1 : 0);

			}
			for (size_t i = val.data.size(); i < num.data.size() && borrow != 0; ++i) {// 还有借位
				base_t temp = num.data[i];
				num.data[i] -= borrow;
				borrow = temp < (base_t)borrow;

			}
			num = num.trim();// 去掉高位多余的0
		}
		else if (flag == 0)
			num = ZERO;
		else {	//a的绝对值小于b的绝对值
			num = b.sub(a);
			num.is_negative = !is_negative;
		}
	}
	else {					//二者异号
		num = a.add(b);    //转换为加法
		num.is_negative = is_negative;
	}
	return num;
}



//大整数乘法运算
big_num big_num::mul(const big_num & val) const
{
	if (my_equal(ZERO) || val.my_equal(ZERO))
		return ZERO;
	//位数少的作为乘数
	const big_num & big = data.size() > val.data.size() ? (*this) : val;
	const big_num & small = (&big) == (this) ? val : (*this);

	big_num num;
	bit t(small);	//转化为二进制进行运算

	for (int i = t.size() - 1; i >= 0; i--)
	{
		if (t.at(i))	//判断第i为是否为1
		{
			big_num temp(big);
			temp.is_negative = false;	//不为负数
			temp = temp.left_shift(i);	//移位对齐
			num = num.add(temp);		//加数该位为1，将此次运算结果加入最终结果
		}
	}
	num.is_negative = !(is_negative == val.is_negative);	//同号得正，异号得负
	return num;
}



//大整数除法运算
big_num big_num::div(const big_num & val)
{
	big_num temp;
	big_num num = div_rem(val, temp);
	return num;
}


//大整数取余运算
big_num big_num::remain(const big_num & val)
{
	big_num num;
	div_rem(val, num);
	return num;
}


//大整数取模运算（与取余的不同是该函数总是返回正余数）
big_num big_num::mod(const big_num & m)
{
	big_num num = remain(m);
	if (num.is_negative)	//如果是负数，就在余数的基础上加一个模数，使得返回值总为正数
		num = num.add(m);
	return num;
}


//大整数整除与取余运算，返回值是整除结果，取余结果由m返回
//val是除数
big_num big_num::div_rem(const big_num & val, big_num & m)
{
	assert(!val.my_equal(ZERO));	//若除数为0，则报错
	big_num a = my_abs();
	big_num b = val.my_abs();
	int flag = a.my_compare(b);
	if (flag == 0)	//二者绝对值相等
	{
		return (is_negative == val.is_negative) ? big_num(1) : big_num(-1);	//同号返回1，异号返回-1
	}
	if (flag == -1)	//除数更大，直接返回本身，整除结果为0
	{
		m = *this;
		return ZERO;
	}

	big_num num;

	bit bit_b(b);	//二进制处理
	while (true)	//常规情况，a的绝对值大于b的绝对值，即被除数更大
	{
		bit bit_a(a);
		int len = bit_a.size() - bit_b.size();

		big_num temp;
		while (len >= 0)	
		{
			temp = b.left_shift(len);		//进行左移
			if (temp.my_compare(a) != 1)	//我们要找到能使a不小于b的最大左移位数
				break;
			len--;
		}
		if (len < 0)	//此时a小于b，运算结束
			break;

		base_t temp_num = 0;
		while (temp.my_compare(a) != 1)	
		{
			a = a.sub(temp);
			temp_num++;	//统计当前的a最多不小于移位几位后的b
		}
		temp = big_num(temp_num);
		if (len)
			temp = temp.left_shift(len);	//移位后表明当前的a是b的几倍
		num = num.add(temp);
	}
	num.is_negative = !(is_negative == val.is_negative);	//同号为正，异号为负
	m.data = a.data;
	m.is_negative = is_negative;
	return num;
}




//大整数幂乘运算
big_num big_num::pow(const big_num & exponent)
{
	big_num num(1);
	bit t(exponent);	//转化为二进制进行处理
	for (int i = t.size() - 1; i >= 0; i--)
	{
		num = num.mul(num);
		if (t.at(i))
			num = mul(num);
	}
	return num;
}



//大整数模幂运算
big_num big_num::mod_pow(const big_num & exponent, const big_num & m) const
{
	assert(!m.my_equal(ZERO));	//模数为0，报错
	big_num num(1);
	bit t(exponent);
	for (int i = t.size() - 1; i >= 0; i--)
	{
		//cout << "test" << i << endl;
		num = num.mul(num).mod(m);
		if (t.at(i))
			num = mul(num).mod(m);
		//cout << "i=" << i << endl;
		//cout << num << endl;
	}
	return num;
}



//扩展欧几里得算法求乘法逆元
//m代表求逆元时的模数
big_num big_num::mod_inverse(const big_num & m)
{
	assert(!is_negative);	//当前大整数为负数时就报错
	assert(!m.is_negative);	//m也要为正数，否则报错
	if (my_equal(ZERO) || m.my_equal(ZERO))
		return ZERO;	//二者之中有一个为0就不存在乘法逆元
	big_num a[3], b[3], t[3];

	// 以下进行初等变换
	a[0] = 0; a[1] = 1; a[2] = *this;
	b[0] = 1; b[1] = 0; b[2] = m;

	for (t[2] = a[2].mod(b[2]); !t[2].my_equal(ZERO); t[2] = a[2].mod(b[2])) {
		big_num temp = a[2].div(b[2]);
		for (int i = 0; i < 3; ++i) {
			t[i] = a[i].sub(temp.mul(b[i]));// 不超过一次a[2]-temp*b[2]就变为大数减小数
			a[i] = b[i];
			b[i] = t[i];
		}

	}
	if (b[2].my_equal(ONE)) {// 最大公约数为1,存在乘法逆元
		if (b[1].is_negative)// 逆元为负数
			b[1] = b[1].add(m);// 变为正数,使其在m的剩余集中
		return b[1];

	}
	return ZERO;// 最大公约数不为1,无乘法逆元
}



/**
 * 函数功能:移位运算,左移
 * 参数含义:len代表移位的位数
 */
big_num big_num::left_shift(const unsigned len) {
	int index = len >> base_bit;    // 大整数每一位需要移动多少位
	int shift = len & base_temp;    // 还剩下多少位
	big_num ans(*this);

	int inc = (shift == 0) ? index : index + 1;// 有多余的位要多开大整数的一位
	for (int i = 0; i < inc; ++i)
		ans.data.push_back(0);    // 高位补0

	if (index) {
		inc = (shift == 0) ? 1 : 2;// 有多余的位要预留一位
		for (int i = ans.data.size() - inc; i >= index; --i)
			ans.data[i] = ans.data[i - index];
		for (int i = 0; i < index; ++i)
			ans.data[i] = 0;

	}
	if (shift) {
		base_t t = base_num;
		t <<= base_int - shift;    // 用于截取高位
		// 左移
		base_t temp = 0;
		for (size_t i = 0; i < ans.data.size(); ++i) {
			base_t tmp = ans.data[i];
			ans.data[i] = (tmp << shift) | temp;// 左移后加上大整数低位的高位
			temp = (tmp&t) >> (base_int - shift);// 获取该大整数位的高位

		}

	}
	ans = ans.trim();
	return ans;

}

/**
 * 函数功能:移位运算,右移
 * 参数含义:len代表移位的位数
 */
big_num big_num::right_shift(const unsigned len) {
	bit val(*this);
	if (len >= val.size())// 当前大整数位数小于等于移位位数,返回0
		return ZERO;
	int index = len >> base_bit;// 大整数每一位需要移动多少位
	int shift = len & base_temp;// 还剩下多少位
	big_num ans(*this);

	if (index) {
		for (int i = 0; i < index; ++i)
			ans.data[i] = ans.data[i + index];
		for (int i = 0; i < index; ++i)
			ans.data.pop_back();    // 高位删除

	}
	if (shift) {
		base_t t = base_num;
		t >>= base_int - shift;    // 用于截取低位
		// 右移
		base_t temp = 0;
		for (int i = ans.data.size() - 1; i >= 0; --i) {
			base_t tmp = ans.data[i];
			ans.data[i] = (tmp >> shift) | temp;// 右移后加上大整数高位的低位
			temp = (tmp&t) << (base_int - shift);// 获取该大整数位的低位

		}

	}
	ans = ans.trim();
	return ans;

}



/**
 * 函数功能:大整数比较函数,-1表示本大整数要小,0表示相等,1表示本大整数要大
 * 参数含义:val代表要与之比较的大整数
 */
int big_num::my_compare(const big_num & val) const {
	if (is_negative != val.is_negative) {// 符号不同,负数必小
		if (is_negative == true)
			return -1;
		return 1;

	}
	int flag = 0;
	if (data.size() < val.data.size())// 位数较小
		flag = -1;
	else if (data.size() > val.data.size())// 位数较大
		flag = 1;
	else {    // 位数相等,从高位开始一一比较
		for (std::vector<base_t>::const_reverse_iterator it = data.rbegin(), ite = val.data.rbegin(); it != data.rend(); ++it, ++ite)
			if ((*it) != (*ite)) {
				flag = (*it) < (*ite) ? -1 : 1;    // 高位小,则小
				break;

			}

	}
	if (is_negative)    // 如为负数,小的反而大
		flag = -flag;
	return flag;

}

/**
 * 函数功能:大整数是否相等函数
 * 参数含义:val表示要与之比较的大整数
 */
bool big_num::my_equal(const big_num & val) const {
	return (is_negative == val.is_negative) && (data == val.data);// 符号和数据都要相等

}

/**
 * 函数功能:将一个long_t类型的数据转换为大整数并返回
 * 参数含义:num表示给定的数
 */
big_num big_num::to_bignum(const long_t & num) {
	return big_num(num);

}

/**
 * 函数功能:将大整数转换为十六进制字符串并返回
 */
std::string big_num::to_string() const {
	std::string ans;
	base_t t = base_num;
	t <<= base_int - 4;    // 用于截取高4位
	for (int i = data.size() - 1; i >= 0; --i) {
		base_t temp = data[i];
		for (int j = 0; j < base_char; ++j) {
			base_t num = t & temp;// 每次截取高4位
			num >>= base_int - 4;    // 将高4位移到低4位
			temp <<= 4;
			if (num < 10)
				ans.push_back((char)('0' + num));
			else
				ans.push_back((char)('A' + num - 10));

		}

	}
	while (ans.size() > 0 && ans.at(0) == '0')// 去掉高位无用的0
		ans = ans.substr(1);
	if (ans.empty())    // 空串说明为0
		ans.push_back('0');
	if (is_negative)    // 为负数加上负号
		ans = "-" + ans;
	return ans;

}



/**
 * 函数功能:返回大整数的绝对值
 */
big_num big_num::my_abs() const {
	big_num ans;
	ans.data = data;// 只复制数据,符号默认为正
	return ans;

}






big_num operator + (const big_num & a, const big_num & b)
{
	big_num t(a);
	return t.add(b);
}

big_num operator - (const big_num & a, const big_num & b)
{
	big_num t(a);
	return t.sub(b);
}

big_num operator * (const big_num & a, const big_num & b)
{
	big_num t(a);
	return t.mul(b);
}

big_num operator / (const big_num & a, const big_num & b)
{
	big_num t(a);
	return t.div(b);
}

big_num operator % (const big_num & a, const big_num & b)
{
	big_num t(a);
	return t.remain(b);
}

bool operator < (const big_num &a, const big_num &b)
{
	return a.my_compare(b) == -1;
}

bool operator > (const big_num &a, const big_num &b)
{
	return b < a;
}

bool operator == (const big_num &a, const big_num &b)
{
	return a.my_equal(b);
}

bool operator <= (const big_num &a, const big_num &b)
{
	return !(a > b);
}

bool operator >= (const big_num &a, const big_num &b)
{
	return !(a < b);
}

bool operator != (const big_num &a, const big_num &b)
{
	return !(a == b);
}

big_num operator + (const big_num & a, const big_num::long_t & b) {
	return a + big_num(b);

}

big_num operator - (const big_num & a, const big_num::long_t & b) {
	return a - big_num(b);

}

big_num operator * (const big_num & a, const big_num::long_t & b) {
	return a * big_num(b);

}

big_num operator / (const big_num & a, const big_num::long_t & b) {
	return a / big_num(b);

}

big_num operator % (const big_num & a, const big_num::long_t & b) {
	return a % big_num(b);

}

bool operator < (const big_num & a, const big_num::long_t & b) {
	return a < big_num(b);

}

bool operator > (const big_num & a, const big_num::long_t & b) {
	return a > big_num(b);

}

bool operator == (const big_num & a, const big_num::long_t & b) {
	return a == big_num(b);

}

bool operator <= (const big_num & a, const big_num::long_t & b) {
	return a <= big_num(b);

}

bool operator >= (const big_num & a, const big_num::long_t & b) {
	return a >= big_num(b);

}

bool operator != (const big_num & a, const big_num::long_t & b) {
	return a != big_num(b);

}

std::ostream & operator << (std::ostream & out, const big_num & val) {
	out << val.to_string();
	return out;
}




/**
 * 函数功能:创建该大整数的一个副本,去除掉高位无用的0后并返回
 */
big_num big_num::trim() {
	size_t cnt = 0;
	// 检查高位为0的元素的数量
	for (std::vector<base_t>::const_reverse_iterator it = data.rbegin(); it != data.rend(); ++it) {
		if ((*it) == 0)
			++cnt;
		else
			break;

	}
	if (cnt == data.size())    // 只有零的情况保留
		--cnt;
	big_num ans(*this);
	for (size_t i = 0; i < cnt; ++i)
		ans.data.pop_back();
	return ans;

}

/**
 * 函数功能:根据给定的字符确定它所对应的十进制数
 * 参数含义:ch代表给定的字符
 */
int big_num::hex_to_dec(char ch) {
	int ans = 0;
	if (isdigit(ch))
		ans = ch - '0';
	else if (islower(ch))
		ans = ch - 'a' + 10;
	else
		ans = ch - 'A' + 10;
	return ans;

}

/**
 * 函数功能:根据给定的大整数初始化
 * 参数含义:val代表给定的大整数
 */
big_num::bit::bit(const big_num & val) {
	bit_vector = val.data;
	base_t temp = bit_vector[bit_vector.size() - 1];// 大整数最高位
	length = bit_vector.size() << base_bit;    // 大整数一位占二进制32位
	base_t t = 1 << (base_int - 1);    // 用于截取一个数的二进制最高位

	if (temp == 0)    // 大整数最高位为0,减去32
		length -= base_int;
	else {
		while (!(temp & t)) {// 从高位开始检测大整数的二进制位,为0长度减一
			--length;
			t >>= 1;    // 右移一位表示检测下一位

		}

	}

}

/**
 * 函数功能:检测大整数的第id位二进制位是否为1
 * 参数含义:id代表第id位
 */
bool big_num::bit::at(size_t id) {
	size_t index = id >> base_bit;// 确定其在大整数第几位
	size_t shift = id & base_temp;// 确定其在大整数那一位的二进制第几位
	base_t t = bit_vector[index];
	return (t & (1 << shift));

}
