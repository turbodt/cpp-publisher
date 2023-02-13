#ifndef H_CPP_PUBLISHER_MODULE_INTERFACES_BEHAVIOR
#define H_CPP_PUBLISHER_MODULE_INTERFACES_BEHAVIOR

#include "./publisher.hpp"

namespace cpp_publisher {

template <typename T> class Behavior : virtual public Publisher<T> {
public:
  virtual T get_value() = 0;
  virtual T const &get_value() const = 0;
};

} // namespace cpp_publisher

#endif
