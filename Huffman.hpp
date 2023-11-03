#pragma once
#include <string>

using namespace std;

//ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
extern float P[26]; //убывание вероятности появления букв в тексте
extern char A[26]; //алфавит отсортированный по убыванию вероятности появления буквы

struct Node { //структура узла дерева
	string symbols, code;
	float freq;
	Node *left, *right, *parent;
};

//ФУНКЦИИ ДЛЯ НАХОЖДЕНИЯ ВСЕХ УЗЛОВ БУДУЩЕГО ДЕРЕВА
Node *allocateMemory(); //выделение памяти для первых начальных узлов
void freeMemory(Node *node); //освобождение выделенной памяти для узлов
void bubbleSorting(int size, Node *node); //пузырьковая сортировка узлов по убыванию вероятностей появления символов
Node *searchAllNodes(Node *node); //нахождение сумм вероятностей по алгоритму Хаффмана и поиск всех узлов

//ФУНКЦИИ ДЛЯ ПОСТРОЕНИЯ ДЕРЕВА НА ОСНОВЕ ВСЕХ РАНЕЕ НАЙДЕННЫХ УЗЛОВ
Node *getFreeNode(float freq, string code, string symbols, Node *parent); //выделение памяти для нового временного узла
void addNode(Node *&tree, Node node); //добавление узла в дерево
Node *codeBuilding(Node *root, char arr); //построение итоговых кодов для каждого узла
void treePrint(Node *tree, int level); //вывод дерева в консоль
void freeTree(Node *tree); //освобождение выделенной памяти для дерева
