#include <iostream>
#include <tuple>

auto GetTuple()
{
  return std::make_tuple<int,float>(4,6.5);
};

//Compile using g++ ./test.cpp -std=c++14 -o ./test
int main(int argc, char* argv[])
{
  int a;
  float b;
  std::tie(a,b) = GetTuple();

  std::cout << "Values are "<<a<<" and "<<b<<std::endl;
  return EXIT_SUCCESS;
}
