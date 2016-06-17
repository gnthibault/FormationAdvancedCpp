#include <iostream>
#include <cstdlib>
#include <vector>
#include <type_traits>
#include <utility>
#include <complex>

template<typename R, typename... T>
using void_ = R;
//Prends plein de type et renvoie le premier

//First val is void, or a struct with nice name, the rest will be a type, whose
//substitution status (fail/not fail), is used for sfinae
#define REQUIRES(RET, ...) void_<RET, decltype( __VA_ARGS__ )>

struct DOES_NOT_DEFINE_A_RANGE {}; //Use that for nice comment by the compiler
#define RANGE(TYPE) REQUIRES ( DOES_NOT_DEFINE_A_RANGE         \
                              , (std::declval<TYPE>().end()),  \
                              , (std::declval<TYPE>().begin()) \
                            )                                  \

struct TYPES_ARE_NOT_COMPARABLE {}; //Use that for nice comment by the compiler
#define COMPARABLE(TYPE) REQUIRES ( TYPES_ARE_NOT_COMPARABLE                      \
                                    , std::declval<TYPE>() < std::declval<TYPE>() \
                                  )                                               \

template<typename T>
COMPARABLE(T) minimum(T a, T b)
{
  return a < b ? a : b;
};

/*
COMPARABLE( Container )
*/
/*template<typename Container>
REQUIRES(std::ptrdiff_t,
        (std::declval<Container>().end()),
        (std::declval<Container>().begin()),
        std::ptrdiff_t(std::declval<Container>().end()-std::declval<Container>().begin())
      )
distance( Container const& c)
{
  return c.end() - c.begin();
}*/

//g++ ./test.cpp -std=c++11 -o test
int main( int argc, char* argv[])
{
  int a=3, b=4;
  std::cout << minimum(a,b) << std::endl;

  //This one should fail
  //std::complex<double> x,y;
  //std::cout << minimum(x,y) << std::endl;


  return EXIT_SUCCESS;
}
