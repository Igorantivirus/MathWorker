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
		explicit MathParser(const Tokenizer& tokenizer);
			
		void setTokenizer(const Tokenizer& tokenizer);

		MathNodeP parse(const std::string_view& str) const;

	private:

		using ProcessMethod = void(MathParser::*)(SignatureNode*, TokenArrayP, size_t) const;

		std::reference_wrapper<const Tokenizer> tokenizer_;
		std::reference_wrapper<const Signature> signature_;

		std::array<ProcessMethod, 5> procesingMethods_{};

	private:

		void initProcessingMethods();

		size_t findTokenWithMaxPriority(std::span<Token> tkns) const;

		MathNodeP finalParse(TokenArrayP tkns) const;

		MathVector parametsParsing(Token token) const;

		void processOperatorTkns(SignatureNode* node, TokenArrayP tkns, size_t minInd) const;
		void processFunctionTkns(SignatureNode* node, TokenArrayP tkns, size_t minInd) const;
		void processRightOperatorTkns(SignatureNode* node, TokenArrayP tkns, size_t minInd) const;
		void processLeftOperatorTkns(SignatureNode* node, TokenArrayP tkns, size_t minInd) const;
		void processSpecFunctionTkns(SignatureNode* node, TokenArrayP tkns, size_t minInd) const;

		MathNodeP parseTokens(TokenArrayP tkns) const;
	};
}
