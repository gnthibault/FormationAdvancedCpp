#include <cstdlib>
#include <iostream>

struct A
{
  A() : m_flag(true) { std::cout <<"Ctor @ "<<(void*)this<<std::endl;};
  ~A()
  {
    if( m_flag )
      std::cout <<"Dtor @ "<<(void*)this<<std::endl;
    else
      std::cout <<"Dumb Dtor @ "<<(void*)this<<std::endl;
  };
  A( const A& other ) : m_flag(true)
  {
    std::cout << "Copy construction @" <<(void*)this <<" from @ "<< (const void*)&other <<std::endl;
  }
  //Assignment operator
  A& operator=(const A& other)
  {
    m_flag=true;
    std::cout << "Assignement @" <<(void*)this <<" from @ "<< (const void*)&other <<std::endl;
    return *this;
  }

  //Move assignment
  /*A& operator=(A&& other)
  {
    m_flag=true;
    other.m_flag = false;
    std::cout << "Move Assignement @" <<(void*)this <<" from @ "<< (const void*)&other <<std::endl;
    return *this;
  }*/
protected:
  bool m_flag;
};

A f()
{
  return {}; //Même pas besoin de remettre le type
}

A g()
{
  A a;
  return a;
}

A h(const A& in)
{
  return in;
}

A i(const A& in)
{
  std::cout << "On passe par la INDEX 0" << std::endl;
  A local(in);
  std::cout << "On passe par la INDEX 1, addr de local is "<<(void*)&local << std::endl;

  //Attention, ne pas faire de retour de std::move( local ); çà ne sert à rien
  return local;
}

A j(const A& in)
{
  A local(in);
  //Si on essaye, on voit que cela contourne l'élision de construction
  return std::move(local);
}

//compile using g++ ./test.cpp -std=c++11 -o ./test
int main(int argc, char* argv[])
{
  //Elision total, vive le compilo
  //A a0 = f(); //Mécanisme d'élision de copie de variable non nommée
  //A a1 = g(); //Mécanisme d'élision de copie de variable nommée
  //A a2 = h(a0);
  //A a3 = i(a0); //La construction par copie se fait directement dans l'objet de retour

  //Comprendre le std::move en forcant les temporaires
  //A a0;
  //A a4;
  //a4 = i(a0); //On peut intercepter cette copie de temporaire par un move constructor
  //On peut fourber le compilo pour le forcer a utiliser le move constructor
  //A a5=std::move(a4);
  //Maintenant a4 est dans un état coquille vide, on peut encore copier un autre élément dedans
  //a4 = a0;

  //Mini test que se passe t'il si pas de move assignement ?
  A a0;
  A a4=j(a0);

  /*A a;
  {
    A a0(a);  //Copy construction
    A a1 = a; //Also a construct by copy, not an affectation
    a1 = a;   //This is an assignement
  }
  A = f();
  */
  return EXIT_SUCCESS;
}
