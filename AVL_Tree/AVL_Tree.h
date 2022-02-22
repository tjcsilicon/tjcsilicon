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
		int leftHeight = 0;
		int rightHeight = 0;
		int reportHeight()
		{
			if (leftHeight > rightHeight)
			{
				return leftHeight + 1;
			}
			else if (rightHeight > leftHeight)
			{
				return rightHeight + 1;
			}
			else if(rightHeight == leftHeight)
			{
				return rightHeight + 1;
			}	
		}
	};

	node* root;

	int size = 0;
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
	int balenceFactor(node*);
	void adjustHeight(node*);
	node* checkBF(T);
	void rebalenceinsert(T);
	void rebalencedelete(node*, T);
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
	adjustHeight(newNode);
	rebalenceinsert(item);
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
	if (temp == root && size == 1)
	{
		delete temp;
		return;
	}
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
					IPparent->right = NULL;;
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
	
	if (size < 3)
	{
		return;
	}
	else if (parent == nullptr)
	{
		adjustHeight(root);
		rebalencedelete(root, item);
		return;
	}
	else
		adjustHeight(parent->left);
	rebalencedelete(parent, item);

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
	adjustHeight(parent);
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
	adjustHeight(parent);
}
template<typename T>
int BinaryTree<T>::balenceFactor(node* n)
{
	if (n == nullptr)
	{
		return 0;
	}
	return n->rightHeight - n->leftHeight;
}

template <typename T>
void BinaryTree<T>::adjustHeight(node* n)
{
	node* temp = n;
	while (temp != root)
	{
		if (!temp->right)
		{
			temp->rightHeight = 0;
		}
		else
		{
			temp->rightHeight = temp->right->reportHeight();
		}
		if (!temp->left)
		{
			temp->leftHeight = 0;
		}
		else
		{
			temp->leftHeight = temp->left->reportHeight();
		}
		if (temp->data < temp->parent->data)
		{
			temp->parent->leftHeight = temp->reportHeight();
		}
		else
		{
			temp->parent->rightHeight = temp->reportHeight();
		}
		
		temp = temp->parent;
	}
	if (root->left)
	{
		root->leftHeight = root->left->reportHeight();
	}
	else
		root->leftHeight = 0;

	if (root->right)
	{
		root->rightHeight = root->right->reportHeight();
	}
	else
		root->rightHeight = 0;
}
template<typename T>
typename BinaryTree<T>::node* BinaryTree<T>::checkBF(T item)
{
	node* temp = findNode(item);
	node* parent;
	if (temp == root && balenceFactor(temp) < -1 || balenceFactor(temp) > 1)
	{
		return root;
	}
	while (temp->parent)
	{
		if (balenceFactor(temp) < -1 || balenceFactor(temp) > 1)
		{
			return temp;
		}
		temp = temp->parent;
	}
	return nullptr;
}
template <typename T>
void BinaryTree<T>::rebalenceinsert(T item)
{
	node* temp = checkBF(item);
	node* parent;
	while (checkBF(item) != NULL)
	{
		if (item < temp->data)
		{
			parent = temp;
			temp = temp->left;
			if (item > temp->data)
			{
				leftRotation(temp->right);
				rightRotation(temp->right);
				return;
			}
			else
			{
				rightRotation(temp);
				return;
			}
		}
		else
		{
			parent = temp;
			temp = temp->right;
			if (item > temp->data)
			{
				rightRotation(temp->right);
				leftRotation(temp->right);
				return;
			}
			else
			{
				leftRotation(temp);
				return;
			}
		}
		temp = temp->parent;
	}
	if (balenceFactor(root) < -1 || balenceFactor(root) > 1)
	{
		if (item < root->data)
		{
			rightRotation(root->left);
		}
		if (item > root->data)
		{
			leftRotation(root->right);
		}
	}

}

template <typename T>
void BinaryTree<T>::rebalencedelete(node* n, T item)
{
	node* temp = n;
	node* parent;
	node* test = checkBF(temp->data);

	while (test != nullptr)
	{
		if (temp == root && balenceFactor(temp) == -2 && balenceFactor(temp->left->right) == 0)
		{
			leftRotation(temp->left->right);
			rightRotation(temp->left);
		}
		else if (temp == root && balenceFactor(temp->left) == 2 && balenceFactor(temp->right->left) == 0)
		{
			rightRotation(temp->right->left);
			leftRotation(temp->right);
		}
		else if (temp == root && !temp->left)
		{
			leftRotation(temp->right);
		}
		else if (temp == root && !temp->right)
		{
			rightRotation(temp->left);
		}
		else if (item < temp->data && balenceFactor(temp->right) == 1)
		{
			leftRotation(temp->right);
		}
		else if (item < temp->data && balenceFactor(temp->right) == 0)
		{
			leftRotation(temp->right);
		}
		else if (item < temp->data && balenceFactor(temp->right) == -1)
		{
			leftRotation(temp->right);
		}
		else if (item < temp->data && balenceFactor(temp->right) == -1 && balenceFactor(temp->right->left) == -1)
		{
			rightRotation(temp->right->left);
			leftRotation(temp->right);
		}
		else if (item < temp->data && balenceFactor(temp->right) == -1 && balenceFactor(temp->right->left) == 1)
		{
			rightRotation(temp->right->left);
			leftRotation(temp->right);
		}

		else if (item > temp->data && balenceFactor(temp->left) == 1)
		{
			rightRotation(temp->left);
		}
		else if (item > temp->data && balenceFactor(temp->left) == 0)
		{
			rightRotation(temp->left);
		}
		else if (item > temp->data && balenceFactor(temp->left) == -1)
		{
			rightRotation(temp->left);
		}
		else if (item > temp->data && balenceFactor(temp->left) == -1 && balenceFactor(temp->left->right) == -1)
		{
			leftRotation(temp->left->right);
			rightRotation(temp->left);
		}
		else if (item > temp->data && balenceFactor(temp->left) == -1 && balenceFactor(temp->left->right) == 1)
		{
			leftRotation(temp->left->right);
			rightRotation(temp->left);
		}

		test = checkBF(temp->data);

	}
}

