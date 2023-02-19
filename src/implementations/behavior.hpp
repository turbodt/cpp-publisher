#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_BEHAVIOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_BEHAVIOR

#include "../interfaces/main.hpp"
#include "./publisher.hpp"
#include <memory>

namespace cpp_publisher {

template <typename T>
class BehaviorConcrete : public PublisherConcrete<T>,
                         virtual public Behavior<T> {
protected:
  T value;

public:
  typedef typename PublisherConcrete<T>::OnCreateCb OnCreateCb;
  typedef typename PublisherConcrete<T>::OnDestroyCb OnDestroyCb;

  BehaviorConcrete(T const &value, OnCreateCb const & = nullptr,
                   OnDestroyCb const & = nullptr);

  BehaviorConcrete(const BehaviorConcrete &) = delete;
  BehaviorConcrete<T> &operator=(const BehaviorConcrete<T> &) = delete;
  BehaviorConcrete(BehaviorConcrete &&);
  BehaviorConcrete<T> &operator=(BehaviorConcrete<T> &&);

  virtual void publish(T const &);
  virtual SubscriptionConcrete<T> *
  subscribe(typename Subscription<T>::OnPublishCb const & = nullptr,
            typename Subscription<T>::OnUnsubscribeCb const & = nullptr);

  virtual T get_value() override;
  virtual T const &get_value() const override;
};

} // namespace cpp_publisher

namespace cpp_publisher {

template <typename T>
BehaviorConcrete<T>::BehaviorConcrete(T const &value,
                                      OnCreateCb const &on_create,
                                      OnDestroyCb const &on_destroy)
    : value(value), PublisherConcrete<T>(on_create, on_destroy){};

template <typename T>
BehaviorConcrete<T>::BehaviorConcrete(BehaviorConcrete &&other)
    : PublisherConcrete<T>(std::move(other)){};

template <typename T>
BehaviorConcrete<T> &
BehaviorConcrete<T>::operator=(BehaviorConcrete<T> &&other) {
  if (this != &other) {
    this->value = std::move(other.value);
    this->PublisherConcrete<T>::operator=(other);
  }
  return *this;
};

template <typename T> void BehaviorConcrete<T>::publish(T const &value) {
  this->value = value;
  this->PublisherConcrete<T>::publish(value);
}

template <typename T>
SubscriptionConcrete<T> *BehaviorConcrete<T>::subscribe(
    typename Subscription<T>::OnPublishCb const &on_publish,
    typename Subscription<T>::OnUnsubscribeCb const &on_unsubscribe) {
  auto subscription =
      this->PublisherConcrete<T>::subscribe(on_publish, on_unsubscribe);
  this->emit_to_subscription(
      this->get_value(), dynamic_cast<SubscriptionConcrete<T> *>(subscription));
  return subscription;
};

template <typename T> T BehaviorConcrete<T>::get_value() {
  return this->value;
};

template <typename T> T const &BehaviorConcrete<T>::get_value() const {
  return this->value;
};

} // namespace cpp_publisher

#endif
