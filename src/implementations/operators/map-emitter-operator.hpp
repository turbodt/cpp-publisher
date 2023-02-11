#ifndef H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR
#define H_CPP_PUBLISHER_MODULE_IMPLEMENTATIONS_MODULE_OPERATORS_MAP_EMITTER_OPERATOR

#include "../../interfaces/main.hpp"
#include "../publisher.hpp"
#include <functional>
#include <memory>

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg = From const &>
class MapEmitterOperator : public EmitterOperator<From, To> {
public:
  typedef typename EmitterOperator<From, To>::ReturnType ReturnType;
  typedef typename std::function<To(MapArg)> MappingType;

private:
  void make_subsrciption(Emitter<From> *,
                         std::unique_ptr<Publisher<To>> &) const;
  std::unique_ptr<Publisher<To>> make_to_unique_ptr(Emitter<From> *) const;
  std::unique_ptr<Publisher<To>>
  make_to_unique_ptr(std::unique_ptr<Emitter<From>> &) const;

protected:
  MappingType mapping;
  virtual ReturnType operate(Emitter<From> *) override;
  virtual ReturnType operate(std::unique_ptr<Emitter<From>> &) override;

public:
  MapEmitterOperator(MappingType const &);
};

} // namespace operators
} // namespace cpp_publisher

namespace cpp_publisher {
namespace operators {

template <typename From, typename To, typename MapArg>
MapEmitterOperator<From, To, MapArg>::MapEmitterOperator(
    MapEmitterOperator<From, To, MapArg>::MappingType const &mapping)
    : mapping(mapping){};

template <typename From, typename To, typename MapArg>
void MapEmitterOperator<From, To, MapArg>::make_subsrciption(
    Emitter<From> *from_ptr,
    std::unique_ptr<Publisher<To>> &to_unique_ptr) const {
  auto to_ptr = to_unique_ptr.get();
  from_ptr->subscribe(
      [this, to_ptr](auto value) { to_ptr->publish(this->mapping(value)); });
}

template <typename From, typename To, typename MapArg>
std::unique_ptr<Publisher<To>>
MapEmitterOperator<From, To, MapArg>::make_to_unique_ptr(
    Emitter<From> *from_ptr) const {
  // TODO: Change this for a factory
  return std::make_unique<PublisherConcrete<To>>();
}

template <typename From, typename To, typename MapArg>
std::unique_ptr<Publisher<To>>
MapEmitterOperator<From, To, MapArg>::make_to_unique_ptr(
    std::unique_ptr<Emitter<From>> &from_unique_ptr) const {
  auto from_ptr = from_unique_ptr.release();
  auto on_delete = [from_ptr](auto emitter_ptr) { delete from_ptr; };
  // TODO: Change this for a factory
  return std::make_unique<PublisherConcrete<To>>(nullptr, on_delete);
}

template <typename From, typename To, typename MapArg>
typename MapEmitterOperator<From, To, MapArg>::ReturnType
MapEmitterOperator<From, To, MapArg>::operate(Emitter<From> *from_ptr) {
  auto to_unique_ptr = this->make_to_unique_ptr(from_ptr);
  this->make_subsrciption(from_ptr, to_unique_ptr);
  return to_unique_ptr;
};

template <typename From, typename To, typename MapArg>
typename MapEmitterOperator<From, To, MapArg>::ReturnType
MapEmitterOperator<From, To, MapArg>::operate(
    std::unique_ptr<Emitter<From>> &from_unique_ptr) {
  auto from_ptr = from_unique_ptr.get();
  auto to_unique_ptr = this->make_to_unique_ptr(from_unique_ptr);
  this->make_subsrciption(from_ptr, to_unique_ptr);
  return to_unique_ptr;
};

} // namespace operators
} // namespace cpp_publisher

#endif
