#include "huffman_tree.h"
#include <fstream>
#include <sstream>
#include <vector>

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){
    string fileContents = "";
    //make string from file
    fileContents = file_as_string(file_name);
    vector<char> chars;
    //make vector of chars from string
    for (int i = 0; i < fileContents.length(); i++) {
        chars.push_back(fileContents.at(i));
    }
    //create map of chars and respective frequencies
	for (int i = 0; i < chars.size(); i++) {
		char c = chars[i];
		if (frequencyMap.find(c) == frequencyMap.end())
			frequencyMap.insert(pair<char, int>(c,1));
		else
			frequencyMap[c]++;
	}
    //create priority queue from frequency map
	priority_queue<huffNode*, vector<huffNode*>, compare> huffTree;
	for (auto it = frequencyMap.begin(); it != frequencyMap.end(); it++) {
		huffTree.push(new huffNode(it->first, it->second));
	}
	huffNode* lc = nullptr;
	huffNode* rc = nullptr;
	//build huffman tree from priority queue
	while (huffTree.size() != 1) {

		lc = huffTree.top();
		huffTree.pop();

		rc = huffTree.top();
		huffTree.pop();

		huffNode* hn = nullptr;
		hn = new huffNode('\0', lc->frequency + rc->frequency);
		hn->left = lc;
		hn->right = rc;
		huffTree.push(hn);

	}
	//create map of characters and respective huffman codes
	//if there is only one character, huffman map will have just the character and "0" for code
	string hc = "";
    if (frequencyMap.size() > 1)
        determineHuffmanMap(huffTree.top(), hc);
    else
        huffmanMap.insert(pair<char, string>(huffTree.top()->character, "0"));
}
void huffman_tree::determineHuffmanMap(huffNode* root, string charCode) {
	if (root->left == nullptr) {
		root->setCharCode(charCode);
		huffmanMap.insert(pair<char, string>(root->getChar(), charCode));
	}
	else {
		determineHuffmanMap(root->left, charCode + "0");
		determineHuffmanMap(root->right, charCode + "1");
	}
}

huffman_tree::~huffman_tree(){

}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
	string cc = "";
	auto it1 = huffmanMap.begin();
	for (it1; it1 != huffmanMap.end(); it1++) {
		if (it1->first == character)
			cc = it1->second;
	}
	return cc;
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
    string encoded = "";
    string fileContents = "";
    //create string from file
    fileContents = file_as_string(file_name);
    vector<char> chars1;
    //make vector of chars from string
    for (int i = 0; i < fileContents.length(); i++) {
        chars1.push_back(fileContents.at(i));
    }
    //create map of every character and its frequency
    map<char, int> textMap;
    for (int i = 0; i < chars1.size(); i++) {
        char c1 = chars1[i];
        if (textMap.find(c1) == textMap.end())
            textMap.insert(pair<char, int>(c1,1));
        else
            textMap[c1]++;
    }

	bool match = false;
	auto it3 = textMap.begin();
	auto it2 = huffmanMap.begin();
	int count = 0;
	//if there is a character in the text that is not in the huffman map, it is invalid
	for (it3; it3 != textMap.end(); it3++) {
		for (it2; it2 != huffmanMap.end(); it2++) {
			if (it3->first == it2->first)
				break;
		}
		if (it3->first != it2->first)
			break;
		it2 = huffmanMap.begin();
		count++;
	}
	if (count == textMap.size())
	{
		match = true;
	}
	//make vector of code strings that correspond to the character
	vector<string> encode;
	auto iter = huffmanMap.begin();
	if (match) {
		for (int i = 0; i < chars1.size(); i++) {
			for (iter = huffmanMap.begin(); iter != huffmanMap.end(); iter++) {
				string s;
				if (chars1[i] == iter->first) {
					s = iter->second;
					encode.push_back(s);
					break;
				}
			}
		}
	}
	//turn vector into one string
	for (int i = 0; i < encode.size(); i++) {
		encoded = encoded + encode[i];
	}
	return encoded;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
	string decoded = "";
	stringstream ss("");
	//if code is not made of only 1s and 0s, it is not valid
	bool valid = true;
	for (int i = 0; i < string_to_decode.length(); i++) {
		if ((string_to_decode.at(i) != '1') && (string_to_decode.at(i) != '0')) {
			valid = false;
			break;
		}
	}
	int prev = 0;
	int endNum = 1;
	string temp = "";
	//if a block of the encoded string matches code in the huffman table, add the corresponding char to the string
	if (valid) {
		for (int i = 0; i < string_to_decode.length(); i++) {
			temp = string_to_decode.substr(prev, endNum);
			bool found = false;
			for (auto it4 = huffmanMap.begin(); it4 != huffmanMap.end(); it4++) {
				if (temp == it4->second) {
					string s(1, it4->first);
					decoded = decoded + s;
					prev = prev + endNum;
					endNum = 1;
					found = true;
				}
			}
			if (!found)
				endNum++;
		}
	}
	return decoded;
}
