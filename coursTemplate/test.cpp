#include <iostream>
#include <cstdlib>
#include <vector>

template<typename C>
auto size(const C& c) -> decltype(c.size())
{
  return c.size();
};

//g++ ./test.cpp -std=c++11 -o test
int main( int argc, char* argv[])
{
  std::vector<double> n(8);
  float x;

  std::cout << size(n) << std::endl;
  //std::cout << size(x) << std::endl; //does not work

  return EXIT_SUCCESS;
}
