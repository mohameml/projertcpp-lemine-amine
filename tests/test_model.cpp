// #include <gtest/gtest.h>
// #include "../src/Model.hpp"

// TEST(BSModelTest, DiscountTest) {
//     BSModel model(0.05, 0.2, 1234);
//     double T = 1.0;

//     EXPECT_NEAR(model.discount(T), std::exp(-0.03), 1e-12);
// }

// TEST(BSModelTest, PathSizeTest) {
//     BSModel model(0.05, 0.2, 42);

//     int nSteps = 10;
//     std::vector<double> path(nSteps + 1);

//     model.generatePath(path, 100.0, 1.0, nSteps);

//     EXPECT_EQ(path.size(), nSteps + 1);
// }

// TEST(BSModelTest, ReproducibilityTest) {
//     BSModel model1(0.05, 0.2, 1337);
//     BSModel model2(0.05, 0.2, 1337);

//     int nSteps = 5;
//     std::vector<double> p1(nSteps + 1), p2(nSteps + 1);

//     model1.generatePath(p1, 100.0, 1.0, nSteps);
//     model2.generatePath(p2, 100.0, 1.0, nSteps);

//     for(int i = 0; i <= nSteps; ++i)
//         EXPECT_DOUBLE_EQ(p1[i], p2[i]);
// }
