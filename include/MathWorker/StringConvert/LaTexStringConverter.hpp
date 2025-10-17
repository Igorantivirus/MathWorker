#pragma once

#include "MathStringConverter.hpp"

namespace mathWorker
{

	class LaTexStringConverter : public BaseStringConverter
	{
	public:

		std::string toString(const std::vector<std::string>& params, const std::string& term, SignatureType type, bool needBrackets = false) const override;

	private:

		std::string processFunction(const std::vector<std::string>& params, const std::string& term, SignatureType type) const;
		std::string processOperation(const std::vector<std::string>& params, const std::string& term, SignatureType type) const;
		std::string processSFunction(const std::vector<std::string>& params, const std::string& term, SignatureType type) const;
		std::string processUnareOper(const std::vector<std::string>& params, const std::string& term, SignatureType type) const;

	};

}
