#include <iostream>
#include "BinaryTree.h"

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
		Tree.insert(5);
		Tree.insert(10);
		Tree.insert(3);
		Tree.insert(4);
		Tree.insert(2);
		Tree.insert(1);
		Tree.rebalence();
	}
	catch (InvalidOp a)
	{
		cout << a.getMessage() << endl;
	}
}

