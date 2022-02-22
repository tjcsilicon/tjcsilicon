#pragma once
#include <vector>
#include <sstream>
#include <stdexcept>
#include "exceptions.h"

using namespace std;


template <typename T>
class node
{
public:
	vector<T> data;
	vector<node*> child;
	node* parent = nullptr;
	void insert(T);
	void remove(T);
	T findIndex(T);
	T findData(T);
	int size = 0;
	int(*compare)(T&, T&);
	node(int (*cmp)(T&, T&))
	{
		compare = cmp;
	}
};

template <typename T>
T node<T>::findIndex(T item)
{
	int test;
	for (int i = 0; i < size; i++)
	{
		if (data[i] == NULL)
		{
			stringstream sstream;
			sstream << "ITEM DOES NOT EXIST";
			string msg;
			getline(sstream, msg);
			throw InvalidOp(msg);
		}
		test = compare(item, data[i]);

		if (test == 0)
		{
			return i;
		}
	}

}

template <typename T>
T node<T>::findData(T index)
{
	if (data.size() < index + 1)
	{
		stringstream sstream;
		sstream << "ITEM DOES NOT EXIST";
		string msg;
		getline(sstream, msg);
		throw InvalidOp(msg);
	}
	return data[index];
}

template <typename T>
void node<T>::insert(T item)
{
	int test;
	if (data.size() == 0)
	{
		data.insert(data.begin(), item);
		size++;
		return;
	}
	for (int i = 0; i < data.size(); i++)
	{
		test = compare(item, data[i]);
		if (test == -1)
		{
			data.insert(data.begin() + i, item);
			size++;
			return;
		}
		else if (test == 0)
		{
			stringstream sstream;
			sstream << "ITEM ALREADY EXISTS";
			string msg;
			getline(sstream, msg);
			throw InvalidOp(msg);
		}
	}
	data.insert(data.begin() + data.size(), item);
	size++;
	return;
}

template <typename T>
void node<T>::remove(T item)
{
	int test;
	if (data.size() == 0)
	{
		stringstream sstream;
		sstream << "ITEM DOES NOT EXIST";
		string msg;
		getline(sstream, msg);
		throw InvalidOp(msg);
	}
	for (int i = 0; i < data.size(); i++)
	{
		test = compare(item, data[i]);
		if (test == -1)
		{
			stringstream sstream;
			sstream << "ITEM DOES NOT EXIST";
			string msg;
			getline(sstream, msg);
			throw InvalidOp(msg);
		}
		else if (test == 0)
		{
			data.erase(data.begin() + i);
			size--;
			return;
		}
	}
	size--;
	data.pop_back();
	return;
	
}

