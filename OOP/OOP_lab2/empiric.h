#pragma once
#include "primary.h"
#include "mixture.h"

class Empiric
{
private:
	int n;
	int k;
	double* data;
	double* freq;
    double data_min;
    double data_max;
public:
	Empiric(std::string size_file_name, std::string data_file_name, std::string freq_file_name);

	Empiric(int n0, Primary& prim, int k0 = 1);
	Empiric(int n0, Mixture& mixt, int k0 = 1);
	Empiric(int n0, Empiric& emp, int k0 = 1);

	~Empiric();

	Empiric(const Empiric& emp);
	Empiric& operator=(const Empiric& emp);

	double randNum() const;
	//M, D, A, E
	double getValue(char value_desired) const;

	double getDensityFunctionOfX(double x) const;

	void save(std::string size_file_name, std::string data_file_name, std::string freq_file_name) const;

	void load(std::string size_file_name, std::string data_file_name, std::string freq_file_name);
};

double* getDataFrequencies(double*& data, const int& N, const int& k);