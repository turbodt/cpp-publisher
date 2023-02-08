#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR

#include "../../interfaces/main.hpp"
#include "../publisher.hpp"
#include <functional>
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg = From const &>
class MapEmitterOperator : public EmitterOperator<From, To> {
public:
  typedef typename EmitterOperator<From, To>::ReturnType ReturnType;
  typedef typename std::function<To(MapArg)> MappingType;

protected:
  MappingType mapping;
  virtual ReturnType operate(Emitter<From> &) override;

public:
  MapEmitterOperator(MappingType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg >
MapEmitterOperator<From, To, MapArg >::MapEmitterOperator(
    MapEmitterOperator<From, To, MapArg>::MappingType const &mapping)
    : mapping(mapping){};

template <typename From, typename To, typename MapArg >
typename MapEmitterOperator<From, To, MapArg>::ReturnType
MapEmitterOperator<From, To, MapArg>::operate(Emitter<From> &from) {
  // TODO: Change this for a factory
  auto to_unique_ptr = std::make_unique<PublisherConcrete<To>>();
  auto to_ptr = to_unique_ptr.get();
  from.subscribe(
      [this, to_ptr](auto value) {
        to_ptr->publish(this->mapping(value));
      }
    );
  return to_unique_ptr;
};

} // namespace operators
} // namespace cpp_publisher

#endif
