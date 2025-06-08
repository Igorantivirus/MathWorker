#include <iostream>
#include <chrono>

#include "include/MathWorker/ValueNode.hpp"
#include "include/MathWorker/SymbolNode.hpp"
#include "include/MathWorker/SignatureNode.hpp"

#include "include/MathWorker/SignatureGenerator.hpp"
#include "include/MathWorker/FunctionConnector.hpp"

#include "include/MathWorker/Parser.hpp"

using namespace mathWorker;

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v)
{
	if (v.empty())
		return out << "{}";
	out << '{' <<'\"' << v[0] << '\"';
	for (size_t i = 1; i < v.size(); ++i)
		out << ", \"" << v[i] << '\"';
	return out << '}';
}

std::string input(std::string s = {})
{
	std::cout << s;
	std::getline(std::cin, s);
	return s;
}

int main()
{
	Signature signature = generateMathSignature();
	VariableContext constants = generateBaseConstants();
	BaseTokenizer tokenizer(&signature);
	MathParser parser(&signature, &tokenizer);
	FunctionConnector connector;

	std::string s;

	while (true)
	{
		try
		{
			s = input(": ");

			auto start = std::chrono::high_resolution_clock::now();

			MathNodeP res = parser.parse(s)->replace(constants)->calculate(signature);
			
			auto stop = std::chrono::high_resolution_clock::now();

			std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << '\n';
			std::cout << res->toString() << '\n';
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << '\n';
		}
		catch (...)
		{
			std::cout << "Unlnown error" << '\n';
		}

	}



	return 0;
}