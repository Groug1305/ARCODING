#include <iostream>
#include <map>
#include <list>
#include <ctime>
#include <string.h>
#include <fstream>
#include <vector>
#include <stdbool.h>
using namespace std;

class Grani {
public:
	char c;
	int key;
	double right, left;
};


class MapKeys {
public:
	map<char, int> Jija;

	///////////////////////////////////////

	void CreateMap(ifstream& file) {
		char c;
		while (file.get(c)) {
			Jija[c]++;
		}
		file.clear();
		file.seekg(0); //���������� ��������� � ������ �����
	}

	int HeadRead(ifstream& file) {
		unsigned long long len = 0;					//����� ������ ������, �� ������ (������) ������
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//��������� ������
			file.read((char*)&key, sizeof(key));	//��������� ��� ��������
			Jija[s] = key;							//�������� ���� � ����
			len += key;
		}

		return len;
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//���������� ��������� ���� " *������_����*: *���_�����*"
		}
	}
};

static bool compare(Grani c1, Grani c2) {
	return c1.key > c2.key;
}


int main() {
	ifstream fin("encode.txt", ios::binary);
	if (!fin.is_open()) {
		cout << "encode_error";
		return 1;
	}

	ofstream fout("decode.txt", ios::binary);
	if (!fout.is_open()) {
		cout << "decode_error";
		return 1;
	}

	MapKeys jija;

	int len = jija.HeadRead(fin);		//�� ���� ���� �������� ����� ������, ����������� ��� ���������� ������
	jija.Print();

	list<Grani> List;
	unsigned long long count = 0;
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++) {
		Grani p;
		p.c = it->first;
		p.key = it->second;
		count += p.key;
		List.push_back(p);
	}
	List.sort(compare);

	double tmp = 0;
	for (auto qwerty = List.begin(); qwerty != List.end(); qwerty++) {
		cout << "qwerty.c: " << qwerty->c << endl;

		qwerty->left = tmp;
		cout << "qwerty.left: " << qwerty->left << endl;

		qwerty->right = tmp + (double)qwerty->key / (double)count;
		cout << "qwerty.right: " << qwerty->right << endl;

		tmp = qwerty->right;
	}

	map<char, Grani> borders;
	for (auto it = List.begin(); it != List.end(); it++) {
		borders[it->c] = (*it);
	}

	double l = 0, h = 1;				//�������������� ����������
	double l2 = 0, h2 = 1;				//����������, ������� ����� ���������� �� ���� ����
	double a = 0, b = 1;				//a - ����� ������� �������, b - ������
	double value = 0;
	int p = 0;
	while (!fin.eof()) {
		p = 5;
		fin.read((char*)&value, sizeof(value));
		//fin >> value;
		cout << "value: " << value << endl;
		do {
			for (auto i = borders.begin(); i != borders.end(); i++) {
				a = i->second.left;
				b = i->second.right;

				l2 = l + a * (h - l);
				h2 = l + b * (h - l);

				if (l2 - value <= 0.00000001 && h2 - value > 0.00000001) {
					cout << "char: " << i->second.c << endl;
					cout << l2 << " <= " << value << " < " << h2 << endl;
					fout << i->second.c;
					l = l2;
					h = h2;
					p--;
				}

				if (p == 0) {
					cout << "moving to next" << endl;
					l = 0;
					h = 1;
					break;
				}
			}
		} 		
		while (p != 0);
	}


	fin.close();
	fout.close();

	return 0;
}