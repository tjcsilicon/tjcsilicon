#pragma once
#include "list.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using std::getline;
using std::stringstream;

template<typename T>
int compare(T a, T b)
{
	if (a < b)
	{
		return -1;
	}
	else if (a == b)
	{
		return 0;
	}
	else if (a > b)
	{
		return 1;
	}

}

class Operator
{
public:
	Operator();
	Operator(string);
	void command(string s);
	void printList(int);
	void W(string);
	void J(string);
	void I(string);
	void A(string);
	void L(string);
	void D(string);
	void Q();
	bool Quit;
private:
	string fileName;
	LineEdit<string> List;
	int currentLine;
	bool saved;
};

Operator::Operator()
{
	saved = false;
	currentLine = 1;
	printList(currentLine);
	fileName = "";
}

Operator::Operator(string Name)
{
	fileName = Name;
	fstream file;
	file.open(fileName, ios::in);
	string str, blank;
	blank = "";
	while (getline(file, str))
	{
		int index = List.size();
		if (compare(str, blank) == 0)
		{
			getline(file, str);
		}
		List.insert(str, index);
	}
	file.close();
	saved = false;
	currentLine = List.size();
	printList(currentLine);
}

void Operator::command(string s)
{
	string cmd;
	char T;
	string line;
	stringstream X(s);
	getline(X, cmd, ' ');
	getline(X, line);
	if (cmd == "")
	{
		return;
	}
	T = cmd.at(0);
	switch (T)
	{
		case 'W':
			saved = true;
			W(fileName);
			break;
		case 'J':
			saved = false;
			J(line);
			break;
		case 'I':
			saved = false;
			I(line);
			break;
		case 'L':
			saved = false;
			L(line);
			break;
		case 'D':
			saved = false;
			D(line);
			break;
		case 'A':
			saved = false;
			A(line);
			break;
		case 'Q':
			Q();
			break;
		default:
			cout << "UNKNOWN COMMAND. PLEASE TRY AGAIN" << endl;
			break;
	}
}


void Operator::printList(int currentLine)
{
	if (List.size() == 0)
	{
		cout << ">Line " << "1:" << endl;
	}
	cout << "Please enter a command:" << endl;
	for (int i = List.size(); i > 0; i--)
	{
		if (List.size() - i == currentLine-1)
		{
			cout << ">Line " << List.size() - i + 1 << ":" << List.at(List.size() - i) << " " << endl;
		}
		else
			cout << " Line " << List.size() - i + 1 << ":" << List.at(List.size() - i) << " " << endl;
	}
	cout << endl;
}


void Operator::W(string Name)
{
	string tmp;
	if (Name == "" && fileName == "")
	{
		cout << "Please enter a file Name" << endl;
		cin >> tmp;
		fileName = tmp;
		ofstream file(fileName);
		int index = 0;
		while (index < List.size())
		{
			file << List.at(index) << endl;
			index++;
		}
		file.close();
	}
	else if (Name != "")
	{
		ofstream file;
		fileName = Name;
		file.open(fileName, ios::trunc);
		int index = 0;
		while (index < List.size())
		{
			file << List.at(index) << endl;
			index++;
		}
		file.close();
	}
	printList(currentLine);
}

void Operator::J(string line)
{
	if (line == "")
	{
		cout << "ERROR :: NO LINE NUMBER PROVIDED" << endl;
		return;
	}
	string str;
	int s;
	stringstream X(line);
	getline(X, str, ' ');
	if ((int)str[0] < 48 || (int)str[0] > 57)
	{
		cout << "ERROR :: NOT A NUMBER" << endl;
		return;
	}
	s = stoi(str);
	if (s == 0)
		currentLine = 1;
	else if (s == -1)
		currentLine = List.size();
	else
		currentLine = s;
	printList(currentLine);
}

void Operator::I(string input)
{
	if (input == "")
	{
		List.insert("", currentLine-1);
	}
	else
		List.insert(input, currentLine-1);
	//currentLine++;
	printList(currentLine);
}

void Operator::A(string input)
{
	if (input == "")
	{
		List.insert("", currentLine);
	}
	else
		List.insert(input, currentLine);
	currentLine++;
	printList(currentLine);
}
void Operator::L(string line)
{
	if (line == "")
	{
		printList(currentLine);
		return;
	}
	string num1, num2;
	int s, e;
	stringstream X(line);
	getline(X, num1, ' ');
	getline(X, num2);
	s = stoi(num1) - 1;
	if (num2 != "")
	{
		e = stoi(num2) - 1;
		if (s > List.size() || e > List.size())
		{
			cout << "ERROR :: INVALID SELECTION";
			return;
		}
		for (int i = s; i <= e; i++)
		{
			cout << List.at(i) << endl;
		}
	}
	else
	{
		cout << List.at(s) << endl;
	}
	
}

void Operator::D(string line)
{
	string num1, num2;
	int s, e;
	stringstream X(line);
	getline(X, num1, ' ');
	getline(X, num2);
	if ((int)num1[0] < 48 || (int)num1[0] > 57)
	{
		cout << "ERROR :: NOT A NUMBER" << endl;
		return;
	}
	s = stoi(num1)-1;
	if (s >= List.size())
	{
		cout << "ERROR :: LINE NUMBER OUT OF RANGE" << endl;
		return;
	}
	if (num2 != "")
	{
		if ((int)num2[0] < 48 || (int)num2[0] > 57)
		{
			cout << "ERROR :: NOT A NUMBER" << endl;
			return;
		}
		e = stoi(num2)-1;
		if (e >= List.size())
		{
			cout << "ERROR :: INVALID SELECTION";
			return;
		}
		for (int i = s; i <= e; i++)
		{
			List.remove(s);
		}
	}
	else
	{
		List.remove(s);
	}
	currentLine = s;
	printList(currentLine);
}

void Operator::Q()
{
	char SAQ;
	string line;
	if (saved == false)
	{
		cout << "File not saved" << endl << "Save now? y/n" << endl;
		cin >> SAQ;
		if (SAQ == 'y')
		{
			cout << "Enter File Name" << endl;
			cin >> line;
			W(line);
			Quit = true;
			return;
		}
		else if (SAQ == 'n')
		{
			Quit = true;
			return;
		}
		else
		{
			cout << "ERROR :: UNKNOWN COMMAND" << endl;
			Quit = false;
			return;
		}
	}
	Quit = true;
}