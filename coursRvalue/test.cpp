#include<iostream>
#include<typeinfo>

void foo( const int& )
{
  std::cout <<"lvalue\n";
}

void foo( int && x )
{
  std::cout << "rvalue\n";
}

int bar()
{
  return 1337;
}

int bar2()
{
  int x = 1337;
  return x;
}

//g++ ./test.cpp -std=c++11 -o ./test
int main()
{
  int x = 3;
  int& y = x;
  int const& z = bar();

  //lvalues
  foo(x);
  foo(y);
  foo(z);

  //rvalues
  foo(4);
  foo(bar());

  //this is also an rvalue
  foo(bar2());
}
