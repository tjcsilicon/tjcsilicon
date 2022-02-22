#pragma once
#include <vector>
#include <sstream>
#include <stdexcept>
#include "exceptions.h"
#include "Node.h"
using namespace std;

template <typename T>
class BTree
{
private:
	node<T>* root = new node<T>(compare);
	int(*compare)(T&, T&);
	int max;
	int size;
public:
	
	BTree<T>(int (*cmp)(T&, T&), int n) 
	{
		compare = cmp;
		root = nullptr;
		max = n;
	}
	BTree<T>(int (*cmp)(T&, T&))
	{
		compare = cmp;
		root = nullptr;
		max = 512 / sizeof(T);
	}
	~BTree<T>()
	{
		node<T>* temp = root;
		node<T>* del;
		while (!root->child.empty())
		{
			temp = root->child[0];
			while (!temp->child.empty())
			{
				temp = temp->child[0];
			}
			del = temp;
			temp = temp->parent;
			while (!temp->child.empty())
			{
				delete del;
				del = temp->child[0];
			}
		}
		delete temp;
	}
	void insert(T);
	void overflow(node<T>*);
	void remove(T);
	void underflow(node<T>*);
	T find(T);
	int count();
};

template <typename T>
void BTree<T>::insert(T item)
{
	node<T>* temp;
	temp = root;
	int test;
	if (!root)
	{
		node<T>* rootNode = new node<T>(compare);
		root = rootNode;
		root->insert(item);
		size++;
		return;
	}
	for (int i = 0; i < max; i++)
	{
		test = compare(item, temp->data[i]);
		if (test == -1)
		{
			if (temp->child.size() > i)
			{
				temp = temp->child[i];
				i = -1;
			}
			else if (temp->data.size() == max)
			{
				temp->insert(item);
				overflow(temp);
				size++;
				return;
			}
			else
			{
				temp->insert(item);
				size++;
				return;
			}
		}
		else if (test == 0)
		{
			stringstream sstream;
			sstream << "ITEM ALREADY EXISTS";
			string msg;
			getline(sstream, msg);
			throw InvalidOp(msg);
		}
		else
		{
			if (temp->child.size() > i + 1)
			{
				temp = temp->child[temp->child.size()-1];
				i = -1;
			}
			else if (temp->data.size() == max)
			{
				temp->insert(item);
				overflow(temp);
				size++;
				return;
			}
			else
			{
				temp->insert(item);
				size++;
				return;
			}
			
		}
	}
}

template <typename T>
void BTree<T>::overflow(node<T>* n)
{
	node<T>* temp = n;
	int mid;

	mid = max / 2;

	if (temp == root)
	{
		node<T>* newRoot = new node<T>(compare);
		node<T>* newChild = new node<T>(compare);
		for (int j = 0; j < mid; j++)
		{
			newChild->insert(temp->data[0]);
			if (!temp->child.empty())
			{
				newChild->child.insert(newChild->child.begin(), temp->child[0]);
				temp->child.erase(temp->child.begin());
			}
			temp->remove(temp->data[0]);
		}
		root = newRoot;
		newRoot->insert(temp->data[0]);
		temp->remove(temp->data[0]);
		if (!temp->child.empty())
		{
			newChild->child.insert(newChild->child.end(), temp->child[0]);
			temp->child.erase(temp->child.begin());
		}
		temp->size = temp->data.size();
		temp->parent = newRoot;
		newChild->parent = newRoot;
		newRoot->child.insert(newRoot->child.begin(), newChild);
		newRoot->child.insert(newRoot->child.begin() + 1, temp);
	}
	else
	{
		node<T>* newNode = new node<T>(compare);
		temp->parent->insert(temp->data[mid]);
		temp->remove(temp->data[mid]);
		for (int j = mid; j <= max - 1; j++)
		{
			newNode->insert(temp->data[mid]);
			temp->remove(temp->data[mid]);
		}
		temp->size = temp->data.size();
		temp = temp->parent;
		newNode->parent = temp;
		temp->child.insert(temp->child.end(), newNode);
	}
	while (temp->parent)
	{
		if (temp->data.size() >= max + 1)
		{
			overflow(temp);
		}
		else
		{
			temp = temp->parent;
		}
	}
	if (root->data.size() > max)
	{
		overflow(root);
	}
}

template <typename T>
void BTree<T>::remove(T item)
{
	node<T>* temp;
	temp = root;
	int test;
	int min = max / 2;
	for (int i = 0; i < max; i++)
	{
		test = compare(item, temp->data[i]);
		if (test == -1)
		{
			if (temp->child.size() >= i)
			{
				temp = temp->child[i];
				i = -1;
			}
		}
		else if (test == 0)
		{
			if (temp->child.empty()) //test if its a leaf node
			{
				temp->remove(item);
				size--;
				if (temp!= root && temp->data.size() < min)
				{
					underflow(temp);
				}
				return;
			}
			else
			{
				node<T>* imPred = temp->child[i];
				while (!imPred->child.empty())
				{
					imPred = imPred->child[imPred->child.size() - 1];
				}
				temp->data[i] = imPred->data[0];
				imPred->remove(imPred->data[0]);
				size--;
				if (imPred->data.size() < min)
				{
					underflow(imPred);
				}
				return;
			}
		}
		else
		{
			if (temp->child.size() >= i + 1 && i == temp->data.size() - 1)
			{
				temp = temp->child[i+1];
				i = -1;
			}
		}
	}
	stringstream sstream;
	sstream << "ITEM DOESN'T EXIST";
	string msg;
	getline(sstream, msg);
	throw InvalidOp(msg);
}

