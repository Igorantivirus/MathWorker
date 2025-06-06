#pragma once

#include <span>

#include "MathDefines.hpp"
#include "ParseUtils.hpp"
#include "ParseException.hpp"

namespace mathWorker
{

	using Token = std::string_view;
	using TokenArray = std::vector<Token>;
	using TokenArrayP = std::span<Token>;

	class Tokenizer
	{
	public:
		Tokenizer() = default;
		Tokenizer(const SignatureContext* context, const std::string& defautltOperator)
		{
			setSettings(context, defautltOperator);
		}
		virtual ~Tokenizer() = default;

		void setSettings(const SignatureContext* context, const std::string& defautltOperator)
		{
			context_ = context;
			if (context->find(defautltOperator) != context->end())
				defautlOperator_ = defautltOperator;
		}

		virtual TokenArray tokenize(const Token& str) const = 0;
		virtual TokenArray tonenizeByComma(const Token& str) const = 0;


	protected:

		const SignatureContext* context_ = nullptr;
		std::string defautlOperator_;
	};

	class BaseTokenizer : public Tokenizer
	{
	public:
		BaseTokenizer() = default;
		BaseTokenizer(const SignatureContext* context, const std::string& defautltOperator) :
			Tokenizer(context, defautltOperator)
		{}

		TokenArray tokenize(const Token& str) const override
		{
			TokenArray tkns = parseToTokens(str);
			return addDefaultOperator(tkns);
		}
		TokenArray tonenizeByComma(const Token& str) const override
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

	private:

		using CheckMethod = bool(*)(const char);

		bool meansDefaultOperator(const Token& a, const Token& b) const
		{
			if (a.empty() || b.empty())
				return false;
			bool bracket1 = isCloseBracket(a.back());
			bool bracket2 = isOpenBracket(b[0]);

			bool isNumber1 = isNumber(a[0]);
			bool isNumber2 = isNumber(b[0]);

			bool isWord1 = isLetter(a[0]);
			bool isWord2 = isLetter(b[0]);

			if (bracket1 && (isNumber2 || isWord2 || bracket2))
				return true;
			if (isNumber1 && (isNumber2 || isWord2 || bracket2))
				return true;
			if (!isTerm(a) && isWord1 && (isNumber2 || isWord2 || bracket2))
				return true;
			return false;
		}

		bool isTerm(const Token& tkn) const
		{
			return context_->find(tkn) != context_->end();
		}

		size_t getEndOf(const Token& str, size_t i, CheckMethod check) const
		{
			while (i < str.size() && check(str[i]))
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

		TokenArray parseToTokens(const Token& str) const
		{
			TokenArray tkns;
			size_t j = 0;
			Token pr;
			for (size_t i = 0; i < str.size(); ++i)
			{
				if (std::isspace(str[i]))
					continue;
				if (isOpenBracket(str[i]))
					pr = str.substr(i, (j = getEndOfBracket(str, i)) - i);
				else if (isCloseBracket(str[i]))
					throw ParseException("Not opened bracket in index: " + std::to_string(i) + '.', ExceptionType::brackets);
				else if (isLetter(str[i]))
					pr = str.substr(i, (j = getEndOf(str, i, isLetter)) - i);
				else if (isNumber(str[i]))
					pr = str.substr(i, (j = getEndOf(str, i, isNumber)) - i);
				else
					pr = str.substr(i, (j = getEndOf(str, i, isNone)) - i);
				if (!pr.empty())
					tkns.push_back(pr);
				i = j - 1;
			}
			return tkns;
		}

		TokenArray addDefaultOperator(const TokenArray& tkns) const
		{
			TokenArray res;
			size_t i = 0;
			for (; i < tkns.size() - 1; ++i)
			{
				auto found = context_->find(tkns[i]);
				if (found == context_->end() || found->second.type == SignatureType::operation)
				{
					res.push_back(tkns[i]);
					continue;
				}
				if (found->second.type == SignatureType::function)
				{
					res.push_back(tkns[i++]);
					res.push_back(tkns[i]);
				}
				if (found->second.type == SignatureType::specialFunction)
				{
					res.push_back(tkns[i++]);
					res.push_back(tkns[i++]);
					res.push_back(tkns[i]);
				}
				if (i + 1 < tkns.size() && meansDefaultOperator(tkns[i], tkns[i + 1]))
					res.push_back("*");
			}
			if (i < tkns.size())
				res.push_back(tkns[i]);
			return res;
		}

	};
}