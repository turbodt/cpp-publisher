#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER

#include "../../interfaces/main.hpp"
#include "./base-operator-emitter.hpp"

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg = From const &>
class MapEmitter : public BaseOperatorEmitter<From, To> {
public:
  typedef typename BaseOperatorEmitter<From, To>::ReturnType ReturnType;
  typedef typename std::function<To(MapArg)> MappingType;

protected:
  MappingType mapping;

  virtual typename Subscription<From>::OnPublishCb
  get_on_publish_from(SubscriptionConcrete<To> *) override;
  virtual typename Subscription<From>::OnUnsubscribeCb
  get_on_unsubscribe_from(SubscriptionConcrete<To> *) override;

public:
  MapEmitter(Emitter<From> *, MappingType const &);
  MapEmitter(std::unique_ptr<Emitter<From>> &, MappingType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg>
MapEmitter<From, To, MapArg>::MapEmitter(
    Emitter<From> *from_ptr,
    MapEmitter<From, To, MapArg>::MappingType const &mapping)
    : BaseOperatorEmitter<From, To>(from_ptr), mapping(mapping){};

template <typename From, typename To, typename MapArg>
MapEmitter<From, To, MapArg>::MapEmitter(
    std::unique_ptr<Emitter<From>> &from_uptr,
    MapEmitter<From, To, MapArg>::MappingType const &mapping)
    : BaseOperatorEmitter<From, To>(from_uptr), mapping(mapping){};

template <typename From, typename To, typename MapArg>
typename Subscription<From>::OnPublishCb
MapEmitter<From, To, MapArg>::get_on_publish_from(
    SubscriptionConcrete<To> *subscription_to_ptr) {
  return [this, subscription_to_ptr](MapArg value) {
    this->emit_to_subscription(this->mapping(value), subscription_to_ptr);
  };
};

template <typename From, typename To, typename MapArg>
typename Subscription<From>::OnUnsubscribeCb
MapEmitter<From, To, MapArg>::get_on_unsubscribe_from(
    SubscriptionConcrete<To> *subscription_to_ptr) {
  return nullptr;
};

} // namespace operators
} // namespace cpp_publisher

#endif
