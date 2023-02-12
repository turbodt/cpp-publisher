#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR_FACTORY
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER_OPERATOR_FACTORY

#include "./filter-emitter-operator.hpp"
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename FilterType, typename MapArg = FilterType const &>
FilterEmitterOperator<FilterType>
filter(typename FilterEmitterOperator<FilterType, MapArg>::FilterType filter) {
  return FilterEmitterOperator<FilterType, MapArg>(filter);
}

} // namespace operators
} // namespace cpp_publisher

#endif
