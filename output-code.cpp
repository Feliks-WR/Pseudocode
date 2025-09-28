#include "custom_lib.h"


int main() {
	std::cout.setf(std::ios::boolalpha);
	_1_ARRAY<long, 10> n;
	std::cout << ("Enter a positive integer: "_s) << std::endl;
	std::cin >> n[1];
	if (n[1] < 0)
	{
		std::cout << ("Error: Please enter a positive integer."_s) << std::endl;
	}
	else
	{
		long factorial;
		factorial = 1;
		for (auto i = 1; i != (n[1] + (std::abs(1) * __SPACESHIP(n[1], 1))); i += 1)
		{
			factorial = factorial * i;
		}
		std::cout << ("Factorial = "_s, factorial) << std::endl;
	}
	return 0;
}
