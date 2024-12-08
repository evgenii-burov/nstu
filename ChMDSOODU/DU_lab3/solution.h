#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
/*
Base class:
has parameters, a constructor,
and method- and equation-independent functions*/
class DifferentialEquation
{
protected:
	double t_start, t_target;
	double y_of_0;
	double h;
	double t_eps = 1e-5;
	std::vector<double> y_analytic;
	std::vector<double> y_numerical;

	virtual double getNextY(double t0, double y0) const = 0;
	virtual double getYDerivative(double t0, double y0) const=0;
	virtual double getAnalyticSolutionFunctionValue(double t0) const=0;
public:
	DifferentialEquation(std::string input_file_name);

	void calculateYNumerical();

	void calculateYAnalytic();

	void printParameters() const;

	void writeToFile(std::string output_file_name) const;
};

class EquationOneEmplicitEulerSimple : public DifferentialEquation
{
	double getNextY(double t0, double y0) const override
	{
		return y0 + h * getYDerivative(t0, y0);
	}
	double getYDerivative(double t0, double y0) const override
	{
		return 2 * t0 * y0;
	}
	double getAnalyticSolutionFunctionValue(double t0) const override
	{
		return exp(pow(t0, 2));
	}
public:
	using DifferentialEquation::DifferentialEquation;
};
