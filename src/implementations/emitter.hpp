#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_EMITTER
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_EMITTER

#include "../interfaces/main.hpp"
#include "./subscription.hpp"
#include <functional>
#include <map>
#include <memory>

namespace cpp_publisher {

template <typename T> class EmitterConcrete : virtual public Emitter<T> {
  friend class SubscriptionConcrete<T>;

public:
  typedef typename Emitter<T>::OnCreateCb OnCreateCb;
  typedef typename Emitter<T>::OnDestroyCb OnDestroyCb;

protected:
  OnCreateCb on_create;
  OnDestroyCb on_destroy;
  Emitter<T> *emitter_ptr = nullptr;

  std::map<unsigned int, std::unique_ptr<SubscriptionConcrete<T>>>
      subscriptions =
          std::map<unsigned int, std::unique_ptr<SubscriptionConcrete<T>>>();

  virtual void emit_to_subscription(T const &, SubscriptionConcrete<T> *) const;
  virtual void emit_to_subscriptions(T const &) const;
  virtual void add_subscription(std::unique_ptr<SubscriptionConcrete<T>> &);
  virtual void remove_subscription(SubscriptionConcrete<T> *);
  virtual void unsubscribe_all();

public:
  EmitterConcrete(OnCreateCb const & = nullptr, OnDestroyCb const & = nullptr);

  EmitterConcrete(const EmitterConcrete &) = delete;
  EmitterConcrete<T> &operator=(const EmitterConcrete<T> &) = delete;
  EmitterConcrete(EmitterConcrete &&);
  EmitterConcrete<T> &operator=(EmitterConcrete<T> &&);
  virtual ~EmitterConcrete();

  virtual Subscription<T> *
  subscribe(typename Subscription<T>::OnPublishCb const & = nullptr,
            typename Subscription<T>::OnUnsubscribeCb const & = nullptr);
};

} // namespace cpp_publisher

namespace cpp_publisher {

template <typename T>
EmitterConcrete<T>::EmitterConcrete(OnCreateCb const &on_create,
                                    OnDestroyCb const &on_destroy)
    : on_create(on_create), on_destroy(on_destroy) {
  if (this->on_create != nullptr) {
    this->on_create(this);
  }
};

template <typename T>
EmitterConcrete<T>::EmitterConcrete(EmitterConcrete &&other){};

template <typename T>
EmitterConcrete<T> &EmitterConcrete<T>::operator=(EmitterConcrete<T> &&other) {
  if (this != &other) {
    this->on_create = other.on_create;
    this->on_destroy = other.on_destroy;

    other.on_create = nullptr;
    other.on_destroy = nullptr;
  }
  return *this;
};

template <typename T> EmitterConcrete<T>::~EmitterConcrete() {
  if (this->on_destroy != nullptr) {
    this->on_destroy(this);
  }
  this->unsubscribe_all();
};

template <typename T>
Subscription<T> *EmitterConcrete<T>::subscribe(
    typename Subscription<T>::OnPublishCb const &on_publish,
    typename Subscription<T>::OnUnsubscribeCb const &on_unsubscribe) {
  auto subscription_up = std::make_unique<SubscriptionConcrete<T>>(
      this, on_publish, on_unsubscribe);
  auto subscription_ptr = subscription_up.get();
  this->add_subscription(subscription_up);
  return subscription_ptr;
};

template <typename T>
void EmitterConcrete<T>::emit_to_subscription(
    T const &value, SubscriptionConcrete<T> *subscription_ptr) const {
  subscription_ptr->run_on_publish(value);
}

template <typename T>
void EmitterConcrete<T>::emit_to_subscriptions(T const &value) const {
  for (auto &it : this->subscriptions) {
    this->emit_to_subscription(value, it.second.get());
  }
}

template <typename T>
void EmitterConcrete<T>::add_subscription(
    std::unique_ptr<SubscriptionConcrete<T>> &subscription_up) {
  unsigned int subscription_index = subscription_up->get_instance_index();
  auto &r = this->subscriptions[subscription_index];
  r = std::move(subscription_up);
}

template <typename T>
void EmitterConcrete<T>::remove_subscription(
    SubscriptionConcrete<T> *subscription_ptr) {
  subscription_ptr->emitter_ptr = nullptr;
  this->subscriptions.erase(subscription_ptr->get_instance_index());
}

template <typename T> void EmitterConcrete<T>::unsubscribe_all() {
  while (this->subscriptions.size() > 0) {
    Subscription<T> *subscription_ptr =
        this->subscriptions.begin()->second.get();
    subscription_ptr->unsubscribe();
  }
}

} // namespace cpp_publisher

#endif
