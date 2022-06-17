#include "stdafx.h"
#include "HuffmanTree.h"
#include <string>
#include <vector>
#include <fstream> 
#include <iostream>
#include <queue>    
#include <algorithm>
#include <iomanip> 
#include <bitset>
#include <unordered_map>
#include <sstream>
#include <ios>
using namespace std;

inline bool HuffmanTree::getBit(unsigned char byte, int position) const
{
	return (byte & BITMASK[position]);
}

// Usage
// mybyte = setBit(mybyte,4);
// This sets the bit in position 4
inline unsigned char HuffmanTree::setBit(unsigned char byte, int position) const
{
	return  byte | BITMASK[position];
}

void HuffmanTree::printBits(char binary, std::ostream& out) const
{
	for (size_t i = 0; i < sizeof(char) * 8; i++) {
		if (getBit(binary, i)) {
			out << 1;
		}
		else {
			out << 0;
		}
	}
}

void HuffmanTree::printBinary(vector<char> bytes, std::ostream& out) const
{
	for (char byte : bytes)
	{
		printBits(byte, out);
		out << "-";
	}

}

string HuffmanTree::getCode(char letter) const
{
	string code = "";
	unordered_map<char, string> codeLookup2;
	codeLookup2 = codeLookup;
	return codeLookup2[letter];
}

void HuffmanTree::makeEmpty(BinaryNode*& t) {
	if (t->left !=nullptr) {
		makeEmpty(t->left);
	}
	
	if (t->right != nullptr) {
		makeEmpty(t->right);
	}
	delete t;
}

void HuffmanTree::printTree(BinaryNode* node, std::ostream& out) const
{
	if (node->left != nullptr) {
		printTree(node->left, out);
	}
	if (node->element.size() == 1) {
		out << node->element << " appeared: " << node->frequency << " times" << endl;
	}
	if (node->right != nullptr) {
		printTree(node->right, out);
	}
}

void HuffmanTree::printCodes(BinaryNode* node, std::ostream& out, string code) const
{
	if (node->left != nullptr) {
		printCodes(node->left, out, code + "0");
	}
	if (node->element.size() == 1) {
		out << node->element << " frequency: " << node->frequency << " Code: " << code << endl;
	}
	if (node->right != nullptr) {
		printCodes(node->right, out, code + "1");
	}
}

void HuffmanTree::saveTree(BinaryNode* current, string code)
{
	if (current == nullptr) {
		return;
	}
	if (current->left != nullptr) {
		saveTree(current->left, code + "0");
	}
	if (current->element.size() == 1) {
		char c = current->element.at(0);
		codeLookup[c] = code;
	}
	if (current->right != nullptr) {
		saveTree(current->right, code + "1");
	}

	
}
// writes tree information to file so the tree can be rebuilt when unzipping
void HuffmanTree::saveTree(std::ostream& compressedFileStream)
{
	saveTree(root, string());
}

void HuffmanTree::rebuildTree(BinaryNode* node, string element, string codedRoute)
{
	node = root;
	for (int i = 0; i < codedRoute.size(); i++) {
		if (codedRoute.at(i) == 0) {
			if (node->left == nullptr) {
				//create a new node
				node->left = new BinaryNode(element, 0, nullptr, nullptr);
				node = node->left;
			}
			else {
				node = node->left;
			}
		}
		else if (codedRoute.at(i) == 1) {
			if (node->right == nullptr) {
				node->right = new BinaryNode(element, 0, nullptr, nullptr);
				node = node->right;
			}
			else {
				node = node->right;
			}
		}
	}
	node->element = element;
}

void HuffmanTree::rebuildTree(ifstream& compressedFile) {
	// read info from file
	// use info to build tree
	string currKey;
	string addElement;
	int startPoint = 0;
	string code;
	string fullCode;
	bool addNode = false;

	ifstream keyFile("key.txt");
	while (!keyFile.eof()) {
		startPoint = 0;
		addNode = true;
		getline(keyFile,currKey);	//issue is here with get line, it breaks from end of line character
		addElement = currKey.at(0);
		
		if (currKey.find("Code: ")) {
			startPoint = currKey.find("Code: ") + 6;
		}
		
		while (startPoint < currKey.size()) {
			if (startPoint != 0) {
				code = currKey.at(startPoint);
				fullCode.append(code);
				startPoint++;
			}
			else {
				addNode = false;
			}
		}
		if (addNode == true) {
			rebuildTree(new BinaryNode(addElement, 0, nullptr, nullptr), addElement, fullCode);
		}
	}

	
	
}

