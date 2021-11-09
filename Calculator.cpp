#include "Calculator.h"
#include <stack>
#include <iostream>

Calculator::Calculator()
{
	standartOper["+"] = [](double x, double y) -> double { return x + y; };
	standartOper["-"] = [](double x, double y) -> double { return x - y; };
	standartOper["*"] = [](double x, double y) -> double { return x * y; };
	standartOper["/"] = [](double x, double y) -> double { return x / y; };
	standartOper["~"] = [](double x, double y) -> double { return -x; };
}

double Calculator::execute(std::string const& expression)
{
	std::queue<token> postfixNotation = getPostfixNotation(expression);

	return calculate(postfixNotation);
}

std::queue<Calculator::token> Calculator::getPostfixNotation(std::string const& expression)
{
	std::queue<token> tokens;
	std::queue<token> postfixNotation;

	createTokensFromExp(expression, tokens);
	createPostfixNotation(postfixNotation, tokens);

	return postfixNotation;
}

double getNumber(std::stack<double>& numbers)
{
	double number = numbers.top();
	numbers.pop();
	return number;
}

double Calculator::calculate(std::queue<token> postfixNotation)
{
	std::stack<double> numbers;

	auto const& binaryMap = plugins.getBinaryMap();
	auto const& unaryMap = plugins.getUnaryMap();

	while (!postfixNotation.empty())
	{
		token t = postfixNotation.front();
		postfixNotation.pop();
		if (t.name == ",")
			continue;

		if (t.type == Type::number)
		{
			numbers.push(std::stod(t.name));
			continue;
		}

		if (t.type == Type::operation)
		{
			if (t.name == "~")
			{
				if (numbers.size() < 1)
					throw std::exception("Not correct expression");

				double a = getNumber(numbers);

				double opRes = standartOper[t.name](a, 0);

				numbers.push(opRes);
				continue;
			}

			if (numbers.size() < 2)
				throw std::exception("Not correct expression");

			double b = getNumber(numbers);
			double a = getNumber(numbers);

			double opRes = standartOper[t.name](a, b);

			numbers.push(opRes);
			continue;
		}

		if (t.type == Type::function)
		{
			double funcRes = 0;
			if (binaryMap.find(t.name) != binaryMap.end())
			{
				if (numbers.size() < 2)
					throw std::exception("Not correct expression");

				double b = getNumber(numbers);
				double a = getNumber(numbers);

				funcRes = (binaryMap.at(t.name)(a, b));
			}
			else
			{
				if (numbers.size() < 1)
					throw std::exception("Not correct expression");
				if (unaryMap.find(t.name) == unaryMap.end())
					throw std::exception("Not correct function");

				double a = getNumber(numbers);

				funcRes = (unaryMap.at(t.name)(a));
			}

			numbers.push(funcRes);
			continue;
		}
	}

	if (numbers.size() != 1)
		throw std::exception("Not correct expression");

	return numbers.top();
}

int Calculator::getPriority(std::string const& operation) const
{
	if (operation == "+" || operation == "-")
		return 2;
	if (operation == "*" || operation == "/")
		return 3;
	if (operation == "~")
		return 4;
	if (operation == ",")
		return 1;

	return 0;
}

Calculator::Type Calculator::getType(std::string const& name) const
{
	if (name == "(")
		return Type::openBracket;
	if (name == ")")
		return Type::closeBracket;
	if (name == "+" || name == "-" || name == "*" || name == "/" || name == "," || name == "~")
		return Type::operation;
	if (strspn(name.c_str(), ".0123456789") == name.size())
		return Type::number;
	return Type::function;
}

void Calculator::createTokensFromExp(std::string const& expression, std::queue<token>& tokens)
{
	auto isDelimetr =
		[](char c)
	{
		std::string delimetrs = " ()+-/*,";
		if (delimetrs.find(c) != std::string::npos)
			return true;
		return false;
	};

	int expSize = expression.size();

	for (int i = 0; i < expSize;)
	{
		std::string name;

		if (isDelimetr(expression[i]))
		{
			if (expression[i] == ' ')
			{
				i++;
				continue;
			}

			if (expression[i] == '-' && 
				(i != 0 && isDelimetr(expression[i - 1]) && expression[i - 1] != ')' || i == 0))
			{
				name = '~';
			}
			else
			{
				name = expression[i];
			}
			i++;
		}
		else
		{
			while (!isDelimetr(expression[i]) && i < expression.size())
			{
				name += expression[i];
				i++;
			}
		}

		token t(name, getType(name));
		tokens.push(t);
	}
}

void Calculator::createPostfixNotation(std::queue<token>& postfixNotation, std::queue<token>& tokens)
{
	std::stack<token> tmpTokenStack;
	int numBracket = 0;

	while (!tokens.empty())
	{
		token t = tokens.front();
		tokens.pop();

		switch (t.type)
		{
		case Type::operation:
			if (!tmpTokenStack.empty())
			{
				int curPriority = getPriority(t.name);
				while (!tmpTokenStack.empty() &&
					(tmpTokenStack.top().type == Type::operation && curPriority <= getPriority(tmpTokenStack.top().name)
						|| tmpTokenStack.top().type == Type::function))
				{
					postfixNotation.push(tmpTokenStack.top());
					tmpTokenStack.pop();
				}
			}
			tmpTokenStack.push(t);
			break;

		case Type::function:
			while (!tmpTokenStack.empty() && tmpTokenStack.top().type == Type::function) {
				postfixNotation.push(tmpTokenStack.top());
				tmpTokenStack.pop();
			}
			tmpTokenStack.push(t);
			break;

		case Type::number:
			postfixNotation.push(t);
			break;

		case Type::openBracket:
			tmpTokenStack.push(t);
			numBracket++;
			break;

		case Type::closeBracket:
			while (tmpTokenStack.top().type != Type::openBracket) {
				if (tmpTokenStack.size() == 1)
					throw std::exception("Not correct expression");

				postfixNotation.push(tmpTokenStack.top());
				tmpTokenStack.pop();
			}
			tmpTokenStack.pop();
			numBracket--;
			break;

		default:
			break;
		}
	}

	if (numBracket != 0)
		throw std::exception("Not correct expression");

	while (!tmpTokenStack.empty()) {
		postfixNotation.push(tmpTokenStack.top());
		tmpTokenStack.pop();
	}
}
