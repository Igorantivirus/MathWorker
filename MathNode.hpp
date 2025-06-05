#pragma once

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <variant>
#include <vector>

#include "MathDefines.hpp"
#include "MathStringConverter.hpp"

namespace mathWorker
{

	class MathNode
	{
	public:
		virtual ~MathNode() = default;

		virtual std::string toString() const = 0;
		virtual std::string toString(const StringConverter& converter) const = 0;
		virtual std::unique_ptr<MathNode> clone() const = 0;

		virtual std::unique_ptr<MathNode> replace(const std::map<std::string, std::unique_ptr<MathNode>>& variabls) const = 0;
		virtual std::unique_ptr<MathNode> calculate
		(
			const std::map
			<
				std::string,
				std::variant
				<
					std::function<std::unique_ptr<MathNode>(const std::vector<std::unique_ptr<MathNode>>&)>,
					std::pair<std::unique_ptr<MathNode>, std::vector<std::string>>
				>
			>&
		) const = 0;

		virtual ComplexType getNumberForced() const = 0;

	};

	using MathNodeP = std::unique_ptr<MathNode>;
	using VariableContext = std::map<std::string, MathNodeP>;

	using NativeRealization = std::function<MathNodeP(const std::vector<MathNodeP>&)>;
	using MatherRealization = std::pair<MathNodeP, std::vector<std::string>>;
	//using MatherRealization = MathNodeP;

	using FunctionContext = std::map<std::string, std::variant<NativeRealization, MatherRealization>>;

}