#include "linear_system.h";

int main()
{
    try
    {
        precision var;
        LinearSystem slau = LinearSystem("parameters.txt");
        slau.print();
        if (sizeof(var) == 4)
            std::cout << std::fixed << std::setprecision(8); //<<std::fixed
        else
            std::cout << std::fixed << std::setprecision(16);
        //for (precision w = 0.01; w <= 1.02; w += 0.01)
        //{
        //    std::cout << "-----------------------------------------\nw:" << w << "\n";
        //    slau.setWeights(w, 0.5, 0.5);
        //    slau.solveJacobi();
        //}
        for (precision w = 0.01; w <= 2.02; w += 0.01)
        {
            std::cout << "-----------------------------------------\nw:" << w << "\n";
            slau.setWeights(0.5, w, 0.5);
            slau.solveGauss_Seidel();
        }
        //slau.solveGauss_Seidel();
        //slau.solveBlockRelaxation(4);
        //slau.print();
        //slau.printAnotha();
    }
    catch (std::string err)
    {
        std::cerr << "\n" << err;
    }
}

