#include "solution.h"

Solution::Solution() : 
	t_start(0), t_target(1), y_of_0(1), h(.1),
	y_analytic(), y_numerical()
{}

Solution::Solution(std::ifstream& input_file)
{
	double t0, t1, y0, h0;
	input_file >> t0 >> t1 >> y0 >> h0;
	t_start = t0;
	t_target = t1 >= t0 ? t1 : throw("Incorrect parameter: t1 must be greater than or equal to t0");
	y_of_0 = y0;
	h = h0 > 0 ? h0 : throw("Incorrect parameter: t1 must be greater than 0");
}

Solution& Solution::operator=(const Solution& sln)
{
	t_start = sln.t_start;
	t_target = sln.t_target;
	y_of_0 = sln.y_of_0;
	h = sln.h;
	y_analytic = sln.y_analytic;
	y_numerical = sln.y_numerical;
	return *this;
}

double Solution::getNextY(double t, double y, int method) const
{
	switch (method)
	{
	default:
		throw("Incorrect solution method");
	case 1:
		return y + h * getYDerivative(t, y);
	case 2:
		return y + h * (1. / 2) * ((getYDerivative(t, y)) + (getYDerivative(t + h, y + h * getYDerivative(t, y))));
	case 3:
		return y + h * getYDerivative(t + h / 2, y + (h / 2) * getYDerivative(t, y));
	case 4:
	{
		double f_of_x, df_over_dx;
		double x = 1;
		int iterations_max = 10;
		int iterations = 0;
		while (true)
		{
			f_of_x = x - y - h * getYDerivative(t + h, x);
			df_over_dx = 1-h*(2*h+2*t);
			x -= f_of_x / df_over_dx;
			if(abs(f_of_x/df_over_dx) < eps)
				return x;
			if (iterations > iterations_max)
			{
				std::cout << "ITERATIONS: " << iterations;
				return x;
			}
			iterations++;
		}
	}
	}
}

void Solution::setYNumerical(int method)
{
	y_numerical = {};
	double t = t_start, y = y_of_0;
	double y_next, k;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_numerical.push_back(y);
		y_next = getNextY(t, y, method);
		y = y_next;
		t += h;
		if (flag_last)
			break;
	}
}

void Solution::setYAnalytic()
{
	double t = t_start;
	bool flag_last = false;
	while (true)
	{
		if (t > t_target || abs(t_target - t) < eps)
		{
			t = t_target;
			flag_last = true;
		}
		y_analytic.push_back(exp(pow(t,2)));
		t += h;
		if (flag_last)
			break;
	}
}

double Solution::getYDerivative(double t0, double y0) const
{
	return 2 * t0 * y0;
}

void Solution::printParameters() const
{
	std::cout << "t_start: " << t_start;
	std::cout << "\nt_target: " << t_target;
	std::cout << "\ny(0): " << y_of_0;
	std::cout << "\nh: " << h << '\n';
}

void Solution::writeToFile(std::ofstream& output_file) const
{
	double t = t_start;
	bool flag_last = false;

	std::cout << std::setprecision(3) << std::scientific;
	output_file << std::setprecision(3) << std::scientific;
	std::cout << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";
	output_file << "t\t\ty_n\t\ty_t\t\t|y_n-y_t|\n";

	for (int i = 0; i < y_analytic.size(); i++)
	{
		if (t > t_target || abs(t_target - t) < eps)
		{
			t = t_target;
		}
		output_file << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		std::cout << t << '\t' << y_numerical[i] << '\t' << y_analytic[i] << '\t' << abs(y_numerical[i] - y_analytic[i]) << '\n';
		t += h;
	}
}