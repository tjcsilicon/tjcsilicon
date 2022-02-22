#include <iostream>
#include "B-Tree.h"
#include "Node.h"

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

int main(int argc, char* argv[])
{
	BTree<int> Tree(cmp, 3); 
    //BTree<int> Tree(cmp); //For max number of keys
	try
	{
		Tree.insert(33);
		Tree.insert(20);
		Tree.insert(16);
		Tree.insert(18);
		Tree.insert(42);
		Tree.insert(51);
		Tree.insert(65);
		Tree.insert(73);
		Tree.insert(100);
		Tree.insert(85);
		Tree.remove(73);
		Tree.remove(65);
		Tree.remove(42);
		Tree.remove(51);
		Tree.remove(100);
		Tree.remove(33);
		Tree.remove(18);
	}
	catch (InvalidOp a)
	{
		cout << a.getMessage() << endl;
	}
	return 0;
}