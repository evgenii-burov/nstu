#include "linear_system.h";

int main()
{
    try
    {
        LinearSystem slau = LinearSystem("parameters.txt");
        //slau.print();
        //slau.solveJacobi();
        //slau.solveGauss_Seidel();
        slau.solveBlockRelaxation(2);
        //slau.print();
        slau.printAnotha();
    }
    catch (std::string err)
    {
        std::cerr << "\n" << err;
    }
}

