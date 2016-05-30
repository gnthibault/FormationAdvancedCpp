#include <iostream>
#include <tuple>
#include <vector>
#include <type_traits>

template<int N>
using IntType = std::integral_constant<int, N>;

//Generic variadic implementation of test functor : this class is only
//here to show how a fully generic case can be defined
template<typename... Tn>
struct Demo
{
  static bool test()
  {
    std::cout <<sizeof...(Tn)<<std::endl;
    return true;
  }
};

//In our case, we specialized the test functor for an arbitrary test case, that
//consider the type 1 and 2, and the ::value of type 3, to be printed out.
//But you can instanciate you real class of interest in the test() method
template<
  typename T,
  typename U,
  typename V>
struct Demo<T,U,V>
{
  static bool test()
  {
    std::cout <<sizeof(T)<<" - "<<sizeof(U)<<" - "<<V::value<<std::endl;
    return true;
  }
};

//All method arguments have been consummed, we can call the test functor
template<
  template<typename...> class TestT,
  typename... Tn>
bool TestImp()
{
  return TestT<Tn...>::test();
};

//Case where at least 1 tuple has been fully consummed : stub it with true value
template<
  template<typename...> class TestT,
  typename... Tn,
  typename... Vn>
bool TestImp( std::tuple<> const& un, Vn const&... vn )
{
  return true;
};

//Generic case: While we have a tuple (used a a type list here), we consume the
//first element of the tuple, and recursively call TestImp until all tuples
//habe been consummed. In the later case, we can instanciate the test class with
//a full set of types
template<
  template<typename...> class TestT,
  typename... Tn,
  typename U0, typename... Un,
  typename... Vn>
bool TestImp( std::tuple<U0,Un...> const& un, Vn const&... vn )
{
  return TestImp<TestT,Tn...,U0>(vn...) &&
  TestImp<TestT,Tn...>(std::tuple<Un...>(),vn...);
};

//Compile using  g++ ./main.cpp -std=c++11 -o ./test
int main()
{
  //We define 3 sets of types
  using T0 = std::tuple<char,short,float,double>;
  using T1 = std::tuple<float,double>;
  using T2 = std::tuple<IntType<101>,IntType<102>>;

  //We challenge the template test functor "Demo" over the
  //cartesian product of the type sets T0,T1,T2
  return TestImp<Demo>(T0(),T1(),T2()) ? EXIT_SUCCESS : -1;
}