#pragma once

#include "MathDefines.hpp"
#include "MathNode.hpp"

namespace mathWorker
{

	class ValueNode : public MathNode
	{
	public:
		ValueNode() = default;
		ValueNode(const ComplexType& value) :
			value_{ value }
		{}
		ValueNode(const RealType& value) :
			value_{ value, 0 }
		{}

		std::string toString() const override
		{
			BaseStringConverter converter;
			return converter.toString(value_);
		}
		std::string toString(const StringConverter& converter) const override
		{
			return converter.toString(value_);
		}
		MathNodeP clone() const override
		{
			return std::make_unique<ValueNode>(value_);
		}
		MathNodeP replace(const VariableContext& variabls) const override
		{
			//Заменять нечего - так и осталось число
			return std::make_unique<ValueNode>(value_);
		}
		MathNodeP calculate(const FunctionContext& context) const override
		{
			//Заменять нечего - так и осталось число
			return std::make_unique<ValueNode>(value_);
		}
		ComplexType getNumberForced() const override
		{
			return value_;
		}

		const ComplexType& getValue() const
		{
			return value_;
		}
		void setValue(const ComplexType& value)
		{
			value_ = value;
		}

	private:

		ComplexType value_{};

	};

}