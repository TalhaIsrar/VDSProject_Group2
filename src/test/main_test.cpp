//
// Created by ludwig on 22.11.16.
//


#include "Tests.h"
#include <gtest/gtest.h>
#include <string>

using namespace ClassProject;

/// Manager Constructor
TEST_F(ManagerTest, constructor_size_test){
    BDD_ID table_size = managerTest.unique_table.size();
    EXPECT_EQ(2,table_size);
}

/// True Node
TEST_F(ManagerTest, true_test){
    BDD_ID True_ID = managerTest.True();
    EXPECT_EQ(TRUE_ID,True_ID);
    EXPECT_EQ("TRUE",managerTest.unique_table[True_ID].label);
    EXPECT_EQ(TRUE_ID,managerTest.unique_table[True_ID].high);
    EXPECT_EQ(TRUE_ID,managerTest.unique_table[True_ID].low);
    EXPECT_EQ(TRUE_ID,managerTest.unique_table[True_ID].top_var);
}

/// False Node
TEST_F(ManagerTest, false_test){
    BDD_ID False_ID = managerTest.False();
    EXPECT_EQ(FALSE_ID,False_ID);
    EXPECT_EQ("FALSE",managerTest.unique_table[False_ID].label);
    EXPECT_EQ(FALSE_ID,managerTest.unique_table[False_ID].high);
    EXPECT_EQ(FALSE_ID,managerTest.unique_table[False_ID].low);
    EXPECT_EQ(FALSE_ID,managerTest.unique_table[False_ID].top_var);
}

/// Manager Size
TEST_F(ManagerTest, manager_size_test){
    BDD_ID table_size = managerTest.uniqueTableSize();
    EXPECT_EQ(2,table_size);
}

/// Create Variable
TEST_F(ManagerTest, create_var_manager_test){
    EXPECT_EQ(2,managerTest.uniqueTableSize());
    managerTest.createVar("a");
    EXPECT_EQ(3,managerTest.uniqueTableSize());
    managerTest.createVar("b");
    EXPECT_EQ(4,managerTest.uniqueTableSize());
}

/// Unique Table Size
TEST_F(TableTest, size_create_var_test){
    BDD_ID table_size = tableTest.uniqueTableSize();
    EXPECT_EQ(5,table_size);
}

/// Create Variable
TEST_F(TableTest, check_create_var_test){
    std::string labels[3] = {"a", "b", "c"};

    for (int i = 2; i <= 4; i++){
        EXPECT_EQ(labels[i - 2],tableTest.unique_table[i].label);
        EXPECT_EQ(TRUE_ID,tableTest.unique_table[i].high);
        EXPECT_EQ(FALSE_ID,tableTest.unique_table[i].low);
        EXPECT_EQ(i,tableTest.unique_table[i].top_var);
    }
}

/// Is Constant
TEST_F(TableTest, constant_test){
    bool output[5] = {true, true, false, false, false};

    for (int i = 0; i <= 4; i++){
        EXPECT_EQ(output[i], tableTest.isConstant(i));   
    }
}

/// Is Variable
TEST_F(TableTest, variable_test){
    bool output[5] = {false, false, true, true, true};

    for (int i = 0; i <= 4; i++){
        EXPECT_EQ(output[i], tableTest.isVariable(i));   
    }
}

/// Top Variable
TEST_F(TableTest, top_var_test){
    for (int i = 2; i <= 4; i++){
        EXPECT_EQ(i, tableTest.topVar(i));   
    }
}

/// Negation
TEST_F(TableTest, neg_test){
    for (int i = 2; i <= 4; i++){
        BDD_ID neg_id_new = tableTest.neg(i);
        EXPECT_EQ(tableTest.unique_table[i].high, tableTest.unique_table[neg_id_new].low);
        EXPECT_EQ(tableTest.unique_table[i].low, tableTest.unique_table[neg_id_new].high);  
        
        BDD_ID neg_id_exists = tableTest.neg(i);
        EXPECT_EQ(tableTest.unique_table[i].high, tableTest.unique_table[neg_id_exists].low);
        EXPECT_EQ(tableTest.unique_table[i].low, tableTest.unique_table[neg_id_exists].high); 
        EXPECT_EQ(neg_id_new, neg_id_exists); 
    }
}

