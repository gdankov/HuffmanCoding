#include "HuffmanCode.h"

void getInput();
void encode();
void decode();

int main() {
	getInput();
	system("pause");
	return 0;
}


void getInput() {

	string input = "";
	cout << ".... Huffman Encoding ...." << endl;
	cout <<  endl << endl;
	do {
		cout << "Type 'e' to encode, type 'd' to decode" << endl;
		cin >> input;
	} while (input != "e" && input != "d" && input != "E" && input != "D");

	if (input == "e" || input  == "E") {
		encode();
	}
	else {
		decode();
	}
}

void encode() {
	string normalFilePath = "";
	string encodedFilePath = "";

	cout << "Type in a location to text file to encode... ";
	cin >> normalFilePath;
	cout << "Type in a location to save encoded file... ";
	cin >> encodedFilePath;

	HuffmanCode hfCode;
	try {
		hfCode.encode(normalFilePath, encodedFilePath);
	}
	catch (invalid_argument& e) {
		cerr << e.what() << endl;
	}
}

void decode() {
	string encodedFilePath = "";
	string decodedFilePath = "";

	cout << "Type in a location to encoded text file... ";
	cin >> encodedFilePath;

	cout << "Type in a location to save decoded file... ";
	cin >> decodedFilePath;

	HuffmanCode hfCode;
	try {
		hfCode.decode(encodedFilePath, decodedFilePath);
	}
	catch (invalid_argument& e) {
		cerr << e.what() << endl;
	}
}
