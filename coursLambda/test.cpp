#include <iostream>

template<typename T>
T sqr ( T a ) { return a*a; };

//On peut passer les fonctions par copie
template<typename F1, typename F2, typename T>
auto compose( F1 f1, F2 f2 )
{
	return [f1,f2](double x) -> decltype( f1( f2(x) ) ) { return f1( f2(x) ); };
};

//g++ ./test.cpp -std=c++11 -o ./test
int main(int argc, char* argv[])
{
	auto ssqr = compose( sqr<double>, sqr<double> );
	std::cout << " val is "<< ssqr(2) << std::endl;
	return EXIT_SUCCESS;
}