template<typename T>
void BTree<T>::underflow(node<T>* n)
{
	node<T>* temp = n;
	node<T>* parent = n->parent;
	node<T>* Rsibling = nullptr;
	node<T>* Lsibling = nullptr;
	int seperator;
	int tempindex;
	int min = max / 2;
	
	if (size == max)
	{
		while(!root->child.empty())
		{
			temp = root->child[0];
			while (!temp->data.empty())
			{
				root->insert(temp->data[0]);
				temp->remove(temp->data[0]);
			}
			root->child.erase(root->child.begin());
		}
		return;
	}
	for (int i = 0; i < parent->child.size(); i++) //find siblings
	{
		if (parent->child[i] == temp)
		{
			tempindex = i;
			if (i >= 1)
			{
				seperator = parent->data[i - 1];
				Lsibling = parent->child[i - 1];
			}
			if (i != parent->child.size() - 1)
			{
				seperator = parent->data[i];
				Rsibling = parent->child[i + 1];
			}
			break;
		}
	}
	if (Rsibling && Rsibling->data.size() > min)
	{
		temp->insert(seperator);
		parent->remove(seperator);
		parent->insert(Rsibling->data[0]);
		Rsibling->remove(Rsibling->data[0]);
	}
	else if (Lsibling && Lsibling->data.size() > min)
	{
		temp->insert(seperator);
		parent->remove(seperator);
		parent->insert(Lsibling->data[Lsibling->data.size() - 1]);
		Lsibling->remove(Lsibling->data[Lsibling->data.size() - 1]);
	}
	else
	{
		if (!Rsibling)
		{
			Rsibling = Lsibling;
		}
		if (parent == root && root->data.size() == 1)
		{
			while (!Rsibling->data.empty())
			{
				root->insert(Rsibling->data[0]);
				Rsibling->remove(Rsibling->data[0]);
			}
			delete Rsibling;
			while (!temp->data.empty())
			{
				root->insert(temp->data[0]);
				temp->remove(temp->data[0]);
			}
			delete temp;	
		}
		else
		{
			while (!temp->data.empty())
			{
				Rsibling->insert(temp->data[0]);
				temp->remove(temp->data[0]);
			}
			Rsibling->insert(seperator);
			parent->remove(seperator);
			parent->child.erase(parent->child.begin() + tempindex);
			if (parent->child.size() <= parent->data.size())
			{
				node<T>* Gparent = parent->parent;
				node<T>* Gchild;
				node<T>* newGpar = new node<T>(compare);
				for (int i = Gparent->child.size() -1; i >= 0; i--)
				{
					Gchild = Gparent->child[i];
					while (!Gchild->data.empty())
					{
						newGpar->insert(Gchild->data[0]);
						Gchild->remove(Gchild->data[0]);
					}
					while (!Gchild->child.empty())
					{
						newGpar->child.insert(newGpar->child.begin(), Gchild->child[Gchild->child.size() - 1]);
						newGpar->child[0]->parent = newGpar;
						Gchild->child.erase(Gchild->child.end() - 1);
					}
				}
				while (!Gparent->data.empty())
				{
					newGpar->insert(Gparent->data[0]);
					Gparent->remove(Gparent->data[0]);
				}
				root = newGpar;
				return;
			}
		}
	}
}

template<typename T>
T BTree<T>::find(T item)
{
	node<T>* temp;
	temp = root;
	int test;
	
	for (int i = 0; i < max; i++)
	{
		test = compare(item, temp->data[i]);
		if (test == -1)
		{
			if (temp->child.size() > i)
			{
				temp = temp->child[i];
				i = -1;
			}
			else
			{
				stringstream sstream;
				sstream << "ITEM DOESN'T EXIST";
				string msg;
				getline(sstream, msg);
				throw InvalidOp(msg);
			}
		}
		else if (test == 0)
		{
			return temp->data[i];
		}
		else
		{
			if (temp->child.size() > i + 1 && i == temp->data.size() - 1)
			{
				temp = temp->child[temp->child.size() - 1];
				i = -1;
			}
			else
			{
				stringstream sstream;
				sstream << "ITEM DOESN'T EXIST";
				string msg;
				getline(sstream, msg);
				throw InvalidOp(msg);
			}
		}
	}
	stringstream sstream;
	sstream << "ITEM DOESN'T EXIST";
	string msg;
	getline(sstream, msg);
	throw InvalidOp(msg);
}

template<typename T>
int BTree<T>::count()
{
	return size;
}