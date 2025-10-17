#include "MathWorker/MathClasses/SignatureNode.hpp"

#include <algorithm>
#include <cmath>

namespace mathWorker
{

	SignatureNode::SignatureNode() = default;

	SignatureNode::SignatureNode(const std::string& name, SignatureType type)
		: type_{ type }, name_{ name }
	{
	}

	SignatureNode::SignatureNode(const std::string& name, const MathVector& params, SignatureType type)
		: type_{ type }, name_{ name }
	{
		setParams(params);
	}

	SignatureNode::SignatureNode(const SignatureNode& other)
		: type_{ other.type_ }, name_{ other.name_ }
	{
		setParams(other.params_);
	}

	SignatureNode::SignatureNode(SignatureNode&& other) noexcept
		: type_{ other.type_ }, name_{ std::move(other.name_) }, params_{ std::move(other.params_) }
	{
	}

	SignatureNode& SignatureNode::operator=(const SignatureNode& other)
	{
		if (this == &other)
			return *this;
		name_ = other.name_;
		type_ = other.type_;
		setParams(other.params_);
		return *this;
	}

	SignatureNode& SignatureNode::operator=(SignatureNode&& other) noexcept
	{
		if (this == &other)
			return *this;
		name_ = std::move(other.name_);
		type_ = other.type_;
		params_ = std::move(other.params_);
		return *this;
	}

	std::string SignatureNode::toString() const
	{
		std::string res = name_ + '(';
		if (params_.empty())
			return res + ')';
		for (size_t i = 0; i + 1 < params_.size(); ++i)
			res += params_[i]->toString() + ',';
		res += params_.back()->toString() + ')';
		return res;
	}

	std::string SignatureNode::toString(const BaseStringConverter& converter, unsigned char prevProiority) const
	{
		std::vector<std::string> params;
		params.reserve(params_.size());
		for (const auto& i : params_)
			params.push_back(i->toString(converter, priority_));

		return converter.toString(params, name_, type_, prevProiority > priority_);
	}

	MathNodeP SignatureNode::clone() const
	{
		return std::make_unique<SignatureNode>(*this);
	}

	MathNodeP SignatureNode::replace(const VariableContext& variabls) const
	{
		std::unique_ptr<SignatureNode> result = std::make_unique<SignatureNode>(name_, type_);
		result->params_.reserve(params_.size());
		for (const auto& i : params_)
			result->params_.push_back(i->replace(variabls));
		return result;
	}

	MathNodeP SignatureNode::calculate(const Signature& signature) const
	{
		const auto realization = signature[name_];
		if (!realization)
			return std::make_unique<SignatureNode>(*this);

		MathVector params;
		params.reserve(params_.size());

		if (realization->policy == ArgEvalPolicy::lazy)
		{
			for (const auto& i : params_)
				params.push_back(i->clone());
		}
		else
		{
			for (const auto& i : params_)
				params.push_back(i->calculate(signature));
		}

		const NativeRealization* native_func_ptr = std::get_if<NativeRealization>(&realization->realization);
		const MatherRealization* mather_node_ptr = std::get_if<MatherRealization>(&realization->realization);

		if (native_func_ptr)
			return (*native_func_ptr)(params);
		if (mather_node_ptr)
		{
			VariableContext varContext;
			for (size_t i = 0; i < std::min(params.size(), mather_node_ptr->second.size()); ++i)
				varContext[mather_node_ptr->second[i]] = params[i]->clone();

			MathNodeP newRes = mather_node_ptr->first->replace(varContext);

			return newRes->calculate(signature);
		}
		return std::make_unique<SignatureNode>(*this);
	}

	ComplexType SignatureNode::getNumberForced() const
	{
		return ComplexType(std::nanl(""), std::nanl(""));
	}

	void SignatureNode::setName(const std::string& str)
	{
		name_ = str;
	}

	void SignatureNode::setParams(const MathVector& params)
	{
		params_.clear();
		params_.reserve(params.size());
		for (const auto& i : params)
			if (i != nullptr)
				params_.push_back(i->clone());
	}

	void SignatureNode::setType(SignatureType type)
	{
		type_ = type;
	}

	unsigned char SignatureNode::getPriority() const
	{
		return priority_;
	}

	void SignatureNode::setPriority(unsigned char priority)
	{
		priority_ = priority;
	}

	SignatureType SignatureNode::getType() const
	{
		return type_;
	}

	const MathVector& SignatureNode::getParams() const
	{
		return params_;
	}

	void SignatureNode::addParam(const MathNodeP& param)
	{
		if (param != nullptr)
			params_.push_back(param->clone());
	}

	void SignatureNode::addParam(const MathNode& param)
	{
		params_.push_back(param.clone());
	}

	void SignatureNode::addParams(const MathVector& params)
	{
		params_.reserve(params_.size() + params.size());
		for (const auto& i : params)
			if (i != nullptr)
				params_.push_back(i->clone());
	}

}
