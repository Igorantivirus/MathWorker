#pragma once

namespace mathWorker
{

	bool isLetter(const char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	bool isNumber(const char c)
	{
		return (c >= '0' && c <= '9') || c == '.';
	}
	bool isOpenBracket(const char c)
	{
		return c == '(';
	}
	bool isCloseBracket(const char c)
	{
		return c == ')';
	}
	bool isComma(const char c)
	{
		return c == ',';
	}
	bool isNone(const char c)
	{
		return !isNumber(c) && !isLetter(c) && !isOpenBracket(c);
	}

}