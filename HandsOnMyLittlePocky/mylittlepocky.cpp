/* MyLittlePocky
 *   -- dices are magic
 *
 *
 *                (( _______
 *      _______     /\O    O\
 *     /O     /\   /  \      \
 *    /   O  /O \ / O  \O____O\ ))
 * ((/_____O/    \\    /O     /
 *   \O    O\    / \  /   O  /
 *    \O    O\ O/   \/_____O/
 *     \O____O\/ )) mrf      ))
 *   ((
 */

#include <random>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>

std::default_random_engine RandomEngine{std::random_device()()};

struct Dice {
  virtual void reroll() = 0;
  virtual unsigned value() const = 0;
  virtual std::string name() const = 0;
};

//Specialize Packed types when they exist
template<size_t NBSTATE> struct StateStorageType { using type = size_t; };//Default Storage type is size_t
template<> struct StateStorageType<1> { using type = std::uint8_t; };
template<> struct StateStorageType<2> { using type = std::uint16_t; };
template<> struct StateStorageType<4> { using type = std::uint32_t; };

namespace details{
  constexpr size_t log2(size_t n, size_t p = 0) {
      return (n <= 1) ? p : log2(n / 2, p + 1);
  }
  constexpr size_t mlog( size_t n){
    return (log2(n)+7)/8;
  }
  template<size_t NBSTATE> struct DiceName {
    std::string operator()() const {  return "d"+std::to_string(NBSTATE); }
  };
  template<> struct DiceName<2> {
    std::string operator()() const {  return "coin"; }
  };
}

template<size_t NBSTATE>
class TemplateDice : public Dice
{
  typedef typename StateStorageType<details::mlog(NBSTATE)>::type StateType;
  StateType state_;
public:
  TemplateDice() { reroll(); }
  unsigned value() const override { return state_; }
  void reroll() override {
    state_ = std::uniform_int_distribution<StateType>(1,NBSTATE)(RandomEngine);
  }
  std::string name() const override { return details::DiceName<NBSTATE>{}(); }
};

using Coin = TemplateDice<2>;
using D6 = TemplateDice<6>;
using D20 = TemplateDice<20>;


class MyBigPocky {
  std::vector<std::unique_ptr<Dice>> pocket_;

  public:
  template<class... Dices>
  MyBigPocky(Dices... dices) {
    std::initializer_list<bool> __attribute__((unused)) _ {(pocket_.emplace_back(new Dices(dices)), true)...};
  }

  auto begin() -> decltype(pocket_.begin()) { return pocket_.begin(); }
  auto begin() const  -> decltype(pocket_.begin()) { return pocket_.begin(); }
  auto end()  -> decltype(pocket_.end()) { return pocket_.end(); }
  auto end() const -> decltype(pocket_.end()) { return pocket_.end(); }
};

template<class... Dices>
class MyLittlePocky {
  std::tuple<Dices...> pocket_;

  public:

  MyLittlePocky() = default;
  std::tuple<Dices...> & pocket() { return pocket_; }
  std::tuple<Dices...> const & pocket() const { return pocket_; }
  // no begin / end

};

namespace details {
  template<class T, class Op>
  void apply(T& , Op&& , std::integral_constant<size_t, 0>) {
  }
  template<class T, class Op, size_t N>
  void apply(T& values, Op&& op, std::integral_constant<size_t, N>) {
    op(std::get<N-1>(values));
    apply(values, std::forward<Op>(op), std::integral_constant<size_t, N - 1>{});
  }
}

template<class Op, class... Types>
void apply(std::tuple<Types...>& values, Op&& op) {
  details::apply(values, std::forward<Op>(op), std::integral_constant<size_t, sizeof...(Types)>{});
}

//FIXME: check concepts for Pocket, and handle MyLittlePocky case
template<class Pocket>
void reroll(Pocket& P) {
  for(auto & dice : P)
    dice->reroll();
}

struct Dumper {
  template<class D>
    void operator()(D const &v) const {
      std::cout << '\t' << v.name() << ": " << v.value() << std::endl;
    }
};

int main() {
  MyBigPocky MBP{D6(), D20(), D6(), Coin()};
  std::cout << "Rock'n'roll!\n";
  for(auto const &dice_ptr : MBP)
    std::cout << '\t' << dice_ptr->name() << ": " << dice_ptr->value() << std::endl;

  std::cout << "rerolling!\n";
  reroll(MBP);

  for(auto const &dice_ptr : MBP)
    std::cout << '\t' << dice_ptr->name() << ": " << dice_ptr->value() << std::endl;

  MyLittlePocky<D6, D20, D6, Coin> MLP;
  std::cout << "Dices are magic!\n";
  apply(MLP.pocket(), Dumper{});

  std::cout << "weight: " << sizeof(MLP) << "\n";
  return 0;
}
