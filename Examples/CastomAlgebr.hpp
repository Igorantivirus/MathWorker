#pragma once

#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int example2()
{
	Signature signature;
	VariableContext constants;
	MathParser parser(signature, std::make_unique<BaseTokenizer>(signature));

	//Кастомный оператор
	signature.addOperator("@", [](const std::vector<MathNodeP>& params)->MathNodeP
		{
			ComplexType left = params[0]->getNumberForced();
			ComplexType right = params[1]->getNumberForced();
			ComplexType res = left + right + left * right;
			return std::make_unique<ValueNode>(res);
		}, 0);

	FunctionConnector connector;

	//Добавление функции (один из 3-х способов)
	connector.addFunction(signature, "f(x)=2@x@17");

	std::string s = "f(1)@2";
	std::cout << parser.parse(s)->replace(constants)->calculate(signature)->toString() << '\n';

	return 0;
}