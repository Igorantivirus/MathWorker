#pragma once

#include <stack>
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
		MathParser(const Signature& signature, std::unique_ptr<const Tokenizer> tokenizer);

		void setsignature(const Signature& signature);
		void setTokenizer(std::unique_ptr<const Tokenizer> tokenizer);

		MathNodeP parse(const std::string_view& str) const;

	private:

		using ProcessMethod = void(MathParser::*)(SignatureNode*, const TokenArrayP, const size_t) const;

		std::reference_wrapper<const Signature> signature_;
		std::unique_ptr<const Tokenizer> tokenizer_;

		std::map<SignatureType, ProcessMethod> procesingMethods_;

	private:

		void initProcessingMethods();

		size_t findTokenWithMaxPriority(const std::span<Token> tkns) const;

		MathNodeP finalParse(const TokenArrayP tkns) const;

		MathVector parametsParsing(const Token token) const;

		void processOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const;
		void processFunctionTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const;
		void processRightOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const;
		void processLeftOperatorTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const;
		void processSpecFunctionTkns(SignatureNode* node, const TokenArrayP tkns, const size_t minInd) const;

		MathNodeP parseTokens(const TokenArrayP tkns) const;
		
	};
}