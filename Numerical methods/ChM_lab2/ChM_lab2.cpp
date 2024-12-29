#include "linear_system.h";

int main()
{
    try
    {
        //changed smth
        precision var;
        LinearSystem slau = LinearSystem("parameters.txt");
        slau.print();
        if (sizeof(var) == 4)
            std::cout << std::fixed << std::setprecision(8); //<<std::fixed
        else
            std::cout << std::fixed << std::setprecision(16);
        for (precision w = 0.01; w <= 1.5; w += 0.01)
        {
            std::cout << "-----------------------------------------\nw:" << w << "\n";
            slau.setWeights(w, 0.5, 0.5);
            slau.solveJacobi();
        }

        //for (precision w = 0.01; w <= 2.2; w += 0.01)
        //{
        //    std::cout << "-----------------------------------------\nw:" << w << "\n";
        //    slau.setWeights(0.5, w, 0.5);
        //    slau.solveGaussSeidel();
        //}
        // 
        //slau.solveJacobi();
        //slau.solveGaussSeidel();
        //slau.solveBlockRelaxation(4);
        //slau.print();
        //slau.printAnotha();
    }
    catch (std::string err)
    {
        std::cerr << "\n" << err;
    }
}

