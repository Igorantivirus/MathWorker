#include <MathWorker/MathClasses/SignatureNode.hpp>

namespace mathWorker
{

#pragma region Initializing

    SignatureNode::SignatureNode(const std::string &name, const SignatureType type) : name_{name}, type_{type}
    {}
    SignatureNode::SignatureNode(const std::string &name, const MathVector &params, const SignatureType type) : name_{name}, type_{type}
    {
        setParams(params);
    }
    SignatureNode::SignatureNode(const SignatureNode &other) : name_{other.name_}, type_{other.type_}
    {
        setParams(other.params_);
    }
    SignatureNode::SignatureNode(SignatureNode &&other) noexcept : name_{std::move(other.name_)}, type_{std::move(other.type_)}, params_{std::move(other.params_)}
    {}

    SignatureNode &SignatureNode::operator=(const SignatureNode &other)
    {
        if (this == &other)
            return *this;
        name_ = other.name_;
        type_ = other.type_;
        setParams(other.params_);
        return *this;
    }
    SignatureNode &SignatureNode::operator=(SignatureNode &&other) noexcept
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

    std::string SignatureNode::toString() const
    {
        std::string res = name_ + '(';
        if (params_.empty())
            return res + ')';
        for (size_t i = 0; i < params_.size() - 1; ++i)
            res += params_[i]->toString() + ',';
        res += params_[params_.size() - 1]->toString() + ')';
        return res;
    }
    std::string SignatureNode::toString(const BaseStringConverter &converter, const unsigned char prevProiority) const
    {
        std::vector<std::string> params;
        params.reserve(params_.size());
        for (const auto &i : params_)
            params.push_back(i->toString(converter, priority_));

        return converter.toString(params, name_, type_, prevProiority > priority_);
    }
    MathNodeP SignatureNode::clone() const
    {
        return std::make_unique<SignatureNode>(*this);
    }

    MathNodeP SignatureNode::replace(const VariableContext &variabls) const
    {
        std::unique_ptr<SignatureNode> result = std::make_unique<SignatureNode>(name_, type_);
        result->params_.reserve(params_.size());
        for (const auto &i : params_)
            result->params_.push_back(i->replace(variabls));
        return std::move(result);
    }
    MathNodeP SignatureNode::calculate(const Signature &signature) const
    {
        MathVector params;
        params.reserve(params_.size());

        for (const auto &i : params_)
            params.push_back(i->calculate(signature));

        const auto realization = signature[name_];

        if (!realization)
            return std::make_unique<SignatureNode>(*this);

        const NativeRealization *native_func_ptr = std::get_if<NativeRealization>(&realization->realization);
        const MatherRealization *mather_node_ptr = std::get_if<MatherRealization>(&realization->realization);

        if (native_func_ptr)
            return (*native_func_ptr)(params);
        if (mather_node_ptr)
        {
            VariableContext varContext;
            // TODO сделать обработку ошибок
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

#pragma endregion

#pragma region Methods

    void SignatureNode::setName(const std::string &str)
    {
        name_ = str;
    }

    void SignatureNode::setParams(const MathVector &params)
    {
        params_.clear();
        params_.reserve(params.size());
        for (const auto &i : params)
            if (i != nullptr)
                params_.push_back(i->clone());
    }

    void SignatureNode::setType(const SignatureType type)
    {
        type_ = type;
    }

    const unsigned char SignatureNode::getPriority() const
    {
        return priority_;
    }
    void SignatureNode::setPriority(const unsigned char priority)
    {
        priority_ = priority;
    }

    const SignatureType SignatureNode::getType() const
    {
        return type_;
    }

    void SignatureNode::addParam(const MathNodeP &param)
    {
        if (param != nullptr)
            params_.push_back(param->clone());
    }
    void SignatureNode::addParam(const MathNode &param)
    {
        params_.push_back(param.clone());
    }
    void SignatureNode::addParams(const MathVector &params)
    {
        params_.reserve(params_.size() + params.size());
        for (const auto &i : params)
            if (i != nullptr)
                params_.push_back(i->clone());
    }

#pragma endregion

}