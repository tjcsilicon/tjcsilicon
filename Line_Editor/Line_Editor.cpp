//CPTS223_Hw02_Line_Editor 
//By Timothy J Cain
//9/20/21
#include "LineEdit.h"
#include "operators.h"
#include <iostream>
#include <fstream>

using namespace std;



int main(int argc, char* argv[])
{
	Operator* op;
	cout << "Welcome to my Line Editor! \nAuthor:: Timothy J Cain" << endl;
	cout << "Start by entering a command!" << endl;
	cout << "'W + fileName' saves the document\n'J + lineNumber' moves the current line\n'I + text' inserts text at the current line\n'A + text' inserts text after the current line\n'L + lineNumber' prints the text at the specified line. Add another line number to print lines between two line numbers\n'D + lineNumber' deletes lines. Add another lineNumber to removes lines between two line numbers\n'Q' to quit" << endl;
	if (argc > 2)
	{
		cout << "ERROR TOO MANY ARGUMENTS" << endl;
		return 1;
	}
	if (argc == 2)
	{
		string fileName = argv[1];
		op = new Operator(fileName); 
	}
	else
	{
		op = new Operator();
	}
	string input;
	while (op->Quit == false)
	{
		getline(cin, input);
		op->command(input);
	}
	delete op;
	return 0;
}