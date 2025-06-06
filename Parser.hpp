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
	enum class ExceptionType : unsigned char
	{
		unknown = 0,
		brackets,
		params,
	};

	class ParseException : public std::exception
	{
	public:
		explicit ParseException(const char* message, const ExceptionType type)
			: msg_(message), type_{type}
		{}
		explicit ParseException(const std::string& message, const ExceptionType type)
			: msg_(message), type_{type}
		{}
		virtual ~ParseException() noexcept
		{}

		virtual const char* what() const noexcept
		{
			return msg_.c_str();
		}
		virtual const ExceptionType type() const noexcept
		{
			return type_;
		}

	protected:

		std::string msg_;

		ExceptionType type_ = ExceptionType::unknown;

	};

	class MathParser
	{
	private:

		std::string exceptionError_;

		SignatureContext* context_ = nullptr;
		std::string defaultOperator_;

	public:

		using Token = std::string_view;
		using TokenArray = std::vector<Token>;

	public:
		MathParser(SignatureContext* context, const std::string& def = {})
		{
			setContext(context, def);
		}

		void setContext(SignatureContext* context, const std::string& def)
		{
			context_ = context;
			auto found = context->find(def);

			if (found != context->end())
				defaultOperator_ = def;
		}

		MathNodeP parse(const std::string_view& str)
		{
			TokenArray tkns = tokenize(str);
			return parsing(tkns);
		}

		const std::string& error = exceptionError_;
	
	private:

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

		MathNodeP lastParams(const std::span<Token> tkns)
		{
			if (tkns.empty())
				return nullptr;
			if (tkns.size() == 1)
			{
				if (isNumber(tkns[0][0]))
					return std::make_unique<ValueNode>(RealType(std::stold(std::string(tkns[0]))));
				if (isLetter(tkns[0][0]))
					return std::make_unique<SymbolNode>(std::string(tkns[0]));
				if (isOpenBracket(tkns[0][0]))
				{
					TokenArray ntkns = tokenize(tkns[0].substr(1, tkns[0].size() - 2));
					return parsing(ntkns);
				}
				return nullptr;
			}
			size_t ind = tkns.size() / 2;
			std::unique_ptr<SignatureNode> node(new SignatureNode{});
			node->setName(defaultOperator_);

			MathNodeP left = parsing(tkns.first(ind));
			MathNodeP right = parsing(tkns.subspan(ind));
			node->setParams(MathRowVector{ left.get(), right.get() });

			return std::move(node);
		}

		MathVector parseParams(const Token& tkn)
		{
			MathVector result;
			TokenArray tkns = tonenizeByComma(tkn.substr(1, tkn.size() - 2));

			for (const auto& i : tkns)
				if (i.empty())
					throw ParseException("Empty parameter of function.", ExceptionType::params);
				else if (!(i.size() == 1 && isComma(i[0])))
					result.push_back(parse(i));

			return std::move(result);
		}

		MathNodeP parsing(const std::span<Token> tkns)
		{
			size_t minInd = findTokenWithMinPriority(tkns);

			if (minInd == std::string::npos)
				return lastParams(tkns);

			std::unique_ptr<SignatureNode> node(new SignatureNode{});
			node->setName(std::string(tkns[minInd]));

			SignatureType type = context_->find(tkns[minInd])->second.type;

			if (type == SignatureType::operation)
			{
				MathNodeP left = parsing(tkns.first(minInd));
				MathNodeP right = parsing(tkns.subspan(minInd + 1));
				node->setParams(MathRowVector{ left.get(), right.get() });
			}
			else if (type == SignatureType::function)
				node->setParams(std::move(parseParams(tkns[minInd + 1])));
			else if (type == SignatureType::unare)
				node->setParams(MathRowVector{ parsing(tkns.first(minInd)).get() });
			else if (type == SignatureType::specialFunction)
			{
				MathNodeP left = parsing(tkns.subspan(minInd + 1, 1));
				MathNodeP right = parsing(tkns.subspan(minInd + 2));
				node->setParams(MathRowVector{ left.get(), right.get() });
			}
			return std::move(node);
		}

	private:

		bool isLetter		(const char c) const
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}
		bool isNumber		(const char c) const
		{
			return (c >= '0' && c <= '9') || c == '.';
		}
		bool isOpenBracket	(const char c) const
		{
			return c == '(';
		}
		bool isCloseBracket	(const char c) const
		{
			return c == ')';
		}
		bool isComma		(const char c) const
		{
			return c == ',';
		}
		bool isNone			(const char c) const
		{
			return !isNumber(c) && !isLetter(c) && !isOpenBracket(c);
		}

		using CheckMethod = bool(MathParser::*)(const char) const;

		size_t getEndOf(const Token& str, size_t i, CheckMethod check) const
		{
			while (i < str.size() && (this->*check)(str[i]))
				++i;
			return i;
		}
		size_t getEndOfBracket(const Token& str, size_t i) const
		{
			int lavel = 1;
			size_t j = i + 1;
			for (; j < str.size() && lavel > 0; ++j)
				if (str[j] == '(')
					++lavel;
				else if (str[j] == ')')
					--lavel;
			if (lavel != 0)
				throw ParseException("Error of brackets. opened in " + std::to_string(i) + '.', ExceptionType::brackets);
			return j;
		}

		TokenArray tokenize(const Token& str) const
		{
			TokenArray tkns;
			size_t j = 0;
			for (size_t i = 0; i < str.size(); ++i)
			{
				if (isOpenBracket(str[i]))
					tkns.push_back(str.substr(i, (j = getEndOfBracket(str, i)) - i));
				else if (isCloseBracket(str[i]))
					throw ParseException("Not opened bracket in index: " + std::to_string(i) + '.', ExceptionType::brackets);
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
		TokenArray tonenizeByComma(const Token& str) const
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
			if (last < str.size())
				tkns.push_back(str.substr(last));
			return tkns;
		}
		
	};

}