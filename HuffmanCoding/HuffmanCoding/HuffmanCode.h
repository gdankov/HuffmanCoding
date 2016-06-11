#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include  "HuffmanTree.h"
#include<fstream>
#include<iostream>

class HuffmanCode {

	unordered_map<char, int> buildMap(string);
	void buildTree();
	
	void writeToFile(unsigned char, ofstream&);
	unsigned char readFromFile(ifstream&);

	void encodeHeader(unordered_map<char, int>, ofstream&);
	unordered_map<char, int> decodeHeader(string, ifstream&);

	void printEncriptionInfo(string, string);
	void printDecriptionInfo(string, string);


public:

	HuffmanCode() { }

	void encode(string, string);
	void decode(string, string);
};

#endif