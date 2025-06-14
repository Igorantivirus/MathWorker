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

		virtual std::string toString(const RealType value) const
		{
			std::stringstream out;
			out << std::setprecision(15) << round(value, 15);
			std::string result = out.str();
			if (result.find('.') == std::string::npos)
				return result;
			while (result.back() == '0')
				result.pop_back();
			if (result.back() == '.')
				result.pop_back();
			return result;
		}
		virtual std::string toString(const ComplexType& value) const
		{
			std::string real = toString(value.real());
			std::string imag = toString(value.imag());
			if (equal(imag, '0'))
				return real;
			if (equal(real, '0'))
				return (equal(imag, '1') ? "" : imag) + 'i';
			return real + (imag[0] == '-' ? "" : "+") + (equal(imag, '1') ? "" : imag) + 'i';
		}
		virtual std::string toString(const std::vector<std::string>& params, const std::string& term, const SignatureType type, const bool needBrackets) const
		{
			std::string res = term + '(';
			if (params.empty())
				return res + ')';
			res += params[0];
			for (size_t i = 1; i < params.size(); ++i)
				res += ',' + params[i];
			res += ')';
			return res;
		}
		//Convertation to other types

	protected:

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

	/*class BaseStringConverter : public StringConverter
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

	


	};*/

}