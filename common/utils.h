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
template<typename Constructable, template<typename> typename ...Mixins>
class Composed : public Constructable, public Mixins<Composed<Constructable, Mixins...>> ... {
 public:
  template<typename ...Args>
  explicit Composed(Args... args) : Constructable(args...) {};
};
}
