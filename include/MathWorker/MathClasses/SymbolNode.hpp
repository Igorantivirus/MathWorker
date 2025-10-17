#pragma once

#include "MathNode.hpp"

namespace mathWorker
{

	class SymbolNode : public MathNode
	{
	public:

		#pragma region Initializing

		SymbolNode() = default;
		explicit SymbolNode(const std::string& name);
		SymbolNode(const SymbolNode& other) = default;
		SymbolNode(SymbolNode&& other) noexcept = default;

		SymbolNode& operator=(const SymbolNode& other);
		SymbolNode& operator=(SymbolNode&& other) noexcept;

		#pragma endregion

		#pragma	region Overrided

		std::string toString() const override;
		std::string toString(const BaseStringConverter& converter, unsigned char) const override;
		MathNodeP clone() const override;
		MathNodeP replace(const VariableContext& context) const override;
		MathNodeP calculate(const Signature& context) const override;
		ComplexType getNumberForced() const override;

		#pragma	endregion

		#pragma region Methods

		const std::string& getName() const;
		void setName(const std::string& name);

		#pragma endregion

	private:

		std::string name_;

	};

}
