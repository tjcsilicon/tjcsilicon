#include <iostream>
#include "AVL_Tree.h"

template <typename T>
int cmp(T item1, T item2)
{
	if (item1 < item2)
	{
		return -1;
	}
	else if (item1 == item2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main()
{
	BinaryTree<int> Tree(cmp);
	try
	{
		Tree.insert(50);
		Tree.insert(20);
		Tree.insert(80);
		Tree.insert(10);
		Tree.insert(60);
		Tree.insert(90);
		Tree.insert(55);
		Tree.remove(90);
		Tree.remove(20);
		Tree.remove(50);
		Tree.remove(80);
		Tree.remove(60);
		Tree.remove(55);
		Tree.remove(10);
	}
	catch (InvalidOp a)
	{
		cout << a.getMessage() << endl;
	}
}

