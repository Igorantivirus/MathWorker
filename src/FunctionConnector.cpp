#include "MathWorker/Signature/FunctionConnector.hpp"

#include <utility>

namespace mathWorker
{

	BufferedSignatureConnector::BufferedSignatureConnector(Signature& signature)
		: signature_{ signature }
		, tokenizer_{ signature }
		, parser_{ tokenizer_ }
	{
	}

	void BufferedSignatureConnector::addConstant(const std::string& name, const std::string& realization)
	{
		constants_[name] = realization;
	}

	void BufferedSignatureConnector::addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization)
	{
		functions_[name] = std::make_pair(std::move(paramNames), realization);
	}

	bool BufferedSignatureConnector::addConstant(const std::string& cStr)
	{
		size_t equalInd = cStr.find('=');
		if (equalInd == std::string::npos)
			return false;

		std::string name = cStr.substr(0, equalInd);
		std::string realization = cStr.substr(equalInd + 1);

		if (isSimpleMathString(realization))
		{
			addFullConstant(name, realization);
			return true;
		}

		addConstant(name, realization);
		return true;
	}

	bool BufferedSignatureConnector::addFunction(const std::string& fStr)
	{
		size_t equalInd = fStr.find('=');
		if (equalInd == std::string::npos)
			return false;

		size_t firstOpenBracket = fStr.find('(');
		if (firstOpenBracket == std::string::npos)
			return false;

		BaseTokenizer tokenizer(signature_.get());

		std::string paramsStr = fStr.substr(firstOpenBracket + 1, (equalInd - 1) - (firstOpenBracket + 1));
		TokenArray paramsP = tokenizer.tokenizeByComma(paramsStr);
		std::vector<std::string> params(paramsP.begin(), paramsP.end());

		addFunction(fStr.substr(0, firstOpenBracket), std::move(params), fStr.substr(equalInd + 1));
		return true;
	}

	bool BufferedSignatureConnector::addTerm(const std::string& str)
	{
		std::size_t equalInd = str.find('=');
		if (equalInd == std::string::npos)
			return false;
		return (str.find('(') < equalInd) ? addFunction(str) : addConstant(str);
	}

	void BufferedSignatureConnector::fillSignature() const
	{
		for (const auto& [name, realization] : functions_)
		{
			MatherRealization pr;
			pr.first = nullptr;
			pr.second = realization.first;
			signature_.get().addFunction(name, std::move(pr));
		}
		for (const auto& [name, realization] : constants_)
			signature_.get().addConstant(name, nullptr);

		addConstants();
		addFunctions();
	}

	void BufferedSignatureConnector::setSimpleSymbols(const std::string& str)
	{
		for (unsigned char c : str)
			lookup_[c] = true;
	}

	void BufferedSignatureConnector::addConstants() const
	{
		for (const auto& [name, realization] : constants_)
		{
			MathNodeP node = parser_.parse(realization)->replace(signature_.get().getVariableContext());
			signature_.get().addConstant(name, std::move(node));
		}
	}

	void BufferedSignatureConnector::addFunctions() const
	{
		for (const auto& [name, realization] : functions_)
		{
			MathNodeP node = parser_.parse(realization.second)->replace(signature_.get().getVariableContext());

			MatherRealization pr;
			pr.first = std::move(node);
			pr.second = realization.first;
			signature_.get().addFunction(name, std::move(pr));
		}
	}

	void BufferedSignatureConnector::addFullConstant(const std::string& name, const std::string& realization)
	{
		MathNodeP node = parser_.parse(realization)->replace(signature_.get().getVariableContext());
		signature_.get().addConstant(name, std::move(node));
	}

	bool BufferedSignatureConnector::isSimpleMathString(const std::string& str)
	{
		for (unsigned char c : str)
			if (!lookup_[c])
				return false;
		return true;
	}

	DirectSignatureConnector::DirectSignatureConnector(Signature& signature)
		: signature_{ signature }
	{
	}

	void DirectSignatureConnector::addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization)
	{
		BaseTokenizer tokenizer(signature_.get());
		MathParser parser(tokenizer);

		MathNodeP node = parser.parse(realization)->replace(signature_.get().getVariableContext());

		MatherRealization pair;
		pair.first = std::move(node);
		pair.second = std::move(paramNames);

		signature_.get().addFunction(name, std::move(pair));
	}

	void DirectSignatureConnector::addConstant(const std::string& name, const std::string realization)
	{
		BaseTokenizer tokenizer(signature_);
		MathParser parser(tokenizer);

		MathNodeP node = parser.parse(realization)->replace(signature_.get().getVariableContext());
		signature_.get().addConstant(name, std::move(node));
	}

	bool DirectSignatureConnector::addFunction(const std::string& fStr)
	{
		size_t equalInd = fStr.find('=');
		if (equalInd == std::string::npos)
			return false;

		size_t firstOpenBracket = fStr.find('(');
		if (firstOpenBracket == std::string::npos)
			return false;

		BaseTokenizer tokenizer(signature_);

		std::string paramsStr = fStr.substr(firstOpenBracket + 1, (equalInd - 1) - (firstOpenBracket + 1));
		TokenArray paramsP = tokenizer.tokenizeByComma(paramsStr);
		std::vector<std::string> params(paramsP.begin(), paramsP.end());

		addFunction(fStr.substr(0, firstOpenBracket), std::move(params), fStr.substr(equalInd + 1));
		return true;
	}

	bool DirectSignatureConnector::addConstant(const std::string& cStr)
	{
		size_t equalInd = cStr.find('=');
		if (equalInd == std::string::npos)
			return false;

		std::string name = cStr.substr(0, equalInd);
		std::string realization = cStr.substr(equalInd + 1);
		addConstant(name, realization);
		return true;
	}

	bool DirectSignatureConnector::addTerm(const std::string& str)
	{
		std::size_t equalInd = str.find('=');
		if (equalInd == std::string::npos)
			return false;
		return (str.find('(') < equalInd) ? addFunction(str) : addConstant(str);
	}

	void DirectSignatureConnector::changeSignature(Signature& signature)
	{
		signature_ = signature;
	}

}
