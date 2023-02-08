#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_EMITTER_OPERATOR

#include "../emitter.hpp"
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename From, typename To> class EmitterOperator {
public:
  typedef std::unique_ptr<Emitter<To>> ReturnType;

protected:
  virtual ReturnType operate(Emitter<From> &) = 0;

public:
  virtual ReturnType operator()(Emitter<From> &);
  virtual ReturnType operator()(Emitter<From> *);
  virtual ReturnType operator()(std::shared_ptr<Emitter<From>>);
  virtual ReturnType operator()(std::unique_ptr<Emitter<From>>);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To>
typename EmitterOperator<From, To>::ReturnType
EmitterOperator<From, To>::operator()(Emitter<From> &emitter) {
  return this->operate(emitter);
};

template <typename From, typename To>
typename EmitterOperator<From, To>::ReturnType
EmitterOperator<From, To>::operator()(Emitter<From> *emitter_ptr) {
  return this->operator()(*emitter_ptr);
};

template <typename From, typename To>
typename EmitterOperator<From, To>::ReturnType
EmitterOperator<From, To>::operator()(
    std::shared_ptr<Emitter<From>> emitter_ptr) {
  return this->operator()(emitter_ptr.get());
};

template <typename From, typename To>
typename EmitterOperator<From, To>::ReturnType
EmitterOperator<From, To>::operator()(
    std::unique_ptr<Emitter<From>> emitter_ptr) {
  return this->operator()(emitter_ptr.get());
};

} // namespace operators
} // namespace cpp_publisher

#endif
