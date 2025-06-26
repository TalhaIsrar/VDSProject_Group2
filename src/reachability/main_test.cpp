//
// Created by ludwig on 22.11.16.
//

#include "Tests.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();  
}


TEST_F(ReachabilityTest, HowTo_Example) { /* NOLINT */

    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});

    ASSERT_TRUE(fsm2->isReachable({false, false}));
    ASSERT_FALSE(fsm2->isReachable({false, true}));
    ASSERT_FALSE(fsm2->isReachable({true, false}));
    ASSERT_TRUE(fsm2->isReachable({true, true}));
}

TEST_F(ReachabilityTest, Constructor_Test)
{
    for (int i = 0; i < stateVars2.size(); i++)
    {
        ASSERT_EQ(fsm2->getTopVarName(stateVars2.at(i)), "s" + std::to_string(i));
    }

    ASSERT_THROW(std::make_unique<ClassProject::Reachability>(0), std::runtime_error);
    ASSERT_THROW(std::make_unique<ClassProject::Reachability>(0, 0), std::runtime_error);
}

TEST_F(ReachabilityTest, SetInitState_errortest)
{
    EXPECT_THROW(fsm2->setInitState({false, false, true});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({false});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({});, std::runtime_error);
}