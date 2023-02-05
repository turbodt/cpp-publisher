#ifndef H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COUNTER
#define H_CPP_PUBLISHER_MODULE_UTILS_INSTANCE_COUNTER

namespace cpp_publisher {
namespace utils {

template <class T>
class InstanceCounter {
private:
  static unsigned int instance_cnt;
  unsigned int instance_index;

public:
  InstanceCounter() {
    this->instance_index = InstanceCounter<T>::instance_cnt++;
  }
  InstanceCounter(InstanceCounter<T> &&other) = default;
  InstanceCounter &operator=(InstanceCounter<T> &&other) = default;
  unsigned int get_instance_index() const { return this->instance_index; }


  bool operator<(const InstanceCounter<T> &rhs) const {
    return this->get_instance_index() < rhs.get_instance_index();
  }
};

template <class T> unsigned int InstanceCounter<T>::instance_cnt = 0;

} // namespace utils
} // namespace cpp_publisher

#endif