HuffmanTree::BinaryNode* HuffmanTree::buildTree(string frequencyText) {
	priority_queue<HuffmanTree::BinaryNode*, vector<HuffmanTree::BinaryNode*>, compareBinaryNodes > nodes;
	int arr[128] = { 0 };
	char c;
	int end = frequencyText.size();
	for (int i = 0; i < end; i++) {
		c = frequencyText.at(i);
		arr[c]++;
	}
	arr[EOFCharacter]++;

	for (int it = 0; it < 128; it++) {
		if (arr[it] != 0) {
			char ch = it;
			string nodeString;
			nodeString.push_back(ch);

			BinaryNode* node = new BinaryNode(nodeString, arr[it], nullptr, nullptr);
			nodes.push(node);
		}
	}

	while (nodes.size() > 1) {
		BinaryNode* a = nodes.top();
		nodes.pop();
		BinaryNode* b = nodes.top();
		nodes.pop();
		int totalFreq = a->frequency + b->frequency;
		string stringAppend = a->element + b->element;

		BinaryNode* parent = new BinaryNode(stringAppend, totalFreq, nullptr, nullptr);
		parent->left = a;
		parent->right = b;
		nodes.push(parent);
	}
	return nodes.top();
}

HuffmanTree::HuffmanTree(string frequencyText)
{
	root = buildTree(frequencyText);


	saveTree(root, string());   
}

HuffmanTree::HuffmanTree(ifstream& frequencyStream) {

	std::string frequencyText((std::istreambuf_iterator<char>(frequencyStream)), std::istreambuf_iterator<char>());
	frequencyStream.close();
	if (frequencyText.size() > 0) {
		root = buildTree(frequencyText);
	}
	saveTree(root, string());
}

HuffmanTree::~HuffmanTree()
{
	makeEmpty();
}

// print out the char and its encoding
void HuffmanTree::printCodes(std::ostream& out) const
{
	printCodes(root, out, "");
}

// prints out the char and frequency
void HuffmanTree::printTree(std::ostream& out) const
{
	printTree(root, out);
}

void HuffmanTree::makeEmpty()
{
	BinaryNode* search = root;
	makeEmpty(search);

}

string HuffmanTree::decode(vector<char> encodedBytes) {
	string decoded;
	bool output = 0;
	vector<bool> BacktoBinary;
	BinaryNode* search;
	int padding = 0;
	int currentByte = 0;
	for (int i = 0; i < encodedBytes.size(); i++) {
		currentByte = encodedBytes.at(i);

		if (currentByte < 0) {
			currentByte = currentByte + 256;	//checks if your char is in the negative range
		}
		for (int it = 0; it < 8; it++) {
			
			output = (currentByte % 2) == 1;
			
			currentByte = currentByte / 2;
			BacktoBinary.push_back(output);
		}
	}
	for (int bitNumber = 0; bitNumber < BacktoBinary.size();) {
		search = root;

		while ((search->left != nullptr || search->right != nullptr) && bitNumber<BacktoBinary.size()) {
			if (BacktoBinary.at(bitNumber) == 0) {
				search = search->left;
			}
			else if (BacktoBinary.at(bitNumber) == 1) {
				search = search->right;
			}
			bitNumber++;
		}
		if (search->element.at(0) == EOFCharacter) {
			return decoded;
		}
			decoded.push_back(search->element.at(0));
	}

	return decoded;
}

vector<char> HuffmanTree::encode(string stringToEncode)
{
	stringToEncode.push_back(EOFCharacter); // needed when encoding message for file I/O
	
	vector<char> encoded;
	vector<bool> buffer;
	int output = 0;
	int padding = 0;

	for (int i = 0; i < stringToEncode.size(); i++) {
		string code = getCode(stringToEncode.at(i));
		for (int it = 0; it < code.size(); it++) {
			buffer.push_back(code.at(it) == '1');
		}
	}

	for (int itr = 0; itr < buffer.size(); ++itr) {
		if (buffer.at(itr)) {
			output = setBit(output,itr%8);
		}
		if (itr % 8 == 7) {					// -128 to 127 for a byte
			encoded.push_back(output);
			output = 0;
		}
	}

	encoded.push_back(output);
	return encoded;
}

void HuffmanTree::uncompressFile(string compressedFileName,
	string uncompressedToFileName) {
	string output;
	char currchar;
	ostringstream ss;
	ifstream keyFile("key.txt");
	//rebuildTree(keyFile);			//rebuilds the tree with the codes

	ifstream BinFile(compressedFileName, ios::in | ios::binary);
	
	ss << BinFile.rdbuf();
	const string& s = ss.str();
	vector<char> encodedText(s.begin(),s.end());
	
	output = this->decode(encodedText);

	ofstream myfile(uncompressedToFileName);
	myfile << output;
	
}

void HuffmanTree::compressFile(string compressToFileName,
	string uncompressedFileName, bool buildNewTree) {	

	string currWord;
	string FinalString;
	string outputString;
	
	ofstream keyFile("key.txt");
	ifstream frequencyStream(uncompressedFileName);
	while (!frequencyStream.eof()) {
		getline(frequencyStream,currWord);
		FinalString.append(currWord + "\n");
	}
	frequencyStream.close();
	vector<char> encodedText = this->encode(FinalString);
	ofstream myfile(compressToFileName, ios::in | ios::binary);

	printCodes(keyFile);	
	//For simplicity I am storing the key in a seperate file that could also be transfered with the original file

	myfile.write(reinterpret_cast<const char*>(&encodedText[0]), encodedText.size() * sizeof(char));
	myfile.close();
	// NOTE: when opening the compressedFile, you need to open in 
	//  binary mode for writing..hmmm..why is that?
}