// If-Then-Else
TEST_F(ITETest, ite_test){   
    BDD_ID A_ID = iteTest.createVar("a");
    BDD_ID B_ID = iteTest.createVar("b");
    BDD_ID C_ID = iteTest.createVar("c");
    BDD_ID D_ID = iteTest.createVar("d");

    //Terminal cases
    BDD_ID ite = iteTest.ite(TRUE_ID, A_ID, B_ID);
    EXPECT_EQ(A_ID,ite);

    ite = iteTest.ite(FALSE_ID, A_ID, B_ID);
    EXPECT_EQ(B_ID,ite);

    ite = iteTest.ite(A_ID, TRUE_ID, FALSE_ID);
    EXPECT_EQ(A_ID,ite);

    ite = iteTest.ite(A_ID, B_ID, B_ID);
    EXPECT_EQ(B_ID,ite);

    ite = iteTest.ite(A_ID, FALSE_ID, TRUE_ID);
    EXPECT_EQ(iteTest.neg(A_ID),ite);

    // a*b
    ite = iteTest.ite(A_ID, B_ID, FALSE_ID);
    BDD_ID expected_id_A_AND_B = iteTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_A_AND_B, ite);
    EXPECT_EQ(B_ID, iteTest.unique_table[ite].high);
    EXPECT_EQ(FALSE_ID, iteTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, iteTest.unique_table[ite].top_var);

    // c*d
    ite = iteTest.ite(C_ID, D_ID, FALSE_ID);
    BDD_ID expected_id_C_AND_D = iteTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_C_AND_D, ite);
    EXPECT_EQ(D_ID, iteTest.unique_table[ite].high);
    EXPECT_EQ(FALSE_ID, iteTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, iteTest.unique_table[ite].top_var);  
    
    // expected_id_A_AND_B + expected_id_C_AND_D
    ite = iteTest.ite(expected_id_A_AND_B, TRUE_ID, expected_id_C_AND_D);

    // Checking intermediate node
    BDD_ID expected_id_intermediate_output = iteTest.uniqueTableSize()-2;

    EXPECT_EQ(expected_id_intermediate_output, ite-1);
    EXPECT_EQ(TRUE_ID, iteTest.unique_table[ite-1].high);
    EXPECT_EQ(expected_id_C_AND_D, iteTest.unique_table[ite-1].low);
    EXPECT_EQ(B_ID, iteTest.unique_table[ite-1].top_var); 
    
    // Checking output node
    BDD_ID expected_id_output = iteTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(expected_id_intermediate_output, iteTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_C_AND_D, iteTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, iteTest.unique_table[ite].top_var);   
}

// Co-factor True
TEST_F(ITETest, co_factor_true_test){
    BDD_ID A_ID = iteTest.createVar("a");
    BDD_ID B_ID = iteTest.createVar("b");
    BDD_ID C_ID = iteTest.createVar("c");
    BDD_ID D_ID = iteTest.createVar("d");

    BDD_ID expected_id_A_AND_B = iteTest.ite(A_ID, B_ID, FALSE_ID);
    BDD_ID expected_id_C_AND_D = iteTest.ite(C_ID, D_ID, FALSE_ID);
    BDD_ID expected_id_output = iteTest.ite(expected_id_A_AND_B, TRUE_ID, expected_id_C_AND_D); //Inserts 2 new nodes

    // With one argument
    EXPECT_EQ(iteTest.coFactorTrue(TRUE_ID), TRUE_ID);
    EXPECT_EQ(iteTest.coFactorTrue(FALSE_ID), FALSE_ID);

    for (int i = 2; i < iteTest.uniqueTableSize()-4 ; i++){
        EXPECT_EQ(iteTest.coFactorTrue(i), TRUE_ID);
    }

    EXPECT_EQ(iteTest.coFactorTrue(expected_id_A_AND_B), B_ID);    
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_C_AND_D), D_ID);
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_output-1), TRUE_ID);
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_output), expected_id_output-1);


    // With two arguments
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_A_AND_B, A_ID), B_ID); 
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_C_AND_D, C_ID), D_ID);   
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_C_AND_D, A_ID), expected_id_C_AND_D);    
    EXPECT_EQ(iteTest.coFactorTrue(expected_id_C_AND_D, B_ID), expected_id_C_AND_D);    

}

