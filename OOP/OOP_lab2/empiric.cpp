#include "primary.h"
#include "mixture.h"
#include "empiric.h"

double* Empiric::getDataFrequencies()
{
	freq = new double[k];
    double R = data_max - data_min;
    double h = R / k;
    int interval_index = 0;
    for (int i = 0; i < k; i++)
    {
        freq[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        interval_index = int((data[i] - data_min) / h); //(data[i] - x_min)/h = 0..k
        interval_index -= int(((data[i] - data_min) / h) / k);
        freq[interval_index]++;
    }
	for (int i = 0; i < k; i++)
	{
		freq[i] /= n;
	}
	return freq;
}

Empiric::Empiric(std::string size_file_name, std::string data_file_name, std::string freq_file_name)
{
	std::ifstream input_file;
	input_file.open(size_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", size_file_name));
	}
	int n0, k0;
	input_file >> n0 >> k0;
	n = n0 > 1 ? n0 : throw(std::pair<std::string, double>("Incorrect parameter: n,", n0));
	k = k0 > 1 ? k0 : int(log2(n)) + 1;
	input_file.close();
	input_file.open(data_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", data_file_name));
	}
	for (int i = 0; i < n; i++)
	{
		input_file >> data[i];
	}
	input_file.close();
	input_file.open(freq_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", freq_file_name));
	}
	for (int i = 0; i < k; i++)
	{
		input_file >> freq[i];
	}
	input_file.close();
}

Empiric::Empiric(int n0, Primary& prim, int k0) :
	n(n0 > 1 ? n0 : throw(std::pair<std::string, double>("Incorrect parameter: n,", n0))),
	k(k0 > 1 ? k0 : int(log2(n)) + 1),
	data(new double[n])
{
	for (int i = 0; i < n; i++)
	{
		data[i] = prim.randNum();
	}
	data_min = DBL_MAX;
	data_max = DBL_MIN;
	for (int i = 0; i < n; i++)
	{
		if(data[i] > data_max)
			data_max = data[i];
		if (data[i] < data_min)
			data_min = data[i];
	}
	freq = getDataFrequencies();
}

Empiric::Empiric(int n0, Mixture& mixt, int k0) :
n(n0 > 1 ? n0 : throw(std::pair<std::string, double>("Incorrect parameter: n,", n0))),
k(k0 > 1 ? k0 : int(log2(n)) + 1),
data(new double[n])
{
	for (int i = 0; i < n; i++)
	{
		data[i] = mixt.randNum();
	}
	data_min = DBL_MAX;
	data_max = DBL_MIN;
	for (int i = 0; i < n; i++)
	{
		if (data[i] > data_max)
			data_max = data[i];
		if (data[i] < data_min)
			data_min = data[i];
	}
	freq = getDataFrequencies();
}

Empiric::Empiric(int n0, Empiric& emp, int k0) :
	n(n0 > 1 ? n0 : throw(std::pair<std::string, double>("Incorrect parameter: n,", n0))),
	k(k0 > 1 ? k0 : int(log2(n)) + 1),
	data(new double[n])
{
	for (int i = 0; i < n; i++)
	{
		data[i] = emp.randNum();
	}
	data_min = DBL_MAX;
	data_max = DBL_MIN;
	for (int i = 0; i < n; i++)
	{
		if (data[i] > data_max)
			data_max = data[i];
		if (data[i] < data_min)
			data_min = data[i];
	}
	freq = getDataFrequencies();
}

Empiric::~Empiric() {
	delete[] data;
	delete[] freq;
}

Empiric::Empiric(const Empiric& emp) :
	n(emp.n),
	k(emp.k),
	data_min(emp.data_min),
	data_max(emp.data_max)
{
	delete[] data;
	data = new double[n];
	for (int i = 0; i < n; i++)
	{
		data[i] = emp.data[i];
	}
	delete[] freq;
	freq = new double[k];
	for (int i = 0; i < k; i++)
	{
		freq[i] = emp.freq[i];
	}
}

Empiric& Empiric::operator=(const Empiric& emp)
{
	if (this == &emp) return *this;

	if (n != emp.n) {
		delete[] data;
		n = emp.n;
		data = new double[n];
		for (int i = 0; i < n; i++)
		{
			data[i] = emp.data[i];
		}
	}
	if (k != emp.k) {
		delete[] freq;
		k = emp.k;
		freq = new double[k];
		for (int i = 0; i < k; i++)
		{
			freq[i] = emp.freq[i];
		}
	}
	return *this;
}

double Empiric::randNum() const
{
	double x_min = data_min;
	double x_max = data_max;
	double h = (x_max - x_min) / k;
	double r;
	int interval_index = 0;
	do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
	for (int i = 0; i < k; i++)
	{
		r -= freq[i];
		if (r <= 0)
		{
			interval_index = i;
			//target_sample[c] = i;
			break;
		}
	}
	do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
	return (x_min + interval_index * h) + h * r;
}

double* Empiric::getCharacteristics() const
{
	double d = 0;
	double m = 0;
	double a = 0;
	double e = 0;
	for (int i = 0; i < n; i++)
	{
		m += data[i];
	}
	m = m / n;
	for (int i = 0; i < n; i++)
	{
		d += pow((data[i] - m), 2);
	}
	d = d / n;
	for (int i = 0; i < n; i++)
	{
		a += pow((data[i] - m), 3);
	}
	a = a / (n * pow(d, 1.5));
	for (int i = 0; i < n; i++)
	{
		e += pow((data[i] - m), 4);
	}
	e = e / (n * pow(d, 2)) - 3;
	double characteristics[4]{ m, d, a, e };
	return characteristics;
}

double Empiric::getDensityFunctionOfX(double x) const
{
	double x_min = data_min;
	double x_max = data_max;

	double R = x_max - x_min;
	double h = R / k;
	int interval_index = 0;

	interval_index = int((x - x_min) / h);
	interval_index -= int(((x - x_min) / h) / k); // prevents freq[k] if x == x_max
	return freq[interval_index];
}

void Empiric::save(std::string size_file_name, std::string data_file_name, std::string freq_file_name) const
{
	std::ofstream output_file;
	output_file.open(size_file_name);
	if (!output_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", size_file_name));
	}
	output_file << n << ' ' << k;
	output_file.close();
	output_file.open(data_file_name);
	if (!output_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", data_file_name));
	}
	for (int i = 0; i < n; i++)
	{
		output_file << data[i] << ' ';
	}
	output_file.close();
	output_file.open(freq_file_name);
	if (!output_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", freq_file_name));
	}
	for (int i = 0; i < k; i++)
	{
		output_file << freq[i] << ' ';
	}
	output_file.close();
}

void Empiric::load(std::string size_file_name, std::string data_file_name, std::string freq_file_name)
{
	std::ifstream input_file;
	input_file.open(size_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", size_file_name));
	}
	int n0, k0;
	input_file >> n0 >> k0;
	n = n0 > 1 ? n0 : throw(std::pair<std::string, double>("Incorrect parameter: n,", n0));
	k = k0 > 1 ? k0 : int(log2(n)) + 1;
	input_file.close();
	input_file.open(data_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", data_file_name));
	}
	for (int i = 0; i < n; i++)
	{
		input_file >> data[i];
	}
	input_file.close();
	input_file.open(freq_file_name);
	if (!input_file.is_open())
	{
		throw(std::pair<std::string, std::string>("Unable to open file: ", freq_file_name));
	}
	for (int i = 0; i < k; i++)
	{
		input_file >> freq[i];
	}
	input_file.close();
}