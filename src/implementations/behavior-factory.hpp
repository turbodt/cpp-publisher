#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_BEHAVIOR_FACTORY
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_BEHAVIOR_FACTORY

#include "../interfaces/main.hpp"
#include "./behavior.hpp"
#include "memory"

namespace cpp_publisher {

template <typename T>
std::unique_ptr<Behavior<T>> make_behavior(
    T const &value,
    typename BehaviorConcrete<T>::OnCreateCb const &on_create = nullptr,
    typename BehaviorConcrete<T>::OnDestroyCb const &on_destroy = nullptr) {
  return std::make_unique<BehaviorConcrete<T>>(value, on_create, on_destroy);
}

} // namespace cpp_publisher

#endif
