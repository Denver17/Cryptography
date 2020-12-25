#include "pch.h"
#include <iostream>
#include "encrypt_decrypt.h"

int main() {
	encrypt_decrypt en_de;
	en_de.reset();// ������Կ����

	char ch;
	std::string str;
	bool ok = true;

	do {
		en_de.menu();// �˵���ʾ
		std::cout << ">";
		std::cin >> str;
		if (str.empty()) {
			std::cout << "�������!����������!" << std::endl;
			continue;

		}
		ch = str.at(0);
		switch (ch) {
		case 'e':
		case 'E':
			if (!en_de.encrypt())
				std::cout << "����ʧ��,������!" << std::endl;
			break;
		case 'd':
		case 'D':
			if (!en_de.decrypt())
				std::cout << "����ʧ��,������!" << std::endl;
			break;
		case 'p':
		case 'P':
			en_de.print();// ��ӡ�����Ϣ
			break;
		case 'r':
		case 'R':
			en_de.reset();// ����������Կ����
			break;
		case 'q':
		case 'Q':
			ok = false;    // �˳�
			break;
		default:
			break;

		}

	} while (ok);
	return 0;

}