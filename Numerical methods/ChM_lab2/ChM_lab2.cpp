#include "linear_system.h";

int main()
{
    LinearSystem slau = LinearSystem("parameters.txt");
    slau.print();
    //slau.solveJacobi();
	slau.solveGauss_Seidel();
    slau.print();

}

