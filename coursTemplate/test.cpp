#include <iostream>
#include <cstdlib>
#include <vector>
#include <type_traits>
#include <utility>

//I want to ask to a type if it posssess a method .size(), called a "detrompeur" in french
//I would be nice if a typedef could gives us that
template<typename T>
struct has_size
{
  template<typename U>
  static auto check( int ) -> decltype( std::declval<U>().size(), //error if no size method on type U
                                        size_t(std::declval<U>().size()), //error if return value of method size is not convertible in size_t
                                        std::true_type() );
  //Pour créer un object factice pour lui prendre sa methode size, il faut une instance,
  //mais toutes les classes n'ont pas de constructeur, il y a un helper declval qui fourni
  //la garantie de renvoyer une instance

  //L'opérateur virgule effectue les deux opération, et renvoie la valeur de la seconde

  // int has no real meaning ...


  //I want this struct to have a typedef like that is bool false, is answer is no, or true if it does
  template<typename U>
  static std::false_type check( ... ); //this is the old C style ellipsis
  //we like to use the C style ellipsis because it is always the last that is chosen for substitution
  //So it is a good choice for default

  using type = std::is_same<decltype( check<T>(0) ), std::true_type>;
  //Small alias that allows to use has_size<T>::Type
};

//Design a simple only if
template<bool C, typename R> struct only_if;
template<typename R> struct only_if<true,R>
{
  using type = R;
};

//Now define a conditional size robust function
template<typename C>
typename only_if< has_size<C>::type::value, std::size_t>::type size(const C& c)
{
  return c.size();
};
template<typename C>
typename only_if< !has_size<C>::type::value, std::size_t>::type size(const C& c)
{
  return 1u;
};

struct Foo
{
  void size() {};
};


//g++ ./test.cpp -std=c++11 -o test
int main( int argc, char* argv[])
{
  std::vector<double> n(8);
  float x;
  Foo f{};

  //std::cout << std::boolalpha << has_size<float>::Type() << std::endl;
  //std::cout << std::boolalpha << has_size<std::vector<double>>::Type() << std::endl;

  std::cout << size(n) << std::endl;
  std::cout << size(x) << std::endl;
  std::cout << size(f) << std::endl;


  return EXIT_SUCCESS;
}
