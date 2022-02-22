#pragma once

template <typename T>
class List
{
public:
	virtual void insert(T item, int index) = 0;
	virtual void remove(int index) = 0;
	virtual T at(int index) = 0;
	//virtual int search(T item, int(*cmp)(const T& item1, const T& item2)) = 0;
	virtual int size() = 0;
};