#include "mixture.h"

Mixture::Mixture(std::string input_file_name)
{
    std::ifstream input_file;
    input_file.open(input_file_name);
    if (!input_file.is_open())
    {
        throw "Unable to open mixture constructor file";
    }
    double shift10, scale10, form10;
    double shift20, scale20, form20;
    double p0;
    input_file >> shift10 >> scale10 >> form10;
    input_file >> shift20 >> scale20 >> form20;
    input_file >> p0;
    input_file.close();
    p = p0 > 0 && p0 < 1 ? p0 : throw "Incorrect parameter: p";
    prm1 = Primary(shift10, scale10, form10);
    prm2 = Primary(shift20, scale20, form20);
}

Mixture::Mixture(Primary prm10, Primary prm20, double p0):
prm1(prm10),
prm2(prm20),
p(p0 > 0 && p0 < 1 ? p0 : throw "Incorrect parameter: p")
{}

Primary& Mixture::component1() { return prm1; }

Primary& Mixture::component2() { return prm2; }

void Mixture::setP(double pNew)
{
	p = pNew > 0 && pNew < 1 ? pNew : throw(std::pair<std::string, double>("Incorrect parameter: ,", pNew));
}

double Mixture::getP() const { return p; }

double Mixture::randNum() const 
{
    double a, K, C;
    double nu, mu, lambda;
    double r, r1, r2, r3, r4, x1, x2;
    do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
    if (r > p)
    {
        a = prm1.getA();
        K = prm1.getK();
        C = prm1.getC();
        nu = prm1.getForm();
        mu = prm1.getShift();;
        lambda = prm1.getScale();;
    }
    else
    {
        a = prm2.getA();
        K = prm2.getK();
        C = prm2.getC();
        nu = prm2.getForm();
        mu = prm2.getShift();;
        lambda = prm2.getScale();;
    }
    do r1 = (double)rand() / RAND_MAX; while (r1 == 0. || r1 == 1.);
    if (r1 <= C)
    {
        do
        {
            do r2 = (double)rand() / RAND_MAX; while (r2 == 0. || r2 == 1.);
            do r3 = (double)rand() / RAND_MAX; while (r3 == 0. || r3 == 1.);
            x1 = (2 * r2 - 1) * nu;
        } while (r3 > pow(cos(PI * x1 / 2), 2));
        return x1 * lambda + mu;
    }
    else
    {
        do r4 = (double)rand() / RAND_MAX; while (r4 == 0. || r4 == 1.);
        x2 = nu - (log(r4) / a);
        if (r1 < ((1 + C) / 2))
            return x2 * lambda + mu;
        else
            return -x2 * lambda + mu;
    }
}

double* Mixture::getCharacteristics() const
{

    double M, D, A, E, F;
    double m1, m2, d1, d2, e1, e2;
    double a_first_part, a_second_part, e_first_part, e_second_part;
    d1 = prm1.getCharacteristics()[1];
    d2 = prm2.getCharacteristics()[1];
    e1 = prm2.getCharacteristics()[3];
    e2 = prm2.getCharacteristics()[3];

    m1 = prm1.getShift();
    m2 = prm2.getShift();

    M = (1 - p) * m1 + p * m2;
    D = (1 - p) * (pow(m1, 2) + d1) + p * (pow(m2, 2) + d2) - pow(M, 2);

    a_first_part = pow((m1 - M), 3) + 3 * (m1 - M) * d1;
    a_second_part = pow((m2 - M), 3) + 3 * (m2 - M) * d2;
    A = ((1 - p) * a_first_part + p * a_second_part) / pow(D, 1.5);

    e_first_part = pow((m1 - M), 4) + 6 * pow((m1 - M), 2) * d1 + pow(d1, 2) * (e1 + 3);
    e_second_part = pow((m2 - M), 4) + 6 * pow((m2 - M), 2) * d2 + pow(d2, 2) * (e2 + 3);
    E = (((1 - p) * e_first_part + p * e_second_part) / pow(D, 2)) - 3;
    double characteristics[4]{ M, D, A, E };
    return characteristics;

}

double Mixture::getDensityFunctionOfX(double x) const
{
    return ((1 - p) * prm1.getDensityFunctionOfX(x) + p * prm2.getDensityFunctionOfX(x));
}

void Mixture::save(std::string output_file_name) const
{
    std::ofstream output_file;
    output_file.open(output_file_name);
    if (!output_file.is_open())
    {
        throw "Unable to open mixture save file";
    }
    output_file << prm1.getShift() << '\t' << prm1.getScale() << '\t' << prm1.getForm() << '\n';
    output_file << prm2.getShift() << '\t' << prm2.getScale() << '\t' << prm2.getForm() << '\n';
    output_file << p << "\n";
    output_file.close();
}

void Mixture::load(std::string input_file_name)
{
    std::ifstream input_file;
    input_file.open(input_file_name);
    if (!input_file.is_open())
    {
        throw "Unable to open mixture load file";
    }
    double shift10, scale10, form10;
    double shift20, scale20, form20;
    double p0;
    input_file >> shift10 >> scale10 >> form10;
    input_file >> shift20 >> scale20 >> form20;
    input_file >> p0;
    input_file.close();
    p = p0 > 0 && p0 < 1 ? p0 : throw "Incorrect parameter: p";
    prm1.setShift(shift10); prm1.setScale(scale10); prm1.setForm(form10);
    prm2.setShift(shift20); prm2.setScale(scale20); prm2.setForm(form20);
}
