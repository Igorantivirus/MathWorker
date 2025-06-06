#pragma once

#include <vector>

#include "MathNode.hpp"

namespace mathWorker
{

	class SignatureNode : public MathNode
	{
	public:

		#pragma region Initializing

		SignatureNode() = default;
		SignatureNode(const std::string& name, const MathVector& params, const SignatureType type = SignatureType::function) :
			name_{ name }, type_{ type }
		{
			setParams(params);
		}
		SignatureNode(const std::string& name, const MathRowVector& params, const SignatureType type = SignatureType::function) :
			name_{ name }, type_{ type }
		{
			setParams(params);
		}
		SignatureNode(const SignatureNode& other) :
			name_{ other.name_ }, type_{other.type_}
		{
			setParams(other.params_);
		}
		SignatureNode(SignatureNode&& other) noexcept :
			name_{ std::move(other.name_) }, type_{ std::move(other.type_) }, params_{std::move(other.params_)}
		{}

		SignatureNode& operator=(const SignatureNode& other)
		{
			if(this == &other)
				return *this;
			name_ = other.name_;
			type_ = other.type_;
			setParams(other.params_);
			return *this;
		}
		SignatureNode& operator=(SignatureNode&& other) noexcept
		{
			if (this == &other)
				return *this;
			name_ = std::move(other.name_);
			type_ = std::move(other.type_);
			params_ = std::move(other.params_);
			return *this;
		}

		#pragma endregion

		#pragma region Overrided

		std::string toString() const override
		{
			std::string res = name_ + '(';
			if (params_.empty())
				return res + ')';
			for (size_t i = 0; i < params_.size() - 1; ++i)
				res += params_[i]->toString() + ',';
			res += params_[params_.size() - 1]->toString() + ')';
			return res;
		}
		std::string toString(const StringConverter& converter) const override
		{
			std::string res = name_ + '(';
			for (size_t i = 0; i < params_.size() - 1; ++i)
				res += params_[i]->toString(converter) + ',';
			res += params_[params_.size() - 1]->toString(converter) + ')';
			return res;
		}
		MathNodeP clone() const override
		{
			return std::make_unique< SignatureNode>(name_, params_, type_);
		}

		MathNodeP replace(const VariableContext& variabls) const override
		{
			std::unique_ptr<SignatureNode> result(new SignatureNode(name_, MathRowVector{}, type_));
			result->params_.reserve(params_.size());
			for (const auto& i : params_)
				result->params_.push_back(i->replace(variabls));
			return std::move(result);
		}
		MathNodeP calculate(const SignatureContext& context) const override
		{
			MathVector params;
			params.reserve(params_.size());

			for (const auto& i : params_)
				params.push_back(std::move(i->calculate(context)));

			const auto& found = context.find(name_);
			if(found == context.end())
				return std::make_unique<SignatureNode>(*this);

			const NativeRealization* native_func_ptr = std::get_if<NativeRealization>(&found->second.realization);
			const MatherRealization* mather_node_ptr = std::get_if<MatherRealization>(&found->second.realization);

			if (native_func_ptr)
				return (*native_func_ptr)(params);
			if (mather_node_ptr)
			{
				VariableContext varContext;

				for (size_t i = 0; i < std::min(params.size(), mather_node_ptr->second.size()); ++i)
					varContext[mather_node_ptr->second[i]] = params[i]->clone();

				MathNodeP newRes = mather_node_ptr->first->replace(varContext);
				
				return std::move(newRes->calculate(context));
			}
			return nullptr;
		}
		ComplexType getNumberForced() const override
		{
			return ComplexType(std::nanl(""), std::nanl(""));
		}

		#pragma endregion

		#pragma region Methods

		void setName(const std::string& str)
		{
			name_ = str;
		}

		void setParams(const MathVector& params)
		{
			params_.clear();
			params_.reserve(params.size());
			for (const auto& i : params)
				if(i != nullptr)
					params_.push_back(std::move(i->clone()));
		}
		void setParams(const MathRowVector& params)
		{
			params_.clear();
			params_.reserve(params.size());
			for (const auto& i : params)
				if(i != nullptr)
					params_.push_back(std::move(i->clone()));
		}

		const SignatureType getType() const
		{
			return type_;
		}
		
		void addParam(const MathNodeP& param)
		{
			if (param != nullptr)
				params_.push_back(std::move(param->clone()));
		}
		void addParam(const MathNode& param)
		{
			params_.push_back(std::move(param.clone()));
		}

		#pragma endregion

	private:

		SignatureType type_ = SignatureType::function;

		std::string name_;

		MathVector params_;

	};

}