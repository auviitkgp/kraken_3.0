// Bring in my package's API, which is what I'm testing
// #include "../src/bottomcam.cpp"
// Bring in gtest
#include <gtest/gtest.h>

// Declare a test
TEST(TestSuite, testCase1)
{
  EXPECT_EQ(1, 1); // 1 == 1
  EXPECT_GT(10, 0); // 10 > 0
}
/*
// Declare another test
TEST(TestSuite, testCase2)
{
<test things here, calling EXPECT_* and/or ASSERT_* macros as needed>
}
*/
// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
