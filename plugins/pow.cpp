#include <cmath>

extern "C" __declspec(dllexport)
double binaryFunc(double x, double a)
{
    return std::pow(a, x);
}
