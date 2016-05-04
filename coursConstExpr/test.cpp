#include <cstdlib>
#include <iostream>

constexpr int factorial( int n )
{
  return n > 2 ? n * factorial( n-1 ) : 1;
};

//c++14 version


template<int N> struct truc
{
  int data[N];
};

//Template parameter in computed at compile time
truc<factorial(4)> glob;

//g++ ./test.cpp -S -std=c++11 -o ./test.s
int main( int argc, char* argv[] )
{
  //not a good runtime function par contre
  constexpr int i = factorial(8);
  std::cout << i << std::endl;

  return EXIT_SUCCESS;
}
