#pragma once

#include <stack>
#include <array>
#include <span>
#include <algorithm>

#include "../MathUtils/MathDefines.hpp"
#include "../MathClasses/MathNode.hpp"

#include "../MathClasses/ValueNode.hpp"
#include "../MathClasses/SymbolNode.hpp"
#include "../MathClasses/SignatureNode.hpp"
#include "ParseException.hpp"
#include "../Signature/Signature.hpp"
#include "Tokenizer.hpp"

namespace mathWorker
{

	class MathParser
	{
	public:
		MathParser(const Tokenizer& tokenizer) :
			tokenizer_{tokenizer}, signature_{tokenizer.getSettings()}
		{
			initProcessingMethods();
		}
			
		void setTokenizer(const Tokenizer& tokenizer)
		{
			tokenizer_ = tokenizer;
			signature_ = tokenizer.getSettings();
		}

		MathNodeP parse(const std::string_view& str) const
		{
			TokenArray tkns = tokenizer_.get().tokenize(str);
			return parseTokens(tkns);
		}

	private:

		using ProcessMethod = void(MathParser::*)(SignatureNode*, const TokenArrayP, const size_t) const;

		std::reference_wrapper<const Tokenizer> tokenizer_;
		std::reference_wrapper<const Signature> signature_;

		std::array<ProcessMethod, 5> procesingMethods_;

	private:

		void initProcessingMethods()
		{
			procesingMethods_[static_cast<unsigned char>(SignatureType::operation)]			= &MathParser::processOperatorTkns;
			procesingMethods_[static_cast<unsigned char>(SignatureType::function)]			= &MathParser::processFunctionTkns;
			procesingMethods_[static_cast<unsigned char>(SignatureType::specialFunction)]	= &MathParser::processSpecFunctionTkns;
			procesingMethods_[static_cast<unsigned char>(SignatureType::unareLeft)]			= &MathParser::processLeftOperatorTkns;
			procesingMethods_[static_cast<unsigned char>(SignatureType::unareRight)]		= &MathParser::processRightOperatorTkns;
		}

		size_t findTokenWithMaxPriority(const std::span<Token> tkns) const
		{
			size_t ind = std::string::npos;
			char priority = -1;

			for (size_t i = 0; i < tkns.size(); ++i)
			{
				auto realization = signature_.get().at(tkns[i]);
				if (!realization)
					continue;

				const unsigned char current_priority = realization->priority;
				const OperatorPriority current_association = realization->assitiation;

				if (current_priority > priority || (current_priority == priority && current_association == OperatorPriority::leftToRight))
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
		}

		MathVector parametsParsing(const Token token) const
		{
			if (token.size() < 2 || !isOpenBracket(token[0]) || !isCloseBracket(token.back()))
				throw ParseException("Invalid function parameters", ExceptionType::params);

			MathVector result;
			TokenArray tkns = tokenizer_.get().tokenizeByComma(token.substr(1, token.size() - 2));

			for (const auto& i : tkns)
			{
				if (i.empty())
					throw ParseException("Empty parameter of function.", ExceptionType::params);
				result.push_back(parse(i));
			}

			return result;
		}

		void processOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			const TokenArrayP left = tkns.first(minInd);
			const TokenArrayP right = tkns.subspan(minInd + 1);
			if (!left.empty())
				node->addParam(parseTokens(left));
			if (!right.empty())
				node->addParam(parseTokens(right));
		}
		void processFunctionTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			if(!(minInd == 0 && tkns.size() == 2))
				throw ParseException("After processing function have prooruty less when somethins else.", ExceptionType::priority);
			node->setParams(parametsParsing(tkns[1]));
		}
		void processRightOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			if (minInd != tkns.size() - 1)
				throw ParseException("After last right unare operator other expressions.", ExceptionType::priority);
			const TokenArrayP left = tkns.first(minInd);
			if(!left.empty())
				node->addParam(parseTokens(left));
			else
				throw ParseException("Empty operands of right unare operator.", ExceptionType::params);
		}
		void processLeftOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const
		{
			if (minInd != 0)
				throw ParseException("Before last left unare operator other expressions.", ExceptionType::priority);
			const TokenArrayP right = tkns.subspan(1);
			if (!right.empty())
				node->addParam(parseTokens(right));
			else
				throw ParseException("Empty operands of left unare operator.", ExceptionType::params);
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
			size_t minInd = findTokenWithMaxPriority(tkns);
			if (minInd == std::string::npos)
				return finalParse(tkns);

			std::unique_ptr<SignatureNode> nodeP = std::make_unique<SignatureNode>(std::string(tkns[minInd]));
			auto term = signature_.get().at(tkns[minInd]);
			
			nodeP->setType(term->type);
			nodeP->setPriority(term->priority);

			const ProcessMethod& method = procesingMethods_[static_cast<unsigned char>(nodeP->getType())];
			(this->*method)(nodeP.get(), tkns, minInd);

			return nodeP;
		}
	};
}