#pragma once

#include<vector>
#include<iostream>

#include "MathNode.hpp"

namespace mathWorker
{

	enum class SignatureType : unsigned char
	{
		operation = 0,
		function,
		unare,
		nameFunction
	};

	class SignatureNode : public MathNode
	{
	public:
		SignatureNode() = default;
		SignatureNode(const std::string& name, const std::vector<MathNodeP>& params, const SignatureType type = SignatureType::function) :
			name_{ name }, type_{type}
		{
			setParams(params);
		}

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
			std::unique_ptr<SignatureNode> result(new SignatureNode(name_, {}, type_));
			result->params_.reserve(params_.size());
			for (const auto& i : params_)
				result->params_.push_back(i->replace(variabls));
			return std::move(result);
		}
		MathNodeP calculate(const FunctionContext& context) const override
		{
			std::vector<MathNodeP> params;
			params.reserve(params_.size());

			for (const auto& i : params_)
				params.push_back(std::move(i->calculate(context)));


			const auto& found = context.find(name_);
			if(found == context.end())
				return std::make_unique< SignatureNode>(name_, params, type_);

			const NativeRealization* native_func_ptr = std::get_if<NativeRealization>(&found->second);
			const MatherRealization* mather_node_ptr = std::get_if<MatherRealization>(&found->second);

			if (native_func_ptr)
				return (*native_func_ptr)(params);
			if (mather_node_ptr)
			{
				VariableContext varContext;

				for (size_t i = 0; i < std::min(params.size(), mather_node_ptr->second.size()); ++i)
					varContext[mather_node_ptr->second[i]] = params[i]->clone();

				MathNodeP newRes = mather_node_ptr->first->replace(varContext);
				std::cout << "NewRes: " << newRes->toString() << '\n';
				/*return 

				MathNodeP newRes = replace(varContext);

				*/

				return std::move(newRes->calculate(context));
			}
			return nullptr;
		}
		ComplexType getNumberForced() const override
		{
			return ComplexType(std::nanl(""), std::nanl(""));
		}

		void setParams(const std::vector<MathNodeP>& params)
		{
			params_.clear();
			params_.reserve(params.size());
			for (const auto& i : params)
				params_.push_back(std::move(i->clone()));
		}

		const SignatureType getType() const
		{
			return type_;
		}
		

	private:

		SignatureType type_ = SignatureType::function;

		std::string name_;

		std::vector<MathNodeP> params_;

	};

}