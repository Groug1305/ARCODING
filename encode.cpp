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
		file.seekg(0); //возвращаем указатель в начало файла
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//печатаетс€ структура вида " *символ_нейм*: *его_число*"
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

	float tmp = 0;
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
	

	int k = jija.Jija.size();
	fout.write((char*)&k, sizeof(k));										//записываем количество символов элементов
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++) {
		fout.write((char*)&it->first, sizeof(it->first));					//пишем символ
		fout.write((char*)&it->second, sizeof(it->second));					//пишем количество
	}

	double l = 0, h = 1;				//предшествующий промежуток
	double l2 = 0, h2 = 1;			//промежуток, который будем определ€ть на этом шаге
	double a = 0, b = 1;				//a - лева€ граница символа, b - права€
	int ncount = 0;
	while (!fin.eof()) {
		char c = fin.get();
		cout << c;
		ncount++;
		a = borders[c].left;
		b = borders[c].right;

		l2 = l + a * (h - l);
		h2 = l + b * (h - l);

		if (/*l2 == h2*/ ncount == 10 || fin.eof()) {				//если они совпали - значит нам уже не хватает точности, и надо выписывать результат
			/*fout << (l + h) / (double)2;
			cout << " result= " << (l + h) / (double)2 << endl;*/
			/*fout << ncount;
			cout << " ncount= " << ncount << endl;*/
			fout.write((char*)&l, sizeof(l));
			cout << " result= " << l << endl;
			l = 0;
			h = 1;
			ncount = 0;
		}
		else {						//если не совпали - считаем дальше с новыми границами
			l = l2;
			h = h2;
		}
	}

	fin.close();
	fout.close();

	return 0;
}