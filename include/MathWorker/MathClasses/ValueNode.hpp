#pragma once

#include "MathNode.hpp"

namespace mathWorker
{

	class ValueNode : public MathNode
	{
	public:

		#pragma region Initializing

		ValueNode() = default;
		ValueNode(const ComplexType& value);
		ValueNode(const RealType& value);
		ValueNode(const ValueNode& other);
		ValueNode(ValueNode&& other) noexcept;

		ValueNode& operator=(const ValueNode& other);
		ValueNode& operator=(ValueNode&& other) noexcept;

		#pragma endregion

		#pragma region Overrided

		std::string toString() const override;
		std::string toString(const BaseStringConverter& converter, const unsigned char) const override;
		MathNodeP clone() const override;
		MathNodeP replace(const VariableContext& variabls) const override;
		MathNodeP calculate(const Signature& context) const override;
		ComplexType getNumberForced() const override;

		#pragma endregion

		#pragma region Methods

		const ComplexType& getValue() const;
		void setValue(const ComplexType& value);

		#pragma endregion

	private:

		ComplexType value_{};

	};

}