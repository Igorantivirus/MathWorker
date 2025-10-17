#pragma once

#include <sstream>
#include <iomanip>
#include <vector>

#include "../MathUtils/MathDefines.hpp"
#include "ConvertStringException.hpp"

namespace mathWorker
{
	class BaseStringConverter
	{
	public:
		virtual ~BaseStringConverter() = default;

		virtual std::string toString(RealType value) const;
		virtual std::string toString(const ComplexType& value) const;
		virtual std::string toString(const std::vector<std::string>& params, const std::string& term, SignatureType type, bool needBrackets) const;
		//Convertation to other types

	protected:

		bool equal(const std::string& str, char c) const;

		long double round(RealType value, unsigned int n) const;
	};

}