// Co-factor False
TEST_F(ITETest, co_factor_false_test){
    BDD_ID A_ID = iteTest.createVar("a");
    BDD_ID B_ID = iteTest.createVar("b");
    BDD_ID C_ID = iteTest.createVar("c");
    BDD_ID D_ID = iteTest.createVar("d");

    BDD_ID expected_id_A_AND_B = iteTest.ite(A_ID, B_ID, FALSE_ID);
    BDD_ID expected_id_C_AND_D = iteTest.ite(C_ID, D_ID, FALSE_ID);
    BDD_ID expected_id_output = iteTest.ite(expected_id_A_AND_B, TRUE_ID, expected_id_C_AND_D); //Inserts 2 new nodes

    // With one argument
    EXPECT_EQ(iteTest.coFactorFalse(TRUE_ID), TRUE_ID);
    EXPECT_EQ(iteTest.coFactorFalse(FALSE_ID), FALSE_ID);

    for (int i = 2; i < iteTest.uniqueTableSize()-4 ; i++){
        EXPECT_EQ(iteTest.coFactorFalse(i), FALSE_ID);
    }

    EXPECT_EQ(iteTest.coFactorFalse(expected_id_A_AND_B), FALSE_ID);
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_C_AND_D), FALSE_ID);
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_output-1), expected_id_C_AND_D);
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_output), expected_id_C_AND_D);


    // With two arguments
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_A_AND_B, A_ID), FALSE_ID); 
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_C_AND_D, C_ID), FALSE_ID);   
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_C_AND_D, A_ID), expected_id_C_AND_D);    
    EXPECT_EQ(iteTest.coFactorFalse(expected_id_C_AND_D, B_ID), expected_id_C_AND_D);   

}

// AND
TEST_F(GateTest, and_test){
    // a*b
    BDD_ID ite = gateTest.and2(A_ID, B_ID);
    BDD_ID expected_id_A_AND_B = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_A_AND_B, ite);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c*d
    ite = gateTest.and2(C_ID, D_ID);
    BDD_ID expected_id_C_AND_D = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_C_AND_D, ite);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // expected_id_A_AND_B * expected_id_C_AND_D
    ite = gateTest.and2(expected_id_A_AND_B, expected_id_C_AND_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(expected_id_output-1, gateTest.unique_table[ite].high);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var); 
}

// OR
TEST_F(GateTest, or_test){
    // a+b
    BDD_ID ite = gateTest.or2(A_ID, B_ID);
    BDD_ID expected_id_A_OR_B = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_A_OR_B, ite);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c+d
    ite = gateTest.or2(C_ID, D_ID);
    BDD_ID expected_id_C_OR_D = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_C_OR_D, ite);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // expected_id_A_OR_B + expected_id_C_OR_D
    ite = gateTest.or2(expected_id_A_OR_B, expected_id_C_OR_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_output-1, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var); 
}

// XOR
TEST_F(GateTest, xor_test){
    // a XOR b
    BDD_ID ite = gateTest.xor2(A_ID, B_ID);
    BDD_ID expected_id_A_XOR_B = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_A_XOR_B, ite);
    EXPECT_EQ(expected_id_A_XOR_B-1, gateTest.unique_table[ite].high);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c XOR d
    ite = gateTest.xor2(C_ID, D_ID);
    BDD_ID expected_id_C_XOR_D = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_C_XOR_D, ite);
    EXPECT_EQ(expected_id_C_XOR_D-1, gateTest.unique_table[ite].high);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // (expected_id_A_XOR_B) XOR (expected_id_C_XOR_D)
    ite = gateTest.xor2(expected_id_A_XOR_B, expected_id_C_XOR_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;
    BDD_ID neg_expected_id_C_XOR_D = expected_id_output-3;
    BDD_ID expected_id_intermediate_1 = expected_id_output-2; //3-9-10
    BDD_ID expected_id_intermediate_2 = expected_id_output-1; // 3-10-9

    // Checking expected_id_intermediate_1 (3-9-10)
    EXPECT_EQ(expected_id_intermediate_1, ite-2);
    EXPECT_EQ(expected_id_C_XOR_D, gateTest.unique_table[ite-2].high);
    EXPECT_EQ(neg_expected_id_C_XOR_D, gateTest.unique_table[ite-2].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-2].top_var);

    // Checking expected_id_intermediate_2 (3-10-9)
    EXPECT_EQ(expected_id_intermediate_2, ite-1);
    EXPECT_EQ(neg_expected_id_C_XOR_D, gateTest.unique_table[ite-1].high);
    EXPECT_EQ(expected_id_C_XOR_D, gateTest.unique_table[ite-1].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-1].top_var);

    // Checking final node
    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(expected_id_intermediate_1, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_intermediate_2, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var); 
}

