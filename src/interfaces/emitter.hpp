#ifndef H_CPP_PUBLISHER_MODULE_INTERFACES_EMITTER
#define H_CPP_PUBLISHER_MODULE_INTERFACES_EMITTER

#include "./subscription.hpp"
#include <functional>

namespace cpp_publisher {

template <typename T> class Emitter {
public:
  typedef typename std::function<void(Emitter<T> *)> OnCreateCb;
  typedef typename std::function<void(Emitter<T> *)> OnDestroyCb;

  virtual ~Emitter(){};

  virtual Subscription<T> *
  subscribe(typename Subscription<T>::OnPublishCb const & = nullptr,
            typename Subscription<T>::OnUnsubscribeCb const & = nullptr) = 0;
};

} // namespace cpp_publisher

#endif
