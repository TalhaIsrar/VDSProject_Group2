//
// Created by ludwig on 22.11.16.
//

#include "Tests.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// Test constructor error handler
TEST_F(ReachabilityTest, ConstructorTest)
{
    // test label of states
    for (int i = 0; i < stateVars2.size(); i++)
    {
        EXPECT_EQ(fsm2->getTopVarName(stateVars2.at(i)), "s" + std::to_string(i));
    }

    // test label of inputs
    for (int i = 0; i < stateVars2.size(); i++)
    {
        EXPECT_EQ(fsm2->getTopVarName(inputVars3.at(i)), "i" + std::to_string(i));
    }

    // construct with stateSize = 0
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(0), std::runtime_error);
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(0, 0), std::runtime_error);
    EXPECT_THROW(std::make_unique<ClassProject::Reachability>(0, 10), std::runtime_error);
}

// Test setTransitionFunc error handler
TEST_F(ReachabilityTest, SetTransitionTestErr)
{
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    // unknown ID of node in transitionFunctions
    for(int i = 7; i < 50; i++)
    {
        transitionFunctions.push_back(i);
        transitionFunctions.push_back(i + 1);
        EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);
        transitionFunctions.pop_back();
        transitionFunctions.pop_back();
    }

    // large size of transitionFunctions
    transitionFunctions.push_back(fsm2->neg(s0));
    transitionFunctions.push_back(fsm2->neg(s1));
    transitionFunctions.push_back(fsm2->and2(fsm2->neg(s1), s0));
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);

    // empty transitionFunctions
    transitionFunctions.pop_back();
    transitionFunctions.pop_back();
    transitionFunctions.pop_back();
    EXPECT_THROW(fsm2->setTransitionFunctions(transitionFunctions), std::runtime_error);
}

// Test setInitState error handler
TEST_F(ReachabilityTest, SetInitStateTestErr)
{
    // mismatching size 
    EXPECT_THROW(fsm2->setInitState({true, true, true});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({false});, std::runtime_error);
    EXPECT_THROW(fsm2->setInitState({});, std::runtime_error);
}

// Example simple
TEST_F(ReachabilityTest, SimpleExampleTest)
{
    BDD_ID s0 = stateVars2.at(0);
    BDD_ID s1 = stateVars2.at(1);

    transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    fsm2->setTransitionFunctions(transitionFunctions);

    fsm2->setInitState({false,false});

    EXPECT_TRUE(fsm2->isReachable({false, false}));
    EXPECT_FALSE(fsm2->isReachable({false, true}));
    EXPECT_FALSE(fsm2->isReachable({true, false}));
    EXPECT_TRUE(fsm2->isReachable({true, true}));
}

// Example 1
TEST_F(ReachabilityExample1Test, IsReachableTest)
{
    EXPECT_THROW(fsm->isReachable({false, true}), std::runtime_error);

    EXPECT_TRUE(fsm->isReachable({false}));
    EXPECT_TRUE(fsm->isReachable({true}));
}

TEST_F(ReachabilityExample1Test, StateDistanceTest)
{
    EXPECT_THROW(fsm->stateDistance({true, false}), std::runtime_error);

    EXPECT_EQ(fsm->stateDistance({false}), 0);
    EXPECT_EQ(fsm->stateDistance({true}), 1);
}

// Example 2
TEST_F(ReachabilityExample2Test, IsReachableWorks)
{
    EXPECT_THROW(fsm->isReachable({false}), std::runtime_error);

    EXPECT_TRUE(fsm->isReachable({false, false}));
    EXPECT_FALSE(fsm->isReachable({false, true}));
    EXPECT_TRUE(fsm->isReachable({true, false}));
    EXPECT_TRUE(fsm->isReachable({true, true}));
}

TEST_F(ReachabilityExample2Test, StateDistanceWorks)
{
    EXPECT_THROW(fsm->stateDistance({false}), std::runtime_error);

    EXPECT_EQ(fsm->stateDistance({false, false}), 0);
    EXPECT_EQ(fsm->stateDistance({false, true}), -1);
    EXPECT_EQ(fsm->stateDistance({true, false}), 1);
    EXPECT_EQ(fsm->stateDistance({true, true}), 2);
}

// Example 3
TEST_F(ReachabilityExample3Test, IsReachableWorks)
{
    EXPECT_THROW(fsm->isReachable({false, false}), std::runtime_error);

    EXPECT_TRUE(fsm->isReachable({false, false, false}));
    EXPECT_TRUE(fsm->isReachable({true, false, false}));
    EXPECT_TRUE(fsm->isReachable({false, true, false}));
    EXPECT_TRUE(fsm->isReachable({true, true, false}));

    EXPECT_FALSE(fsm->isReachable({false, false, true}));
    EXPECT_FALSE(fsm->isReachable({true, false, true}));
    EXPECT_FALSE(fsm->isReachable({false, true, true}));
    EXPECT_FALSE(fsm->isReachable({true, true, true}));
}

TEST_F(ReachabilityExample3Test, StateDistanceWorks)
{
    EXPECT_THROW(fsm->stateDistance({false, true}), std::runtime_error);

    EXPECT_EQ(fsm->stateDistance({false, false, false}), 0);
    EXPECT_EQ(fsm->stateDistance({true, false, false}), 1);
    EXPECT_EQ(fsm->stateDistance({true, true, false}), 2);
    EXPECT_EQ(fsm->stateDistance({false, true, false}), 2);

    EXPECT_EQ(fsm->stateDistance({false, false, true}), -1); 
    EXPECT_EQ(fsm->stateDistance({true, false, true}), -1);
    EXPECT_EQ(fsm->stateDistance({false, true, true}), -1);
    EXPECT_EQ(fsm->stateDistance({true, true, true}), -1);
}

// Example 4
TEST_F(ReachabilityExample4Test, IsReachableWorks)
{
    EXPECT_THROW(fsm->isReachable({false, false}), std::runtime_error);

    EXPECT_TRUE(fsm->isReachable({false, false, false}));
    EXPECT_TRUE(fsm->isReachable({true, false, false}));
    EXPECT_TRUE(fsm->isReachable({false, true, false}));
    EXPECT_TRUE(fsm->isReachable({true, true, false}));
    EXPECT_TRUE(fsm->isReachable({false, true, true}));
    EXPECT_TRUE(fsm->isReachable({true, true, true}));

    EXPECT_FALSE(fsm->isReachable({false, false, true}));
    EXPECT_FALSE(fsm->isReachable({true, false, true}));
}

TEST_F(ReachabilityExample4Test, StateDistanceWorks)
{
    EXPECT_THROW(fsm->stateDistance({false, true}), std::runtime_error);

    EXPECT_EQ(fsm->stateDistance({false, false, false}), 0);
    EXPECT_EQ(fsm->stateDistance({true, false, false}), 1);
    EXPECT_EQ(fsm->stateDistance({false, true, false}), 2);
    EXPECT_EQ(fsm->stateDistance({true, true, false}), 2);
    EXPECT_EQ(fsm->stateDistance({false, true, true}), 3);
    EXPECT_EQ(fsm->stateDistance({true, true, true}), 3);

    EXPECT_EQ(fsm->stateDistance({false, false, true}), -1);
    EXPECT_EQ(fsm->stateDistance({true, false, true}), -1);
}