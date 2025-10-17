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

		std::string toString(const RealType value) const override;
		std::string toString(const ComplexType& value) const override;

    private:

        std::pair<std::string, std::string> toRadiusAndArgument(const ComplexType& value) const;

	};
}
