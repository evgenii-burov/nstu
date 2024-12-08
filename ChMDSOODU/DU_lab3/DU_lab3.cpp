#include "solution.h"

int main()
{
	EquationOneEmplicitEulerSimple du1 = EquationOneEmplicitEulerSimple("parameters.txt");
	du1.calculateYAnalytic();
	du1.calculateYNumerical();
	du1.writeToFile("table.txt");
}
