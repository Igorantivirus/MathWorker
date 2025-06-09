#pragma once

#include <iostream>

#include <MathWorker/MathWorker.hpp>

using namespace mathWorker;

int example1()
{
	//Основная сигнатура и константы алгебры
	Signature signature = generator::mathSignature();
	VariableContext constants = generator::baseConstants();

	MathParser parser(signature, std::make_unique<BaseTokenizer>(signature));
	FunctionConnector connector;

	std::string s;
	while (true)
	{
		try
		{
			std::getline(std::cin, s);
			std::cout << parser.parse(s)->replace(constants)->calculate(signature)->toString() << '\n';
		}
		catch (const ParseException& e)
		{
			std::cout << "Error: " << e.what() << ", type of error: " << static_cast<int>(e.type()) << '\n';
		}
		catch (...)
		{
			std::cout << "Unknown error" << '\n';
		}
	}
	return 0;
}