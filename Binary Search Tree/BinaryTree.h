#pragma once
#include <iostream>
#include "exceptions.h"
#include <sstream>
#include <stdexcept>
#include <math.h>


using namespace std;
template <typename T>
class BinaryTree
{
private:
	class node
	{
	public:
		T data;
		node* left = nullptr;
		node* right = nullptr;
		node* parent = nullptr;
	};

	node* root;
	
	int size;
	int(*compare)(T&, T&);
	
	
public:
	BinaryTree<T>(int (*cmp)(T&, T&))
	{
		compare = cmp;
		root = nullptr;
	}
	int getSize();
	node* findNode(T);
	void insert(T);
	void remove(T);
	T search(T);
	void rightRotation(node*);
	void leftRotation(node*);
	void rebalence();
};

template <typename T>
typename BinaryTree<T>::node* BinaryTree<T>::findNode(T item)
{
	node* temp = root;
	int test = compare(item, temp->data);
	while (test != 0)
	{
		if (test == -1)
		{
			if (temp->left)
			{
				temp = temp->left;
			}
			else
			{
				stringstream sstream;
				sstream << "ITEM DOES NOT EXIST";
				string msg;
				getline(sstream, msg);
				throw InvalidOp(msg);
			}
		}
		else if (test == 1)
		{
			if (temp->right)
			{
				temp = temp->right;
			}
			else
			{
				stringstream sstream;
				sstream << "ITEM DOES NOT EXIST";
				string msg;
				getline(sstream, msg);
				throw InvalidOp(msg);
			}
		}
		test = compare(item, temp->data);
	}
	return temp;
}
template <typename T>
void BinaryTree<T>::insert(T item)
{
	node* newNode = new node;
	node* temp = root;
	node* parent;
	newNode->data = item;
	bool swap = false;
	if (!root)
	{
		root = newNode;
		size++;
		return;
	}
	while (!swap)
	{
		int test = compare(newNode->data, temp->data);
		if (test == -1)
		{
			if (temp->left)
			{
				temp = temp->left;
			}
			else
			{
				newNode->parent = temp;
				temp->left = newNode;
				swap = true;
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
			if (temp->right != NULL)
			{
				temp = temp->right;
			}
			else
			{
				newNode->parent = temp;
				temp->right = newNode;
				swap = true;
			}
		}
	}
	size++;

}

template <typename T>
void BinaryTree<T>::remove(T item)
{
	node* temp = root;
	node* IPparent;
	node* parent;
	node* imPred;
	int test = compare(item, temp->data);
	temp = findNode(item);
	if (!temp->left && !temp->right)
	{
		parent = temp->parent;
		if (parent->right == temp)
		{
			parent->right = NULL;
		}
		else
		{
			parent->left = NULL;
		}
		delete temp;
	}
	else if (!temp->left || !temp->right)
	{
		parent = temp->parent;
		if (temp->right)
		{
			parent->right = temp->right;
			temp->right->parent = parent;
		}
		if (temp->left)
		{
			imPred = temp->left;
			while (imPred->right)
			{
				imPred = imPred->right;
			}
			IPparent = imPred->parent;
			temp->data = imPred->data;
			if (IPparent == temp)
			{
				if (imPred->left)
				{
					temp->left = IPparent->left;
					IPparent->right = NULL;
				}
				else
				{
					temp->left = NULL;
				}
			}
			else
			{
				if (imPred->left)
				{
					IPparent->right = imPred->left;
					imPred->left->parent = IPparent;
				}
				else
				{
					IPparent->right = NULL;
				}
			}
		}
	}
	else
	{
		parent = temp->parent;
		imPred = temp->left;
		while (imPred->right)
		{
			imPred = imPred->right;
		}
		IPparent = imPred->parent;
		temp->data = imPred->data;
		if (IPparent == temp)
		{
			if (imPred->left)
			{
				temp->left = IPparent->left;
			}
			else
			{
				temp->left = NULL;
			}
		}
		else
		{
			if (imPred->left)
			{
				IPparent->right = imPred->left;
				imPred->left->parent = IPparent;
			}
			else
			{
				IPparent->right = NULL;
			}
		}
		
	}

	size--;
	
}

template <typename T>
T BinaryTree<T>::search(T item)
{
	node* temp = root;
	int test = compare(item, temp->data);
	temp = findNode(item);
	return temp->data;
}

template <typename T>
int BinaryTree<T>::getSize()
{
	return size;
}

template <typename T>
void BinaryTree<T>::rightRotation(node* n)
{
	node* parent = n->parent;
	node* gParent = parent->parent;
	node* nRight = n->right;
	if (parent && parent->left == n)
	{
		if (gParent)
		{
			gParent->left == parent ? gParent->left = n : gParent->right = n;
		}
		if (parent == root)
		{
			root = n;
		}
		n->parent = gParent;
		n->right = parent;
		parent->parent = n;
		parent->left = nRight;
		if (nRight)
		{
			nRight->parent = parent;
		}
	}
	else
	{
		cout << "CANNOT PERFORM RIGHT ROTATION" << endl;
	}
}
template <typename T>
void BinaryTree<T>::leftRotation(node* n)
{
	node* parent = n->parent;
	node* gParent = parent->parent;
	node* nLeft = n->left;

	if (parent && parent->right == n)
	{
		if (gParent)
		{
			gParent->left == parent ? gParent->left = n : gParent->right = n;
		}
		if (parent == root)
		{
			root = n;
		}
		n->parent = gParent;
		n->left = parent;
		parent->parent = n;
		parent->right = nLeft;
		if (nLeft)
		{
			nLeft->parent = parent;
		}
	}
	else
	{
		cout << "CANNOT PERFORM LEFT ROTATION" << endl;
	}
}
template <typename T>
void BinaryTree<T>::rebalence()
{
	node* temp = root;
	int M;
	int N;
	bool bone = true;
	while (bone)
	{
		while (temp->left)
		{
			temp = temp->left;
			rightRotation(temp);
			temp = root;
		}
		if (temp->right)
		{
			temp = temp->right;
		}
		else
		{
			bone = false;
		}
	} 
	temp = root;
	N = size;
	M = pow(2, (int)log2(N + 1)) - 1; //calculated M
	//perform N-M rotations
	for (int i=0; i < N - M; i++)
	{
		temp = temp->right;
		leftRotation(temp);
		temp = temp->right;
	}
	temp = root;
	while (M > 1)
	{
		M = M / 2;
		temp = root;
		for (int i=0; i < M; i++)
		{
			temp = temp->right;
			leftRotation(temp);
			temp = temp->right;
		}
	}
}