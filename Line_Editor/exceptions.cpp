#include "exceptions.h"

BaseException::BaseException(string msg = "An unknown error occurred") : msg(msg)
{
}


BaseException::~BaseException()
{
}

string BaseException::getMessage()
{
	return this->msg;
}

void BaseException::setMessage(string msg)
{
	this->msg = msg;
}

IndexOutOfBounds::IndexOutOfBounds(string msg = "Attempt to access element out of range") : BaseException(msg)
{
}

IndexOutOfBounds::~IndexOutOfBounds()
{
}
