#include "SymbolNode.hpp"

namespace mathWorker
{

#pragma region Initializing

	SymbolNode::SymbolNode(const std::string &name) : name_{name}
	{
	}
	SymbolNode::SymbolNode(const SymbolNode &other) : name_{other.name_}
	{
	}
	SymbolNode::SymbolNode(SymbolNode &&other) noexcept : name_{std::move(other.name_)}
	{
	}

	SymbolNode &SymbolNode::operator=(const SymbolNode &other)
	{
		name_ = other.name_;
		return *this;
	}
	SymbolNode &SymbolNode::operator=(SymbolNode &&other) noexcept
	{
		name_ = std::move(other.name_);
		return *this;
	}

#pragma endregion

#pragma region Overrided

	std::string SymbolNode::toString() const
	{
		return name_;
	}
	std::string SymbolNode::toString(const BaseStringConverter &converter, const unsigned char) const
	{
		return name_;
	}
	MathNodeP SymbolNode::clone() const
	{
		return std::make_unique<SymbolNode>(*this);
	}
	MathNodeP SymbolNode::replace(const VariableContext &context) const
	{
		const auto found = context.find(name_);
		if (found == context.end())
			return std::make_unique<SymbolNode>(*this);
		return found->second->clone();
	}
	MathNodeP SymbolNode::calculate(const Signature &context) const
	{
		return std::make_unique<SymbolNode>(*this);
	}
	ComplexType SymbolNode::getNumberForced() const
	{
		return ComplexType(std::nanl(""), std::nanl(""));
	}

#pragma endregion

#pragma region Methods

	const std::string &SymbolNode::getName() const
	{
		return name_;
	}
	void SymbolNode::setName(const std::string &name)
	{
		name_ = name;
	}

#pragma endregion

}