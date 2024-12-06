#pragma once
#include "primary.h"

class Mixture
{
private:
	Primary prm1, prm2;
	double p;
public:
	Mixture(std::string prm1_file_name, std::string prm2_file_name, std::string p_file_name);
	Mixture(Primary prm10, Primary prm20, double p0);

	Primary& component1();
	Primary& component2();

	void setP(double pNew);
	double getP() const;

	double randNum() const;
	//M, D, A, E
	double getValue(char value_desired) const;

	double getDensityFunctionOfX(double x) const;

	void save(std::string prm1_file_name, std::string prm2_file_name, std::string p_file_name) const;

	void load(std::string prm1_file_name, std::string prm2_file_name, std::string p_file_name);
};