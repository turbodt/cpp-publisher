#include "cpp-publisher.hpp"
#include "gtest/gtest.h"
#include <iostream>

using namespace cpp_publisher;

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
