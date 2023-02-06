#ifndef H_CPP_PUBLISHER_MODULE_INTERFACES_PUBLISHER
#define H_CPP_PUBLISHER_MODULE_INTERFACES_PUBLISHER

#include "./emitter.hpp"
#include "./source.hpp"
#include <memory>

namespace cpp_publisher {

template <typename T> class Publisher : Emitter<T>, Source<T> {
  virtual Emitter<T> *as_emitter() = 0;
  virtual Source<T> *as_source() = 0;
};

} // namespace cpp_publisher

#endif
