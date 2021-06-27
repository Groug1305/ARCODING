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

	int HeadRead(ifstream& file) {
		unsigned long long len = 0;					//длина общего текста, не считая (собаки) головы
		int key, q;
		char s;

		file.read((char*)&q, sizeof(q));
		for (int i = 0; i < q; i++) {
			file.read((char*)&s, sizeof(s));		//считываем символ
			file.read((char*)&key, sizeof(key));	//считываем его значение
			Jija[s] = key;							//помещаем ключ в мапу
			len += key;
		}

		return len;
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

	int len = jija.HeadRead(fin);		//из мапы жижи получаем общую длинну, получаенную при считывании хэдера
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


	list<Grani>::iterator it;
	int last = List.back().right;
	unsigned short l = 0, h = 65535;
	unsigned short First_qtr = (h + 1) / 4;										//16384
	unsigned short Half = First_qtr * 2;										//32768
	unsigned short Thride_qtr = First_qtr * 3;									//49152
	int value = (fin.get() << 8) | fin.get();
	char tmp = fin.get();
	int ncount = 0;
	//cout<<endl<<delitel<<endl;
	while (len){
		int d = h - l + 1;
		unsigned int freq = (((value - l + 1) * last) - 1) / d;
		for (it = List.begin(); it->right <= freq; it++);
		
		h = l + it->right * d / last - 1;
		l = l + it->left * d / last;
		while(1){
			if (h < Half);
			else if (l >= Half){
				value -= Half;
				l -= Half;
				h -= Half;
			}
			else if ((l >= First_qtr) && (h < Thride_qtr)){
				value -= First_qtr;
				l -= First_qtr;
				h -= First_qtr;
			}
			else break;
			l <<= 1;
			h <<= 1;
			h++;
			value <<= 1;
			value += ((tmp & (1 << (7 - ncount))) >> (7 - ncount));
			ncount++;
			if (ncount == 8){
				tmp = fin.get();
				ncount = 0;
			}
		}
		fout << it->c;
		len--;
	}


	fin.close();
	fout.close();

	return 0;
}