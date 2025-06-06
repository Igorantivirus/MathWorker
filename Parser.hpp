#pragma once

#include <stack>
#include <span>
#include <algorithm>

#include "MathDefines.hpp"
#include "MathNode.hpp"

#include "ValueNode.hpp"
#include "SymbolNode.hpp"
#include "SignatureNode.hpp"
#include "ParseException.hpp"
#include "Tokenizer.hpp"

namespace mathWorker
{

	class MathParser
	{
	public:
		MathParser() = default;
		MathParser(const SignatureContext* context, const Tokenizer* tokenizer) :
			context_{ context }, tokenizer_{tokenizer}
		{}

		void setContext(const SignatureContext* context)
		{
			context_ = context;
		}
		void setTokenizer(const Tokenizer* tokenizer)
		{
			tokenizer_ = tokenizer;
		}

		MathNodeP parse(const std::string_view& str)
		{
			TokenArray tkns = tokenizer_->tokenize(str);
			return parsing(tkns);
		}

	private:

		const SignatureContext* context_ = nullptr;
		const Tokenizer* tokenizer_ = nullptr;

	private:

		bool isTerm(const Token& tkn) const
		{
			return context_->find(tkn) != context_->end();
		}

		size_t findTokenWithMinPriority(const std::span<Token> tkns) const
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
					TokenArray ntkns = tokenizer_->tokenize(tkns[0].substr(1, tkns[0].size() - 2));
					return parsing(ntkns);
				}
				return nullptr;
			}
			size_t ind = tkns.size() / 2;
			std::unique_ptr<SignatureNode> node(new SignatureNode{});
			node->setName("");

			MathNodeP left = parsing(tkns.first(ind));
			MathNodeP right = parsing(tkns.subspan(ind));
			node->setParams(MathRowVector{ left.get(), right.get() });

			return std::move(node);
		}

		MathVector parseParams(const Token& tkn)
		{
			MathVector result;
			TokenArray tkns = tokenizer_->tonenizeByComma(tkn.substr(1, tkn.size() - 2));

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
		
	};

}