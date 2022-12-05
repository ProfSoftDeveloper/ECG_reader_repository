
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(ECG_reader_tests_suite, ECG_reader_test_case)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

