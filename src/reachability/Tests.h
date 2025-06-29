#ifndef VDSPROJECT_REACHABILITY_TESTS_H
#define VDSPROJECT_REACHABILITY_TESTS_H

#include <gtest/gtest.h>
#include "Reachability.h"

using namespace ClassProject;

struct ReachabilityTest : testing::Test {
    
    // 2 state bits, 3 inputs
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);

    std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    std::vector<BDD_ID> inputVars3 = fsm2->getInputs();

    std::vector<BDD_ID> transitionFunctions;

};

class ReachabilityExample1Test : public testing::Test {
protected:

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;
    void SetUp() override
    {
        // 1 state bit, 2 inputs
        fsm = std::make_unique<ClassProject::Reachability>(1, 2);
        
        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID i0 = inputBits.at(0);
        BDD_ID i1 = inputBits.at(1);

        std::vector<BDD_ID> transitionFunctions;

        // s0' = neg(s0)*i1 + s0*neg(i0)
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->neg(s0), i1), fsm->and2(s0, fsm->neg(i0)))); 

        fsm->setTransitionFunctions(transitionFunctions);

        // Initial state: 0
        fsm->setInitState({false});
    }
};


class ReachabilityExample2Test : public testing::Test {
protected:

    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        // 2 state bits, 1 inputs
        fsm = std::make_unique<ClassProject::Reachability>(2, 1);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);
        BDD_ID s1 = stateBits.at(1);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID i0 = inputBits.at(0);

        std::vector<BDD_ID> transitionFunctions;

        // s0' = neg(s1)*neg(s0) + neg(s1)*s0
        transitionFunctions.push_back(fsm->or2(fsm->and2(fsm->neg(s1), fsm->neg(s0)), fsm->and2(fsm->neg(s1), s0))); 

        // s1' = neg(s1)*s0*neg(i0)
        transitionFunctions.push_back(fsm->and2(fsm->and2(fsm->neg(s1), s0), fsm->neg(i0))); 

        fsm->setTransitionFunctions(transitionFunctions);

        // Initial state: 00   
        fsm->setInitState({false, false});
    }
};

class ReachabilityExample3Test : public testing::Test {
protected:
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        // 3 state bits, 2 inputs
        fsm = std::make_unique<ClassProject::Reachability>(3, 2);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);
        BDD_ID s1 = stateBits.at(1);
        BDD_ID s2 = stateBits.at(2);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID in0 = inputBits.at(0);
        BDD_ID in1 = inputBits.at(1);

        std::vector<BDD_ID> transitionFunctions;

        // s0' = s0 XOR in0
        transitionFunctions.push_back(fsm->xor2(s0, in0));

        // s1' = s1 OR (s0 AND in1)
        transitionFunctions.push_back(fsm->or2(s1, fsm->and2(s0, in1)));

        // s2' = (s2 AND s1)
        transitionFunctions.push_back(fsm->and2(s2, s1));

        fsm->setTransitionFunctions(transitionFunctions);

        // Initial state: 000
        fsm->setInitState({false, false, false});
    }
};


class ReachabilityExample4Test : public testing::Test {
protected:
    std::unique_ptr<ClassProject::ReachabilityInterface> fsm;

    void SetUp() override
    {
        // 3 state bits, 2 inputs
        fsm = std::make_unique<ClassProject::Reachability>(3, 2);

        std::vector<BDD_ID> stateBits = fsm->getStates();
        BDD_ID s0 = stateBits.at(0);
        BDD_ID s1 = stateBits.at(1);
        BDD_ID s2 = stateBits.at(2);

        std::vector<BDD_ID> inputBits = fsm->getInputs();
        BDD_ID in0 = inputBits.at(0);
        BDD_ID in1 = inputBits.at(1);

        std::vector<BDD_ID> transitionFunctions;

        // s0' = s0 XOR in0
        transitionFunctions.push_back(fsm->xor2(s0, in0));

        // s1' = s1 OR (s0 AND in1)
        transitionFunctions.push_back(fsm->or2(s1, fsm->and2(s0, in1)));

        // s2' = s2 OR (s1 AND s0 AND in1)
        transitionFunctions.push_back(fsm->or2(
            s2,
            fsm->and2(s1, fsm->and2(s0, in1))));

        fsm->setTransitionFunctions(transitionFunctions);

        // Initial state: 000
        fsm->setInitState({false, false, false});
    }
};

#endif
