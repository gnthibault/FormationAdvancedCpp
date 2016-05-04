#include <iostream>
#include <cstdlib>
#include <vector>
#include <functional>

class Foo
{
public:
	Foo() : m_val(5555) {};
	virtual ~Foo()=default;
	
	int returnInt() const { return m_val; };
	double returnDouble() const { return static_cast<double>(m_val); };
protected:
	int m_val;
};

/*
*C++11 :
* attention : ‘transformator’ function uses ‘auto’ type specifier without trailing return type
*C++14 :
* auto seul suffit, mais ne préserve pas les références
*decltype(auto) en c++14 marche aussi
*Attention aux fonctions récursives
*/
template<class T, class U>
auto transformator( T func, U obj ) -> decltype(func(obj))
{
	//prolog specific to my application
	return func(obj);
}


//g++ ./test.cpp -std=c++11 -o ./test
int main(int argc, char* argv[])
{
	decltype(3.0f) h;
	decltype(h) g;
	
	//Erreur car (f) renvoie la référence
	//decltype((f)) h;

	Foo f;
	auto func = std::mem_fn( &Foo::returnInt );
	std::cout << "val is "<< func(f) << std::endl;
	std::cout << "val is "<< transformator(func,f) << std::endl;

	return EXIT_SUCCESS;
}
