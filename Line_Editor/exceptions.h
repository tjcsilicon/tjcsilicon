#pragma once
#pragma once
#include <string>

using std::string;

class BaseException
{
public:
	BaseException(string);
	~BaseException();

	string getMessage();
	void setMessage(string);
protected:
	string msg;
};

class IndexOutOfBounds : public BaseException
{
public:
	IndexOutOfBounds(string);
	~IndexOutOfBounds();

private:

};
