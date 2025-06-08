#pragma once

#include "../Parse/Parser.hpp"

namespace mathWorker
{

	class FunctionConnector
	{
	public:

		void addFunction(Signature& signature, const std::string& name, const std::vector<std::string>& paramNames, const std::string realization)
		{
			BaseTokenizer tokenizer(&signature);
			MathParser parser(&signature, &tokenizer);

			MathNodeP node = parser.parse(realization);

			//std::cout << node->toString() << '\n';

			MatherRealization pair;
			pair.first = std::move(node);
			pair.second = paramNames;

			signature.addFunction(name, std::move(pair));
		}

		bool addFunction(Signature& signature, const std::string& fStr)
		{
			size_t equalInd = fStr.find('=');
			if (equalInd == std::string::npos)
				return false;

			size_t firstOpenBracket = fStr.find('(');
			if (firstOpenBracket == std::string::npos)
				return false;

			BaseTokenizer tokenizer(&signature);

			std::string paramsStr = fStr.substr(firstOpenBracket + 1, (equalInd - 1) - (firstOpenBracket + 1));
			TokenArray paramsP = tokenizer.tokenizeByComma(paramsStr);
			std::vector<std::string> params(paramsP.begin(), paramsP.end());

			addFunction(signature, fStr.substr(0, firstOpenBracket), params, fStr.substr(equalInd + 1));
			return true;
		}

	private:



	};

	


}