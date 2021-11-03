#include <cmath>

extern "C" __declspec(dllexport)
double unaryFunc(double x, double a)
{
    return std::sin(x);
}
