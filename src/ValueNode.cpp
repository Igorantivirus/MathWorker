#include "MathWorker/MathClasses/ValueNode.hpp"

namespace mathWorker
{

	ValueNode::ValueNode(const ComplexType& value)
		: value_{ value }
	{
	}

	ValueNode::ValueNode(const RealType& value)
		: value_{ value, 0 }
	{
	}

	ValueNode& ValueNode::operator=(const ValueNode& other)
	{
		if (this != &other)
			value_ = other.value_;
		return *this;
	}

	ValueNode& ValueNode::operator=(ValueNode&& other) noexcept
	{
		if (this != &other)
			value_ = std::move(other.value_);
		return *this;
	}

	std::string ValueNode::toString() const
	{
		BaseStringConverter converter;
		return converter.toString(value_);
	}

	std::string ValueNode::toString(const BaseStringConverter& converter, unsigned char) const
	{
		return converter.toString(value_);
	}

	MathNodeP ValueNode::clone() const
	{
		return std::make_unique<ValueNode>(*this);
	}

	MathNodeP ValueNode::replace(const VariableContext&) const
	{
		return std::make_unique<ValueNode>(*this);
	}

	MathNodeP ValueNode::calculate(const Signature&) const
	{
		return std::make_unique<ValueNode>(*this);
	}

	ComplexType ValueNode::getNumberForced() const
	{
		return value_;
	}

	const ComplexType& ValueNode::getValue() const
	{
		return value_;
	}

	void ValueNode::setValue(const ComplexType& value)
	{
		value_ = value;
	}

}
