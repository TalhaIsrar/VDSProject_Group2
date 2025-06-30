#include "Reachability.h"

namespace ClassProject {

    // Constructor
    Reachability::Reachability(unsigned int stateSize, unsigned int inputSize)
     : ReachabilityInterface(stateSize, inputSize)
    {
        if (stateSize == 0) {
            throw std::runtime_error("StateSize must be greater than zero");
        }

        numStates = stateSize;
        numInputs = inputSize;

        // init BDD for states and inputs
        // init State
        for(int i = 0; i < stateSize; i++)
        {
            BDD_ID bitState = createVar("s" + std::to_string(i));
            stateBits.push_back(bitState);
            initState.push_back(false);
        }

        for(int i = 0; i < inputSize; i++)
        {
            BDD_ID bitInput = createVar("i" + std::to_string(i));
            inputBits.push_back(bitInput);
        }

        for(int i = 0; i < stateSize; i++)
        {
            BDD_ID nextBitState = createVar("s'" + std::to_string(i));
            nextStateBits.push_back(nextBitState);
        }

        // init transition function (identity function)
        std::vector<BDD_ID> initTransitionFunctions;
        for(int i = 0; i < stateSize; i++)
        {
            BDD_ID tempState = stateBits.at(i);
            initTransitionFunctions.push_back(tempState);
        }
        setTransitionFunctions(initTransitionFunctions);

        // init State
        setInitState(initState);

        reachableReady  = false;
    }

    // Return state BDD_IDs
    const std::vector<BDD_ID>& Reachability::getStates() const 
    {
        return stateBits;
    }

    // Return input BDD_IDs
    const std::vector<BDD_ID>& Reachability::getInputs() const 
    {
        return inputBits;
    }

    // Check whether a given state is reachable
    bool Reachability::isReachable(const std::vector<bool>& stateVector) 
    {
        if(stateVector.size() != numStates) 
        {
            throw std::runtime_error("Size does not match with number of state bits");
        }

        if(!reachableReady)
        {
            // 3. compute the BDD for the transition relation
            BDD_ID transitionRelation = TRUE_NODE;
            for(int i = 0; i < numStates; i++)
            {
                // get BDD of next state i
                BDD_ID _s_i = nextStateBits.at(i);
            
                // get BDD of transition function
                BDD_ID delta_i = transitionFunctions.at(i);

                // update transition relation
                transitionRelation = and2(
                    transitionRelation, 
                    or2(
                        and2(_s_i, delta_i),
                        and2(neg(_s_i), neg(delta_i))
                    )
                );
            }

            // 4. compute BDD for the characteristic function of the initial state
            BDD_ID c_s = TRUE_NODE;
            for(int i = 0; i < numStates; i++)
            {
                // get BDD of current state i
                BDD_ID s_i = stateBits.at(i);

                // update characteristic function
                if(initState[i])
                {
                    c_s = and2( c_s, xnor2( s_i, TRUE_NODE));
                } else{
                    c_s = and2( c_s, xnor2( s_i, FALSE_NODE));
                }
            }

            // 5. 
            BDD_ID c_Rit = c_s;
            stepReachableStateSet.push_back(c_Rit);

            // 6.
            BDD_ID c_R = 0;
            while(c_R != c_Rit)
            {
                c_R = c_Rit;

                // 7. compute the BDD for img(s')
                BDD_ID img_s_ = and2(c_R, transitionRelation);
                for(int i = 0; i < numStates; i++)
                {
                    BDD_ID s_i = stateBits.at(i);
                    img_s_ = or2( coFactorTrue( img_s_, s_i), coFactorFalse( img_s_, s_i));
                }
                for (int i = 0; i < numInputs; i++)
                {
                    BDD_ID input_i = inputBits.at(i);
                    img_s_ = or2( coFactorTrue( img_s_, input_i), coFactorFalse( img_s_, input_i));
                }

                // 8. compute the BDD for img(s) (swap s' by s in img(s'))
                BDD_ID img_s = img_s_;
                for(int i = 0; i < numStates; i++)
                {
                    BDD_ID s_i = stateBits.at(i);
                    BDD_ID _s_i = nextStateBits.at(i);
                    img_s = and2( img_s, xnor2( s_i, _s_i));
                }
                for(int i = 0; i < numStates; i++)
                {
                    BDD_ID _s_i = nextStateBits.at(i);
                    img_s = or2( coFactorTrue( img_s, _s_i), coFactorFalse( img_s, _s_i));
                }
                
                // 9. compute the BDD for the new c_Rit
                c_Rit = or2(c_R, img_s);     
                stepReachableStateSet.push_back(c_Rit);    
            }

            reachableStateSet = c_R;
            reachableReady = true;
        }
        return isInSet(reachableStateSet, stateVector);
    }

    // Return shortest path from initial state to target state (in steps)
    int Reachability::stateDistance(const std::vector<bool>& stateVector) 
    {
        if (stateVector.size() != numStates) {
            throw std::runtime_error("Size does not match with number of state bits");
        }

        if (!isReachable(stateVector)) return -1;

        for (int i = 0; i < stepReachableStateSet.size(); i++) {
            if (isInSet(stepReachableStateSet[i], stateVector)) 
                return i;
        }

        // This should never happen if isReachable() worked correctly
        throw std::logic_error("State is reachable but not found in stepReachableStateSet");
    }

    // Set transition function for each state bit
    void Reachability::setTransitionFunctions(const std::vector<BDD_ID>& transitionFunctions) 
    {
        if(transitionFunctions.size() != numStates) {
            throw std::runtime_error("The number of transition functions does not match with that of state bits");
        }

        for(int i = 0; i < numStates; i++)
        {
            BDD_ID tempRoot = transitionFunctions[i];

            // case 1: the root does not exist in the unique table
            if(tempRoot >= uniqueTableSize())
                    throw std::runtime_error("An unknown ID is provided");

            // case 2: the BDD contains an unknown variables 
            std::set<BDD_ID> setVars;
            findVars(tempRoot, setVars);
            for(const BDD_ID& var : setVars) {
                if( (var < 2) || (var > (numStates + numInputs + 1)))
                    throw std::runtime_error("An unknown ID is provided");
            }
        }

        this -> transitionFunctions = transitionFunctions;
        reachableReady = false;
    }

    // Set a new initial state
    void Reachability::setInitState(const std::vector<bool>& stateVector) 
    {
        if (stateVector.size() != numStates) {
            throw std::runtime_error("Size does not match with number of state bits");
        }

        initState = stateVector;
        reachableReady = false;
    }

    // check if a state is in a set
    bool Reachability::isInSet(const BDD_ID &root, const std::vector<bool> &stateVector)
    {
        if(isConstant(root))
        {
            if(root == TRUE_NODE) return true;
            else return false;
        }

        BDD_ID _topVar = topVar(root);

        if(stateVector[_topVar - 2])            // the 2 first elements are constant
            return isInSet(coFactorTrue(root), stateVector);
        else 
            return isInSet(coFactorFalse(root), stateVector);
    }

}