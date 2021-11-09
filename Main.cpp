#include <iostream>
#include <string>
#include "Calculator.h"

int main()
{
	Calculator calculator;
	PluginLoader pl;

	while(true)
	{
		std::string expression;
		std::getline(std::cin, expression);
		if (expression == "")
			break;
		
		try
		{
			double res = calculator.execute(expression);
			std::cout << "= " << res << std::endl;
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	return 0;
}