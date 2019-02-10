#pragma once
#include <random>
#include <type_traits>

class PRNG {
 public:
  PRNG();
  explicit PRNG(long seed);
  template<class V, typename std::enable_if_t<std::is_integral_v<V>, int> = 0>
  V get(V val_min, V val_max){
    V minn = std::min(val_min, val_max);
    V maxx = std::max(val_min, val_max);
    std::uniform_int_distribution<V> dist(minn, maxx);
    return dist(gen_);
  };
  template<class V, typename std::enable_if_t<!std::is_integral_v<V>, int> = 0>
  void get(...){};
 private:
  std::mt19937 gen_;
};
