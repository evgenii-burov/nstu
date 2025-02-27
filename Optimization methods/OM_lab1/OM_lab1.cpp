#include "functions.h"

int main()
{
    FunctionMinimization f_m("parameters.txt");
    std::cout << f_m.dichotomyMethod();
    std::cout << "\n";
    std::cout << f_m.goldenRatioMethod();
}
