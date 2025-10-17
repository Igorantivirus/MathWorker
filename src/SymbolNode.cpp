#include "MathWorker/MathClasses/SymbolNode.hpp"

#include <cmath>

namespace mathWorker
{

	SymbolNode::SymbolNode(const std::string& name)
		: name_{ name }
	{
	}

	SymbolNode& SymbolNode::operator=(const SymbolNode& other)
	{
		if (this != &other)
			name_ = other.name_;
		return *this;
	}

	SymbolNode& SymbolNode::operator=(SymbolNode&& other) noexcept
	{
		if (this != &other)
			name_ = std::move(other.name_);
		return *this;
	}

	std::string SymbolNode::toString() const
	{
		return name_;
	}

	std::string SymbolNode::toString(const BaseStringConverter&, unsigned char) const
	{
		return name_;
	}

	MathNodeP SymbolNode::clone() const
	{
		return std::make_unique<SymbolNode>(*this);
	}

	MathNodeP SymbolNode::replace(const VariableContext& context) const
	{
		const auto found = context.find(name_);
		if (found == context.end())
			return std::make_unique<SymbolNode>(*this);
		return found->second->clone();
	}

	MathNodeP SymbolNode::calculate(const Signature&) const
	{
		return std::make_unique<SymbolNode>(*this);
	}

	ComplexType SymbolNode::getNumberForced() const
	{
		return ComplexType(std::nanl(""), std::nanl(""));
	}

	const std::string& SymbolNode::getName() const
	{
		return name_;
	}

	void SymbolNode::setName(const std::string& name)
	{
		name_ = name;
	}

}
