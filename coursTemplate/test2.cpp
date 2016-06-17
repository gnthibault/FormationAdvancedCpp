#include <iostream>
#include <cstdlib>
#include <vector>
#include <type_traits>
#include <utility>


//Now define a conditional size robust function
template<typename C>
auto size(const C& c)->decltype(
  size_t(std::declval<C>().size()),
  std::declval<C>().size())
{
  return c.size();
};

size_t size(...)
{
  return 1u;
};

struct Foo
{
  void size() const {};
};

//g++ ./test2.cpp -std=c++11 -o test
int main( int argc, char* argv[])
{
  std::vector<double> n(8);
  float x;
  Foo f;

  //std::cout << std::boolalpha << has_size<float>::Type() << std::endl;
  //std::cout << std::boolalpha << has_size<std::vector<double>>::Type() << std::endl;

  std::cout << size(n) << std::endl;
  std::cout << size(x) << std::endl;
  std::cout << size(f) << std::endl;

  return EXIT_SUCCESS;
}