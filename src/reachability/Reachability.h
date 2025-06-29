#ifndef VDSPROJECT_REACHABILITY_H
#define VDSPROJECT_REACHABILITY_H

#include "ReachabilityInterface.h"
#include <stdexcept>

namespace ClassProject {

    class Reachability : public ReachabilityInterface {
    public:
        // Constructor
        Reachability(unsigned int stateSize, unsigned int inputSize = 0);
        
        // Implementations of the virtual function
        const std::vector<BDD_ID> &getStates() const override;
        const std::vector<BDD_ID> &getInputs() const override;
        bool isReachable(const std::vector<bool> &stateVector) override;
        int stateDistance(const std::vector<bool> &stateVector) override;
        void setTransitionFunctions(const std::vector<BDD_ID> &transitionFunctions) override;
        void setInitState(const std::vector<bool> &stateVector) override;
    
    private:
        int numStates;
        int numInputs;
        
        std::vector<BDD_ID> stateBits;
        std::vector<BDD_ID> nextStateBits;
        std::vector<BDD_ID> inputBits;
        std::vector<BDD_ID> transitionFunctions;
        std::vector<bool> initState;
        std::vector<BDD_ID> stepReachableStateSet;

        bool reachableReady;
        BDD_ID reachableStateSet;
        bool isInSet(const BDD_ID &root, const std::vector<bool> &stateVector);
    };

}
#endif
