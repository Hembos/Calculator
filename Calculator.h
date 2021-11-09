#pragma once
#include <string>
#include <queue>
#include <map>
#include "PluginLoader.h"

class Calculator
{
private:
	enum class Type
	{
		operation,
		function,
		number,
		openBracket,
		closeBracket
	};

	struct token
	{
		std::string name;
		Type type;
		token(std::string name, Type type)
			: name(name), type(type) {}
	};

	PluginLoader plugins;

	using opfunc = double (*)(double x, double y);
	std::map<std::string, opfunc> standartOper;

	int getPriority(std::string const& operation) const;
	Type getType(std::string const& name) const;
	void createTokensFromExp(std::string const& expression, std::queue<token>& tokens);
	void createPostfixNotation(std::queue<token>& postfixNotation, std::queue<token>& tokens);
	std::queue<token> getPostfixNotation(std::string const& expression);
	double calculate(std::queue<token> postfixNotation);
public:
	Calculator();
	double execute(std::string const& expression);
};