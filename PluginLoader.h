#pragma once
#include <map>
#include <string>

class PluginLoader
{
public:
	using unaryFunc = double (*)(double x);
	using binaryFunc = double (*)(double x, double y);

private:
	using unaryMap = std::map<std::string, unaryFunc>;
	using binaryMap = std::map<std::string, binaryFunc>;
	unaryMap unaryFuncs;
	binaryMap binaryFuncs;
	std::string pluginsPath;

	void load();
	void getFuncFromDll(std::string const& fileName);

public:
	PluginLoader(std::string pluginsPath="./plugins/");

	const unaryMap& getUnaryMap() const;
	const binaryMap& getBinaryMap() const;
};