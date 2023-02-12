#include "cpp-publisher.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <memory>

using namespace cpp_publisher;

TEST(emitter_filter_operator, one_iteration_factory) {
  int tested_value = 0;
  auto publisher_unique_ptr = std::make_unique<PublisherConcrete<int>>();
  auto emitter_ptr = publisher_unique_ptr->as_emitter();
  auto mapping = operators::filter<int>([](auto x) { return x % 3 == 0; });
  std::unique_ptr<Emitter<int>> emitter1_unique_ptr = mapping(emitter_ptr);
  auto subscription = emitter1_unique_ptr->subscribe(
      [&tested_value](auto value) { tested_value = value; });
  EXPECT_EQ(0, tested_value);
  publisher_unique_ptr->publish(3);
  EXPECT_EQ(3, tested_value);
  publisher_unique_ptr->publish(6);
  EXPECT_EQ(6, tested_value);
  publisher_unique_ptr->publish(5);
  EXPECT_EQ(6, tested_value);
  publisher_unique_ptr->publish(18);
  EXPECT_EQ(18, tested_value);
  publisher_unique_ptr->publish(2);
  EXPECT_EQ(18, tested_value);
}

TEST(emitter_map_operator, three_iterations_factory) {
  int tested_value = 0;
  auto publisher_unique_ptr = std::make_unique<PublisherConcrete<int>>();
  auto emitter_ptr = publisher_unique_ptr->as_emitter();
  auto mapping = utils::compose<std::unique_ptr<Emitter<int>>>(
      operators::map<int, int>([](auto x) { return x * x; }),
      operators::map<int, int>([](auto x) { return x + 1; }),
      operators::map<int, int>([](auto x) { return 2 * x; }));
  std::unique_ptr<Emitter<int>> emitter1_unique_ptr = mapping(emitter_ptr);
  auto subscription = emitter1_unique_ptr->subscribe(
      [&tested_value](auto value) { tested_value = value; });
  EXPECT_EQ(0, tested_value);
  publisher_unique_ptr->publish(3);
  EXPECT_EQ(20, tested_value);
  publisher_unique_ptr->publish(6);
  EXPECT_EQ(74, tested_value);
}

TEST(emitter_map_operator, three_iterations) {
  int tested_value = 0;
  auto publisher_unique_ptr = std::make_unique<PublisherConcrete<int>>();
  auto emitter_ptr = publisher_unique_ptr->as_emitter();
  auto mapping = utils::compose<std::unique_ptr<Emitter<int>>>(
      operators::MapEmitterOperator<int, int>([](auto x) { return x * x; }),
      operators::MapEmitterOperator<int, int>([](auto x) { return x + 1; }),
      operators::MapEmitterOperator<int, int>([](auto x) { return 2 * x; }));
  std::unique_ptr<Emitter<int>> emitter1_unique_ptr = mapping(emitter_ptr);
  auto subscription = emitter1_unique_ptr->subscribe(
      [&tested_value](auto value) { tested_value = value; });
  EXPECT_EQ(0, tested_value);
  publisher_unique_ptr->publish(3);
  EXPECT_EQ(20, tested_value);
  publisher_unique_ptr->publish(6);
  EXPECT_EQ(74, tested_value);
}

TEST(emitter_map_operator, one_iteration) {
  bool b = false;
  auto publisher_unique_ptr = std::make_unique<PublisherConcrete<int>>();
  auto emitter_ptr = publisher_unique_ptr->as_emitter();
  auto map1 = operators::MapEmitterOperator<int, bool>(
      [](auto x) { return (x % 2) == 0; });
  auto emitter1_unique_ptr = map1(emitter_ptr);
  auto subscription =
      emitter1_unique_ptr->subscribe([&b](bool const &value) { b = value; });
  EXPECT_EQ(false, b);
  publisher_unique_ptr->publish(42);
  EXPECT_EQ(true, b);
  publisher_unique_ptr->publish(41);
  EXPECT_EQ(false, b);
}

TEST(publisher_concrete, segregate_into_emitter_and_source) {
  bool b = false;
  auto publisher = PublisherConcrete<bool>();
  auto source_ptr = publisher.as_source();
  auto emitter_ptr = publisher.as_emitter();
  auto subscription =
      emitter_ptr->subscribe([&b](bool const &value) { b = value; });
  EXPECT_EQ(false, b);
  source_ptr->publish(true);
  EXPECT_EQ(true, b);
}

TEST(publisher_concrete, emit_and_subsribe) {
  bool b = false;
  auto publisher = PublisherConcrete<bool>();
  publisher.subscribe([&b](bool const &value) { b = value; });
  EXPECT_EQ(false, b);
  publisher.publish(true);
  EXPECT_EQ(true, b);
}

TEST(emitter_concrete, constructor) {
  bool b = false;
  auto emitter = EmitterConcrete<bool>();
  EXPECT_EQ(true, true);
}

TEST(subscription_concrete, constructor) {
  bool b = false;
  auto subscription = SubscriptionConcrete<bool>(nullptr);
  EXPECT_EQ(true, true);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