// XNOR
TEST_F(GateTest, xnor_test){
    // a XNOR b
    BDD_ID ite = gateTest.xnor2(A_ID, B_ID);
    BDD_ID expected_id_A_XNOR_B = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_A_XNOR_B, ite);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_A_XNOR_B-1, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c XNOR d
    ite = gateTest.xnor2(C_ID, D_ID);
    BDD_ID expected_id_C_XNOR_D = gateTest.uniqueTableSize()-1;

    EXPECT_EQ(expected_id_C_XNOR_D, ite);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_C_XNOR_D-1, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // (expected_id_A_XNOR_B) XNOR (expected_id_C_XNOR_D)
    ite = gateTest.xnor2(expected_id_A_XNOR_B, expected_id_C_XNOR_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;
    BDD_ID neg_expected_id_C_XNOR_D = expected_id_output-3;
    BDD_ID expected_id_intermediate_1 = expected_id_output-2; //3-9-10
    BDD_ID expected_id_intermediate_2 = expected_id_output-1; // 3-10-9

    // Checking expected_id_intermediate_1 (3-9-10)
    EXPECT_EQ(expected_id_intermediate_1, ite-2);
    EXPECT_EQ(expected_id_C_XNOR_D, gateTest.unique_table[ite-2].high);
    EXPECT_EQ(neg_expected_id_C_XNOR_D, gateTest.unique_table[ite-2].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-2].top_var);

    // Checking expected_id_intermediate_2 (3-10-9)
    EXPECT_EQ(expected_id_intermediate_2, ite-1);
    EXPECT_EQ(neg_expected_id_C_XNOR_D, gateTest.unique_table[ite-1].high);
    EXPECT_EQ(expected_id_C_XNOR_D, gateTest.unique_table[ite-1].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-1].top_var);

    // Checking final node
    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(expected_id_intermediate_1, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_intermediate_2, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var); 
}

// NOR
TEST_F(GateTest, nor_test){
    // a NOR b
    BDD_ID ite = gateTest.nor2(A_ID, B_ID);
    BDD_ID expected_id_A_NOR_B = gateTest.uniqueTableSize()-1;
    BDD_ID neg_B_ID = expected_id_A_NOR_B-1;

    EXPECT_EQ(expected_id_A_NOR_B, ite);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(neg_B_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c NOR d
    ite = gateTest.nor2(C_ID, D_ID);
    BDD_ID expected_id_C_NOR_D = gateTest.uniqueTableSize()-1;
    BDD_ID neg_D_ID = expected_id_C_NOR_D-1;

    EXPECT_EQ(expected_id_C_NOR_D, ite);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(neg_D_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // (expected_id_A_NOR_B) NOR (expected_id_C_NOR_D)
    ite = gateTest.nor2(expected_id_A_NOR_B, expected_id_C_NOR_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;
    BDD_ID neg_expected_id_C_NOR_D = expected_id_output-2;
    BDD_ID expected_id_intermediate = expected_id_output-1; // 3-10-0

    // Checking neg_expected_id_C_NOR_D
    EXPECT_EQ(neg_expected_id_C_NOR_D, ite-2);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite-2].high);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite-2].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite-2].top_var);

    // Checking intermediate node (3-10-0)
    EXPECT_EQ(expected_id_intermediate, ite-1);
    EXPECT_EQ(neg_expected_id_C_NOR_D, gateTest.unique_table[ite-1].high);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite-1].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-1].top_var);

    // Checking final node
    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(neg_expected_id_C_NOR_D, gateTest.unique_table[ite].high);
    EXPECT_EQ(expected_id_intermediate, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var); 
}

