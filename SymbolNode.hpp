#pragma once

#include "MathNode.hpp"
#include "MathDefines.hpp"

namespace mathWorker
{

	class SymbolNode : public MathNode
	{
	public:
		SymbolNode() = default;
		SymbolNode(const std::string name) :
			name_{ name }
		{}

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
			return std::make_unique<SymbolNode>(name_);
		}
		MathNodeP replace(const VariableContext& context) const override
		{
			//Заменяем на новое значение
			const auto found = context.find(name_);
			if(found == context.end())
				return std::make_unique<SymbolNode>(name_);
			return found->second->clone();
		}
		MathNodeP calculate(const FunctionContext& context) const override
		{
			return std::make_unique<SymbolNode>(name_);
		}
		ComplexType getNumberForced() const override
		{
			return ComplexType(std::nanl(""), std::nanl(""));
		}

		const std::string& getName() const
		{
			return name_;
		}
		void setName(const std::string& name)
		{
			name_ = name;
		}

	private:

		std::string name_;

	};

}