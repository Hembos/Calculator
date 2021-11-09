#include "PluginLoader.h"
#include <Windows.h>
#include <iostream>

PluginLoader::PluginLoader(std::string pluginsPath)
	: pluginsPath(pluginsPath)
{
	load();
}

const PluginLoader::unaryMap& PluginLoader::getUnaryMap() const
{
	return unaryFuncs;
}

const PluginLoader::binaryMap& PluginLoader::getBinaryMap() const
{
	return binaryFuncs;
}

void PluginLoader::load()
{
    WIN32_FIND_DATAA wfd;

    HANDLE const hFind = FindFirstFileA((pluginsPath + std::string("*.dll")).c_str(), &wfd);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            std::string dllFileName(wfd.cFileName);
            getFuncFromDll(dllFileName);
        } while (NULL != FindNextFileA(hFind, &wfd));

        FindClose(hFind);
    }
    else
    {
        throw std::exception("invalid plpugin path");
    }
}

void PluginLoader::getFuncFromDll(std::string const& fileName)
{
    HMODULE hm = LoadLibraryA((pluginsPath + fileName).c_str());

    if (hm == nullptr)
    {
        throw std::exception("dll not found");
    }
    
    std::string funcName = fileName.substr(0, fileName.find(".dll"));

    unaryFunc ufunc = (unaryFunc)GetProcAddress(hm, "unaryFunc");

    if (ufunc == nullptr)
    {
        binaryFunc bfunc = (binaryFunc)GetProcAddress(hm, "binaryFunc");
        if (bfunc == nullptr)
        {
            throw std::exception("function not found");
        }

        binaryFuncs.insert(std::pair<std::string, binaryFunc>(funcName, bfunc));
    }
    else
    {
        unaryFuncs.insert(std::pair<std::string, unaryFunc>(funcName, ufunc));
    }
}
