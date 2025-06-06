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
		MathParser(const SignatureContext* context, const Tokenizer* tokenizer)
		{
			setContext(context);
			setTokenizer(tokenizer);
		}

		void setContext(const SignatureContext* context)
		{
			context_ = context;
		}
		void setTokenizer(const Tokenizer* tokenizer)
		{
			tokenizer_ = tokenizer;
		}

		MathNodeP parse(const std::string_view& str) const
		{
			TokenArray tkns = tokenizer_->tokenize(str);
			return parseTokens(tkns);
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

				const unsigned char current_priority = found->second.priority;
				const OperatorPriority current_association = found->second.assitiation;

				if (current_priority < priority || (current_priority == priority && current_association == OperatorPriority::leftToRight))
				{
					priority = current_priority;
					ind = i;
				}
			}
			return ind;
		}

		MathNodeP finalParse(const TokenArrayP tkns) const
		{
			if (tkns.empty())
				throw ParseException("Empty list of tokens", ExceptionType::unknown);
			if (tkns.size() == 1)
			{
				if (isNumber(tkns[0][0]))
					return std::make_unique<ValueNode>(RealType(std::stold(std::string(tkns[0]))));
				if (isLetter(tkns[0][0]))
					return std::make_unique<SymbolNode>(std::string(tkns[0]));
				if (tkns[0].size() >= 2)
					return parse(tkns[0].substr(1, tkns[0].size() - 2));
				throw ParseException(std::string("Unknown token \"") + std::string(tkns[0]) + '\"', ExceptionType::unknown);
			}
			throw ParseException(std::string("Unknown tokens "), ExceptionType::unknown);
			/*auto pr = context_->find(defautlOperator_)->second.assitiation;
			size_t ind = context_->find(defautlOperator_)->second.assitiation == OperatorPriority::leftToRight ? tkns.size() - 1 : 0;
			
			SignatureNode* node = new SignatureNode{ defautlOperator_ };
			processOperatorTkns(node, tkns.first(ind), tkns.subspan(ind));
			return std::move(MathNodeP(node));*/
		}

		MathVector parametsParsing(const Token token) const
		{
			if (token.size() < 2 || !isOpenBracket(token[0]) || !isCloseBracket(token.back()))
				throw ParseException("Invalid function parameters", ExceptionType::params);

			MathVector result;
			TokenArray tkns = tokenizer_->tonenizeByComma(token.substr(1, token.size() - 2));

			for (const auto& i : tkns)
			{
				if (i.empty())
					throw ParseException("Empty parameter of function.", ExceptionType::params);
				result.push_back(parse(i));
			}

			return std::move(result);
		}

		void processOperatorTkns(SignatureNode* node, const TokenArrayP left, const TokenArrayP right) const
		{
			if (!left.empty())
				node->addParam(parseTokens(left));
			if (!right.empty())
				node->addParam(parseTokens(right));
		}
		void processFunctionTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			if(!(minInd == 0 && tkns.size() == 2))
				throw ParseException("After processing function have prooruty less whet somethins else.", ExceptionType::priority);
			node->setParams(parametsParsing(tkns[1]));
		}
		void processSpecFunctionTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			if (!(minInd == 0 && tkns.size() == 3))
				throw ParseException("After processing function have prooruty less whet somethins else.", ExceptionType::priority);
			node->addParam(finalParse(tkns.subspan(1, 1)));
			node->addParams(parametsParsing(tkns[2]));
		}

		MathNodeP parseTokens(const TokenArrayP tkns) const
		{
			size_t minInd = findTokenWithMinPriority(tkns);
			if (minInd == std::string::npos)
				return finalParse(tkns);

			SignatureNode* node = new SignatureNode{ std::string(tkns[minInd]) };
			SignatureType type = context_->find(tkns[minInd])->second.type;

			if (type == SignatureType::operation)
				processOperatorTkns(node, tkns.first(minInd), tkns.subspan(minInd + 1));
			else if (type == SignatureType::function)
				processFunctionTkns(node, tkns, minInd);
			else if (type == SignatureType::specialFunction)
				processSpecFunctionTkns(node, tkns, minInd);
			return std::move(MathNodeP(node));
		}
	};

}