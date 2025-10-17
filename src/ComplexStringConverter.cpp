#include "MathWorker/StringConvert/ComplexStringConverter.hpp"

namespace mathWorker
{

	std::string ComplexStringConverter::toString(RealType value) const
	{
		std::stringstream out;
		out << std::setprecision(precession) << round(value, precession);
		std::string result = out.str();
		if (result.find('.') == std::string::npos)
			return result;
		while (!result.empty() && result.back() == '0')
			result.pop_back();
		if (!result.empty() && result.back() == '.')
			result.pop_back();
		return result;
	}

	std::string ComplexStringConverter::toString(const ComplexType& value) const
	{
		if (complexType == ComplexOutputType::algebraic)
			return BaseStringConverter::toString(value);
		auto [rad, arg] = toRadiusAndArgument(value);
		if (complexType == ComplexOutputType::trigonometric)
			return rad + "(cos(" + arg + ")+i*sin(" + arg + "))";
		if (complexType == ComplexOutputType::indicative)
			return rad + "*e^(i*" + arg + ")";
		if (complexType == ComplexOutputType::polar)
			return '(' + rad + '|' + arg + ')';
		return "nan+nani";
	}

	std::pair<std::string, std::string> ComplexStringConverter::toRadiusAndArgument(const ComplexType& value) const
	{
		RealType rad = std::abs(value);
		RealType arg = std::arg(value);

		std::string radius = toString(rad);
		std::string angle;
		if (angleType == AngleOutputType::radians)
			angle = toString(arg);
		else
			angle = toString(arg * RealType(180 / 3.1415926535897932l));
		return std::make_pair(radius, angle);
	}

}
