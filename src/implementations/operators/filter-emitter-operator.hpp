#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR

#include "../../interfaces/main.hpp"
#include "./filter-emitter.hpp"
#include <functional>
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename Type, typename MapArg = Type const &>
class FilterEmitterOperator : public EmitterOperator<Type, Type> {
public:
  typedef typename EmitterOperator<Type, Type>::ReturnType ReturnType;
  typedef typename std::function<bool(MapArg)> FilterType;

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
typename FilterEmitterOperator<Type, MapArg>::ReturnType
FilterEmitterOperator<Type, MapArg>::operate(Emitter<Type> *from_ptr) {
  return std::make_unique<FilterEmitter<Type, MapArg>>(from_ptr, this->filter);
};

template <typename Type, typename MapArg>
typename FilterEmitterOperator<Type, MapArg>::ReturnType
FilterEmitterOperator<Type, MapArg>::operate(
    std::unique_ptr<Emitter<Type>> &from_uptr) {
  return std::make_unique<FilterEmitter<Type, MapArg>>(from_uptr, this->filter);
};

} // namespace operators
} // namespace cpp_publisher

#endif
