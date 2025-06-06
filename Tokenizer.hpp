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
		Tokenizer(const SignatureContext* context)
		{
			setSettings(context);
		}
		virtual ~Tokenizer() = default;

		void setSettings(const SignatureContext* context)
		{
			context_ = context;
		}

		virtual TokenArray tokenize(const Token& str) const = 0;
		virtual TokenArray tonenizeByComma(const Token& str) const = 0;


	protected:

		const SignatureContext* context_ = nullptr;

	};

	class BaseTokenizer : public Tokenizer
	{
	public:
		BaseTokenizer() = default;
		BaseTokenizer(const SignatureContext* context) :
			Tokenizer(context)
		{}

		TokenArray tokenize(const Token& str) const override
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
				if(!pr.empty())
					tkns.push_back(pr);
				i = j - 1;
			}
			return tkns;
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

	};
}