#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_SUBSCRIPTION
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_SUBSCRIPTION

#include "../interfaces/main.hpp"
#include "../utils/main.hpp"

namespace cpp_publisher {

template <typename T> class EmitterConcrete;

template <typename T>
class SubscriptionConcrete
    : virtual public Subscription<T>,
      public utils::InstanceCounter<SubscriptionConcrete<void>> {
  friend class EmitterConcrete<T>;

public:
  typedef typename Subscription<T>::OnPublishCb OnPublishCb;
  typedef typename Subscription<T>::OnUnsubscribeCb OnUnsubscribeCb;

protected:
  OnPublishCb on_publish;
  OnUnsubscribeCb on_unsubscribe;
  EmitterConcrete<T> *emitter_ptr = nullptr;

  void run_on_publish(T const &) const;
  void run_on_unsubscribe();

public:
  SubscriptionConcrete(EmitterConcrete<T> *, OnPublishCb const & = nullptr,
                       OnUnsubscribeCb const & = nullptr);

  SubscriptionConcrete(const SubscriptionConcrete &) = delete;
  SubscriptionConcrete<T> &operator=(const SubscriptionConcrete<T> &) = delete;
  SubscriptionConcrete(SubscriptionConcrete &&);
  SubscriptionConcrete<T> &operator=(SubscriptionConcrete<T> &&);
  virtual ~SubscriptionConcrete();

  virtual Emitter<T> *get_emitter();
  virtual void unsubscribe();
};

} // namespace cpp_publisher

namespace cpp_publisher {

template <typename T>
SubscriptionConcrete<T>::SubscriptionConcrete(
    EmitterConcrete<T> *emitter_ptr, OnPublishCb const &on_publish,
    OnUnsubscribeCb const &on_unsubscribe)
    : emitter_ptr(emitter_ptr), on_publish(on_publish),
      on_unsubscribe(on_unsubscribe),
      utils::InstanceCounter<SubscriptionConcrete<void>>(){};

template <typename T> SubscriptionConcrete<T>::~SubscriptionConcrete() {
  this->unsubscribe();
};

template <typename T>
SubscriptionConcrete<T>::SubscriptionConcrete(SubscriptionConcrete<T> &&other)
    : emitter_ptr(std::move(other.emitter_ptr)),
      on_publish(std::move(other.on_publish)),
      on_unsubscribe(std::move(other.on_unsubscribe)),
      utils::InstanceCounter<SubscriptionConcrete<void>>(std::move(other)){};

template <typename T>
SubscriptionConcrete<T> &
SubscriptionConcrete<T>::operator=(SubscriptionConcrete<T> &&other) {
  if (this != &other) {
    this->utils::InstanceCounter<SubscriptionConcrete<T>>::operator=(
        std::move(other));
    this->emitter_ptr = other.emitter_ptr;
    this->on_publish = other.on_publish;
    this->on_unsubscribe = other.on_unsubscribe;

    other.emitter_ptr = nullptr;
    other.on_publish = nullptr;
    other.on_unsubscribe = nullptr;
  }
  return *this;
};

template <typename T> Emitter<T> *SubscriptionConcrete<T>::get_emitter() {
  return (Emitter<T> *)this->emitter_ptr;
};

template <typename T> void SubscriptionConcrete<T>::unsubscribe() {
  if (this->emitter_ptr != nullptr) {
    // run unsubscribe callback
    this->run_on_unsubscribe();
    // run unsubscribe handler
    this->emitter_ptr->remove_subscription(this);
    this->emitter_ptr = nullptr;
  }
};

template <typename T>
void SubscriptionConcrete<T>::run_on_publish(T const &value) const {
  if (this->on_publish != nullptr) {
    this->on_publish(value);
  }
};

template <typename T> void SubscriptionConcrete<T>::run_on_unsubscribe() {
  if (this->on_unsubscribe != nullptr) {
    this->on_unsubscribe(this);
  }
};

} // namespace cpp_publisher

#endif
