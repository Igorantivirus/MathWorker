#pragma once

#include <map>
#include <string>
#include <vector>

#include "../Parse/Parser.hpp"

namespace mathWorker
{
	class [[deprecated("Use the ABOBA class better, it has a better chance of not making an error.")]] BufferedSignatureConnector
	{
	public:
		explicit BufferedSignatureConnector(Signature& signature);

		void addConstant(const std::string& name, const std::string& realization);
		void addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization);

		bool addConstant(const std::string& cStr);
		bool addFunction(const std::string& fStr);

		bool addTerm(const std::string& str);

		void fillSignature() const;

		void setSimpleSymbols(const std::string& str);

	private:
		void addConstants() const;
		void addFunctions() const;
		void addFullConstant(const std::string& name, const std::string& realization);
		bool isSimpleMathString(const std::string& str);

		std::reference_wrapper<Signature> signature_;

		std::map<std::string, std::string> constants_;
		std::map<std::string, std::pair<std::vector<std::string>, std::string>> functions_;

		BaseTokenizer tokenizer_;
		MathParser parser_;

		std::string simpleSymbols_;

		bool lookup_[256]{};
	};

	class DirectSignatureConnector
	{
	public:
		explicit DirectSignatureConnector(Signature& signature);

		void addFunction(const std::string& name, std::vector<std::string> paramNames, const std::string& realization);
		void addConstant(const std::string& name, const std::string realization);

		bool addFunction(const std::string& fStr);
		bool addConstant(const std::string& cStr);

		bool addTerm(const std::string& str);

		void changeSignature(Signature& signature);

	private:

		std::reference_wrapper<Signature> signature_;

	};

}
