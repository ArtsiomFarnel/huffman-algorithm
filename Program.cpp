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
	//���������� ��������� �� �����
	string buf; //����� ����������� �� ����� ������
	ifstream fin("input.txt"); //������� �������� �����
	if (!fin.is_open()) { //���� ���� �� ������
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else {
		getline(fin, buf); //����� ���������� ����������� �����
	}
	//cout << "Text file content uploaded: " << buf << endl;
	fin.close(); //�������� ��������� ������

	//���������� ���� ����� �������� ������
	Node *allNodes = new Node[51]; //��������� ������ ��� ���� �����

	Node *startNodes = allocateMemory(); //��������� ������ ��� ��������� 26 �����
	for (int counter = 0; counter < 26; counter++) //��������� ��������� ���� � ������ ���� �����
		allNodes[counter] = startNodes[counter];

	Node* sums = new Node[25]; //��������� ������ ��� ��������� �����
	sums = searchAllNodes(startNodes); //���������� ��������� �����
	for (int counter = 26; counter < 51; counter++) //��������� ��������� ���� � ������ ���� �����
		allNodes[counter] = sums[counter-26];
	
	bubbleSorting(51, allNodes); //���������� ���� ����� �� �������� ������������

	for (int counter = 1; counter < 51; counter++) { //������������ ���� ����� �����������. 1 - ����, 0 - �����
		if(counter %2 == 0)
			allNodes[counter].code = "1";
		else allNodes[counter].code = "0";
	}

	//���������� ������
	Node *root = nullptr; //��������� ������ ��� ������
	for(int counter = 0; counter < 51; counter++) //���������� ������ �� 51 ����
		addNode(root, allNodes[counter]);

	treePrint(root, 0); //����� ������ � �������

	int length[26]; //������ ��� �������� ���� ������� ����
	Node *leaf = new Node[26]; //��������� ������ ��� �������������� �����
	for (int counter = 0; counter < 26; counter++) { //���������� �������� ����� ��� 26 ������ ������
		leaf[counter] = *codeBuilding(root, A[counter]);
		length[counter] = leaf[counter].code.size();
		cout << A[counter] << " : " << leaf[counter].code << " | Frequensy: " << leaf[counter].freq << endl;
		//cout << "Size: " << arr[counter] << endl; //����� �������� �����
	}
	cout << endl << "Entropy: " << entropy(P) << endl; //��������
	cout << "Average codeword length: " << averageCodewordLength(length) << endl << endl; //������� ����� �������� �����

	//������ ��������������� ��������� � ����
	string codes; //����� ������������ � ���� �����
	ofstream fout("output.txt"); //������� �������� �����
	if (!fout.is_open()) { //���� ���� �� ������
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	else { //����� ������ �������� ����� � ����
		const char *str = buf.c_str();
		for (size_t first = 0; first < buf.length(); first++) {
			for (int second = 0; second < 26; second++) {
				if (str[first] == A[second]) {
					fout << leaf[second].code;
					codes += leaf[second].code; //������������ � ���� ���� ���������� � ������
				}
			}
		}
	}
	fout.close(); //�������� ��������� ������

	//������ ��������������� ��������� � ���� � �������� ����
	while (1) { //���������� �������������� ����� � ������ ����� ��� ������ � �������� ����
		codes += "0";
		if (codes.size() % 8 == 0) break;
	}
	//cout << endl << codes.size() << endl;

	ofstream fbout("binary.txt"); //������� �������� �����
	if (!fbout.is_open()) { //���� ���� �� ������
		cout << "File opening error: " << strerror(errno) << endl;
		system("pause");
		return EXIT_FAILURE;
	}
	size_t counterr = 0;
	while (counterr < codes.size()) { //������ �������� ����� � ���� � �������� ����
		bool bits[8];
		for (int i = 0; i < 8; i++) {
			if (codes[i + counterr] == '1')
				bits[i] = 1;
			if (codes[i + counterr] == '0')
				bits[i] = 0;
			//cout << bits[i];
		}
		byte b = pack_byte(bits); //����� ������� ������������ ������
		fbout << b;
		//cout << endl;
		counterr += 8;
	}
	fbout.close();

	//������������ ���������� ������
	freeMemory(startNodes); //������������ ���������� ������ ��� ��������� 26 �����
	delete[] sums; //������������ ������ ���������� ������ ��� ��������� �����
	delete[] allNodes; //������������ ���������� ������ ��� ���� �����
	freeTree(root); //������������ ���������� ������ ��� ������
	delete[] leaf; //������������ ���������� ������ ��� �������������� �����

	system("pause");
	return EXIT_SUCCESS;
}

//������� ������������ ������
byte pack_byte(bool bits[8]) {
	byte result(0);
	for (unsigned i(8); i--;) {
		result <<= 1;
		result |= byte(bits[i]);
	}
	return result;
}

//������� �������� ������� ����� �������� �����
float averageCodewordLength(int length[]) {
	float L = 0.0f;
	for (int count = 0; count < 26; count++)
		L += P[count] * length[count];
	return L;
}

//������� �������� ��������
double entropy(float P[]) {
	double H = 0.0;
	for (int count = 0; count < 26; count++)
		H += -P[count] * log(P[count]) / log(2);
	return H;
}