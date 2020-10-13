#pragma once
#include <cstdint>
#include <type_traits>
#include <limits>
#include <concepts>
#include <functional>

namespace Utils {
template<uint64_t value, typename std::enable_if<value <= std::numeric_limits<uint8_t>::max()>::type* = nullptr>
uint8_t _uintFitting();
template<uint64_t value, typename std::enable_if<std::numeric_limits<uint8_t>::max() < value
    && value <= std::numeric_limits<uint16_t>::max()>::type* = nullptr>
uint16_t _uintFitting();
template<uint64_t value, typename std::enable_if<std::numeric_limits<uint16_t>::max() < value
    && value <= std::numeric_limits<uint32_t>::max()>::type* = nullptr>
uint32_t _uintFitting();
template<uint64_t value, typename std::enable_if<std::numeric_limits<uint32_t>::max() < value
    && value <= std::numeric_limits<uint64_t>::max()>::type* = nullptr>
uint64_t _uintFitting();
template<uint64_t value>
struct uintFitting {
  using type = decltype(_uintFitting<value>());
};
template<uint64_t value>
using uintFitting_t = typename uintFitting<value>::type;
template<std::unsigned_integral T>
class UintContainer {
  T _value;
  template<uint8_t start, uint8_t end>
  static constexpr T _calculateBitmask() {
    T res = 0;
    for(auto i = start; i <= end; ++i) {
      res += T(1) << i;
    }
    return res;
  }
  template<uint8_t start, uint8_t end = start>
  static constexpr T _bitmask = _calculateBitmask<start, end>();
 public:
  template<uint8_t bit>
  [[nodiscard]] bool get() const {
    return (_value & _bitmask<bit>);
  }
  template<uint8_t start, uint8_t end>
  [[nodiscard]] T get() const {
    return (_value & _bitmask<start, end>) >> start;
  }
  template<uint8_t start, uint8_t end = start>
  void set(T value) {
    _value |= _bitmask<start, end>;
    _value ^= _bitmask<start, end>;
    _value |= (value << start);
  }
};
class BytePair {
  Utils::UintContainer<uint8_t> _byte;
 public:
  explicit BytePair(uint8_t first = 0, uint8_t second = 0) {
    _byte.set<0,3>(first);
    _byte.set<4,7>(second);
  }
  template<uint8_t i>
  uint8_t get() {
    return _byte.get<0 + 4 * i, 3 + 4 * i>();
  }
  template<uint8_t i>
  void set(uint8_t value) {
    _byte.set<0 + 4 * i, 3 + 4 * i>(value);
  }
};
template<size_t count>
class Flags {
  UintContainer<uint8_t> containers[count / 8 + (count % 8 ? 1 : 0)];
 public:
  bool get(size_t i) {
    switch(i%8) {
      case 0: return containers[i/8].template get<0>();
      case 1: return containers[i/8].template get<1>();
      case 2: return containers[i/8].template get<2>();
      case 3: return containers[i/8].template get<3>();
      case 4: return containers[i/8].template get<4>();
      case 5: return containers[i/8].template get<5>();
      case 6: return containers[i/8].template get<6>();
      case 7: return containers[i/8].template get<7>();
      default: return false;
    }
  }
  void set(size_t i, bool value) {
    switch(i%8) {
      case 0: containers[i/8].template set<0>(value); break;
      case 1: containers[i/8].template set<1>(value); break;
      case 2: containers[i/8].template set<2>(value); break;
      case 3: containers[i/8].template set<3>(value); break;
      case 4: containers[i/8].template set<4>(value); break;
      case 5: containers[i/8].template set<5>(value); break;
      case 6: containers[i/8].template set<6>(value); break;
      case 7: containers[i/8].template set<7>(value); break;
    }
  }
};
template<typename Constructable, template<typename> typename ...Mixins>
class Composed : public Constructable, public Mixins<Composed<Constructable, Mixins...>> ... {
 public:
  template<typename ...Args>
  explicit Composed(Args... args) : Constructable(args...) {};
};
}
