#include <array>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

template <typename T>
struct Filter0 {
  static const constexpr std::array<T,2> Buff = {0,1};

};
template <typename T>
struct Filter1 {
  static const constexpr std::array<T,2> Buff = {1,2};
};

template<typename T>
const constexpr std::array<T,2> Filter0<T>::Buff;
template<typename T>
const constexpr std::array<T,2> Filter1<T>::Buff;

template<typename T, typename U>
class Accumulator {
 public:
  Accumulator(T* ptr): acc(0), dst(ptr) {}
  void accumulate(T val) {
   acc+=val;
 }
  void write(size_t idx) {
   dst[idx]=acc;
  }
 protected:
  U acc;
  T* dst;
};

template<class Filt, class... Filtn>
struct Updater {
  template<typename I, typename M, class Acc, class... AccN>
  static void accumulate(I idx, M mult, Acc&& acc, AccN&& ... accn) {
    Updater<Filt>::accumulate(idx, mult, std::forward<Acc>(acc));
    Updater<Filtn...>::accumulate(idx, mult, std::forward<AccN>(accn)...);
  }
  template<typename I, class Acc, class... AccN>
  static void write(I idx, Acc&& acc, AccN&& ... accn) {
    Updater<Filt>::write(idx, std::forward<Acc>(acc));
    Updater<Filtn...>::write(idx, std::forward<AccN>(accn)...);
  }

};

/// Actually performs the accumulation
template<class Filt>
struct Updater<Filt> {
  template<typename I, typename M, class Acc>
  static void accumulate(I idx, M mult, Acc&& acc) {
    acc.accumulate(mult*Filt::Buff[idx]);
  }
  template<typename I, class Acc>
  static void write(I idx, Acc&& acc) {
   acc.write(idx);
  }
};

template<typename T, class... Filtn>
class Worker {
 public:

  template<class... AccN>
  static int Perform(AccN&& ... accn) {
    T val=1;
    for (int ox=0; ox<2; ox++) {
      Updater<Filtn...>::accumulate(ox, val, std::forward<AccN>(accn)...);
    }
    Updater<Filtn...>::write(5,accn...);
    return 1;
  }
};

//Compile with g++ -std=c++14 ./test.cpp -o ./test
int main(int argc, char* argv[]) {

  std::vector<float> out0(10);
  std::vector<float> out1(10);

  auto print = [](auto in) {std::cout<<in<<std::endl;};

  Worker<float,Filter0<float>,Filter1<float>> w;
  w.Perform(Accumulator<float,float>(out0.data()),
    Accumulator<float,float>(out1.data()));

  std::cout<<"First vec is :"<<std::endl;
  std::for_each(out0.cbegin(), out0.cend(), print);
  std::cout<<"Second vec is :"<<std::endl;
  std::for_each(out1.cbegin(), out1.cend(), print);

  std::cout << "It works !" << std::endl;
  return EXIT_SUCCESS;
}
