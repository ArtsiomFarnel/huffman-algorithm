#pragma once
#include <string>

using namespace std;

//���������� ����������
extern float P[26]; //�������� ����������� ��������� ���� � ������
extern char A[26]; //������� ��������������� �� �������� ����������� ��������� �����

struct Node { //��������� ���� ������
	string symbols, code;
	float freq;
	Node *left, *right, *parent;
};

//������� ��� ���������� ���� ����� �������� ������
Node *allocateMemory(); //��������� ������ ��� ������ ��������� �����
void freeMemory(Node *node); //������������ ���������� ������ ��� �����
void bubbleSorting(int size, Node *node); //����������� ���������� ����� �� �������� ������������ ��������� ��������
Node *searchAllNodes(Node *node); //���������� ���� ������������ �� ��������� �������� � ����� ���� �����

//������� ��� ���������� ������ �� ������ ���� ����� ��������� �����
Node *getFreeNode(float freq, string code, string symbols, Node *parent); //��������� ������ ��� ������ ���������� ����
void addNode(Node *&tree, Node node); //���������� ���� � ������
Node *codeBuilding(Node *root, char arr); //���������� �������� ����� ��� ������� ����
void treePrint(Node *tree, int level); //����� ������ � �������
void freeTree(Node *tree); //������������ ���������� ������ ��� ������
