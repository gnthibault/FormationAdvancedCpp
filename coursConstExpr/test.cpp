#include <cstdlib>
#include <iostream>
#include <array>

constexpr int factorial( int n ) {
  return n > 1 ? n * factorial( n-1 ) : 1;
};

//c++14 version, not only "return statements" functions are allowed in
//constexpr, this allows to provide better implementation
constexpr int factorialPerf( int n ) {
  int r = 1;
  for (int i=2; i<=n; i++) {
    r *= i;
  }
  return r;
};

template<int N> struct truc
{
  int data[N];
};

enum class filter {DAUB, ANTON};

template<typename T, typename EnumClass, EnumClass EnumVal>
struct Test {
  static constexpr T buff[4] = {1,2,3,4};
};

//Semi specialization
template<typename T>
struct Test<T,filter,filter::DAUB> {
  static constexpr T buff[4] = {4,3,2,1};
};

/*template<typename T>
const T Test<T,filter,filter::DAUB>::buff = {4,3,2,1};*/

//Template parameter in computed at compile time
truc<factorial(4)> glob;
//C++14
truc<factorialPerf(4)> glob2;

//g++ ./test.cpp -S -std=c++14 -o ./test.s to see assembler generated
int main( int argc, char* argv[] )
{
  //no runtime computation
  constexpr int i = factorial(8);

  //Even array can be evaluated at compile time
  constexpr std::array<int,i> x{1,2,3,4,5,6};
  std::array<int,x[Test<float,filter,filter::DAUB>::buff[2]]> y;

  //C++14, good runtime function
  constexpr int j = factorialPerf(8);

  std::cout << i <<" = "<< j << std::endl;

  return EXIT_SUCCESS;
}