// NAND
TEST_F(GateTest, nand_test){
    // a NAND b
    BDD_ID ite = gateTest.nand2(A_ID, B_ID);
    BDD_ID expected_id_A_NAND_B = gateTest.uniqueTableSize()-1;
    BDD_ID neg_B_ID = expected_id_A_NAND_B-1;

    EXPECT_EQ(expected_id_A_NAND_B, ite);
    EXPECT_EQ(neg_B_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);

    // c NAND d
    ite = gateTest.nand2(C_ID, D_ID);
    BDD_ID expected_id_C_NAND_D = gateTest.uniqueTableSize()-1;
    BDD_ID neg_D_ID = expected_id_C_NAND_D-1;

    EXPECT_EQ(expected_id_C_NAND_D, ite);
    EXPECT_EQ(neg_D_ID, gateTest.unique_table[ite].high);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite].top_var); 

    // (expected_id_A_NAND_B) NAND (expected_id_C_NAND_D)
    ite = gateTest.nand2(expected_id_A_NAND_B, expected_id_C_NAND_D);
    BDD_ID expected_id_output = gateTest.uniqueTableSize()-1;
    BDD_ID neg_expected_id_C_NAND_D = expected_id_output-2;
    BDD_ID expected_id_intermediate = expected_id_output-1; // 3-1-10

    // Checking neg_expected_id_C_NAND_D
    EXPECT_EQ(neg_expected_id_C_NAND_D, ite-2);
    EXPECT_EQ(D_ID, gateTest.unique_table[ite-2].high);
    EXPECT_EQ(FALSE_ID, gateTest.unique_table[ite-2].low);
    EXPECT_EQ(C_ID, gateTest.unique_table[ite-2].top_var);

    // Checking intermediate node (3-1-10)
    EXPECT_EQ(expected_id_intermediate, ite-1);
    EXPECT_EQ(TRUE_ID, gateTest.unique_table[ite-1].high);
    EXPECT_EQ(neg_expected_id_C_NAND_D, gateTest.unique_table[ite-1].low);
    EXPECT_EQ(B_ID, gateTest.unique_table[ite-1].top_var);

    // Checking final node
    EXPECT_EQ(expected_id_output, ite);
    EXPECT_EQ(expected_id_intermediate, gateTest.unique_table[ite].high);
    EXPECT_EQ(neg_expected_id_C_NAND_D, gateTest.unique_table[ite].low);
    EXPECT_EQ(A_ID, gateTest.unique_table[ite].top_var);
}

// Top Variable Name
TEST_F(TableTest, top_var_name_test){
    std::string labels[5] = {"FALSE", "TRUE", "a", "b", "c"};

    for (int i = 0; i < 5; i ++){
        EXPECT_EQ(labels[i], tableTest.getTopVarName(i));
    }

    for (int i = 2; i < 5; i ++){
        BDD_ID neg_id = tableTest.neg(i);
        EXPECT_EQ(labels[i], tableTest.getTopVarName(neg_id));
    }
}

// Find Nodes
TEST_F(GateTest, find_nodes_test){
    BDD_ID expected_id_A_AND_B = gateTest.and2(A_ID, B_ID);
    std::set<BDD_ID> reachable_nodes = {FALSE_ID, TRUE_ID, B_ID, expected_id_A_AND_B};
    std::set<BDD_ID> nodes = {};

    gateTest.findNodes(expected_id_A_AND_B, nodes);
    EXPECT_EQ(nodes, reachable_nodes);

    BDD_ID expected_id_C_XOR_D = gateTest.xor2(C_ID, D_ID);
    reachable_nodes = {FALSE_ID, TRUE_ID, D_ID, expected_id_C_XOR_D-1, expected_id_C_XOR_D};
    nodes = {};

    gateTest.findNodes(expected_id_C_XOR_D, nodes);
    EXPECT_EQ(nodes, reachable_nodes);
}

