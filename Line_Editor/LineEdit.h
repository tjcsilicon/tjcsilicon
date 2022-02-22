#pragma once 
#include "list.h"
#include "exceptions.h"
#include <sstream>

using namespace std;
using std::stringstream;
using std::getline;

const int INITIAL_ARRAY_SIZE = 10;

template <typename T>
class LineEdit : public List<T>
{
public:
	LineEdit();
	~LineEdit();
	void insert(T item, int index);
	void remove(int index);
	T at(int index);
	int size();

private:
	T* data;
	int tail;
	int currentArraySize;

	void increaseArraySize();

};

template <typename T>
LineEdit<T>::LineEdit()
{
	currentArraySize = INITIAL_ARRAY_SIZE;
	data = new T[currentArraySize];
	tail = 0;
}

template <typename T>
LineEdit<T>::~LineEdit()
{
	delete[] data;
}

template<typename T>
void LineEdit<T>::insert(T item, int index)
{
	// deal with the case where our array is full already
	if (tail == currentArraySize)
	{
		// Call the increaseArraySize helper function
		increaseArraySize();
	}

	if (index == -1) // Insert at tail
	{
		index = tail;
	}
	for (int i = tail - 1; i >= index; i--)
	{
		// shift element at data[i] to data[i+1]
		data[i + 1] = data[i];
	}
	data[index] = item;
	tail++;
}

template<typename T>
void LineEdit<T>::remove(int index)
{
	if (index < 0 || index >= tail)
	{
		// Throw an index out of bounds exception
		// FYI, stringstream is similar to both cin and cout. When used with <<,
		// information flows to the stringstream. When used with >>, information flows
		// from the stringstream. This allows me to convert an "int" to a string and
		// store it in the message, so I can grab the index number that was invalid
		stringstream sstream;
		sstream << "Index " << index << " is out of bounds";
		string msg;
		getline(sstream, msg);
		throw IndexOutOfBounds(msg);
	}

	for (int i = index; i < tail-1; i++)
	{
		data[i] = data[i + 1];
	}
	tail--;
}

template<typename T>
T LineEdit<T>::at(int index)
{
	if (index < 0 || index >= tail)
	{
		// Throw an index out of bounds exception
		stringstream sstream;
		sstream << "Index " << index << " is out of bounds";
		string msg;
		getline(sstream, msg);
	}

	return data[index];
}


template<typename T>
int LineEdit<T>::size()
{
	return tail;
}

template<typename T>
void LineEdit<T>::increaseArraySize()
{
	// Create a new array at 2x the original size
	T* tmp = new T[currentArraySize * 2];

	// Copy all the elements from the original array to the new one
	for (int i = 0; i < currentArraySize; i++)
	{
		tmp[i] = data[i];
	}

	// Delete the original array
	delete[] data;

	// Set the new array to be the "data" member
	data = tmp;

	// Set the currentArraySize to the new size
	currentArraySize *= 2;
}
