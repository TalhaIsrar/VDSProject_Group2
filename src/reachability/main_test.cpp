//
// Created by ludwig on 22.11.16.
//

#include "Tests.h"

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    // std::unique_ptr<ClassProject::ReachabilityInterface> fsm2 = std::make_unique<ClassProject::Reachability>(2);
    // std::vector<BDD_ID> stateVars2 = fsm2->getStates();
    // std::vector<BDD_ID> transitionFunctions;
    // BDD_ID s0 = stateVars2.at(0);
    // BDD_ID s1 = stateVars2.at(1);    
    // transitionFunctions.push_back(fsm2->neg(s0)); // s0' = not(s0)
    // transitionFunctions.push_back(fsm2->neg(s1)); // s1' = not(s1)
    // fsm2->setTransitionFunctions(transitionFunctions);
    // fsm2->setInitState({false,false});

    // bool b = fsm2->isReachable({false, false}); // true
    // b = fsm2->isReachable({false, true}); // true
    // b = fsm2->isReachable({true, false}); // false
    // b = fsm2->isReachable({true, true}); // false   
}
