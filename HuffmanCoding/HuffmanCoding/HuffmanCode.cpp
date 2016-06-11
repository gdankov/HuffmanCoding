#include "HuffmanCode.h"

unordered_map<char, int> HuffmanCode::buildMap(string readFilePath) {
	unordered_map<char, int> lettersTable;

	ifstream readFile;
	readFile.open(readFilePath, ios::out);
	
	if (readFile.is_open()) {
		char letter;
		while (readFile.get(letter)) {
			if (lettersTable.count(letter) == 0) {
				lettersTable[letter] = 1;
			}
			else {
				int prevCount = lettersTable[letter];
				lettersTable[letter] = prevCount + 1;
			}
		}
	}
	else { 
		throw invalid_argument("Couldn't open file at... " + readFilePath);
	}
	readFile.close();

	return lettersTable;
}

void HuffmanCode :: printEncriptionInfo(string readFilePath, string writeFilePath) {
	cout << endl << endl;
	cout << "File is encoded at... " << writeFilePath << endl;

	ifstream originalFile(readFilePath, ios::binary | ios::ate);
	long originalSize = originalFile.tellg();
	cout << "Original file size: " << originalSize << " bytes."<< endl;
	originalFile.close();


	ifstream newFile(writeFilePath, ios::binary | ios::ate);
	long newSize = newFile.tellg();
	cout << "Compressed file size: " <<newSize <<" bytes." << endl;
	newFile.close();

	cout << "Compressed: " << originalSize - newSize << " bytes." << endl << endl;
}

void HuffmanCode :: printDecriptionInfo(string readFilePath, string writeFilePath) {
	cout << endl << endl;
	cout << "File is decoded at... " << writeFilePath << endl;

	ifstream originalFile(readFilePath, ios::binary | ios::ate);
	long originalSize = originalFile.tellg();
	cout << "Compressed file size: " << originalSize << " bytes." << endl;
	originalFile.close();


	ifstream newFile(writeFilePath, ios::binary | ios::ate);
	long newSize = newFile.tellg();
	cout << "Decompressed file size: " << newSize << " bytes." << endl;
	newFile.close();

	cout << "Decompressed: " << newSize - originalSize << " bytes." << endl << endl;
}

// ENCODE & DECODE HELPER FUNCTIONS

void HuffmanCode :: writeToFile(unsigned char bit, ofstream& outfile) {
	static int bitCurrentPosition = 0;
	static unsigned char byte;

	if (bit != 2) {
		if (bit == 1) {
			byte = byte | (bit << (7 - bitCurrentPosition));
		}
		else {
			byte = byte & static_cast<unsigned char>(255 - (1 << (7 - bitCurrentPosition)));
		}

		++bitCurrentPosition;
		bitCurrentPosition %= 8;
		
		if (bitCurrentPosition == 0) {
			outfile.put(byte);
			byte = '\0';
		}
	}
	else {
		outfile.put(byte);
	}
}


unsigned char HuffmanCode::readFromFile(ifstream& infile) {
	static int bitCurrentPosition = 0;
	static unsigned char byte = infile.get();

	unsigned char currentBit;

	currentBit = (byte >> (7 - bitCurrentPosition)) % 2;

	++bitCurrentPosition;
	bitCurrentPosition %= 8;

	if (bitCurrentPosition == 0) {
		if (!infile.eof()) {
			byte = infile.get();
		}
		else {
			currentBit = 2;
		}
	}

	return currentBit;
}

// encode & decode 


