#pragma once

#include <MathWorker/MathClasses/ValueNode.hpp>

namespace mathWorker
{

#pragma region Initializing

    ValueNode::ValueNode(const ComplexType &value) : value_{value}
    {}
    ValueNode::ValueNode(const RealType &value) : value_{value, 0}
    {}
    ValueNode::ValueNode(const ValueNode &other) : value_{other.value_}
    {}
    ValueNode::ValueNode(ValueNode &&other) noexcept : value_{std::move(other.value_)}
    {}

    ValueNode &ValueNode::operator=(const ValueNode &other)
    {
        value_ = other.value_;
        return *this;
    }
    ValueNode &ValueNode::operator=(ValueNode &&other) noexcept
    {
        value_ = std::move(other.value_);
        return *this;
    }

#pragma endregion

#pragma region Overrided

    std::string ValueNode::toString() const
    {
        BaseStringConverter converter;
        return converter.toString(value_);
    }
    std::string ValueNode::toString(const BaseStringConverter &converter, const unsigned char) const
    {
        return converter.toString(value_);
    }
    MathNodeP ValueNode::clone() const
    {
        return std::make_unique<ValueNode>(*this);
    }
    MathNodeP ValueNode::replace(const VariableContext &variabls) const
    {
        return std::make_unique<ValueNode>(*this);
    }
    MathNodeP ValueNode::calculate(const Signature &context) const
    {
        return std::make_unique<ValueNode>(*this);
    }
    ComplexType ValueNode::getNumberForced() const
    {
        return value_;
    }

#pragma endregion

#pragma region Methods

    const ComplexType &ValueNode::getValue() const
    {
        return value_;
    }
    void ValueNode::setValue(const ComplexType &value)
    {
        value_ = value;
    }

#pragma endregion

}