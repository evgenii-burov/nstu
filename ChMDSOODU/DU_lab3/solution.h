#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

class Solution
{
private:
	double t_start, t_target;
	double y_of_0;
	double h;
	double eps = 1e-3;
	std::vector<double> y_analytic;
	std::vector<double> y_numerical;
	double getYDerivative(double t, double y) const;
	/*
	1: simple explicit Euler's method
	2: modified Euler's method
	3: refined Euler's method
	4: simple implicit Euler's method;
	*/
	double getNextY(double t, double y, int method) const;
	//for Newton's method
public:
	Solution();

	Solution(std::ifstream& input_file);

	Solution& operator=(const Solution& sln);
	/*
	1: simple explicit Euler's method
	2: modified Euler's method
	3: refined Euler's method
	4: simple implicit Euler's method;
	*/
	void setYNumerical(int method);

	void setYAnalytic();

	void printParameters() const;

	void writeToFile(std::ofstream& output_file) const;
};
