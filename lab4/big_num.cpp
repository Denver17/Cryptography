
#include "pch.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include "big_num.h"

//��̬������ֵ
const big_num big_num::ZERO = big_num(0);
const big_num big_num::ONE = big_num(1);
const big_num big_num::TWO = big_num(2);
const big_num big_num::TEN = big_num(10);


big_num::~big_num()
{

}



//���ݸ����Ĵ���������һ���µĴ�����,valΪ�����Ĵ�����
big_num::big_num(const big_num & val)
{
	*this = val;
}


/**
 * ��������:���ݸ�����ʮ�������ַ������ݹ���һ��������
 * ��������:str�������������
 */
big_num::big_num(const std::string & str) : is_negative(false) {
	std::string t(str);
	if (t.size() && t.at(0) == '-') {
		if (t.size() > 1)
			is_negative = true;
		t = t.substr(1);

	}
	int cnt = (8 - (t.size() % 8)) % 8;// ���ĳ��Ȳ���8�ı���,����0
	std::string temp;

	for (int i = 0; i < cnt; ++i)
		temp.push_back('0');

	t = temp + t;

	for (size_t i = 0; i < t.size(); i += base_char) {
		base_t sum = 0;
		for (int j = 0; j < base_char; ++j) {    // 8λʮ��������ɴ�������һλ
			char ch = t[i + j];
			int num = hex_to_dec(ch);
			sum = ((sum << 4) | (num));

		}
		data.push_back(sum);

	}
	reverse(data.begin(), data.end());// ��λ�ں�
	*this = trim();// ȥ����λ��0
}

/**
 * ��������:���ݸ�����long_t�������ݹ���һ��������
 * ��������:num�������������
 */
big_num::big_num(const long_t & num) : is_negative(false) {
	long_t t = num;
	if (t < 0) {
		is_negative = true;
		t = -t;

	}
	do {
		base_t temp = (t&base_num);    // ÿ�ν�ȡ��32λ
		data.push_back(temp);
		t >>= base_int;

	} while (t);

}








//�������ӷ�����
big_num big_num::add(const big_num & val)
{
	big_num num(*this);
	if (num.is_negative == val.is_negative)	//������ͬ��
	{
		int len = val.data.size() - num.data.size();

		while ((len--) > 0)	//������λ���٣������λ��0
		{
			num.data.push_back(0);
		}
		int carry = 0;	//��λ
		for (size_t i = 0; i < val.data.size(); i++)
		{
			base_t temp = num.data[i];
			num.data[i] += val.data[i] + carry;
			// �޷��������,����ȡ������
			// ��λ:һ�������޽�λ������,һ�����н�λ�ų���(����ʮ�������,9+9+1,��9,9+0+0,��9)
			carry = (temp > num.data[i] ? 1 : (temp > (temp + val.data[i]) ? 1 : 0));
		}

		for (size_t i = val.data.size(); i < num.data.size() && carry != 0; ++i) {// ���н�λ
			            base_t temp = num.data[i];
			            num.data[i] += carry;
			             carry = temp > num.data[i];
		}
		if (carry)	// ���н�λ
			num.data.push_back(carry);
	}
	else	//���������
	{
		big_num a = my_abs();
		big_num b = val.my_abs();
		int flag = a.my_compare(b);	//���߾���ֵ�Ƚϴ�С
		if (flag == -1)	//val�ľ���ֵ���󣬷�����Ϊval��
		{
			num = b.sub(a);
			num.is_negative = val.is_negative;
		}
		else if (flag == 0)	//���߾���ֵ���
		{
			num = ZERO;
		}
		else //����ľ���ֵ���󣬷���ʹ�ñ����
		{
			num = a.sub(b);
			num.is_negative = is_negative;
		}
	}

	return num;
}



//��������������
big_num big_num::sub(const big_num & val)
{
	big_num num(*this);
	big_num a = my_abs();
	big_num b = val.my_abs();
	if (num.is_negative == val.is_negative)	//����ͬ��
	{
		int flag = a.my_compare(b);
		if (flag == 1)
		{
			int borrow = 0;    // ��λ
			// ������С��
			for (size_t i = 0; i < val.data.size(); ++i) {
				base_t temp = num.data[i];
				num.data[i] -= val.data[i] + borrow;
				// ��λ:һ�������޽�λ������,��һ�����н�λ�ų���(����ʮ�������,9-0-0,��9,9-9-1,��9)
				borrow = temp < num.data[i] ? 1 : (temp - borrow < val.data[i] ? 1 : 0);

			}
			for (size_t i = val.data.size(); i < num.data.size() && borrow != 0; ++i) {// ���н�λ
				base_t temp = num.data[i];
				num.data[i] -= borrow;
				borrow = temp < (base_t)borrow;

			}
			num = num.trim();// ȥ����λ�����0
		}
		else if (flag == 0)
			num = ZERO;
		else {	//a�ľ���ֵС��b�ľ���ֵ
			num = b.sub(a);
			num.is_negative = !is_negative;
		}
	}
	else {					//�������
		num = a.add(b);    //ת��Ϊ�ӷ�
		num.is_negative = is_negative;
	}
	return num;
}



