#pragma once

#include "../Parse/Parser.hpp"

namespace mathWorker
{

	class FunctionConnector
	{
	public:

		void addFunction(Signature& signature, const std::string& name, std::vector<std::string> paramNames, const std::string realization)
		{
			BaseTokenizer tokenizer(signature);
			MathParser parser(tokenizer);

			MathNodeP node = parser.parse(realization)->replace(signature.getVariableContext());

			MatherRealization pair;
			pair.first = std::move(node);
			pair.second = std::move(paramNames);

			signature.addFunction(name, std::move(pair));
		}

		void addConstant(Signature& signature, const std::string& name, const std::string realization)
		{
			BaseTokenizer tokenizer(signature);
			MathParser parser(tokenizer);

			MathNodeP node = parser.parse(realization)->replace(signature.getVariableContext());
			signature.addConstant(name, std::move(node));
		}

		bool addFunction(Signature& signature, const std::string& fStr)
		{
			size_t equalInd = fStr.find('=');
			if (equalInd == std::string::npos)
				return false;

			size_t firstOpenBracket = fStr.find('(');
			if (firstOpenBracket == std::string::npos)
				return false;

			BaseTokenizer tokenizer(signature);

			std::string paramsStr = fStr.substr(firstOpenBracket + 1, (equalInd - 1) - (firstOpenBracket + 1));
			TokenArray paramsP = tokenizer.tokenizeByComma(paramsStr);
			std::vector<std::string> params(paramsP.begin(), paramsP.end());

			addFunction(signature, fStr.substr(0, firstOpenBracket), std::move(params), fStr.substr(equalInd + 1));
			return true;
		}

		bool addConstant(Signature& signature, const std::string& cStr)
		{
			size_t equalInd = cStr.find('=');
			if (equalInd == std::string::npos)
				return false;

			std::string name = cStr.substr(0, equalInd);
			std::string realization = cStr.substr(equalInd + 1);
			addConstant(signature, name, realization);
			return true;
		}

		bool addTerm(Signature& signature, const std::string& str)
		{
			std::size_t equalInd = str.find('=');
			if(equalInd == std::string::npos)
				return false;
			return (str.find('(') < equalInd) ? addFunction(signature, str) : addConstant(signature, str);			
		}

	private:



	};

	


}