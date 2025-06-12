#pragma once

#include "../MathUtils/MathDefines.hpp"
#include "../MathUtils/MathStringConverter.hpp"
#include "../Signature/Signature.hpp"

namespace mathWorker
{

	class MathNode
	{
	public:
		virtual ~MathNode() = default;

		virtual std::string toString() const = 0;
		virtual std::string toString(const BaseStringConverter&) const = 0;

		virtual std::unique_ptr<MathNode> clone() const = 0;

		virtual std::unique_ptr<MathNode> replace(const VariableContext&) const = 0;
		virtual std::unique_ptr<MathNode> calculate(const Signature&) const = 0;

		virtual ComplexType getNumberForced() const = 0;

	};

	

}