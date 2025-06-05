#pragma once

#include <stack>

#include "MathDefines.hpp"
#include "MathNode.hpp"

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"

namespace mathWorker
{

	class MathParser
	{
	private:

		std::string exceptionError_;

	public:



		const std::string& error = exceptionError_;


	//private:

		bool isLetter(const char c) const
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
		bool isNumber(const char c) const
		{
			return (c >= '0' && c <= '9') || c == '.';
		}
		bool isOpenBracket(const char c) const
		{
			return c == '(';
		}
		bool isCloseBracket(const char c) const
		{
			return c == '(';
		}

		std::vector<std::string> tokenizer(const std::string& str)
		{
			std::vector<std::string> result;
			size_t j;
			for (size_t i = 0; i < str.size(); ++i)
			{
				if (isLetter(str[i]))
				{
					for (j = i + 1; j < str.size() && isLetter(str[j]); ++j);
					result.push_back(str.substr(i, j - i));
					i = j - 1;
				}
				else if (isNumber(str[i]))
				{
					for (j = i + 1; j < str.size() && isNumber(str[j]); ++j);
					result.push_back(str.substr(i, j - i));
					i = j - 1;
				}
				else if (isOpenBracket(str[i]))
				{
					std::stack<char> stack;
					stack.push(str[i]);
					for (j = i + 1; j < str.size() && !stack.empty(); ++j)
					{
						if (isOpenBracket(str[j]))
							stack.push(str[j]);
						else if (isCloseBracket(str[j]))
							stack.pop();
					}
					result.push_back(str.substr(i, j - i));
					i = j - 1;
				}
				else if (isCloseBracket(str[i]))
				{
					exceptionError_ = "Error of bracket in index: " + std::to_string(i) + '.';
					return {};
				}
				else
					result.push_back(std::string(1, str[i]));
			}
			return result;
		}


	};

}