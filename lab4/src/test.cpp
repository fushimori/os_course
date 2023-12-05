#include <gtest/gtest.h>
#include "GCF/gcf.h"
#include "E/exp.h"

TEST(GCF, gcf_test) {
    ASSERT_EQ(gcf(1, 2), 1);
    ASSERT_EQ(gcf(2, 4), 2);
    ASSERT_EQ(gcf(1, 1), 1);
    ASSERT_EQ(gcf(47, 11), 1 );
    ASSERT_EQ(gcf(800, 10000), 400);
}

TEST(GCF, gcf_native_test) {
    ASSERT_EQ(gcf_naive(1, 2), 1);
    ASSERT_EQ(gcf_naive(2, 4), 2);
    ASSERT_EQ(gcf_naive(1, 1), 1);
    ASSERT_EQ(gcf_naive(47, 11), 1 );
    ASSERT_EQ(gcf_naive(800, 10000), 400);
}


TEST(E, exp_test) {
    EXPECT_FLOAT_EQ(my_exp(2), 2.25);
    EXPECT_FLOAT_EQ(my_exp(1), 2);
    EXPECT_FLOAT_EQ(my_exp(-3), 3.3750007);
    EXPECT_FLOAT_EQ(my_exp(0), 1);
    EXPECT_FLOAT_EQ(my_exp(100), 2.7048113);
}

TEST(E, exp_series_test) {
    EXPECT_FLOAT_EQ(my_exp_series(2), 2.5);
    EXPECT_FLOAT_EQ(my_exp_series(1), 2);
    EXPECT_FLOAT_EQ(my_exp_series(-3), 0);
    EXPECT_FLOAT_EQ(my_exp_series(0), 1);
    EXPECT_FLOAT_EQ(my_exp_series(100), 2.718282);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 