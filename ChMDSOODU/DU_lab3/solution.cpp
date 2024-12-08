#include "solution.h"

DifferentialEquation::DifferentialEquation(std::string input_file_name) :
	y_analytic(),
	y_numerical()
{
	std::ifstream input_file;
	input_file.open(input_file_name);
	if (!input_file.is_open())
		throw "Unable to open input file";
	double t0, t1, y0, h0;
	input_file >> t0 >> t1 >> y0 >> h0;
	t_start = t0;
	t_target = t1 >= t0 ? t1 : throw("Incorrect parameter: t1 must be greater than or equal to t0");
	y_of_0 = y0;
	h = h0 > 0 ? h0 : throw("Incorrect parameter: t1 must be greater than 0");
	input_file.close();
}

//double DifferentialEquation::getNextY(double t, double y, int method) const
//{
//	switch (method)
//	{
//	default:
//		throw("Incorrect DifferentialEquation method");
//	case 1:
//		return y + h * getYDerivative(t, y);
//	case 2:
//		return y + h * (1. / 2) * ((getYDerivative(t, y)) + (getYDerivative(t + h, y + h * getYDerivative(t, y))));
//	case 3:
//		return y + h * getYDerivative(t + h / 2, y + (h / 2) * getYDerivative(t, y));
//	case 4:
//	{
//		double f_of_x, df_over_dx;
//		double x = 1;
//		int iterations_max = 10;
//		int iterations = 0;
//		while (true)
//		{
//			f_of_x = x - y - h * getYDerivative(t + h, x);
//			df_over_dx = 1-h*(2*h+2*t);
//			x -= f_of_x / df_over_dx;
//			if(abs(f_of_x/df_over_dx) < t_eps)
//				return x;
//			if (iterations > iterations_max)
//			{
//				std::cout << "ITERATIONS: " << iterations;
//				return x;
//			}
//			iterations++;
//		}
//	}
//	}
//}

void DifferentialEquation::calculateYAnalytic()
{
	double t = t_start;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_analytic.push_back(getAnalyticSolutionFunctionValue(t));
		t += h;
		if (flag_last)
			break;
	}
}

void DifferentialEquation::calculateYNumerical()
{
	double t = t_start, y = y_of_0;
	double y_next, k;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_numerical.push_back(y);
		y_next = getNextY(t, y);
		y = y_next;
		t += h;
		if (flag_last)
			break;
	}
}

//void DifferentialEquation::calculateYAnalytic()
//{
//	double t = t_start;
//	bool flag_last = false;
//	while (true)
//	{
//		if (t > t_target || abs(t_target - t) < t_eps)
//		{
//			t = t_target;
//			flag_last = true;
//		}
//		y_analytic.push_back(exp(pow(t,2)));
//		t += h;
//		if (flag_last)
//			break;
//	}
//}

void DifferentialEquation::printParameters() const
{
	std::cout << "t_start: " << t_start;
	std::cout << "\nt_target: " << t_target;
	std::cout << "\ny(0): " << y_of_0;
	std::cout << "\nh: " << h << '\n';
}

void DifferentialEquation::writeToFile(std::string output_file_name) const
{
	std::ofstream output_file;
	output_file.open(output_file_name);
	if (!output_file.is_open())
		throw "Unable to open output file";

	double t = t_start;
	bool flag_last = false;

	std::cout << std::setprecision(3) << std::scientific;
	output_file << std::setprecision(3) << std::scientific;
	std::cout << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";
	output_file << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";

	for (int i = 0; i < y_analytic.size(); i++)
	{
		if (t > t_target || abs(t_target - t) < t_eps)
		{
			t = t_target;
		}
		output_file << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		std::cout << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		t += h;
	}
	output_file.close();
}