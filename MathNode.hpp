#pragma once

#include "MathDefines.hpp"
#include "MathStringConverter.hpp"

namespace mathWorker
{

	class MathNode
	{
	public:
		virtual ~MathNode() = default;

		virtual std::string toString() const = 0;
		virtual std::string toString(const StringConverter&) const = 0;
		virtual std::unique_ptr<MathNode> clone() const = 0;

		virtual std::unique_ptr<MathNode> replace(const VariableContext&) const = 0;
		virtual std::unique_ptr<MathNode> calculate(const SignatureContext&) const = 0;

		virtual ComplexType getNumberForced() const = 0;

	};

	

}