//�������˷�����
big_num big_num::mul(const big_num & val) const
{
	if (my_equal(ZERO) || val.my_equal(ZERO))
		return ZERO;
	//λ���ٵ���Ϊ����
	const big_num & big = data.size() > val.data.size() ? (*this) : val;
	const big_num & small = (&big) == (this) ? val : (*this);

	big_num num;
	bit t(small);	//ת��Ϊ�����ƽ�������

	for (int i = t.size() - 1; i >= 0; i--)
	{
		if (t.at(i))	//�жϵ�iΪ�Ƿ�Ϊ1
		{
			big_num temp(big);
			temp.is_negative = false;	//��Ϊ����
			temp = temp.left_shift(i);	//��λ����
			num = num.add(temp);		//������λΪ1�����˴��������������ս��
		}
	}
	num.is_negative = !(is_negative == val.is_negative);	//ͬ�ŵ�������ŵø�
	return num;
}



//��������������
big_num big_num::div(const big_num & val)
{
	big_num temp;
	big_num num = div_rem(val, temp);
	return num;
}


//������ȡ������
big_num big_num::remain(const big_num & val)
{
	big_num num;
	div_rem(val, num);
	return num;
}


//������ȡģ���㣨��ȡ��Ĳ�ͬ�Ǹú������Ƿ�����������
big_num big_num::mod(const big_num & m)
{
	big_num num = remain(m);
	if (num.is_negative)	//����Ǹ��������������Ļ����ϼ�һ��ģ����ʹ�÷���ֵ��Ϊ����
		num = num.add(m);
	return num;
}


//������������ȡ�����㣬����ֵ�����������ȡ������m����
//val�ǳ���
big_num big_num::div_rem(const big_num & val, big_num & m)
{
	assert(!val.my_equal(ZERO));	//������Ϊ0���򱨴�
	big_num a = my_abs();
	big_num b = val.my_abs();
	int flag = a.my_compare(b);
	if (flag == 0)	//���߾���ֵ���
	{
		return (is_negative == val.is_negative) ? big_num(1) : big_num(-1);	//ͬ�ŷ���1����ŷ���-1
	}
	if (flag == -1)	//��������ֱ�ӷ��ر����������Ϊ0
	{
		m = *this;
		return ZERO;
	}

	big_num num;

	bit bit_b(b);	//�����ƴ���
	while (true)	//���������a�ľ���ֵ����b�ľ���ֵ��������������
	{
		bit bit_a(a);
		int len = bit_a.size() - bit_b.size();

		big_num temp;
		while (len >= 0)	
		{
			temp = b.left_shift(len);		//��������
			if (temp.my_compare(a) != 1)	//����Ҫ�ҵ���ʹa��С��b���������λ��
				break;
			len--;
		}
		if (len < 0)	//��ʱaС��b���������
			break;

		base_t temp_num = 0;
		while (temp.my_compare(a) != 1)	
		{
			a = a.sub(temp);
			temp_num++;	//ͳ�Ƶ�ǰ��a��಻С����λ��λ���b
		}
		temp = big_num(temp_num);
		if (len)
			temp = temp.left_shift(len);	//��λ�������ǰ��a��b�ļ���
		num = num.add(temp);
	}
	num.is_negative = !(is_negative == val.is_negative);	//ͬ��Ϊ�������Ϊ��
	m.data = a.data;
	m.is_negative = is_negative;
	return num;
}




//�������ݳ�����
big_num big_num::pow(const big_num & exponent)
{
	big_num num(1);
	bit t(exponent);	//ת��Ϊ�����ƽ��д���
	for (int i = t.size() - 1; i >= 0; i--)
	{
		num = num.mul(num);
		if (t.at(i))
			num = mul(num);
	}
	return num;
}



