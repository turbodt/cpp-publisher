#ifndef H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COMPOSER
#define H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COMPOSER

// Source:
// https://stackoverflow.com/questions/19071268/function-composition-in-c-c11

namespace cpp_publisher {
namespace utils {

template <typename Ret, typename F, typename... Tail> class Composer {
  F f;
  Composer<Ret, Tail...> tail;

public:
  Composer(F f, Tail... tail) : f(f), tail(tail...) {}
  template <typename Arg> Ret operator()(Arg x) { return this->tail(this->f(x)); }
};

template <typename Ret, typename F> class Composer<Ret, F> {
  F f;

public:
  Composer(F f) : f(f) {}
  template <typename Arg> Ret operator()(Arg x) { return this->f(x); }
};

template <typename Ret, typename... Tail> Composer<Ret, Tail...> compose(Tail... tail) {
  return Composer<Ret, Tail...>(tail...);
};

} // namespace utils
} // namespace cpp_publisher

#endif
