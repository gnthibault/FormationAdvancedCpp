#include <iostream>

template<typename T0, typename... Tn>
auto comp( T0 f0, Tn... fn)
{
  return [f0,fn...](auto a){ return f0(comp(fn...)(a)); };
}

template<typename T0>
auto comp( T0 f0 )
{
  return [f0](auto a){ return f0(a); };
}

auto square = [](auto n) { return n*n; };

int main(int argc, char* argv[])
{
  int t = 3;
  auto square2 = comp(square,square);
  std::cout << "Square of square of "<<t<<" is "<<square2(t)<<std::endl;
  return EXIT_SUCCESS;
}
