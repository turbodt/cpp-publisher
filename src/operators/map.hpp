#ifndef H_CPP_PUBLISHER_MODULE_OPERATORS_MAP
#define H_CPP_PUBLISHER_MODULE_OPERATORS_MAP

#include "../publisher.hpp"
#include "../subject.hpp"
#include "../subscription.hpp"
#include <functional>
#include <memory>

namespace rxcpp {
namespace operators {

template <typename From, typename To>
std::function<std::unique_ptr<Publisher<To>>(Publisher<From> *)>
map(std::function<To(From const &)> fn) {
  return [fn](Publisher<From> *from) {
    std::cerr << "On map operator" << std::endl;
    bool is_to_alive = true;
    auto to = make_subject<To>(nullptr, [&is_to_alive](auto subject_ptr) {
      std::cerr << "Deleting to" << std::endl;
      is_to_alive = false;
    });
    auto to_ptr = to.get();
    Subscription<From> *subscription_ptr;
    subscription_ptr = from->subscribe(
        [fn, to_ptr, from, subscription_ptr, &is_to_alive](From const &value) {
          if (!is_to_alive) {
            std::cerr << "To is not alive" << std::endl;
            subscription_ptr->unsubscribe();
            return;
          }
          std::cerr << "To is alive" << std::endl;
          To new_value = fn(value);
          to_ptr->publish(new_value);
        });
    std::cerr << "Exiting map operator" << std::endl;
    return to;
  };
};

/*
template <typename From, typename To>
std::function<Publisher<To>(Publisher<From>)>
tap(std::function<To(From const &)> fn) {
  auto concrete_map = map([fn](From const &value) -> From const & {
    fn(value);
    return value;
  });
  return [concrete_map](Publisher<From> from) { return concrete_map(from); };
};
*/

} // namespace operators
} // namespace rxcpp
#endif
