#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include<string>
#include<vector>
#include<queue>
#include<unordered_map>
using namespace std;

struct Node {
	char info;
	int amount;
	Node* left;
	Node* right;

	Node(char info, int amount, Node* left, Node* right) {
		this->info = info;
		this->amount = amount;
		this->left = left;
		this->right = right;
	}
	
	~Node() {
		delete left;
		delete right;
	}
};

class Comparator {
public:
	bool operator()(Node* n1, Node* n2) {
		if (n1->amount == n2->amount) {
			return n1->info > n2->info;
		}
		else {
			return n2->amount < n1->amount;
		}
	}
};

class HuffmanTree {

	Node* root;

	void buildTree(unordered_map<char, int> lettersTable);
	priority_queue<Node*, vector<Node*>, Comparator> buildPQueue(unordered_map<char, int> lettersTable);

	void traverseTreeEncode(Node* root, unordered_map<char, vector<bool>> &map, vector<bool> &vec);
	bool traverseTreeDecode(Node* currentNode, string toDecode, int currentIndex, char&);

public:

	HuffmanTree(unordered_map<char, int> lettersTable) : root(NULL) {
		buildTree(lettersTable);
	}
	~HuffmanTree() {
		delete root;
	}

	unordered_map<char, vector<bool>> getEncoding();
	int getAllCharsAmount();
	bool findChar(string path, char& );
};

#endif