#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <fstream>
using namespace std;

class huffman_tree {
public:
	struct huffNode{
		int frequency;
		char character;
		huffNode *left;
		huffNode *right;
		string charCode;
		huffNode() {
			frequency = 0;
			character = ' ';
			left = nullptr;
			right = nullptr;
			charCode = "";
		}
		huffNode(char c, int f) {
			frequency = f;
			character = c;
			left = nullptr;
			right = nullptr;
			charCode = "";
		}
		huffNode(huffNode* lc, huffNode* rc) {
            character = ' ';
			this->left = lc;
			this->right = rc;
			frequency = left->frequency + right->frequency;
            charCode = "";
		}
		~huffNode() {
			delete left;
			delete right;
		}
		huffNode(const huffNode& hn) {
		    this->left = new huffNode;
		    *(this->left) = *(hn.left);
		    this->right = new huffNode;
		    *(this->right) = *(hn.right);
		    this->frequency = hn.frequency;
		    this->character = hn.character;
		    this->charCode = hn.charCode;
		}
		huffNode& operator=(const huffNode& hn) {
            this->left = new huffNode;
            *(this->left) = *(hn.left);
            this->right = new huffNode;
            *(this->right) = *(hn.right);
            this->frequency = hn.frequency;
            this->character = hn.character;
            this->charCode = hn.charCode;
            return *this;
		}
		bool operator <(const huffNode &hn) {
			return this->frequency < hn.frequency;
		}
		int getFreq() {
			return frequency;
		}
		char getChar() {
			return character;
		}
		string getCharCode() {
			return charCode;
		}
		huffNode* getLeft() {
			return left;
		}
		huffNode* getRight() {
			return right;
		}
		void setCharCode(string code) {
			charCode = code;
		}
	};
	struct compare
	{
		bool operator()(huffNode* l, huffNode* r)
		{
			return (l->frequency > r->frequency);
		}
	};
	map<char, int> frequencyMap;
	map<char, string> huffmanMap;
		huffman_tree(const std::string &file_name);
		~huffman_tree();
    string file_as_string(const std::string file) const {
        std::ifstream in(file);
        std::string string1((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());
        return string1;
    }
	void determineHuffmanMap(huffNode* root, string charCode);
		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	
};

#endif
