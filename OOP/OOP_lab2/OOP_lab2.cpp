#include "primary.h"
#include "mixture.h"
#include "header.h"
#include "empiric.h"

int* func()
{
	int a = 5;
	int b = 7;
	int arr[2]{a, b};
	return arr;
}

int main()
{
	srand(12);
	int* arr = func();
	printf("%d, %d\n", arr[0], arr[1]);

}
