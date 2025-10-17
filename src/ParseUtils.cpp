#include "MathWorker/Parse/ParseUtils.hpp"

namespace mathWorker
{

	bool isLetter(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}

	bool isNumber(char c)
	{
		return (c >= '0' && c <= '9') || c == '.';
	}

	bool isOpenBracket(char c)
	{
		return c == '(';
	}

	bool isCloseBracket(char c)
	{
		return c == ')';
	}

	bool isComma(char c)
	{
		return c == ',';
	}

	bool isNone(char c)
	{
		return !isNumber(c) && !isLetter(c) && !isOpenBracket(c) && !(c == ' ');
	}

}
