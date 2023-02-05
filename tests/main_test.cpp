#include "cpp-publisher.hpp"
#include "gtest/gtest.h"
#include <iostream>

TEST(emitter_concrete, constructor) {
  bool b = false;
  auto emitter = cpp_publisher::EmitterConcrete<bool>();
  EXPECT_EQ(true, true);
}

TEST(subscription_concrete, constructor) {
  bool b = false;
  auto subscription = cpp_publisher::SubscriptionConcrete<bool>(nullptr);
  EXPECT_EQ(true, true);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
