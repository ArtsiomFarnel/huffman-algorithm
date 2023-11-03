#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include "Huffman.hpp"

using namespace std;

typedef unsigned char byte;

byte pack_byte(bool bits[8]);
float averageCodewordLength(int length[]);
double entropy(float P[]);

int main() {
	//Ñ×ÈÒÛÂÀÍÈÅ ÑÎÎÁÙÅÍÈß ÈÇ ÔÀÉËÀ
	string buf; //áóôåð ñ÷èòûâàåìîé èç ôàéëà ñòðîêè
	ifstream fin("input.txt"); //ïîïûòêà îòêðûòèÿ ôàéëà
	if (!fin.is_open()) { //åñëè ôàéë íå íàéäåí
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else {
		getline(fin, buf); //èíà÷å ñ÷èòûâàíèå ñîäåðæèìîãî ôàéëà
	}
	//cout << "Text file content uploaded: " << buf << endl;
	fin.close(); //çàêðûòèå ôàéëîâîãî ïîòîêà

	//ÍÀÕÎÆÄÅÍÈÅ ÂÑÅÕ ÓÇËÎÂ ÁÓÄÓÙÅÃÎ ÄÅÐÅÂÀ
	Node *allNodes = new Node[51]; //âûäåëåíèå ïàìÿòè äëÿ âñåõ óçëîâ

	Node *startNodes = allocateMemory(); //âûäåëåíèå ïàìÿòè äëÿ íà÷àëüíûõ 26 óçëîâ
	for (int counter = 0; counter < 26; counter++) //ïîìåñòèòü íà÷àëüíûå óçëû â ñïèñîê âñåõ óçëîâ
		allNodes[counter] = startNodes[counter];

	Node* sums = new Node[25]; //âûäåëåíèå ïàìÿòè äëÿ îñòàëüíûõ óçëîâ
	sums = searchAllNodes(startNodes); //íàõîæäåíèå îñòàëüíûõ óçëîâ
	for (int counter = 26; counter < 51; counter++) //ïîìåñòèòü îñòàëüíûå óçëû â ñïèñîê âñåõ óçëîâ
		allNodes[counter] = sums[counter-26];
	
	bubbleSorting(51, allNodes); //ñîðòèðîâêà âñåõ óçëîâ ïî óáûâàíèþ âåðîÿòíîñòåé

	for (int counter = 1; counter < 51; counter++) { //ïðèñâàèâàíèå âñåì óçëàì íàïðàâëåíèÿ. 1 - ëåâî, 0 - ïðàâî
		if(counter %2 == 0)
			allNodes[counter].code = "1";
		else allNodes[counter].code = "0";
	}

	//ÏÎÑÒÐÎÅÍÈÅ ÄÅÐÅÂÀ
	Node *root = nullptr; //âûäåëåíèå ïàìÿòè äëÿ äåðåâà
	for(int counter = 0; counter < 51; counter++) //ïîñòðîåíèå äåðåâà èç 51 óçëà
		addNode(root, allNodes[counter]);

	treePrint(root, 0); //âûâîä äåðåâà â êîíñîëü

	int length[26]; //ìàññèâ äëÿ õðàíåíèÿ äëèí êîäîâûõ ñëîâ
	Node *leaf = new Node[26]; //âûäåëåíèå ïàìÿòè äëÿ çàêîäèðîâàííûõ óçëîâ
	for (int counter = 0; counter < 26; counter++) { //ïîñòðîåíèå èòîãîâûõ êîäîâ äëÿ 26 ëèñòîâ äåðåâà
		leaf[counter] = *codeBuilding(root, A[counter]);
		length[counter] = leaf[counter].code.size();
		cout << A[counter] << " : " << leaf[counter].code << " | Frequensy: " << leaf[counter].freq << endl;
		//cout << "Size: " << arr[counter] << endl; //äëèíà êîäîâîãî ñëîâà
	}
	cout << endl << "Entropy: " << entropy(P) << endl; //ýíòðîïèÿ
	cout << "Average codeword length: " << averageCodewordLength(length) << endl << endl; //ñðåäíÿÿ äëèíà êîäîâîãî ñëîâà

	//ÇÀÏÈÑÜ ÇÀÊÎÄÈÐÎÂÀÍÍÎÃÎ ÑÎÎÁÙÅÍÈß Â ÔÀÉË
	string codes; //áóôåð çàïèñûâàåìûõ â ôàéë êîäîâ
	ofstream fout("output.txt"); //ïîïûòêà îòêðûòèÿ ôàéëà
	if (!fout.is_open()) { //åñëè ôàéë íå íàéäåí
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else { //èíà÷å çàïèñü èòîãîâûõ êîäîâ â ôàéë
		const char *str = buf.c_str();
		for (size_t first = 0; first < buf.length(); first++) {
			for (int second = 0; second < 26; second++) {
				if (str[first] == A[second]) {
					fout << leaf[second].code;
					codes += leaf[second].code; //çàïèñûâàåìûå â ôàéë êîäû ïîìåùàåòñÿ â ñòðîêó
				}
			}
		}
	}
	fout.close(); //çàêðûòèå ôàéëîâîãî ïîòîêà

	//ÇÀÏÈÑÜ ÇÀÊÎÄÈÐÎÂÀÍÍÎÃÎ ÑÎÎÁÙÅÍÈß Â ÔÀÉË Â ÁÀÉÒÎÂÎÌ ÂÈÄÅ
	while (1) { //äîáàâëåíèå äîïîëíèòåëüíûõ íóëåé ê ñòðîêå êîäîâ äëÿ çàïèñè â áàéòîâîì âèäå
		codes += "0";
		if (codes.size() % 8 == 0) break;
	}
	//cout << endl << codes.size() << endl;

	ofstream fbout("binary.txt"); //ïîïûòêà îòêðûòèÿ ôàéëà
	if (!fbout.is_open()) { //åñëè ôàéë íå íàéäåí
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	size_t counterr = 0;
	while (counterr < codes.size()) { //çàïèñü èòîãîâûõ êîäîâ â ôàéë â áàéòîâîì âèäå
		bool bits[8];
		for (int i = 0; i < 8; i++) {
			if (codes[i + counterr] == '1')
				bits[i] = 1;
			if (codes[i + counterr] == '0')
				bits[i] = 0;
			//cout << bits[i];
		}
		byte b = pack_byte(bits); //âûçîâ ôóíêöèè óïàêîâûâàíèÿ áàéòîâ
		fbout << b;
		//cout << endl;
		counterr += 8;
	}
	fbout.close();

	//ÎÑÂÎÁÎÆÄÅÍÈÅ ÂÛÄÅËÅÍÍÎÉ ÏÀÌßÒÈ
	freeMemory(startNodes); //îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ íà÷àëüíûõ 26 óçëîâ
	delete[] sums; //îñâîáîæäåíèå ïàìÿòè âûäåëåííîé ïàìÿòè äëÿ îñòàëüíûõ óçëîâ
	delete[] allNodes; //îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ âñåõ óçëîâ
	freeTree(root); //îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ äåðåâà
	delete[] leaf; //îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ çàêîäèðîâàííûõ óçëîâ

	system("pause");
	return EXIT_SUCCESS;
}

//ÔÓÍÊÖÈß ÓÏÀÊÎÂÛÂÀÍÈß ÁÀÉÒÎÂ
byte pack_byte(bool bits[8]) {
	byte result(0);
	for (unsigned i(8); i--;) {
		result <<= 1;
		result |= byte(bits[i]);
	}
	return result;
}

//ÔÓÍÊÖÈß ÏÎÄÑ×ÅÒÀ ÑÐÅÄÍÅÉ ÄËÈÍÛ ÊÎÄÎÂÎÃÎ ÑËÎÂÀ
float averageCodewordLength(int length[]) {
	float L = 0.0f;
	for (int count = 0; count < 26; count++)
		L += P[count] * length[count];
	return L;
}

//ÔÓÍÊÖÈß ÏÎÄÑ×ÅÒÀ ÝÍÒÐÎÏÈÈ
double entropy(float P[]) {
	double H = 0.0;
	for (int count = 0; count < 26; count++)
		H += -P[count] * log(P[count]) / log(2);
	return H;
}
