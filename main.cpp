#include <iostream>
#include <chrono>

#include "include/MathWorker/MathWorker.hpp"
#include "include/MathWorker/StringConvert/LaTexStringConverter.hpp"

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
	Signature signature = generator::mathSignature();
	VariableContext constants = generator::baseConstants();

	MathParser parser(signature, std::make_unique<BaseTokenizer>(signature));

	FunctionConnector connector;

	LaTexStringConverter converter;

	std::string s;

	while (true)
	{
		try
		{
			s = input(": ");

			auto start = std::chrono::high_resolution_clock::now();

			//MathNodeP res = parser.parse(s)->replace(constants)->calculate(signature);
			MathNodeP res = parser.parse(s);
			
			auto stop = std::chrono::high_resolution_clock::now();

			std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << '\n';
			std::cout << res->toString(converter) << '\n';
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