#include "linear_system.h"

LinearSystem::LinearSystem(std::string input_file_name)
{
	std::ifstream input_file;
	input_file.open(input_file_name);
	if (!input_file.is_open())
		throw("Input file couldn't be opened: " + input_file_name);
	int n0, m0, matrix_type;
	input_file >> n0 >> m0 >> matrix_type;
	
	if (n0 < 3)
		throw("Incorrect parameter: n must be 3 or greater");
	if (m0 < n0 - 3)
		throw("Incorrect parameter: m must be n-1 or greater");
	n = n0;
	m = m0;

	shift = { -m, -1, 0, 1, m , 0, 0, 0, 0, 0};
	initializeMatrix(matrix_type);
	initializeVectorB(matrix_type);
	initializeVectorX(matrix_type);
}

void LinearSystem::initializeMatrix(int matrix_type)
{
	matrix.resize(n);
	switch (matrix_type)
	{
	case 1:
		for (int i = 0; i < n; i++)
		{
			matrix[i].push_back(i > m - 1 ? i + 1 : 0);
			matrix[i].push_back(i > 0 ? i + 2 : 0);
			matrix[i].push_back(i + 3);
			matrix[i].push_back(i < n - 1 ? i + 4 : 0);
			matrix[i].push_back(i < n - m ? i + 5 : 0);
		}
		break;
	case 2:
	{
		std::ifstream input_file;
		input_file.open("matrix.txt");
		for (int i = 0; i < n; i++)
		{
			matrix[i].resize(5);
			for (int j = 0; j < 5; j++)
			{
				input_file >> matrix[i][j];
			}
		}
	}
	break;
	}
}

void LinearSystem::initializeVectorB(int matrix_type)
{
	switch (matrix_type)
	{
	case 1:
		for (int i = 0; i < n; i++)
		{
			b.push_back(i + 1);
		}
		break;
	case 2:
		for (int i = 0; i < n; i++)
		{
			b.push_back(i + 1);
		}
		break;
	}
}

void LinearSystem::initializeVectorX(int matrix_type)
{
	for (int i = 0; i < n; i++)
	{
		x.push_back(0);
	}
}

precision LinearSystem::getRelativeDiscrepancy() const
{
	precision b_norm=0;
	precision b_minus_ax_norm=0;
	precision b_minus_ax_norm_ith = 0;
	for (int i = 0; i < n; i++)
	{
		b_minus_ax_norm_ith = b[i];
		int first_diag_start_offset = (i > 0 ? 1 : 0);
		int second_diag_start_offset = (i > m - 1 ? 1 : 0);
		int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
		int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
		//j only goes over non-zero elements
		for (int j = 2 - first_diag_start_offset - second_diag_start_offset,j_dense= 0; j < 5 - first_diag_end_offset - second_diag_end_offset;j++)
		{
			j_dense = i + shift[j];
			b_minus_ax_norm_ith -= matrix[i][j] * x[j_dense];
		}
		b_minus_ax_norm += pow(b_minus_ax_norm_ith,2);
		b_norm += pow(b[i],2);
	}
	return sqrt(b_minus_ax_norm/b_norm);
}

precision LinearSystem::getScalarProductOfIthRowAndX(int i, std::vector<precision> x) const
{
	precision scalar_product = 0;
	int first_diag_start_offset = (i > 0 ? 1 : 0);
	int second_diag_start_offset = (i > m - 1 ? 1 : 0);
	int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
	int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
	//j only goes over non-zero elements
	for (int j = 2 - first_diag_start_offset - second_diag_start_offset, j_dense = 0; j < 5 - first_diag_end_offset - second_diag_end_offset; j++)
	{
		j_dense = i + shift[j];
		scalar_product += matrix[i][j] * x[j_dense];
	}
	return scalar_product;
}

precision LinearSystem::getNextIterationOfXIth(int i, precision omega) const
{
	precision x_next = b[i];
	int first_diag_start_offset = (i > 0 ? 1 : 0);
	int second_diag_start_offset = (i > m - 1 ? 1 : 0);
	int first_diag_end_offset = (i > n - 1 - 1 ? 1 : 0);
	int second_diag_end_offset = (i > n - m - 1 ? 1 : 0);
	//j only goes over non-zero elements
	for (int j = 2 - first_diag_start_offset - second_diag_start_offset, j_dense = 0; j < 5 - first_diag_end_offset - second_diag_end_offset; j++)
	{
		j_dense = i + shift[j];
		x_next -= matrix[i][j] * x[j_dense];
	}
	x_next = x_next * omega / matrix[i][2] + x[i];
	return x_next;
}

void LinearSystem::solveJacobi()
{
	std::vector<precision> x_new;
	x_new.resize(n);
	precision norm;
	int iterations = 0;
	do
	{
		for (int i = 0; i < n; i++)
		{
			x_new[i] = getNextIterationOfXIth(i, omega_jacobi);
		}
		
		for (int i = 0; i < n; i++)
		{
			x[i] = x_new[i];
		}
		iterations++;
	} while (getRelativeDiscrepancy() > eps && iterations < max_iterations); //while (norm > eps && iterations < max_iterations);
	std::cout << "Jacobi iterations: " << iterations << "\n";
}

void LinearSystem::solveGauss_Seidel()
{
	precision norm;
	int iterations = 0;
	do
	{
		for (int i = 0; i < n; i++)
		{
			x[i] = getNextIterationOfXIth(i, omega_jacobi);
		}
		iterations++;
	} while (getRelativeDiscrepancy() > eps && iterations < max_iterations); //while (norm > eps && iterations < max_iterations);
	std::cout << "Gauss-Seidel iterations: " << iterations << "\n";
}

void LinearSystem::solveBlockRelaxation(int block_size)
{
	if (n % block_size != 0)
		throw("Incorrect parameter: block_size must divide the matrix evenly");
	std::vector<std::vector<double>> matrix_block;
	std::vector<double> x_block;
	std::vector<double> b_block;
}

void LinearSystem::print() const
{
	std::cout << "\nn: " << n << "\tm: " << m;
	std::cout << "\nMatrix (diagonal format):\n";
	for (const auto row : matrix)
	{
		for (const auto elem : row)
		{
			std::cout << elem << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\nMatrix (dense format):\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//if not the non-zero diagonal
			if (abs(i - j) > 1 && abs(i - j) != m)
			{
				std::cout << 0 << "\t";
				continue;
			}

			if (abs(i - j) <= 1)
			{
				std::cout << matrix[i][2 - (i - j)] << "\t";
				continue;
			}

			if (i - j == m)
			{
				std::cout << matrix[i][2 - 2] << "\t";
				continue;
			}

			if (i - j == -m)
			{
				std::cout << matrix[i][2 + 2] << "\t";
				continue;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "\nVector b:\n";
	for (const auto elem : b)
	{
		std::cout << elem << "\t";
	}
	std::cout << "\nVector x:\n";
	for (const auto elem : x)
	{
		std::cout << elem << "\t";
	}
}