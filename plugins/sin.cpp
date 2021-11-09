#include <cmath>

extern "C" __declspec(dllexport)
double unaryFunc(double x)
{
    return std::sin(x);
}
