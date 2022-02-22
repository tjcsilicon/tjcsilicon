#include <iostream>
#include "HuffmanTree.h"
#include <sstream>
#include <fstream>
#include "exceptions.h"
using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 4)
	{
		cout << "ERROR TOO MANY ARGUMENTS" << endl;
		return 1;
	}
	if (argc == 4)
	{
		string func = argv[1];
		if (func == "encode")
		{
			fstream alphafile;
			fstream msgfile;
			string msgName = argv[3];
			string alphName = argv[2];
			alphafile.open(alphName, ios::in);
			msgfile.open(msgName, ios::in);
			string alphabet;
			string message;
			getline(alphafile, alphabet);
			getline(msgfile, message);
			try
			{
				for (int i = 0; i < message.size(); i++)
				{
					if (alphabet.find(message[i]) == -1)
					{
						stringstream sstream;
						sstream << "MESSAGE CONTAINS LETTERS NOT IN ALPHABET";
						string msg;
						getline(sstream, msg);
						throw InvalidOp(msg);
					}
				}
			}
			catch (InvalidOp a)
			{
				cout << a.getMessage() << endl;
			}
			huffTree huff(alphabet);
			string fileName = msgName + ".encoded";
			ofstream file(fileName);
			file << huff.encode(message);
			file.close();
			
		}
		else if (func == "decode")
		{
			fstream alphafile;
			fstream msgfile;
			string msgName = argv[3];
			string alphName = argv[2];
			alphafile.open(alphName, ios::in);
			msgfile.open(msgName, ios::in);
			string alphabet;
			string message;
			getline(alphafile, alphabet);
			getline(msgfile, message);
			huffTree huff(alphabet);
			string fileName = msgName + ".decoded";
			ofstream file(fileName);
			file << huff.decode(message);
			file.close();
		}
		else
		{
			cout << "NOT A FUNCTION";
		}
	}
}