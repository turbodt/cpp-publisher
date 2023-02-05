#ifndef H_CPP_PUBLISHER_MODULE_INTERFACES_SUBSCRIPTION
#define H_CPP_PUBLISHER_MODULE_INTERFACES_SUBSCRIPTION

#include <functional>

namespace cpp_publisher {

template <typename T> class Emitter;

template <typename T> class Subscription {
public:
  typedef typename std::function<void(T const &)> OnPublishCb;
  typedef typename std::function<void(Subscription<T> *)> OnUnsubscribeCb;

  virtual ~Subscription(){};

  virtual Emitter<T> *get_emitter() = 0;
  virtual void unsubscribe() = 0;
};

} // namespace cpp_publisher

#endif
