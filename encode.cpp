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
	unsigned int key;
	int right, left;
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
		file.seekg(0); //возвращаем указатель в начало файла
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//печатается структура вида " *символ_нейм*: *его_число*"
		}
	}
};

static bool compare(Grani c1, Grani c2) {
	return c1.key > c2.key;
}


int main() {
	ifstream fin("test.txt", ios::binary);
	if (!fin.is_open()) {
		cout << "test_error";
		return 1;
	}

	ofstream fout("encode.txt", ios::binary);
	if (!fout.is_open()) {
		cout << "encode_error";
		return 1;
	}

	MapKeys jija;

	jija.CreateMap(fin);					//создаем мапу
	cout << jija.Jija.size() << endl;
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

	int tmp64 = 0;
	for (auto qwerty = List.begin(); qwerty != List.end(); qwerty++) {
		cout << "qwerty.c: " << qwerty->c << endl;

		qwerty->left = tmp64;
		cout << "qwerty.left: " << qwerty->left << endl;

		qwerty->right = tmp64 + qwerty->key;
		cout << "qwerty.right: " << qwerty->right << endl;

		tmp64 = qwerty->right;
	}
	

	int k = jija.Jija.size();
	fout.write((char*)&k, sizeof(k));										//записываем количество символов элементов
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++) {
		fout.write((char*)&it->first, sizeof(it->first));					//пишем символ
		fout.write((char*)&it->second, sizeof(it->second));					//пишем количество
	}

	list<Grani>::iterator it;
	unsigned short l = 0, h = 65535;
	int delitel = List.back().right;
	unsigned short First_qtr = (h + 1) / 4;									//16384
	unsigned short Half = First_qtr * 2;										//32768
	unsigned short Thride_qtr = First_qtr * 3;									//49152
	int bitsf = 0;								//сколько битов сбрасывать
	char tmp = 0;
	int ncount = 0;
	while (!fin.eof()) {
		int c = fin.get();
		if (c == EOF) break;
		for (it = List.begin(); it != List.end() && it->c != c; it++);
		int d = h - l + 1;
		h = l + it->right * d / delitel - 1;
		l = l + it->left * d / delitel;
		while(1) {
			if (h < Half) {
				ncount++;
				if (ncount == 8)
				{
					fout.put(tmp);
					tmp = 0;
					ncount = 0;
				}
				for (; bitsf > 0; bitsf--)
				{
					tmp = tmp | (1 << (7 - ncount));
					ncount++;
					if (ncount == 8)
					{
						fout.put(tmp);
						tmp = 0;
						ncount = 0;
					}
				}
			}
			else if (l >= Half) {
				tmp = tmp | (1 << (7 - ncount));
				ncount++;
				if (ncount == 8)
				{
					fout.put(tmp);
					tmp = 0;
					ncount = 0;
				}
				for (; bitsf > 0; bitsf--)
				{
					ncount++;
					if (ncount == 8)
					{
						fout.put(tmp);
						tmp = 0;
						ncount = 0;
					}
				}
				h -= Half;
				l -= Half; 
			}
			else if ((l >= First_qtr) && (h < Thride_qtr)) {
				bitsf++;
				h -= First_qtr;
				l -= First_qtr; 
			}
			else break;

			h <<= 1;
			l <<= 1;
			h++;
		}
	}

	fin.close();
	fout.close();

	return 0;
}