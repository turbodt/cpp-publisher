#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_FILTER_EMITTER

#include "../../interfaces/main.hpp"
#include "./base-operator-emitter.hpp"

namespace cpp_publisher {
namespace operators {

template <typename Type, typename FilterArg = Type const &>
class FilterEmitter : public BaseOperatorEmitter<Type, Type> {
public:
  typedef typename BaseOperatorEmitter<Type, Type>::ReturnType ReturnType;
  typedef typename std::function<Type(FilterArg)> FilterType;

protected:
  FilterType filter;

  virtual typename Subscription<Type>::OnPublishCb
  get_on_publish_from(SubscriptionConcrete<Type> *) override;
  virtual typename Subscription<Type>::OnUnsubscribeCb
  get_on_unsubscribe_from(SubscriptionConcrete<Type> *) override;

public:
  FilterEmitter(Emitter<Type> *, FilterType const &);
  FilterEmitter(std::unique_ptr<Emitter<Type>> &, FilterType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename Type, typename FilterArg>
FilterEmitter<Type, FilterArg>::FilterEmitter(
    Emitter<Type> *from_ptr,
    FilterEmitter<Type, FilterArg>::FilterType const &filter)
    : BaseOperatorEmitter<Type, Type>(from_ptr), filter(filter){};

template <typename Type, typename FilterArg>
FilterEmitter<Type, FilterArg>::FilterEmitter(
    std::unique_ptr<Emitter<Type>> &from_uptr,
    FilterEmitter<Type, FilterArg>::FilterType const &filter)
    : BaseOperatorEmitter<Type, Type>(from_uptr), filter(filter){};

template <typename Type, typename FilterArg>
typename Subscription<Type>::OnPublishCb
FilterEmitter<Type, FilterArg>::get_on_publish_from(
    SubscriptionConcrete<Type> *subscription_to_ptr) {
  return [this, subscription_to_ptr](FilterArg value) {
    if (this->filter(value)) {
      this->emit_to_subscription(value, subscription_to_ptr);
    }
  };
};

template <typename Type, typename FilterArg>
typename Subscription<Type>::OnUnsubscribeCb
FilterEmitter<Type, FilterArg>::get_on_unsubscribe_from(
    SubscriptionConcrete<Type> *subscription_to_ptr) {
  return nullptr;
};

} // namespace operators
} // namespace cpp_publisher

#endif
