#include "distr.h"

using namespace Distr;

//void testMixture()
//{
//    try
//    {
//        Primary primary1(-1, 1, .5);
//        getParametersPrimary(primary1);
//        Primary primary2("primary_init.txt");
//        getParametersPrimary(primary2);
//        Mixture mix1 = Mixture(primary1, primary2, .5);
//
//        int n = 100;
//        double* mixture_sample = new double[n];
//        int k = int(log2(n)) + 1;
//        double* mixture_frequencies = new double[k];
//        for (int i = 0; i < n; i++)
//        {
//            mixture_sample[i] = mix1.randNum();
//        }
//        getSampleFrequencies(mixture_sample, mixture_frequencies, n, k);
//
//        std::cout << std::setprecision(3);
//        calculateEmpiricSampleCharacteristic(mixture_sample, n, '0');
//        std::cout << "Theoretical values:\n";
//        char values[4] = { 'M', 'D', 'A', 'E' };
//        for (int i = 0; i < 4; i++)
//        {
//            std::cout << values[i] << '\t' << primary1.getValue(values[i]) << "\n";
//        }
//        std::cout << "\n\n";
//
//        writeSampleToFile("mixture_frequencies.txt", mixture_frequencies, k);
//        writeSampleToFile("mixture_frequencies.txt", mixture_sample, n);
//        delete[] mixture_sample;
//        delete[] mixture_frequencies;
//
//        std::cout << "Density function of 0: " << mix1.getDensityFunctionOfX(0);
//        primary1.save("primary_save.txt");
//    }
//    catch (std::pair<std::string, double> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//    catch (std::pair<std::string, std::string> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//    catch (std::pair<std::string, char> error)
//    {
//        std::cerr << error.first << error.second;
//    }
//}

int main()
{
    Primary prim1, prim2(0.5, 2.5, 1.5), prim3(1.5, 1.5, 1), prim4(2.5);
    Mixture<Primary, Primary> mixt1(prim1, prim2, 0.5);
    Mixture<Primary, Primary> mixt2(prim3, prim4, 0.5);
    Mixture<Mixture<Primary, Primary>, Mixture<Primary, Primary>>
        deepMixt(mixt1, mixt2, 0.5);
    int n = 1000000;
    Empiric deep_mix_sample(n, deepMixt);
    deep_mix_sample.save("deep_mix_sample");
}
