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

		virtual std::string toString(const RealType& value) const = 0;
		virtual std::string toString(const ComplexType& value) const = 0;
		//Convertation to other types
	};

	class BaseStringConverter : public StringConverter
	{
	public:

		std::string toString(const RealType& value) const override
		{
			std::stringstream out;
			out << std::setprecision(16) << value;
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
			if (imag.size() == 1 && imag[0] == '0')
				return real;
			if (real.size() == 1 && real[0] == '0')
				return imag + 'i';
			return real + (imag[0] == '-' ? "" : "+") + imag + 'i';
		}

	};

}