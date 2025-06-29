//
// Created by tobias on 21.11.16.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include <gtest/gtest.h>
#include "../Manager.h"

using namespace ClassProject;

class ManagerTest : public testing::Test {
public:
    Manager managerTest;
    BDD_ID TRUE_ID = 1;
    BDD_ID FALSE_ID = 0; 
};

class TableTest : public testing::Test {
public:
    Manager tableTest;

    void SetUp() override
    {
        tableTest.createVar("a");
        tableTest.createVar("b");
        tableTest.createVar("c");
    }

    BDD_ID TRUE_ID = 1;
    BDD_ID FALSE_ID = 0; 
};

class ITETest : public testing::Test {
public:
    Manager iteTest;

    BDD_ID TRUE_ID = 1;
    BDD_ID FALSE_ID = 0; 
};

class GateTest : public testing::Test {
public:
    Manager gateTest;

    BDD_ID A_ID = gateTest.createVar("a");
    BDD_ID B_ID = gateTest.createVar("b");
    BDD_ID C_ID = gateTest.createVar("c");
    BDD_ID D_ID = gateTest.createVar("d");

    BDD_ID TRUE_ID = 1;
    BDD_ID FALSE_ID = 0; 
};


class BDDTest : public testing::Test {
public:
    Manager manager;

    BDD_ID true_id = 1;
    BDD_ID false_id = 0; 

    BDD_ID a_id = manager.createVar("a");
    BDD_ID b_id = manager.createVar("b");
    BDD_ID c_id = manager.createVar("c");
    BDD_ID d_id = manager.createVar("d");

    BDD_ID neg_a_id = manager.neg(a_id);
    BDD_ID neg_b_id = manager.neg(b_id);

    BDD_ID a_and_b_id = manager.and2(a_id, b_id);
    BDD_ID a_or_b_id = manager.or2(a_id, b_id);
    BDD_ID a_xor_b_id = manager.xor2(a_id, b_id);
    BDD_ID a_nand_b_id = manager.nand2(a_id, b_id);
    BDD_ID a_nor_b_id = manager.nor2(a_id, b_id);
    BDD_ID a_xnor_b_id = manager.xnor2(a_id, b_id);

    BDD_ID c_or_d_id = manager.or2(c_id, d_id);

    // f1 = a*b+c+d
    BDD_ID f1_id = manager.or2(a_and_b_id, c_or_d_id);

};


#endif
