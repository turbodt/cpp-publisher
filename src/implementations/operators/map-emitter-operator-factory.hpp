#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR_FACTORY
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR_FACTORY

#include "./map-emitter-operator.hpp"
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg = From const &>
MapEmitterOperator<From, To>
map(typename MapEmitterOperator<From, To, MapArg>::MappingType mapping) {
  return MapEmitterOperator<From, To, MapArg>(mapping);
}

} // namespace operators
} // namespace cpp_publisher

#endif
