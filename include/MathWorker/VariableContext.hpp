#pragma once

#include <map>

#include "MathDefines.hpp"
#include "MathNode.hpp"

namespace mathWorker
{

	using VariableContext = std::map<std::string, MathNodeP>;

}