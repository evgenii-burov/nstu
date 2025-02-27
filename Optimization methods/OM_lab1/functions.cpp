#include "functions.h"

double FunctionMinimization::function(double x)
{
	return pow(x - 1, 2);
}

FunctionMinimization::FunctionMinimization(std::string file_name)
{
	std::ifstream input_stream(file_name);
	if (!input_stream)
	{
		throw std::string("Couldn\'t open file: " + file_name);
	}
	double a0;
	double b0;
	double eps0;
	input_stream >> a0 >> b0 >> eps0;
	if (a0 > b0)
	{
		throw std::string("Incorrect parameters: a must be less than b");
	}
	if (eps0 <= 0)
	{
		throw std::string("Incorrect parameters: epsilon must be positive");
	}
	a = a0;
	b = b0;
	eps = eps0;
}

double FunctionMinimization::dichotomyMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double x1;
	double x2;
	while (right_edge - left_edge >= eps)
	{
		x1 = (left_edge + right_edge - delta) / 2;
		x2 = (left_edge + right_edge + delta) / 2;
		if (function(x1) >= function(x2))
		{
			left_edge = x1;
		}
		else
		{
			right_edge = x2;
		}
	}
	std::cout << "Left: " << left_edge << " Right: " << right_edge << "\n";
	minimum = (right_edge + left_edge) / 2;
	return minimum;
}

double FunctionMinimization::goldenRatioMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double x1;
	double x2;
	const double sqrt5 = sqrt(5);
	x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
	x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
	while (right_edge - left_edge >= eps)
	{
		if (function(x1) >= function(x2))
		{
			left_edge = x1;
			x1 = x2;
		}
		else
		{
			right_edge = x2;
			x2 = x1;
		}
	}
	minimum = (right_edge + left_edge) / 2;
	return minimum;
}

double FunctionMinimization::nthFibonacciNumber(int n)
{
	double fibonacci_number = 0;
	const double sqrt5 = sqrt(5);
	fibonacci_number = (pow((1+sqrt5)/2, n) - pow((1-sqrt5)/2, n)) / sqrt5;
	return fibonacci_number;
}

double FunctionMinimization::fibonacciMethod()
{
	double minimum = 0;
	double delta = eps / 2;
	double left_edge = a;
	double right_edge = b;
	double x1;
	double x2;
	const double sqrt5 = sqrt(5);
	while (right_edge - left_edge >= eps)
	{
		x1 = left_edge + (right_edge - left_edge) * (3 - sqrt5) / 2;
		x2 = left_edge + (right_edge - left_edge) * (sqrt5 - 1) / 2;
		if (function(x1) >= function(x2))
		{
			left_edge = x1;
		}
		else
		{
			right_edge = x2;
		}
	}
	minimum = (right_edge + left_edge) / 2;
	return minimum;
}