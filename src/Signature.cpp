#include "MathWorker/Signature/Signature.hpp"

#include <MathWorker/MathClasses/MathNode.hpp>

#include <utility>

namespace mathWorker
{

	Signature::Signature() = default;

	void Signature::addFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization, ArgEvalPolicy policy)
	{
		functionalContext_[name] = SignatureRealization{ std::move(realization), 0, SignatureType::function, OperatorPriority::none, policy };
	}

	void Signature::addSpecialFunction(const std::string& name, std::variant<NativeRealization, MatherRealization>&& realization)
	{
		functionalContext_[name] = SignatureRealization{ std::move(realization), 0, SignatureType::specialFunction };
	}

	void Signature::addUnareRightOperator(const std::string& name, NativeRealization&& realization)
	{
		functionalContext_[name] = SignatureRealization{ realization, 1, SignatureType::unareRight };
	}

	void Signature::addUnareLeftOperator(const std::string& name, NativeRealization&& realization)
	{
		functionalContext_[name] = SignatureRealization{ realization, 2, SignatureType::unareLeft };
	}

	void Signature::addOperator(const std::string& name, NativeRealization&& realization, unsigned char priority, OperatorPriority direction, ArgEvalPolicy policy)
	{
		functionalContext_[name] = SignatureRealization{ realization, static_cast<unsigned char>(priority + 3), SignatureType::operation, direction, policy };
	}

	void Signature::setDefaultOperator(const std::string& defautlOperator)
	{
		if (isTerm(defautlOperator))
			defaultOperator_ = defautlOperator;
	}

	const std::string& Signature::getDefaultOperator() const
	{
		return defaultOperator_;
	}

	bool Signature::isTerm(std::string_view str) const
	{
		return functionalContext_.find(str) != functionalContext_.end();
	}

	bool Signature::isSignatureType(std::string_view str, SignatureType type) const
	{
		const auto& found = functionalContext_.find(str);
		return found == functionalContext_.end() ? false : (found->second.type == type);
	}

	const SignatureRealization* Signature::operator[](std::string_view str) const
	{
		return at(str);
	}

	SignatureRealization* Signature::operator[](std::string_view str)
	{
		return at(str);
	}

	const SignatureRealization* Signature::at(std::string_view str) const
	{
		auto found = functionalContext_.find(str);
		return (found == functionalContext_.end()) ? (nullptr) : (&found->second);
	}

	SignatureRealization* Signature::at(std::string_view str)
	{
		auto found = functionalContext_.find(str);
		return (found == functionalContext_.end()) ? (nullptr) : (&found->second);
	}

	void Signature::addConstant(const std::string& name, MathNodeP v)
	{
		constantContext_[name] = std::move(v);
	}

	const VariableContext& Signature::getVariableContext() const
	{
		return constantContext_;
	}

}
