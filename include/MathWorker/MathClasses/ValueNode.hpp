#pragma once

#include "MathNode.hpp"

namespace mathWorker
{

	class ValueNode : public MathNode
	{
	public:

		#pragma region Initializing

		ValueNode() = default;
		ValueNode(const ComplexType& value) :
			value_{ value }
		{}
		ValueNode(const RealType& value) :
			value_{ value, 0 }
		{}
		ValueNode(const ValueNode& other) :
			value_{other.value_}
		{}
		ValueNode(ValueNode&& other) noexcept :
			value_{ std::move(other.value_) }
		{}

		ValueNode& operator=(const ValueNode& other)
		{
			value_ = other.value_;
			return *this;
		}
		ValueNode& operator=(ValueNode&& other) noexcept
		{
			value_ = std::move(other.value_);
			return *this;
		}

		#pragma endregion

		#pragma region Overrided

		std::string toString() const override
		{
			BaseStringConverter converter;
			return converter.toString(value_);
		}
		std::string toString(const BaseStringConverter& converter) const override
		{
			return converter.toString(value_);
		}
		MathNodeP clone() const override
		{
			return std::make_unique<ValueNode>(*this);
		}
		MathNodeP replace(const VariableContext& variabls) const override
		{
			return std::make_unique<ValueNode>(*this);
		}
		MathNodeP calculate(const Signature& context) const override
		{
			return std::make_unique<ValueNode>(*this);
		}
		ComplexType getNumberForced() const override
		{
			return value_;
		}

		#pragma endregion

		#pragma region Methods

		const ComplexType& getValue() const
		{
			return value_;
		}
		void setValue(const ComplexType& value)
		{
			value_ = value;
		}

		#pragma endregion

	private:

		ComplexType value_{};

	};

}