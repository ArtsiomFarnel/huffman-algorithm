#include <iostream>
#include "Huffman.hpp"

using namespace std;

//óáûâàíèå âåðîÿòíîñòè ïîÿâëåíèÿ áóêâ â òåêñòå
float P[26] = {
		0.127f, 0.097f, 0.075f, 0.073f,
		0.068f, 0.067f, 0.067f, 0.064f,
		0.049f, 0.045f, 0.04f, 0.031f,
		0.03f, 0.027f, 0.024f, 0.024f,
		0.021f, 0.017f, 0.016f, 0.013f,
		0.008f, 0.008f, 0.005f, 0.002f,
		0.001f, 0.001f }; 

//àëôàâèò îòñîðòèðîâàííûé ïî óáûâàíèþ âåðîÿòíîñòè ïîÿâëåíèÿ áóêâû
char A[26] = {
		'e', 't', 'i', 'a',
		'o', 'n', 's', 'r',
		'h', 'c', 'l', 'd',
		'p', 'y', 'u', 'm',
		'f', 'b', 'g', 'w',
		'v', 'k', 'x', 'q',
		'z', 'j' }; 

//âûäåëåíèå ïàìÿòè äëÿ ïåðâûõ íà÷àëüíûõ óçëîâ
Node *allocateMemory() { 
	Node *node = new Node[26];
	for (int counter = 0; counter < 26; counter++) {
		node[counter].left = nullptr;
		node[counter].right = nullptr;
		node[counter].freq = P[counter];
		node[counter].symbols = A[counter];
		node[counter].code = "";
	}
	return node;
}

//îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ óçëîâ
void freeMemory(Node *node) {
	delete[] node;
}

//ïóçûðüêîâàÿ ñîðòèðîâêà óçëîâ ïî óáûâàíèþ âåðîÿòíîñòåé ïîÿâëåíèÿ ñèìâîëîâ
void bubbleSorting(int size, Node *node) {
	for (int first = 0; first < size - 1; first++) {
		for (int second = 0; second < size - first - 1; second++) {
			if (node[second].freq < node[second + 1].freq) {
				Node tmp = node[second];
				node[second] = node[second + 1];
				node[second + 1] = tmp;
			}
		}
	}
}

//íàõîæäåíèå ñóìì âåðîÿòíîñòåé ïî àëãîðèòìó Õàôôìàíà è ïîèñê âñåõ óçëîâ
Node * searchAllNodes(Node *node) {
	int start_node_counter = 26;
	Node *tmp = new Node[start_node_counter - 1];
	for (int counter = 0; counter < start_node_counter - 1; counter++) {
		tmp[counter].left = nullptr;
		tmp[counter].right = nullptr;
		tmp[counter].freq = 0.0f;
		tmp[counter].symbols = "";
		tmp[counter].code = "";
	}

	int tmp_count = 0;
	while (start_node_counter >= 2) {
		bubbleSorting(start_node_counter, node);
		for (int i = 0; i < start_node_counter; i++)
			cout << node[i].freq << " ";
		cout << endl;

		node[start_node_counter - 1].code = "1";
		cout << "First index: " << start_node_counter - 1 << "| Left: " << node[start_node_counter - 1].freq << " Code: " << node[start_node_counter - 1].code << endl;
		node[start_node_counter - 2].code = "0";
		cout << "Second index: " << start_node_counter - 2 << "| Right: " << node[start_node_counter - 2].freq << " Code: " << node[start_node_counter - 2].code << endl;
		if (start_node_counter == 2) {
			tmp[tmp_count].freq = 1;
			tmp[tmp_count].code = "";
			tmp[tmp_count].symbols = node[start_node_counter - 1].symbols + node[start_node_counter - 2].symbols;
			cout << "Sum = 1" << " Symbols = " << tmp[tmp_count].symbols << endl;
			break;
		}
		tmp[tmp_count].freq = node[start_node_counter - 1].freq + node[start_node_counter - 2].freq;
		tmp[tmp_count].symbols = node[start_node_counter - 1].symbols + node[start_node_counter - 2].symbols;
		node[start_node_counter - 2] = tmp[tmp_count];
		cout << "Sum = " << node[start_node_counter - 2].freq << " Symbols = " << tmp[tmp_count].symbols << endl << endl;
		tmp_count++;
		start_node_counter--;
		
	}

	return tmp;
}

//âûäåëåíèå ïàìÿòè äëÿ íîâîãî âðåìåííîãî óçëà
Node *getFreeNode(float freq, string code, string symbols, Node *parent) {
	Node *tmp = new Node;
	tmp->left = tmp->right = nullptr;
	tmp->freq = freq;
	tmp->code = code;
	tmp->symbols = symbols;
	tmp->parent;
	return tmp;
}

//äîáàâëåíèå óçëà â äåðåâî
void addNode(Node *&tree, Node node) {
	Node *tmp = nullptr;
	if (tree == nullptr) {
		tree = getFreeNode(node.freq, "", node.symbols, nullptr);
		return;
	}
	tmp = tree;
	while (tmp) {
		if (tmp->symbols.find(node.symbols) != std::string::npos) {
			if (tmp->right && tmp->right->symbols.find(node.symbols) != std::string::npos) {
				tmp = tmp->right;
				continue;
			}
			else if (!tmp->right) {
				if (node.code == "0")
					tmp->right = getFreeNode(node.freq, "0", node.symbols, tmp);
				else
					tmp->left = getFreeNode(node.freq, "1", node.symbols, tmp);
				return;
			}
			if (tmp->left && tmp->left->symbols.find(node.symbols) != std::string::npos) {
				tmp = tmp->left;
				continue;
			}
			else if (!tmp->left) {
				if (node.code == "1")
					tmp->left = getFreeNode(node.freq, "1", node.symbols, tmp);
				else
					tmp->right = getFreeNode(node.freq, "0", node.symbols, tmp);
				return;
			}
		}
	}
}

//ïîñòðîåíèå èòîãîâûõ êîäîâ äëÿ êàæäîãî óçëà
Node *codeBuilding(Node *root, char arr) {
	string str(1, arr);
	string makeCode = "";

	while (root) {
		if (root->symbols == str) {
			root->code = makeCode;
			return root;
		}
		if (root->right->symbols.find(str) != std::string::npos) {
			makeCode += root->right->code;
			root = root->right;
			continue;
		}
		else if (root->left->symbols.find(str) != std::string::npos) {
			makeCode += root->left->code;
			root = root->left;
			continue;
		}
	}

	return nullptr;
}

//âûâîä äåðåâà â êîíñîëü
int rec[10];
void treePrint(Node *tree, int level) {
	if (tree != nullptr) {
		for (int i = 0; i < level; i++)
			if (i == level - 1)
				printf("%s", rec[level - 1] ? "->" : "|->");
			else
				printf("%s   ", rec[i] ? "|" : "  ");
		printf(" %f\n", tree->freq);
		rec[level] = 1;
		treePrint(tree->left, level + 1);
		rec[level] = 0;
		treePrint(tree->right, level + 1);
	}
}

//îñâîáîæäåíèå âûäåëåííîé ïàìÿòè äëÿ äåðåâà
void freeTree(Node *tree) {
	if (tree != nullptr) {
		freeTree(tree->left);
		freeTree(tree->right);
		delete tree;
	}
}
