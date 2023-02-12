#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR

#include "../../interfaces/main.hpp"
#include "../publisher-factory.hpp"
#include "../publisher.hpp"
#include <functional>
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename Type, typename MapArg = Type const &>
class FilterEmitterOperator : public EmitterOperator<Type, Type> {
public:
  typedef typename EmitterOperator<Type, Type>::ReturnType ReturnType;
  typedef typename std::function<bool(MapArg)> FilterType;

private:
  void make_subsrciption(Emitter<Type> *,
                         std::unique_ptr<Publisher<Type>> &) const;
  std::unique_ptr<Publisher<Type>> make_to_unique_ptr(Emitter<Type> *) const;
  std::unique_ptr<Publisher<Type>>
  make_to_unique_ptr(std::unique_ptr<Emitter<Type>> &) const;

protected:
  FilterType filter;
  virtual ReturnType operate(Emitter<Type> *) override;
  virtual ReturnType operate(std::unique_ptr<Emitter<Type>> &) override;

public:
  FilterEmitterOperator(FilterType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename Type, typename MapArg>
FilterEmitterOperator<Type, MapArg>::FilterEmitterOperator(
    FilterEmitterOperator<Type, MapArg>::FilterType const &filter)
    : filter(filter){};

template <typename Type, typename MapArg>
void FilterEmitterOperator<Type, MapArg>::make_subsrciption(
    Emitter<Type> *from_ptr,
    std::unique_ptr<Publisher<Type>> &to_unique_ptr) const {
  auto to_ptr = to_unique_ptr.get();
  from_ptr->subscribe([this, to_ptr](auto value) {
    if (this->filter(value)) {
      to_ptr->publish(value);
    };
  });
}

template <typename Type, typename MapArg>
std::unique_ptr<Publisher<Type>>
FilterEmitterOperator<Type, MapArg>::make_to_unique_ptr(
    Emitter<Type> *from_ptr) const {
  return make_publisher<Type>();
}

template <typename Type, typename MapArg>
std::unique_ptr<Publisher<Type>>
FilterEmitterOperator<Type, MapArg>::make_to_unique_ptr(
    std::unique_ptr<Emitter<Type>> &from_unique_ptr) const {
  auto from_ptr = from_unique_ptr.release();
  auto on_delete = [from_ptr](auto emitter_ptr) { delete from_ptr; };
  return make_publisher<Type>(nullptr, on_delete);
}

template <typename Type, typename MapArg>
typename FilterEmitterOperator<Type, MapArg>::ReturnType
FilterEmitterOperator<Type, MapArg>::operate(Emitter<Type> *from_ptr) {
  auto to_unique_ptr = this->make_to_unique_ptr(from_ptr);
  this->make_subsrciption(from_ptr, to_unique_ptr);
  return to_unique_ptr;
};

template <typename Type, typename MapArg>
typename FilterEmitterOperator<Type, MapArg>::ReturnType
FilterEmitterOperator<Type, MapArg>::operate(
    std::unique_ptr<Emitter<Type>> &from_unique_ptr) {
  auto from_ptr = from_unique_ptr.get();
  auto to_unique_ptr = this->make_to_unique_ptr(from_unique_ptr);
  this->make_subsrciption(from_ptr, to_unique_ptr);
  return to_unique_ptr;
};

} // namespace operators
} // namespace cpp_publisher

#endif
