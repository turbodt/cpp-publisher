#ifndef H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COMPOSER
#define H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COMPOSER

// Source:
// https://stackoverflow.com/questions/19071268/function-composition-in-c-c11

namespace cpp_publisher {
namespace utils {

template <typename F0, typename... F> class Composer {
  F0 f0_;
  Composer<F...> tail_;

public:
  Composer(F0 f0, F... f) : f0_(f0), tail_(f...) {}
  template <typename T> T operator()(const T &x) const { return f0_(tail_(x)); }
};
template <typename F> class Composer<F> {
  F f_;

public:
  Composer(F f) : f_(f) {}
  template <typename T> T operator()(const T &x) const { return f_(x); }
};
template <typename... F> Composer<F...> compose(F... f) {
  return Composer<F...>(f...);
};

} // namespace utils
} // namespace cpp_publisher

#endif
