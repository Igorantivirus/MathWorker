#include "MathWorker/StringConvert/MathStringConverter.hpp"

#include <cmath>

namespace mathWorker
{

	std::string BaseStringConverter::toString(RealType value) const
	{
		std::stringstream out;
		out << std::setprecision(6) << round(value, 6);
		std::string result = out.str();
		if (result.find('.') == std::string::npos)
			return result;
		while (!result.empty() && result.back() == '0')
			result.pop_back();
		if (!result.empty() && result.back() == '.')
			result.pop_back();
		return result;
	}

	std::string BaseStringConverter::toString(const ComplexType& value) const
	{
		std::string real = toString(value.real());
		std::string imag = toString(value.imag());
		if (equal(imag, '0'))
			return real;
		if (equal(real, '0'))
			return (equal(imag, '1') ? "" : imag) + 'i';
		return real + (imag[0] == '-' ? "" : "+") + (equal(imag, '1') ? "" : imag) + 'i';
	}

	std::string BaseStringConverter::toString(const std::vector<std::string>& params, const std::string& term, SignatureType, bool) const
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

	bool BaseStringConverter::equal(const std::string& str, char c) const
	{
		return str.size() == 1 && str[0] == c;
	}

	long double BaseStringConverter::round(RealType value, unsigned int n) const
	{
		if (n > 28)
			return value;
		RealType multiplier = std::pow(10.0L, n);
		RealType rounded_value = std::round(value * multiplier);
		RealType res = rounded_value / multiplier;
		return (std::abs(res) == 0.0l) ? 0.l : res;
	}

}
