#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class FunctionMinimization
{
private:
	double a;
	double b;
	double eps;
	double function(double x);
	double nthFibonacciNumber(int n);
public:
	FunctionMinimization(std::string file_name);

	double dichotomyMethod();
	double goldenRatioMethod();
	double fibonacciMethod();
};