#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include "Huffman.h"

using namespace std;

typedef unsigned char byte;

byte pack_byte(bool bits[8]);
float averageCodewordLength(int length[]);
double entropy(float P[]);

int main() {
	//СЧИТЫВАНИЕ СООБЩЕНИЯ ИЗ ФАЙЛА
	string buf; //буфер считываемой из файла строки
	ifstream fin("input.txt"); //попытка открытия файла
	if (!fin.is_open()) { //если файл не найден
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else {
		getline(fin, buf); //иначе считывание содержимого файла
	}
	//cout << "Text file content uploaded: " << buf << endl;
	fin.close(); //закрытие файлового потока

	//НАХОЖДЕНИЕ ВСЕХ УЗЛОВ БУДУЩЕГО ДЕРЕВА
	Node *allNodes = new Node[51]; //выделение памяти для всех узлов

	Node *startNodes = allocateMemory(); //выделение памяти для начальных 26 узлов
	for (int counter = 0; counter < 26; counter++) //поместить начальные узлы в список всех узлов
		allNodes[counter] = startNodes[counter];

	Node* sums = new Node[25]; //выделение памяти для остальных узлов
	sums = searchAllNodes(startNodes); //нахождение остальных узлов
	for (int counter = 26; counter < 51; counter++) //поместить остальные узлы в список всех узлов
		allNodes[counter] = sums[counter-26];
	
	bubbleSorting(51, allNodes); //сортировка всех узлов по убыванию вероятностей

	for (int counter = 1; counter < 51; counter++) { //присваивание всем узлам направления. 1 - лево, 0 - право
		if(counter %2 == 0)
			allNodes[counter].code = "1";
		else allNodes[counter].code = "0";
	}

	//ПОСТРОЕНИЕ ДЕРЕВА
	Node *root = nullptr; //выделение памяти для дерева
	for(int counter = 0; counter < 51; counter++) //построение дерева из 51 узла
		addNode(root, allNodes[counter]);

	treePrint(root, 0); //вывод дерева в консоль

	int length[26]; //массив для хранения длин кодовых слов
	Node *leaf = new Node[26]; //выделение памяти для закодированных узлов
	for (int counter = 0; counter < 26; counter++) { //построение итоговых кодов для 26 листов дерева
		leaf[counter] = *codeBuilding(root, A[counter]);
		length[counter] = leaf[counter].code.size();
		cout << A[counter] << " : " << leaf[counter].code << " | Frequensy: " << leaf[counter].freq << endl;
		//cout << "Size: " << arr[counter] << endl; //длина кодового слова
	}
	cout << endl << "Entropy: " << entropy(P) << endl; //энтропия
	cout << "Average codeword length: " << averageCodewordLength(length) << endl << endl; //средняя длина кодового слова

	//ЗАПИСЬ ЗАКОДИРОВАННОГО СООБЩЕНИЯ В ФАЙЛ
	string codes; //буфер записываемых в файл кодов
	ofstream fout("output.txt"); //попытка открытия файла
	if (!fout.is_open()) { //если файл не найден
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else { //иначе запись итоговых кодов в файл
		const char *str = buf.c_str();
		for (size_t first = 0; first < buf.length(); first++) {
			for (int second = 0; second < 26; second++) {
				if (str[first] == A[second]) {
					fout << leaf[second].code;
					codes += leaf[second].code; //записываемые в файл коды помещается в строку
				}
			}
		}
	}
	fout.close(); //закрытие файлового потока

	//ЗАПИСЬ ЗАКОДИРОВАННОГО СООБЩЕНИЯ В ФАЙЛ В БАЙТОВОМ ВИДЕ
	while (1) { //добавление дополнительных нулей к строке кодов для записи в байтовом виде
		codes += "0";
		if (codes.size() % 8 == 0) break;
	}
	//cout << endl << codes.size() << endl;

	ofstream fbout("binary.txt"); //попытка открытия файла
	if (!fbout.is_open()) { //если файл не найден
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	size_t counterr = 0;
	while (counterr < codes.size()) { //запись итоговых кодов в файл в байтовом виде
		bool bits[8];
		for (int i = 0; i < 8; i++) {
			if (codes[i + counterr] == '1')
				bits[i] = 1;
			if (codes[i + counterr] == '0')
				bits[i] = 0;
			//cout << bits[i];
		}
		byte b = pack_byte(bits); //вызов функции упаковывания байтов
		fbout << b;
		//cout << endl;
		counterr += 8;
	}
	fbout.close();

	//ОСВОБОЖДЕНИЕ ВЫДЕЛЕННОЙ ПАМЯТИ
	freeMemory(startNodes); //освобождение выделенной памяти для начальных 26 узлов
	delete[] sums; //освобождение памяти выделенной памяти для остальных узлов
	delete[] allNodes; //освобождение выделенной памяти для всех узлов
	freeTree(root); //освобождение выделенной памяти для дерева
	delete[] leaf; //освобождение выделенной памяти для закодированных узлов

	system("pause");
	return EXIT_SUCCESS;
}

//ФУНКЦИЯ УПАКОВЫВАНИЯ БАЙТОВ
byte pack_byte(bool bits[8]) {
	byte result(0);
	for (unsigned i(8); i--;) {
		result <<= 1;
		result |= byte(bits[i]);
	}
	return result;
}

//ФУНКЦИЯ ПОДСЧЕТА СРЕДНЕЙ ДЛИНЫ КОДОВОГО СЛОВА
float averageCodewordLength(int length[]) {
	float L = 0.0f;
	for (int count = 0; count < 26; count++)
		L += P[count] * length[count];
	return L;
}

//ФУНКЦИЯ ПОДСЧЕТА ЭНТРОПИИ
double entropy(float P[]) {
	double H = 0.0;
	for (int count = 0; count < 26; count++)
		H += -P[count] * log(P[count]) / log(2);
	return H;
}