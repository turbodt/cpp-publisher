#ifndef H_CPP_PUBLISHER_MODULE_INTERFACES_SOURCE
#define H_CPP_PUBLISHER_MODULE_INTERFACES_SOURCE

namespace cpp_publisher {

template <typename T> class Source {
public:
  virtual ~Source(){};
  virtual void publish(T const &) = 0;
};

} // namespace cpp_publisher

#endif
