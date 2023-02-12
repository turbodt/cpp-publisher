#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_PUBLISHER_FACTORY
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_PUBLISHER_FACTORY

#include "../interfaces/main.hpp"
#include "./publisher.hpp"
#include "memory"

namespace cpp_publisher {

template <typename T>
std::unique_ptr<Publisher<T>> make_publisher(
    typename PublisherConcrete<T>::OnCreateCb const &on_create = nullptr,
    typename PublisherConcrete<T>::OnDestroyCb const &on_destroy = nullptr) {
  return std::make_unique<PublisherConcrete<T>>(on_create, on_destroy);
}

} // namespace cpp_publisher

#endif
