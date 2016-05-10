#include <cstdlib>
#include <iostream>
#include <cassert>
#include <algorithm>

class bag_of_double
{
public:
  bag_of_double() : m_data(nullptr), m_size(0) {};
  bag_of_double( std::size_t size )
  {
    m_data = new double[size];
    m_size = size;
  };
  virtual ~bag_of_double()
  {
    delete [] m_data;
  };
  bag_of_double( bag_of_double&& other ) : bag_of_double() //forwarded construction
  {
    swap(other);
  };
  bag_of_double( const bag_of_double& other )
  {
    m_data = new double[other.m_size];//if that throw a bad alloc, we are fucked
    m_size = other.m_size;
    std::copy( other.m_data, other.m_data+other.m_size, m_data );
  };
  bag_of_double& operator=( const bag_of_double& other )
  {
    if( this != &other ) //check if I am not copying myself
    {
      delete [] m_data;
      m_size = other.m_size;
      m_data = new double[other.m_size];//if that throw a bad alloc, we are fucked
      std::copy( other.m_data, other.m_data+other.m_size, m_data );
    }
    return *this;
  };
  bag_of_double& operator=( bag_of_double&& other )
  {
    //On ne check pas pour l'autocopie, car c'est bien fait si on a fait un a=move(a)
    bag_of_double tmp;
    swap(tmp); //swap myself with a void object
    swap(other);//swap other with my void state
    return *this;
  };

  void swap( bag_of_double& other )
  {
    std::swap(m_size,other.m_size);
    std::swap(m_data,other.m_data);
  }

  std::size_t size()    const { return m_size; };
  const double* data()  const { return m_data; };
  double* data()              { return m_data; };

private:
  double*     m_data;
  std::size_t m_size;
};

//compile using g++ ./test2.cpp -std=c++11 -o ./test
int main( int argc, char* argv[] )
{
  bag_of_double a;
  assert(a.size() == 0);

  bag_of_double d(5);
  assert(d.size() == 5);

  bag_of_double e(d);
  assert(e.size() == 5);
  double const * dptr = d.data();
  assert( std::all_of(e.data(), e.data()+e.size(), [dptr](double i) mutable {return i==*dptr++;}) );

  return EXIT_SUCCESS;
}
