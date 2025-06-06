#pragma once

#include <stack>
#include <span>
#include <algorithm>

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

		SignatureContext* context_ = nullptr;

	public:

		//using SpecContext = std::vector<const SignatureContext::value_type*>;
		using Token = std::string_view;
		using TokenArray = std::vector<Token>;

		void setContext(SignatureContext* context)
		{
			context_ = context;
		}



		MathNodeP parse(const std::string_view& str)
		{
			TokenArray tkns = tokenize(str);

			return parsing(tkns);

		}




		const std::string& error = exceptionError_;
	
	private:

		//return index of this token
		size_t findTokenWithMinPriority(const std::span<Token> tkns)
		{
			size_t ind = std::string::npos;
			unsigned char priority = 255;
			
			for (size_t i = 0; i < tkns.size(); ++i)
			{
				auto found = context_->find(tkns[i]);
				if (found == context_->end())
					continue;
				if (found->second.priority < priority)
				{
					priority = found->second.priority;
					ind = i;
				}
			}
			return ind;
		}

		MathNodeP lastPars(const Token& tkn)
		{
			if (isNumber(tkn[0]))
				return std::make_unique<ValueNode>(RealType(std::stold(std::string(tkn))));
			if (isLetter(tkn[0]))
				return std::make_unique<SymbolNode>(std::string(tkn));
			if (isOpenBracket(tkn[0]))
			{
				TokenArray tkns = tokenize(tkn.substr(1, tkn.size() - 2));
				return parsing(tkns);
			}
			return nullptr;
		}
		MathVector parseParams(const Token& tkn)
		{
			MathVector result;

			TokenArray tkns = tonenizeByComma(tkn.substr(1, tkn.size() - 2));

			for (const auto& i : tkns)
				if (!(i.size() == 1 && isComma(i[0])))
					result.push_back(parse(i));

			return std::move(result);
		}

		MathNodeP parsing(const std::span<Token> tkns)
		{
			if (tkns.empty())
				return nullptr;
			size_t minInd = findTokenWithMinPriority(tkns);

			if (minInd == std::string::npos)
				return lastPars(tkns[0]);


			std::unique_ptr<SignatureNode> node(new SignatureNode{});
			node->setName(std::string(tkns[minInd]));

			SignatureType type = context_->find(tkns[minInd])->second.type;

			if (type == SignatureType::operation)
			{
				MathNodeP left = parsing(tkns.first(minInd));
				MathNodeP right = parsing(tkns.subspan(minInd + 1));
				if(left == nullptr)
					node->setParams(MathRowVector{right.get()});
				else
					node->setParams(MathRowVector{left.get(), right.get()});
			}
			else if (type == SignatureType::function)
			{
				MathVector params = std::move(parseParams(tkns[minInd + 1]));
				node->setParams(params);
			}
			else if (type == SignatureType::unare)
			{
				MathNodeP param = parsing(tkns.subspan(minInd - 1, 1));
				node->setParams(MathRowVector{ param.get() });
			}
			else if (type == SignatureType::specialFunction)
			{
				MathNodeP left = parsing(tkns.subspan(minInd + 1, 1));
				MathNodeP right = parsing(tkns.subspan(minInd + 2, 1));
				node->setParams(MathRowVector{ left.get(), right.get() });
			}
			return std::move(node);
		}


	private:

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
			return c == ')';
		}
		bool isComma(const char c) const
		{
			return c == ',';
		}
		bool isNone(const char c) const
		{
			return !isNumber(c) && !isLetter(c) && !isOpenBracket(c);
		}

		using CheckMethod = bool(MathParser::*)(const char c) const;

		size_t getEndOf(const Token& str, size_t i, CheckMethod check)
		{
			while (i < str.size() && (this->*check)(str[i]))
				++i;
			return i;
		}
		size_t getEndOfBracket(const Token& str, size_t i)
		{
			int lavel = 1;
			size_t j = i + 1;
			for (; j < str.size() && lavel > 0; ++j)
				if (str[j] == '(')
					++lavel;
				else if (str[j] == ')')
					--lavel;
			if (lavel != 0)
				exceptionError_ = "Error of brackets. opened in " + std::to_string(i) + '.';
			return j;
		}

		TokenArray tokenize(const Token& str)
		{
			TokenArray tkns;
			size_t j = 0;
			for (size_t i = 0; i < str.size(); ++i)
			{
				if (isOpenBracket(str[i]))
					tkns.push_back(str.substr(i, (j = getEndOfBracket(str, i)) - i));
				else if (isLetter(str[i]))
					tkns.push_back(str.substr(i, (j = getEndOf(str, i, &MathParser::isLetter)) - i));
				else if (isNumber(str[i]))
					tkns.push_back(str.substr(i, (j = getEndOf(str, i, &MathParser::isNumber)) - i));
				else
					tkns.push_back(str.substr(i, (j = getEndOf(str, i, &MathParser::isNone)) - i));
				i = j - 1;
			}
			return tkns;
		}
		TokenArray tonenizeByComma(const Token& str)
		{
			TokenArray tkns;
			size_t last = 0;
			for (size_t i = 0; i < str.size(); ++i)
			{
				if (isOpenBracket(str[i]))
					i = getEndOfBracket(str, i) - 1;
				else if (isComma(str[i]))
				{
					tkns.push_back(str.substr(last, i - last));
					last = i + 1;
				}
			}
			if(last < str.size())
				tkns.push_back(str.substr(last));
			return tkns;
		}
		
	};

}