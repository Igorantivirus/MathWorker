#pragma once

#include "MathStringConverter.hpp"

namespace mathWorker
{

	class LaTexStringConverter : public BaseStringConverter
	{
	public:

		std::string toString(const std::vector<std::string>& params, const std::string& term, const SignatureType type, const bool needBrackets = false) const override
		{
			if (type == SignatureType::function)
				return processFunction(params, term, type);
			if (type == SignatureType::operation)
				return needBrackets ? ("\\left(" + processOperation(params, term, type) + "\\right)") : processOperation(params, term, type);
			if (type == SignatureType::specialFunction)
				return processSFunction(params, term, type);
			return processUnareOper(params, term, type);
		}

	private:

		std::string processFunction(const std::vector<std::string>& params, const std::string& term, const SignatureType type) const
		{
			std::string res = term + "\\left(";
			if (params.empty())
				return res + "\\right)";
			res += params[0];
			for (size_t i = 1; i < params.size(); ++i)
				res += ',' + params[i];
			res += "\\right)";
			return res;
		}

		std::string processOperation(const std::vector<std::string>& params, const std::string& term, const SignatureType type) const
		{
			if (params.size() != 2)
				throw ConvertStringException("Two arguments were expected for the term \"" + term + "\".");
			if (equal(term, '/'))
				return "\\frac{" + params[0] + "}{" + params[1] + '}';
			if (equal(term, '^'))
				return params[0] + "^{" + params[1] + '}';
			if (equal(term, '*'))
				return params[0] + "\\cdot" + params[1];
			return params[0] + term + params[1];
		}

		std::string processSFunction(const std::vector<std::string>& params, const std::string& term, const SignatureType type) const
		{
			if (params.size() != 2)
				throw ConvertStringException("Two arguments were expected for the term \"" + term + "\".");
			if (term == "root")
				return "\\sqrt[" + params[0] + "]{" + params[1] + '}';
			return "\\log_{" + params[0] + "}{(" + params[1] + ")}";
		}

		std::string processUnareOper(const std::vector<std::string>& params, const std::string& term, const SignatureType type) const
		{
			if (params.size() != 1)
				throw ConvertStringException("One argument was expected for this term \"" + term + "\".");
			return type == SignatureType::unareLeft ? (term + params[0]) : (params[0] + term);
		}

	};

}