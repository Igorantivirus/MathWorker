#pragma once

#include "MathStringConverter.hpp"

namespace mathWorker
{

    enum class ComplexOutputType : unsigned char
    {
        algebraic = 0,
        trigonometric,
        indicative,
        polar
    };
    enum class AngleOutputType : bool
    {
        radians = false,
        degrees
    };

    class ComplexStringConverter : public BaseStringConverter
	{
    public:

        ComplexOutputType complexType = ComplexOutputType::algebraic;
        AngleOutputType angleType = AngleOutputType::degrees;
        unsigned char precession = 6;

	public:

		std::string toString(const RealType value) const override
		{
			std::stringstream out;
			out << std::setprecision(precession) << round(value, precession);
			std::string result = out.str();
			if (result.find('.') == std::string::npos)
				return result;
			while (result.back() == '0')
				result.pop_back();
			if (result.back() == '.')
				result.pop_back();
			return result;
		}
		std::string toString(const ComplexType& value) const override
		{
            if(complexType == ComplexOutputType::algebraic)
                return BaseStringConverter::toString(value);
            auto [rad, arg] = toRadiusAndArgument(value);
            if(complexType == ComplexOutputType::trigonometric)
                return rad + "(cos(" + arg + ")+i*sin(" + arg + "))";
            else if(complexType == ComplexOutputType::indicative)
                return rad + "*e^(i*" + arg + ")";
            else if(complexType == ComplexOutputType::polar)
                return '(' + rad + '|' + arg + ')';
            return "nan+nani";
		}

    private:

        std::pair<std::string, std::string> toRadiusAndArgument(const ComplexType& value) const
        {
            RealType rad = std::abs(value);
            RealType arg = std::arg(value);

            std::string radius = toString(rad);
            std::string angle;
            if(angleType == AngleOutputType::radians)
                angle = toString(arg);
            else
                angle = toString(arg * RealType(180 / 3.1415926535897932l));
            return std::make_pair(radius, angle);
        }

	};
}