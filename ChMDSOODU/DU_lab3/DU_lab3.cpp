#include "solution.h"

int main()
{
	std::ifstream input_file;
	Solution sln;
	try
	{
		input_file.open("parameters.txt");
		sln = Solution(input_file);
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << "Exception opening/reading file";
	}
	catch (std::string s)
	{
		std::cout << s;
	}
	input_file.close();

	sln.printParameters();
	sln.setYAnalytic();
	try
	{
		sln.setYNumerical(4);
	}
	catch (std::string s)
	{
		std::cout << s;
	}
	std::ofstream output_file;
	try
	{
		output_file.open("table.txt");
		sln.writeToFile(output_file);
	}
	catch (const std::ifstream::failure& e)
	{
		std::cout << "Exception opening/reading file";
	}
	output_file.close();
}
