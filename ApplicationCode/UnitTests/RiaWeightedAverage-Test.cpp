#include "gtest/gtest.h"

#include "RiaWeightedAverageCalculator.h"


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RiaWeightedAverageCalculator, BasicUsage)
{
    {
        RiaWeightedAverageCalculator calc;

        EXPECT_DOUBLE_EQ(0.0, calc.aggregatedWeight());
        EXPECT_DOUBLE_EQ(0.0, calc.weightedAverage());
    }


    {
        RiaWeightedAverageCalculator calc;

        std::vector<double> values {3.0, 6.0};
        std::vector<double> weights {1.0, 2.0};

        for (size_t i = 0; i< values.size(); i++)
        {
            calc.addValueAndWeight(values[i], weights[i]);
        }

        EXPECT_DOUBLE_EQ(3.0, calc.aggregatedWeight());
        EXPECT_DOUBLE_EQ(5.0, calc.weightedAverage());
    }
}