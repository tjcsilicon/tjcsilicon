#pragma once
#include <iostream>
#include <string.h>
#include <bitset>
#include "exceptions.h"
#include <sstream>
using namespace std;
using std::string;

class huffTree
{
private:
	class huffNode
	{
	public:
		char character;
		int occur = 0;
		huffNode* parent;
		huffNode* left;
		huffNode* right;
		huffNode* next;
		huffNode* prev;
	};
	huffNode* root;
	huffNode* zeroNode;
	string alphabet;
	string seenalphabet;
	huffNode* nodes[256] = {};
public:
	huffTree(string alph)
	{
		alphabet = alph;
		zeroNode = root;
		root = nullptr;
	}
	string encode(string);
	void newChar(char);
	void siblingCheck(huffNode*);
	string decode(string);
};

string huffTree::encode(string msg)
{
	huffNode* temp = root;
	string encoded;
	huffNode* toRoot;
	string tempstring = "";
	if (temp == zeroNode)
	{
		huffNode* newRoot = new huffNode;
		huffNode* newNode = new huffNode;
		huffNode* newZero = new huffNode;
		newRoot->character = '\0';
		newNode->character = msg[0];
		seenalphabet = msg[0];
		newZero->character = '0';
		root = newRoot;
		root->left = newZero;
		root->right = newNode;
		newNode->parent = root;
		newZero->parent = root;
		zeroNode = newZero;
		root->next = newNode;
		newNode->next = newZero;
		newZero->prev = newNode;
		newNode->prev = root;
		newNode->occur++;
		root->occur++;
		nodes[msg[0]] = newNode;
		bitset<8> bin(msg[0]);
		encoded = encoded + bin.to_string();
	}
	temp = root;
	for (int i = 1; i < msg.size(); i++)
	{
		if (seenalphabet.find(msg[i]) == -1)
		{
			huffNode* toRoot = zeroNode;
			temp = zeroNode;
			tempstring = "";
			while (toRoot != root)
			{
				toRoot = toRoot->parent;
				toRoot->occur++;
				if (toRoot->left == temp)
				{
					tempstring = '0' + tempstring;
				}
				else
				{
					tempstring = '1' + tempstring;
				}
				temp = toRoot;
			}
			encoded = encoded + tempstring;
			bitset<8> bin(msg[i]);
			encoded = encoded + bin.to_string();
			newChar(msg[i]);
		}
		else
		{
			temp = nodes[msg[i]];
			temp->occur++;
			toRoot = temp;
			tempstring = "";
			while (toRoot != root)
			{
				toRoot = toRoot->parent;
				toRoot->occur = toRoot->left->occur + toRoot->right->occur;
				if (toRoot->left == temp)
				{
					tempstring = '0' + tempstring;
				}
				else
				{
					tempstring = '1' + tempstring;
				}
				temp = toRoot;
			}
			encoded = encoded + tempstring;
			siblingCheck(nodes[msg[i]]);
		}
	}
	return encoded;
}

void huffTree::newChar(char n)
{
	huffNode* temp = zeroNode;
	huffNode* newNode = new huffNode;
	huffNode* newZero = new huffNode;
	newNode->character = n;
	seenalphabet = seenalphabet + n;
	newZero->character = '0';
	temp->character = '\0';
	temp->left = newZero;
	temp->right = newNode;
	newNode->parent = temp;
	newZero->parent = temp;
	zeroNode = newZero;
	temp->next = newNode;
	newNode->next = newZero;
	newZero->prev = newNode;
	newNode->prev = temp;
	newNode->occur++;
	temp->occur++;
	nodes[n] = newNode;
	siblingCheck(newNode);
	return;
}

