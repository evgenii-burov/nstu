#pragma once
#include <string>
#include <iostream>
#include <fstream>

#define interface struct

namespace Distr
{
	const double PI = M_PI;

	interface IDistribution
	{
		virtual double randNum() const = 0;

		virtual double* getCharacteristics() const = 0;

		virtual double getDensityFunctionOfX(double x) const = 0;
	};

	interface IPersistent
	{
		virtual void save(std::string output_file_name) const = 0;

		virtual void load(std::string input_file_name) = 0;
	};

	class Primary : public IDistribution, public IPersistent
	{
	private:
		double shift;
		double scale;
		double form;
		double a;
		double K;
		double C; // C := P
	public:
		Primary(double shift0 = 0, double scale0 = 1, double form0 = .5);

		Primary(std::string input_file_name);

		void setShift(double newShift);
		double getShift() const;

		void setScale(double newScale);
		double getScale() const;

		void setForm(double newForm);
		double getForm() const;

		double getA() const;
		double getK() const;
		double getC() const;

		double randNum() const override;
		//M, D, A, E
		double* getCharacteristics() const override;

		double getDensityFunctionOfX(double x) const override;

		void save(std::string output_file_name) const override;

		void load(std::string input_file_name) override;
	};

	class Empiric : public IDistribution, public IPersistent
	{
	private:
		//data size
		int n;
		//freq size
		int k;
		//data sample
		double* data;
		//data sample frequencies
		double* freq;
		double data_min;
		double data_max;

		double* getDataFrequencies();
	public:
		Empiric(std::string input_file_name);

		Empiric(int n0, IDistribution& d, int k0 = 1);

		~Empiric();

		Empiric(const Empiric& emp);
		Empiric& operator=(const Empiric& emp);

		double randNum() const override;
		//M, D, A, E
		double* getCharacteristics() const override;

		double getDensityFunctionOfX(double x) const override;

		void save(std::string output_file_name) const override;

		void load(std::string input_file_name) override;
	};

	template<class Distribution1, class Distribution2>
	class Mixture : public IDistribution, public IPersistent
	{
	private:
		double p;
		Distribution1 d1;
		Distribution2 d2;
	public:
		Mixture(Distribution1& d10, Distribution2& d20, double p0) :
			p = p0 > 0 && p0 < 1 ? p0 : throw "Incorrect parameter: p",
			d1(d10),
			d2(d20)
		{
		}
		Distribution1& component1() { return d1; }

		Distribution2& component2() { return d2; }

        void Mixture::setP(double pNew)
        {
            p = pNew > 0 && pNew < 1 ? pNew : throw(std::pair<std::string, double>("Incorrect parameter: ,", pNew));
        }

        double Mixture::getP() const { return p; }

        double randNum() const
        {
            double a, K, C;
            double nu, mu, lambda;
            double r, r1, r2, r3, r4, x1, x2;
            do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
            if (r > p)
            {
                a = d1.getA();
                K = d1.getK();
                C = d1.getC();
                nu = d1.getForm();
                mu = d1.getShift();;
                lambda = d1.getScale();;
            }
            else
            {
                a = d2.getA();
                K = d2.getK();
                C = d2.getC();
                nu = d2.getForm();
                mu = d2.getShift();;
                lambda = d2.getScale();;
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

        double* getCharacteristics() const
        {

            double M, D, A, E, F;
            double m1, m2, sigma_sqr1, sigma_sqr2, e1, e2;
            double a_first_part, a_second_part, e_first_part, e_second_part;
            sigma_sqr1 = d1.getCharacteristics()[1];
            sigma_sqr2 = d2.getCharacteristics()[1];
            e1 = d2.getCharacteristics()[3];
            e2 = d2.getCharacteristics()[3];

            m1 = d1.getShift();
            m2 = d2.getShift();

            M = (1 - p) * m1 + p * m2;
            D = (1 - p) * (pow(m1, 2) + sigma_sqr1) + p * (pow(m2, 2) + sigma_sqr2) - pow(M, 2);

            a_first_part = pow((m1 - M), 3) + 3 * (m1 - M) * sigma_sqr1;
            a_second_part = pow((m2 - M), 3) + 3 * (m2 - M) * sigma_sqr2;
            A = ((1 - p) * a_first_part + p * a_second_part) / pow(D, 1.5);

            e_first_part = pow((m1 - M), 4) + 6 * pow((m1 - M), 2) * sigma_sqr1 + pow(sigma_sqr1, 2) * (e1 + 3);
            e_second_part = pow((m2 - M), 4) + 6 * pow((m2 - M), 2) * sigma_sqr2 + pow(sigma_sqr2, 2) * (e2 + 3);
            E = (((1 - p) * e_first_part + p * e_second_part) / pow(D, 2)) - 3;
            double characteristics[4]{ M, D, A, E };
            return characteristics;

        }

        double getDensityFunctionOfX(double x) const
        {
            return ((1 - p) * d1.getDensityFunctionOfX(x) + p * d2.getDensityFunctionOfX(x));
        }

        void save(std::string output_file_name) const
        {
            std::ofstream output_file;
            output_file.open(output_file_name);
            if (!output_file.is_open())
            {
                throw "Unable to open mixture save file";
            }
            output_file << d1.getShift() << '\t' << d1.getScale() << '\t' << d1.getForm() << '\n';
            output_file << d2.getShift() << '\t' << d2.getScale() << '\t' << d2.getForm() << '\n';
            output_file << p << "\n";
            output_file.close();
        }

        void load(std::string input_file_name)
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
            d1.setShift(shift10); d1.setScale(scale10); d1.setForm(form10);
            d2.setShift(shift20); d2.setScale(scale20); d2.setForm(form20);
        }

	};
}