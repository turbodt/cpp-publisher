#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_BASE_OPERATOR_EMITTER
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_BASE_OPERATOR_EMITTER

#include "../../interfaces/main.hpp"
#include "../publisher-factory.hpp"
#include "../publisher.hpp"
#include <map>

namespace cpp_publisher {
namespace operators {

template <typename From, typename To>
class BaseOperatorEmitter : public PublisherConcrete<To> {
public:
  typedef typename EmitterOperator<From, To>::ReturnType ReturnType;

protected:
  Emitter<From> *from_ptr;
  std::map<unsigned int, Subscription<From> *> to_from_subscription_mapping =
      std::map<unsigned int, Subscription<From> *>();

  virtual typename Subscription<From>::OnPublishCb
  get_on_publish_from(SubscriptionConcrete<To> *) = 0;
  virtual typename Subscription<From>::OnUnsubscribeCb
  get_on_unsubscribe_from(SubscriptionConcrete<To> *) = 0;

public:
  BaseOperatorEmitter(Emitter<From> *);
  BaseOperatorEmitter(std::unique_ptr<Emitter<From>> &);

  virtual void remove_subscription(SubscriptionConcrete<To> *) override;

  virtual SubscriptionConcrete<To> *subscribe(
      typename Subscription<To>::OnPublishCb const & = nullptr,
      typename Subscription<To>::OnUnsubscribeCb const & = nullptr) override;
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To>
BaseOperatorEmitter<From, To>::BaseOperatorEmitter(Emitter<From> *from_ptr)
    : PublisherConcrete<To>(nullptr, nullptr), from_ptr(from_ptr){};

template <typename From, typename To>
BaseOperatorEmitter<From, To>::BaseOperatorEmitter(
    std::unique_ptr<Emitter<From>> &from_uptr)
    : PublisherConcrete<To>(
          nullptr, [this](auto publisher_ptr) { delete this->from_ptr; }),
      from_ptr(from_uptr.release()){};

template <typename From, typename To>
SubscriptionConcrete<To> *BaseOperatorEmitter<From, To>::subscribe(
    typename Subscription<To>::OnPublishCb const &on_publish,
    typename Subscription<To>::OnUnsubscribeCb const &on_unsubscribe) {

  auto subscription_to_ptr =
      this->PublisherConcrete<To>::subscribe(on_publish, on_unsubscribe);

  auto subscription_from_ptr = this->from_ptr->subscribe(
      this->get_on_publish_from(subscription_to_ptr),
      this->get_on_unsubscribe_from(subscription_to_ptr));

  this->to_from_subscription_mapping.insert(
      {subscription_to_ptr->get_instance_index(), subscription_from_ptr});

  return subscription_to_ptr;
};
template <typename From, typename To>
void BaseOperatorEmitter<From, To>::remove_subscription(
    SubscriptionConcrete<To> *subscription_to_ptr) {

  auto it = this->to_from_subscription_mapping.find(
      subscription_to_ptr->get_instance_index());
  auto subscription_from_ptr = it->second;

  subscription_from_ptr->unsubscribe();

  this->to_from_subscription_mapping.erase(it);

  this->PublisherConcrete<To>::remove_subscription(subscription_to_ptr);
};

} // namespace operators
} // namespace cpp_publisher

#endif
