#pragma once

#include "MathNode.hpp"
#include "MathDefines.hpp"

namespace mathWorker
{

	class SymbolNode : public MathNode
	{
	public:

		#pragma region Initializing

		SymbolNode() = default;
		SymbolNode(const std::string name) :
			name_{ name }
		{}
		SymbolNode(const SymbolNode& other) :
			name_{ other.name_ }
		{}
		SymbolNode(SymbolNode&& other) noexcept :
			name_{ std::move(other.name_) }
		{}

		SymbolNode& operator=(const SymbolNode& other)
		{
			name_ = other.name_;
			return *this;
		}
		SymbolNode& operator=(SymbolNode&& other) noexcept
		{
			name_ = std::move(other.name_);
			return *this;
		}

		#pragma endregion

		#pragma	region Overrided

		std::string toString() const override
		{
			return name_;
		}
		std::string toString(const StringConverter& converter) const override
		{
			return name_;
		}
		MathNodeP clone() const override
		{
			return std::make_unique<SymbolNode>(*this);
		}
		MathNodeP replace(const VariableContext& context) const override
		{
			const auto found = context.find(name_);
			if(found == context.end())
				return std::make_unique<SymbolNode>(*this);
			return found->second->clone();
		}
		MathNodeP calculate(const SignatureContext& context) const override
		{
			return std::make_unique<SymbolNode>(*this);
		}
		ComplexType getNumberForced() const override
		{
			return ComplexType(std::nanl(""), std::nanl(""));
		}

		#pragma	endregion

		#pragma region Methods

		const std::string& getName() const
		{
			return name_;
		}
		void setName(const std::string& name)
		{
			name_ = name;
		}

		#pragma endregion

	private:

		std::string name_;

	};

}