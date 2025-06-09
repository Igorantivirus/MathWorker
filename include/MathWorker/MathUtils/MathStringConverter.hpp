#pragma once

#include <sstream>
#include <iomanip>

#include "MathDefines.hpp"

namespace mathWorker
{

	class StringConverter
	{
	public:
		virtual ~StringConverter() = default;

		virtual std::string toString(const RealType value) const = 0;
		virtual std::string toString(const ComplexType& value) const = 0;
		//Convertation to other types
	};

	class BaseStringConverter : public StringConverter
	{
	public:

		std::string toString(const RealType value) const override
		{
			std::stringstream out;
			out << std::setprecision(15) << round(value, 15);
			std::string result = out.str();
			if (result.find('.') == std::string::npos)
				return result;
			while (result.back() == '0')
				result.pop_back();
			if(result.back() == '.')
				result.pop_back();
			return result;
		}
		std::string toString(const ComplexType& value) const override
		{
			std::string real = toString(value.real());
			std::string imag = toString(value.imag());
			if (equal(imag, '0'))
				return real;
			if (equal(real, '0'))
				return (equal(imag, '1') ? "" : imag) + 'i';
			return real + (imag[0] == '-' ? "" : "+") + (equal(imag, '1') ? "" : imag) + 'i';
		}

	private:

		bool equal(const std::string& str, const char c) const
		{
			return str.size() == 1 && str[0] == c;
		}

		long double round(RealType value, unsigned int n) const
		{
			if (n > 28)
				return value;
			RealType multiplier = std::pow(10.0L, n);
			RealType rounded_value = std::round(value * multiplier);
			RealType res = rounded_value / multiplier;
			return (std::abs(res) == 0.0l) ? 0.l : res;
		}


	};

}