//������ģ������
big_num big_num::mod_pow(const big_num & exponent, const big_num & m) const
{
	assert(!m.my_equal(ZERO));	//ģ��Ϊ0������
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



//��չŷ������㷨��˷���Ԫ
//m��������Ԫʱ��ģ��
big_num big_num::mod_inverse(const big_num & m)
{
	assert(!is_negative);	//��ǰ������Ϊ����ʱ�ͱ���
	assert(!m.is_negative);	//mҲҪΪ���������򱨴�
	if (my_equal(ZERO) || m.my_equal(ZERO))
		return ZERO;	//����֮����һ��Ϊ0�Ͳ����ڳ˷���Ԫ
	big_num a[3], b[3], t[3];

	// ���½��г��ȱ任
	a[0] = 0; a[1] = 1; a[2] = *this;
	b[0] = 1; b[1] = 0; b[2] = m;

	for (t[2] = a[2].mod(b[2]); !t[2].my_equal(ZERO); t[2] = a[2].mod(b[2])) {
		big_num temp = a[2].div(b[2]);
		for (int i = 0; i < 3; ++i) {
			t[i] = a[i].sub(temp.mul(b[i]));// ������һ��a[2]-temp*b[2]�ͱ�Ϊ������С��
			a[i] = b[i];
			b[i] = t[i];
		}

	}
	if (b[2].my_equal(ONE)) {// ���Լ��Ϊ1,���ڳ˷���Ԫ
		if (b[1].is_negative)// ��ԪΪ����
			b[1] = b[1].add(m);// ��Ϊ����,ʹ����m��ʣ�༯��
		return b[1];

	}
	return ZERO;// ���Լ����Ϊ1,�޳˷���Ԫ
}



/**
 * ��������:��λ����,����
 * ��������:len������λ��λ��
 */
big_num big_num::left_shift(const unsigned len) {
	int index = len >> base_bit;    // ������ÿһλ��Ҫ�ƶ�����λ
	int shift = len & base_temp;    // ��ʣ�¶���λ
	big_num ans(*this);

	int inc = (shift == 0) ? index : index + 1;// �ж����λҪ�࿪��������һλ
	for (int i = 0; i < inc; ++i)
		ans.data.push_back(0);    // ��λ��0

	if (index) {
		inc = (shift == 0) ? 1 : 2;// �ж����λҪԤ��һλ
		for (int i = ans.data.size() - inc; i >= index; --i)
			ans.data[i] = ans.data[i - index];
		for (int i = 0; i < index; ++i)
			ans.data[i] = 0;

	}
	if (shift) {
		base_t t = base_num;
		t <<= base_int - shift;    // ���ڽ�ȡ��λ
		// ����
		base_t temp = 0;
		for (size_t i = 0; i < ans.data.size(); ++i) {
			base_t tmp = ans.data[i];
			ans.data[i] = (tmp << shift) | temp;// ���ƺ���ϴ�������λ�ĸ�λ
			temp = (tmp&t) >> (base_int - shift);// ��ȡ�ô�����λ�ĸ�λ

		}

	}
	ans = ans.trim();
	return ans;

}

/**
 * ��������:��λ����,����
 * ��������:len������λ��λ��
 */
big_num big_num::right_shift(const unsigned len) {
	bit val(*this);
	if (len >= val.size())// ��ǰ������λ��С�ڵ�����λλ��,����0
		return ZERO;
	int index = len >> base_bit;// ������ÿһλ��Ҫ�ƶ�����λ
	int shift = len & base_temp;// ��ʣ�¶���λ
	big_num ans(*this);

	if (index) {
		for (int i = 0; i < index; ++i)
			ans.data[i] = ans.data[i + index];
		for (int i = 0; i < index; ++i)
			ans.data.pop_back();    // ��λɾ��

	}
	if (shift) {
		base_t t = base_num;
		t >>= base_int - shift;    // ���ڽ�ȡ��λ
		// ����
		base_t temp = 0;
		for (int i = ans.data.size() - 1; i >= 0; --i) {
			base_t tmp = ans.data[i];
			ans.data[i] = (tmp >> shift) | temp;// ���ƺ���ϴ�������λ�ĵ�λ
			temp = (tmp&t) << (base_int - shift);// ��ȡ�ô�����λ�ĵ�λ

		}

	}
	ans = ans.trim();
	return ans;

}



/**
 * ��������:�������ȽϺ���,-1��ʾ��������ҪС,0��ʾ���,1��ʾ��������Ҫ��
 * ��������:val����Ҫ��֮�ȽϵĴ�����
 */
int big_num::my_compare(const big_num & val) const {
	if (is_negative != val.is_negative) {// ���Ų�ͬ,������С
		if (is_negative == true)
			return -1;
		return 1;

	}
	int flag = 0;
	if (data.size() < val.data.size())// λ����С
		flag = -1;
	else if (data.size() > val.data.size())// λ���ϴ�
		flag = 1;
	else {    // λ�����,�Ӹ�λ��ʼһһ�Ƚ�
		for (std::vector<base_t>::const_reverse_iterator it = data.rbegin(), ite = val.data.rbegin(); it != data.rend(); ++it, ++ite)
			if ((*it) != (*ite)) {
				flag = (*it) < (*ite) ? -1 : 1;    // ��λС,��С
				break;

			}

	}
	if (is_negative)    // ��Ϊ����,С�ķ�����
		flag = -flag;
	return flag;

}

/**
 * ��������:�������Ƿ���Ⱥ���
 * ��������:val��ʾҪ��֮�ȽϵĴ�����
 */
bool big_num::my_equal(const big_num & val) const {
	return (is_negative == val.is_negative) && (data == val.data);// ���ź����ݶ�Ҫ���

}

/**
 * ��������:��һ��long_t���͵�����ת��Ϊ������������
 * ��������:num��ʾ��������
 */
big_num big_num::to_bignum(const long_t & num) {
	return big_num(num);

}

/**
 * ��������:��������ת��Ϊʮ�������ַ���������
 */
std::string big_num::to_string() const {
	std::string ans;
	base_t t = base_num;
	t <<= base_int - 4;    // ���ڽ�ȡ��4λ
	for (int i = data.size() - 1; i >= 0; --i) {
		base_t temp = data[i];
		for (int j = 0; j < base_char; ++j) {
			base_t num = t & temp;// ÿ�ν�ȡ��4λ
			num >>= base_int - 4;    // ����4λ�Ƶ���4λ
			temp <<= 4;
			if (num < 10)
				ans.push_back((char)('0' + num));
			else
				ans.push_back((char)('A' + num - 10));

		}

	}
	while (ans.size() > 0 && ans.at(0) == '0')// ȥ����λ���õ�0
		ans = ans.substr(1);
	if (ans.empty())    // �մ�˵��Ϊ0
		ans.push_back('0');
	if (is_negative)    // Ϊ�������ϸ���
		ans = "-" + ans;
	return ans;

}



/**
 * ��������:���ش������ľ���ֵ
 */
big_num big_num::my_abs() const {
	big_num ans;
	ans.data = data;// ֻ��������,����Ĭ��Ϊ��
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
 * ��������:�����ô�������һ������,ȥ������λ���õ�0�󲢷���
 */
big_num big_num::trim() {
	size_t cnt = 0;
	// ����λΪ0��Ԫ�ص�����
	for (std::vector<base_t>::const_reverse_iterator it = data.rbegin(); it != data.rend(); ++it) {
		if ((*it) == 0)
			++cnt;
		else
			break;

	}
	if (cnt == data.size())    // ֻ������������
		--cnt;
	big_num ans(*this);
	for (size_t i = 0; i < cnt; ++i)
		ans.data.pop_back();
	return ans;

}

/**
 * ��������:���ݸ������ַ�ȷ��������Ӧ��ʮ������
 * ��������:ch����������ַ�
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
 * ��������:���ݸ����Ĵ�������ʼ��
 * ��������:val��������Ĵ�����
 */
big_num::bit::bit(const big_num & val) {
	bit_vector = val.data;
	base_t temp = bit_vector[bit_vector.size() - 1];// ���������λ
	length = bit_vector.size() << base_bit;    // ������һλռ������32λ
	base_t t = 1 << (base_int - 1);    // ���ڽ�ȡһ�����Ķ��������λ

	if (temp == 0)    // ���������λΪ0,��ȥ32
		length -= base_int;
	else {
		while (!(temp & t)) {// �Ӹ�λ��ʼ���������Ķ�����λ,Ϊ0���ȼ�һ
			--length;
			t >>= 1;    // ����һλ��ʾ�����һλ

		}

	}

}

/**
 * ��������:���������ĵ�idλ������λ�Ƿ�Ϊ1
 * ��������:id�����idλ
 */
bool big_num::bit::at(size_t id) {
	size_t index = id >> base_bit;// ȷ�����ڴ������ڼ�λ
	size_t shift = id & base_temp;// ȷ�����ڴ�������һλ�Ķ����Ƶڼ�λ
	base_t t = bit_vector[index];
	return (t & (1 << shift));

}
