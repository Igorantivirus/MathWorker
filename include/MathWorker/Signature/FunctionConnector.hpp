#pragma once

#include "../Parse/Parser.hpp"

namespace mathWorker
{

	class SignatureConnector
	{
	public:
		SignatureConnector(Signature& signature) :
			signature_{signature}
		{}

		void addConstant(const std::string& name, const std::string& realization)
		{
			constants_[name] = realization;
		}
		void addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization)
		{
			functions_[name] = std::make_pair(paramNames, realization);
		}

		bool addConstant(const std::string& cStr)
		{
			size_t equalInd = cStr.find('=');
			if (equalInd == std::string::npos)
				return false;

			std::string name = cStr.substr(0, equalInd);
			std::string realization = cStr.substr(equalInd + 1);
			addConstant(name, realization);
			return true;
		}
		bool addFunction(const std::string& fStr)
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

		bool addTerm(const std::string& str)
		{
			std::size_t equalInd = str.find('=');
			if(equalInd == std::string::npos)
				return false;
			return (str.find('(') < equalInd) ? addFunction(str) : addConstant(str);		
		}

		void fillSignature() const
		{
			BaseTokenizer tokenizer(signature_);
			MathParser parser(tokenizer);

			//ДОбавлены заголовки функций
			for(const auto& [name, realization] : functions_)
			{
				MatherRealization pr;
				pr.first = nullptr;
				pr.second = realization.first;
				signature_.get().addFunction(name, std::move(pr));
			}
			//Добавление заголовок констант
			for(const auto& [name, realization] : constants_)
				signature_.get().addConstant(name, nullptr);
			
			addConstants(parser);
			addFunctions(parser);
		}

	private:

		// using;

	private:

		std::reference_wrapper<Signature> signature_;

		std::map<std::string, std::string> constants_;
		std::map<std::string, std::pair<std::vector<std::string>, std::string>> functions_;

	private:

		void addConstants(const MathParser& parser) const
		{
			for(const auto& [name, realization] : constants_)
			{
				MathNodeP node = parser.parse(realization)->replace(signature_.get().getVariableContext());
				signature_.get().addConstant(name, std::move(node));
			}
		}
		void addFunctions(const MathParser& parser) const
		{
			for(const auto& [name, realization] : functions_)
			{
				MathNodeP node = parser.parse(realization.second)->replace(signature_.get().getVariableContext());

				MatherRealization pr;
				pr.first = std::move(node);
				pr.second = realization.first;
				signature_.get().addFunction(name, std::move(pr));
			}
		}

	};

	class FunctionConnector
	{
	public:
		FunctionConnector(Signature& signature) :
			signature_{signature}
		{}

		void addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization)
		{
			BaseTokenizer tokenizer(signature_.get());
			MathParser parser(tokenizer);

			MathNodeP node = parser.parse(realization)->replace(signature_.get().getVariableContext());

			MatherRealization pair;
			pair.first = std::move(node);
			pair.second = std::move(paramNames);

			signature_.get().addFunction(name, std::move(pair));
		}

		void addConstant(const std::string& name, const std::string realization)
		{
			BaseTokenizer tokenizer(signature_);
			MathParser parser(tokenizer);

			MathNodeP node = parser.parse(realization)->replace(signature_.get().getVariableContext());
			signature_.get().addConstant(name, std::move(node));
		}

		bool addFunction(const std::string& fStr)
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

		bool addConstant(const std::string& cStr)
		{
			size_t equalInd = cStr.find('=');
			if (equalInd == std::string::npos)
				return false;

			std::string name = cStr.substr(0, equalInd);
			std::string realization = cStr.substr(equalInd + 1);
			addConstant(name, realization);
			return true;
		}

		bool addTerm(const std::string& str)
		{
			std::size_t equalInd = str.find('=');
			if(equalInd == std::string::npos)
				return false;
			return (str.find('(') < equalInd) ? addFunction(str) : addConstant(str);			
		}

	private:

		std::reference_wrapper<Signature> signature_;

	};

	


}