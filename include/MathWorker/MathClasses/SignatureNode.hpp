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

		#pragma region Initializing

		SignatureNode() = default;
		SignatureNode(const std::string& name, const SignatureType type = SignatureType::function);
		SignatureNode(const std::string& name, const MathVector& params, const SignatureType type = SignatureType::function);
		/*SignatureNode(const std::string& name, const std::initializer_list<MathNodeP>&& list, const SignatureType type = SignatureType::function) :
			name_{ name }, type_{ type }, params_{std::move(list)}
		{}*/
		/*SignatureNode(const std::string& name, const MathRowVector& params, const SignatureType type = SignatureType::function) :
			name_{ name }, type_{ type }
		{
			setParams(params);
		}*/
		SignatureNode(const SignatureNode& other);
		SignatureNode(SignatureNode&& other) noexcept;

		SignatureNode& operator=(const SignatureNode& other);
		SignatureNode& operator=(SignatureNode&& other) noexcept;

		#pragma endregion

		#pragma region Overrided

		std::string toString() const override;
		std::string toString(const BaseStringConverter& converter, const unsigned char prevProiority) const override;
		MathNodeP clone() const override;

		MathNodeP replace(const VariableContext& variabls) const override;
		MathNodeP calculate(const Signature& signature) const override;
		ComplexType getNumberForced() const override;

		#pragma endregion

		#pragma region Methods

		void setName(const std::string& str);

		void setParams(const MathVector& params);

		void setType(const SignatureType type);

		const unsigned char getPriority() const;
		void setPriority(const unsigned char priority);

		const SignatureType getType() const;
		
		void addParam(const MathNodeP& param);
		void addParam(const MathNode& param);
		void addParams(const MathVector& params);

		#pragma endregion

	private:

		SignatureType type_ = SignatureType::function;
		unsigned char priority_ = 0;

		std::string name_;

		MathVector params_;

	};

}