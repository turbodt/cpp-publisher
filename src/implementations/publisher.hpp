#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_PUBLISHER
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_PUBLISHER

#include "../interfaces/main.hpp"
#include "./emitter.hpp"
#include <memory>

namespace cpp_publisher {

template <typename T>
class PublisherConcrete : public EmitterConcrete<T>,
                          virtual public Publisher<T> {
  using EmitterConcrete<T>::EmitterConcrete;
  using EmitterConcrete<T>::subscribe;

public:
  typedef typename EmitterConcrete<T>::OnCreateCb OnCreateCb;
  typedef typename EmitterConcrete<T>::OnDestroyCb OnDestroyCb;

  virtual void publish(T const &);
  virtual Subscription<T> *
  subscribe(typename Subscription<T>::OnPublishCb const & = nullptr,
            typename Subscription<T>::OnUnsubscribeCb const & = nullptr);

  virtual Emitter<T> *as_emitter();
  virtual Source<T> *as_source();
};

} // namespace cpp_publisher

namespace cpp_publisher {

template <typename T> void PublisherConcrete<T>::publish(T const &value) {
  this->emit_to_subscriptions(value);
}

template <typename T>
Subscription<T> *PublisherConcrete<T>::subscribe(
    typename Subscription<T>::OnPublishCb const &on_publish,
    typename Subscription<T>::OnUnsubscribeCb const &on_unsubscribe) {
  return this->EmitterConcrete<T>::subscribe(on_publish, on_unsubscribe);
};

template <typename T> Emitter<T> *PublisherConcrete<T>::as_emitter() {
  return static_cast<EmitterConcrete<T> *>(this);
};

template <typename T> Source<T> *PublisherConcrete<T>::as_source() {
  return (Source<T> *)this;
}
} // namespace cpp_publisher

#endif
