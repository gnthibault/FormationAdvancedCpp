#include <iostream>
#include <cstdlib>


template<typename T0>
auto sum( T0 v0 )
{
	return v0;
};

template<typename T0, typename... Ts>
auto sum( T0 v0, Ts... vs ) //->decltype(v0+sum(vs...)) ne marche pas car récursif
{
	return v0+sum(vs...);
};


//g++ ./test.cpp -std=c++11 -o ./test
int main(int argc, char* argv[])
{

	std::cout << "Sum is " << sum(3,4) << std::endl;
	std::cout << "Sum is " << sum(3,4,5) << std::endl;
	std::cout << "Sum is " << sum(3,4,5.12) << std::endl;

	return EXIT_SUCCESS;
};