void HuffmanCode::encode(string readFilePath, string writeFilePath) {
	
	if (!ifstream(readFilePath))
	{
		throw invalid_argument("File " + readFilePath + " does not exist!");
	}

	if (ifstream(writeFilePath))
	{
		throw invalid_argument("File " + writeFilePath + " already exists!");
	}
	
	// build letter/frequency table
	unordered_map<char, int> lettersTable;
	try {
		lettersTable = buildMap(readFilePath);
	}
	catch (invalid_argument& e) {
		throw e;
	}

	// build huffman tree using the table
	HuffmanTree huffTree(lettersTable);
	// get every letter's new encoding
	unordered_map<char, vector<bool>> encoding = huffTree.getEncoding();

	ifstream readFile;
	readFile.open(readFilePath, ios::binary | ios::in);

	ofstream fileToWrite;
	fileToWrite.open(writeFilePath, ios::binary | ios::out);


	if (readFile.is_open() && fileToWrite.is_open()) {
		// encode letter/frequency table at the start of the file
		encodeHeader(lettersTable, fileToWrite);
		
		char currentLetter;
		while (readFile.get(currentLetter)) {
			
			char bitToWrite;
			for each (bool bit in encoding[currentLetter]) {
				if (bit) {
					bitToWrite = 1;
				}
				else {
					bitToWrite = 0;
				}
				writeToFile(bitToWrite, fileToWrite);
			}
		}
	}
	else {
		throw invalid_argument("Can't open one of the read/write files!");
	}

	//eof flag
	unsigned char eof = 2;
	writeToFile(eof, fileToWrite);

	//close streams
	fileToWrite.close();
	readFile.close();

	// print info
	printEncriptionInfo(readFilePath, writeFilePath);
}

void HuffmanCode::encodeHeader(unordered_map<char, int> lettersTable, ofstream& fileToWrite) {

	const int tableSize = lettersTable.size();
	// write map size as first element
	fileToWrite.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));

	for each (pair<char, int> pair in lettersTable) {
		fileToWrite.write(reinterpret_cast<const char *>(&pair.second), sizeof(pair.second));
		fileToWrite.write(reinterpret_cast<const char *>(&pair.first), sizeof(pair.first));
	}
}


void HuffmanCode :: decode(string readFilePath, string writeFilePath) {
	
	if (!ifstream(readFilePath))
	{
		throw invalid_argument("File " + readFilePath + " does not exist!");
	}

	if (ifstream(writeFilePath))
	{
		throw invalid_argument("File " + writeFilePath + " already exists!");
	}

	ifstream readFile;
	readFile.open(readFilePath, ios::binary | ios::in);
	
	ofstream writeToFile;
	writeToFile.open(writeFilePath, ios::out);

	if (!readFile.is_open()) {
		throw invalid_argument("Can not open file at... " + readFilePath);
	}

	if (!writeToFile.is_open()) {
		throw invalid_argument("Can not open file at... " + writeFilePath);
	}


	// decode header file and get letter/frequency table
	unordered_map<char, int> mapDecoder = decodeHeader(readFilePath, readFile);

	// rebuild the tree with the map
	HuffmanTree huffTree(mapDecoder);

	int allChars = huffTree.getAllCharsAmount();
	char found;
	string traversePath;

	while (allChars > 0) {
		traversePath = "";
		do {
			unsigned char currentBit = readFromFile(readFile);
			if (currentBit == 0) {
				traversePath += '0';
			}
			else if (currentBit == 1) {
				traversePath += '1';
			}
			
		} while (!huffTree.findChar(traversePath, found));
		writeToFile.put(found);

		--allChars;
	}

	readFile.close();
	writeToFile.close();

	printDecriptionInfo(readFilePath, writeFilePath);
}



unordered_map<char, int> HuffmanCode::decodeHeader(string readFilePath, ifstream& infile) {


	int mapSize = 0;
	infile.read((char*)&mapSize, sizeof(mapSize));

	unordered_map<char, int> restoredMap;
	int restoredCount = 0;
	char restoredLetter;

	for (size_t i = 0; i < mapSize; i++) {
		infile.read((char*)&restoredCount, sizeof(restoredCount));
		infile.read((char*)&restoredLetter, sizeof(restoredLetter));
		
		restoredMap[restoredLetter] = restoredCount;
	}

	return restoredMap;
}