void huffTree::siblingCheck(huffNode* n)
{
	int noccur = n->occur;
	huffNode* temp = n->prev;
	huffNode* node = n;
	if(n == root)
	{
		return;
	}
	while (temp != root)
	{
		if (noccur > temp->occur)
		{
			if (temp->parent->left == temp)
			{
				temp->parent->left = node;
			}
			else
			{
				temp->parent->right = node;
			}
			if (n->parent->left == node)
			{
				n->parent->left = temp;
			}
			else
			{
				n->parent->right = temp;
			}
			node = n;
			huffNode* nprev = node->prev;
			huffNode* nnext = node->next;
			huffNode* tempprev = temp->prev;
			huffNode* tempnext = temp->next;
			huffNode* tempparent = temp->parent;
			huffNode* nparent = node->parent;
			if (n->prev == temp)
			{
				temp->prev = node;
				temp->next = nnext;
				node->next = temp;
				node->prev = tempprev;
				tempprev->next = node;
				nnext->prev = temp;
			}
			else
			{
				temp->parent--;
				tempprev->next = node;
				temp->next = nnext;
				temp->prev = nprev;
				tempnext->prev = node;
				temp->parent = nparent;
				nprev->next = temp;
				nnext->prev = temp;
				node->next = tempnext;
				node->prev = tempprev;
				node->parent = tempparent;

			}
			return;
		}
		temp = temp->prev;
	}
	siblingCheck(n->prev);
	return;
}

string huffTree::decode(string msg)
{
	huffNode* temp = root;
	string decoded;
	huffNode* toRoot;
	string tempstring = "";
	char tempchar;
	long templong;
	string message = msg;
	if (temp == zeroNode)
	{
		for (int i = 0; i < 8; i++)
		{
			tempstring = tempstring + message[0];
			message.erase(0, 1);
		}
		bitset<8> bin(tempstring);
		templong = bin.to_ulong();
		decoded = decoded + (char)templong;
		huffNode* newRoot = new huffNode;
		huffNode* newNode = new huffNode;
		huffNode* newZero = new huffNode;
		newRoot->character = '\0';
		newNode->character = (char)templong;
		seenalphabet = (char)templong;
		newZero->character = '0';
		root = newRoot;
		root->left = newZero;
		root->right = newNode;
		newNode->parent = root;
		newZero->parent = root;
		zeroNode = newZero;
		root->next = newNode;
		newNode->next = newZero;
		newZero->prev = newNode;
		newNode->prev = root;
		newNode->occur++;
		root->occur++;
		nodes[templong] = newNode;
	}
	while (message.size() > 0)
	{
		int i = 0;
		tempstring = "";
		temp = root;
		while (1)
		{
			tempchar = message[i];
			if (seenalphabet.find(temp->character) != -1)
			{
				for (int i = 0; i < tempstring.size(); i++)
				{
					message.erase(0, 1);
				}
				decoded = decoded + temp->character;
				temp->occur++;
				siblingCheck(temp);
				break;
			}
			if(temp->character != '0')
			{
				tempstring = tempstring + tempchar;
			}
			if (tempchar == '0')
			{
				if (temp->left)
				{
					temp->occur = temp->left->occur + temp->right->occur + 1;
					temp = temp->left;
				}
				else
				{
					for (int i = 0; i < tempstring.size(); i++)
					{
						message.erase(0, 1);
					}
					tempstring = "";
					for (int i = 0; i < 8; i++)
					{
						tempstring = tempstring + message[0];
						message.erase(0, 1);
					}
					bitset<8> bin(tempstring);
					templong = bin.to_ulong();
					decoded = decoded + (char)templong;
					newChar((char)templong);
					siblingCheck(temp);
					break;
				}
			}
			if (tempchar == '1')
			{
				if (temp->right)
				{
					temp->occur = temp->left->occur + temp->right->occur + 1;
					temp = temp->right;
				}
				else
				{
					for (int i = 0; i < tempstring.size(); i++)
					{
						message.erase(0, 1);
					}
					tempstring = "";
					for (int i = 0; i < 8; i++)
					{
						tempstring = tempstring + message[0];
						message.erase(0, 1);
					}
					bitset<8> bin(tempstring);
					templong = bin.to_ulong();
					decoded = decoded + (char)templong;
					newChar((char)templong);
					siblingCheck(temp);
					break;
				}
			}
			i++;
		}
	}
	try
	{
		for (int i = 0; i < decoded.size(); i++)
		{
			if (alphabet.find(decoded[i]) == -1)
			{
				stringstream sstream;
				sstream << "MESSAGE CONTAINS LETTER NOT IN ALPHABET";
				string msg;
				getline(sstream, msg);
				throw InvalidOp(msg);
			}
		}
	}
	catch (InvalidOp a)
	{
		cout << a.getMessage() << endl;
	}
	return decoded;
}