// Find Variables
TEST_F(GateTest, find_var_test){
    BDD_ID expected_id_A_AND_B = gateTest.and2(A_ID, B_ID);
    std::set<BDD_ID> reachable_vars = {A_ID, B_ID};
    std::set<BDD_ID> vars = {};

    gateTest.findVars(expected_id_A_AND_B, vars);
    EXPECT_EQ(vars, reachable_vars);

    BDD_ID expected_id_C_XOR_D = gateTest.xor2(C_ID, D_ID);
    reachable_vars = {C_ID, D_ID};
    vars = {};

    gateTest.findVars(expected_id_C_XOR_D, vars);
    EXPECT_EQ(vars, reachable_vars);
}

// one more Find Variables
TEST_F(GateTest, find_var_test_2){
    // F = a*b + a_bar*b_bar*c
    BDD_ID _B_AND_C = gateTest.and2(gateTest.neg(B_ID), C_ID);
    BDD_ID _A_AND__B_AND_C = gateTest.and2(gateTest.neg(A_ID), _B_AND_C);
    BDD_ID A_AND_B = gateTest.and2(A_ID, B_ID);
    BDD_ID F = gateTest.or2(A_AND_B, _A_AND__B_AND_C);

    std::set<BDD_ID> reachable_vars = {A_ID, B_ID, C_ID};
    std::set<BDD_ID> vars = {};
    gateTest.findVars(F, vars);
    EXPECT_EQ(vars, reachable_vars);
}



TEST_F(BDDTest, CoFactorTrueTest) /* NOLINT */
{

    EXPECT_EQ(manager.coFactorTrue(false_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(true_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(a_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(b_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id), b_id);
    EXPECT_EQ(manager.coFactorTrue(c_or_d_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(f1_id), manager.or2(b_id, c_or_d_id));

    EXPECT_EQ(manager.coFactorTrue(false_id, false_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, true_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorTrue(false_id, b_id), false_id);

    EXPECT_EQ(manager.coFactorTrue(true_id, false_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(true_id, true_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(true_id, a_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(true_id, b_id), true_id);

    EXPECT_EQ(manager.coFactorTrue(a_id, false_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, true_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, a_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(a_id, b_id), a_id);

    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, false_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, true_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, a_id), b_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, b_id), a_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, c_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorTrue(a_and_b_id, a_and_b_id), a_and_b_id);

    EXPECT_EQ(manager.coFactorTrue(c_or_d_id, c_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(c_or_d_id, d_id), true_id);

    EXPECT_EQ(manager.coFactorTrue(f1_id, a_id), manager.or2(b_id, c_or_d_id));

    EXPECT_EQ(manager.coFactorTrue(f1_id, b_id), manager.or2(a_id, c_or_d_id));

    EXPECT_EQ(manager.coFactorTrue(f1_id, c_id), true_id);
    EXPECT_EQ(manager.coFactorTrue(f1_id, d_id), true_id);

}

TEST_F(BDDTest, CoFactorFalseTest) /* NOLINT */
{

    EXPECT_EQ(manager.coFactorFalse(false_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(true_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(a_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(b_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(c_or_d_id), d_id);
    EXPECT_EQ(manager.coFactorFalse(f1_id), c_or_d_id);

    EXPECT_EQ(manager.coFactorFalse(false_id, false_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, true_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(false_id, b_id), false_id);

    EXPECT_EQ(manager.coFactorFalse(true_id, false_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(true_id, true_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(true_id, a_id), true_id);
    EXPECT_EQ(manager.coFactorFalse(true_id, b_id), true_id);

    EXPECT_EQ(manager.coFactorFalse(a_id, false_id), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, true_id), a_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_id, b_id), a_id);

    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, false_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, true_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, a_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, b_id), false_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, c_id), a_and_b_id);
    EXPECT_EQ(manager.coFactorFalse(a_and_b_id, a_and_b_id), a_and_b_id);

    EXPECT_EQ(manager.coFactorFalse(c_or_d_id, c_id), d_id);
    EXPECT_EQ(manager.coFactorFalse(c_or_d_id, d_id), c_id);

    EXPECT_EQ(manager.coFactorFalse(f1_id, a_id), c_or_d_id);
    EXPECT_EQ(manager.coFactorFalse(f1_id, b_id), c_or_d_id);
    EXPECT_EQ(manager.coFactorFalse(f1_id, c_id), manager.or2(a_and_b_id, d_id));
    EXPECT_EQ(manager.coFactorFalse(f1_id, d_id), manager.or2(a_and_b_id, c_id));
}


int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
