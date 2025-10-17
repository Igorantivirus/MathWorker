#include "MathWorker/Parse/Tokenizer.hpp"

#include <cctype>

namespace mathWorker
{

	Tokenizer::Tokenizer(const Signature& signature)
		: signature_(signature)
	{
	}

	void Tokenizer::setSettings(const Signature& signature)
	{
		signature_ = signature;
	}

	const Signature& Tokenizer::getSettings() const
	{
		return signature_;
	}

	BaseTokenizer::BaseTokenizer(const Signature& signature)
		: Tokenizer{ signature }
	{
	}

	TokenArray BaseTokenizer::tokenize(Token str) const
	{
		return addDefaultOperator(parseToTokens(str));
	}

	TokenArray BaseTokenizer::tokenizeByComma(Token str) const
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

	bool BaseTokenizer::meansDefaultOperator(Token a, Token b) const
	{
		if (a.empty() || b.empty())
			return false;

		bool bracket2 = isOpenBracket(b[0]);
		bool isNumber2 = isNumber(b[0]);
		bool isWord2 = isLetter(b[0]) &&
			!signature_.get().isSignatureType(b, SignatureType::unareRight) &&
			!signature_.get().isSignatureType(b, SignatureType::operation);

		if (isCloseBracket(a.back()) && (isNumber2 || isWord2 || bracket2))
			return true;
		if (isNumber(a[0]) && (isNumber2 || isWord2 || bracket2))
			return true;
		if (!signature_.get().isTerm(a) && isLetter(a[0]) && (isNumber2 || isWord2 || bracket2))
			return true;

		return false;
	}

	size_t BaseTokenizer::getEndOf(Token str, size_t i, CheckMethod check) const
	{
		while (i < str.size() && check(str[i]))
			++i;
		return i;
	}

	size_t BaseTokenizer::getEndOfBracket(Token str, size_t i) const
	{
		int lavel = 1;
		size_t j = i + 1;
		for (; j < str.size() && lavel > 0; ++j)
			if (str[j] == '(')
				++lavel;
			else if (str[j] == ')')
				--lavel;
		if (lavel != 0)
			throw ParseException("Uncompensated brackets.", ExceptionType::brackets);
		return j;
	}

	TokenArray BaseTokenizer::parseToTokens(Token str) const
	{
		TokenArray tkns;
		size_t j = 0;
		Token pr;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (std::isspace(static_cast<unsigned char>(str[i])))
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

	TokenArray BaseTokenizer::addDefaultOperator(TokenArray tkns) const
	{
		TokenArray res;
		res.reserve(tkns.size() * 3 / 2);
		size_t i = 0;
		for (; i + 1 < tkns.size(); ++i)
		{
			res.push_back(tkns[i]);

			auto realization = signature_.get().at(tkns[i]);

			if (realization)
			{
				if (realization->type == SignatureType::specialFunction)
					res.push_back(tkns[++i]);
				continue;
			}
			if (meansDefaultOperator(tkns[i], tkns[i + 1]))
				res.push_back(signature_.get().getDefaultOperator());
		}
		if (i < tkns.size())
			res.push_back(tkns[i]);
		return res;
	}

}
