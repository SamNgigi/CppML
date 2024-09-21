#include "ETL.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(ExampleTest, TestHello){
  EXPECT_THAT(printHello(), "Hello World!");
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
