#pragma once

#include <vector>

#include "MathNode.hpp"

namespace mathWorker
{

	using MathVector = std::vector<MathNodeP>;
	using MathRowVector = std::vector<MathNode*>;

	class SignatureNode : public MathNode
	{
	public:
		SignatureNode();
		explicit SignatureNode(const std::string& name, SignatureType type = SignatureType::function);
		SignatureNode(const std::string& name, const MathVector& params, SignatureType type = SignatureType::function);
		SignatureNode(const SignatureNode& other);
		SignatureNode(SignatureNode&& other) noexcept;

		SignatureNode& operator=(const SignatureNode& other);
		SignatureNode& operator=(SignatureNode&& other) noexcept;

		std::string toString() const override;
		std::string toString(const BaseStringConverter& converter, unsigned char prevProiority) const override;
		MathNodeP clone() const override;

		MathNodeP replace(const VariableContext& variabls) const override;
		MathNodeP calculate(const Signature& signature) const override;
		ComplexType getNumberForced() const override;

		void setName(const std::string& str);
		void setParams(const MathVector& params);
		void setType(SignatureType type);

		unsigned char getPriority() const;
		void setPriority(unsigned char priority);

		SignatureType getType() const;
		const MathVector& getParams() const;

		void addParam(const MathNodeP& param);
		void addParam(const MathNode& param);
		void addParams(const MathVector& params);

	private:
		SignatureType type_ = SignatureType::function;
		unsigned char priority_ = 0;

		std::string name_;

		MathVector params_;
	};

}
