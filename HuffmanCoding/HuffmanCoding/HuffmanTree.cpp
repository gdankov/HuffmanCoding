#include "HuffmanTree.h"


priority_queue<Node*, vector<Node*>, Comparator> HuffmanTree::buildPQueue(unordered_map<char, int> lettersTable) {
	priority_queue<Node*, vector<Node*>, Comparator> priorityQueue;
	
	unordered_map<char, int> ::iterator mapIterator = lettersTable.begin();
	while (mapIterator != lettersTable.end()) {
		Node* temp = new Node(mapIterator->first, mapIterator->second, NULL, NULL);
		priorityQueue.push(temp);

		++mapIterator;
	}
	return priorityQueue;
}
	
void HuffmanTree::buildTree(unordered_map<char, int> lettersTable) {
	priority_queue<Node*, vector<Node*>, Comparator> priorityQueue = buildPQueue(lettersTable);

	while (priorityQueue.size() > 1) {
		Node* n1 = priorityQueue.top();
		priorityQueue.pop();
		Node* n2 = priorityQueue.top();
		priorityQueue.pop();

		Node* newNode = new Node(' ', n1->amount + n2->amount, n1, n2);
		priorityQueue.push(newNode);
	}
	root = priorityQueue.top();
	priorityQueue.pop();
}

unordered_map<char, vector<bool>> HuffmanTree::getEncoding() {
	unordered_map<char, vector<bool>> encodedMap;
	vector<bool> vec;
	traverseTreeEncode(root, encodedMap, vec);
	return encodedMap;
}

void HuffmanTree::traverseTreeEncode(Node* root, unordered_map<char, vector<bool>>& map, vector<bool>& vec) {
	if (root->left == NULL && root->right == NULL) {
		map[root->info] = vec;
	}
	else {
		if (root->left != NULL) {
			vec.push_back(0);
			traverseTreeEncode(root->left, map, vec);
			vec.pop_back();

		}
		if (root->right != NULL) {
			vec.push_back(1);
			traverseTreeEncode(root->right, map, vec);
			vec.pop_back();
		}
	}
}


int HuffmanTree::getAllCharsAmount() {
	return root->amount;
}

bool HuffmanTree :: findChar(string path, char& foundChar) {
	return traverseTreeDecode(root, path, 0, foundChar);
}


bool HuffmanTree::traverseTreeDecode(Node* currentNode, string toDecode, int currentIndex, char& foundChar) {
	Node* current = currentNode;
	for (size_t i = 0; i < toDecode.size(); i++) {
		if (toDecode[i] == '0') {
			current = current->left;
		}
		if (toDecode[i] == '1') {
			current = current->right;
		}
	}

	if (current->left == NULL && current->right == NULL) {
		foundChar = current->info;
		return true;
	}
	return false;
}