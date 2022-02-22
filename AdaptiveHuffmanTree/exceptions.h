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

class InvalidOp : public BaseException
{
public:
	InvalidOp(string);
	~InvalidOp();

private:

};
