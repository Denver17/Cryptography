#include "pch.h"
#include <iostream>
#include "encrypt_decrypt.h"

int main() {
	encrypt_decrypt en_de;
	en_de.reset();// 设置密钥长度

	char ch;
	std::string str;
	bool ok = true;

	do {
		en_de.menu();// 菜单显示
		std::cout << ">";
		std::cin >> str;
		if (str.empty()) {
			std::cout << "输入错误!请重新输入!" << std::endl;
			continue;

		}
		ch = str.at(0);
		switch (ch) {
		case 'e':
		case 'E':
			if (!en_de.encrypt())
				std::cout << "加密失败,请重试!" << std::endl;
			break;
		case 'd':
		case 'D':
			if (!en_de.decrypt())
				std::cout << "解密失败,请重试!" << std::endl;
			break;
		case 'p':
		case 'P':
			en_de.print();// 打印相关信息
			break;
		case 'r':
		case 'R':
			en_de.reset();// 重新设置密钥长度
			break;
		case 'q':
		case 'Q':
			ok = false;    // 退出
			break;
		default:
			break;

		}

	} while (ok);
	return 0;

}