#include<iostream>

void foo( const int& ) { std::cout <<"lvalue\n"; }
void foo( int&& x ) { std::cout << "rvalue\n"; }

//Inside the scope of f,g,h, even rvalue references are lvalues
template<typename T>
void f( T&& x ) { foo(x); }
template<typename T> //Please don't do that
void g( T&& x ) { foo(std::move(x)); }
template<typename T>
void h( T&& x ) { foo(std::forward<T>(x)); }

//g++ ./test2.cpp -std=c++11 -o ./test
int main()
{
  int x = 3;

  //Check who's who
  std::cout << "Test lvalue, then rvalue (litteral)"
    << "for f without forwarding"<<std::endl;
  f(x); f(20);
  std::cout << "Test lvalue, then rvalue (litteral)"
    << "for g with explicit rvalue cast (move)"<<std::endl;
  g(x); g(20);
  std::cout << "Test lvalue, then rvalue (litteral)"
    << "for h with explicit forwarding"<<std::endl;
  h(x); h(20);

}
