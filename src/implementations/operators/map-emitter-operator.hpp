#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR

#include "../../interfaces/main.hpp"
#include "../publisher-factory.hpp"
#include "../publisher.hpp"
#include "./map-emitter.hpp"
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
  virtual ReturnType operate(Emitter<From> *) override;
  virtual ReturnType operate(std::unique_ptr<Emitter<From>> &) override;

public:
  MapEmitterOperator(MappingType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg>
MapEmitterOperator<From, To, MapArg>::MapEmitterOperator(
    MapEmitterOperator<From, To, MapArg>::MappingType const &mapping)
    : mapping(mapping){};

template <typename From, typename To, typename MapArg>
typename MapEmitterOperator<From, To, MapArg>::ReturnType
MapEmitterOperator<From, To, MapArg>::operate(Emitter<From> *from_ptr) {
  return std::make_unique<MapEmitter<From, To, MapArg>>(from_ptr,
                                                        this->mapping);
};

template <typename From, typename To, typename MapArg>
typename MapEmitterOperator<From, To, MapArg>::ReturnType
MapEmitterOperator<From, To, MapArg>::operate(
    std::unique_ptr<Emitter<From>> &from_uptr) {
  return std::make_unique<MapEmitter<From, To, MapArg>>(from_uptr,
                                                        this->mapping);
};

} // namespace operators
} // namespace cpp_publisher

#endif
