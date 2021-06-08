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
		file.seekg(0); //âîçâðàùàåì óêàçàòåëü â íà÷àëî ôàéëà
	}

	void Print() {
		for (auto it = Jija.begin(); it != Jija.end(); it++) {
			cout << it->first << ": " << it->second << endl;					//ïå÷àòàåòñÿ ñòðóêòóðà âèäà " *ñèìâîë_íåéì*: *åãî_÷èñëî*"
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

	jija.CreateMap(fin);					//ñîçäàåì ìàïó
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
	fout.write((char*)&k, sizeof(k));										//çàïèñûâàåì êîëè÷åñòâî ñèìâîëîâ ýëåìåíòîâ
	for (auto it = jija.Jija.begin(); it != jija.Jija.end(); it++) {
		fout.write((char*)&it->first, sizeof(it->first));					//ïèøåì ñèìâîë
		fout.write((char*)&it->second, sizeof(it->second));					//ïèøåì êîëè÷åñòâî
	}

	double l = 0, h = 1;				//ïðåäøåñòâóþùèé ïðîìåæóòîê
	double l2 = 0, h2 = 1;			//ïðîìåæóòîê, êîòîðûé áóäåì îïðåäåëÿòü íà ýòîì øàãå
	double a = 0, b = 1;				//a - ëåâàÿ ãðàíèöà ñèìâîëà, b - ïðàâàÿ
	int ncount = 0;
	while (!fin.eof()) {
		char c = fin.get();
		cout << c;
		ncount++;
		a = borders[c].left;
		b = borders[c].right;

		l2 = l + a * (h - l);
		h2 = l + b * (h - l);

		if (/*l2 == h2*/ ncount == 10 || fin.eof()) {				//åñëè îíè ñîâïàëè - çíà÷èò íàì óæå íå õâàòàåò òî÷íîñòè, è íàäî âûïèñûâàòü ðåçóëüòàò
			fout << (l + h) / (double)2;
			cout << " result= " << (l + h) / (double)2 << endl;
			/*fout << ncount;
			cout << " ncount= " << ncount << endl;*/
			l = 0;
			h = 1;
			ncount = 0;
		}
		else {						//åñëè íå ñîâïàëè - ñ÷èòàåì äàëüøå ñ íîâûìè ãðàíèöàìè
			l = l2;
			h = h2;
		}
	}

	fin.close();
	fout.close();

	return 0;
}
