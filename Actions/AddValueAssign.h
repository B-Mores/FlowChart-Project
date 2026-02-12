#ifndef ADD_VALUE_ASSIGN_H
#define ADD_VALUE_ASSIGN_H

#include "Action.h"
#include "..\Statements\ValueAssign.h" 

// Add Value Assignment Statement Action
// This class is responsible for 
// 1 - Getting Assignment stat. coordinates from the user (Position).
// 2 - Getting the LHS (variable name) and RHS (numeric value) from the user.
// 3 - Creating an object of ValueAssign class.
// 4 - Adding the created object to the statements list of the Application Manager.
class AddValueAssign : public Action
{
private:
   
    Point Position; // Position where the user clicks to add the stat.
    string LHS; // Left Hand Side: The variable being assigned to
    double RHS;      // Right Hand Side: The numeric value

public:
    AddValueAssign(ApplicationManager* pAppManager);

    // Reads assignment statement parameters (Position, LHS, RHS)
    virtual void ReadActionParameters();

    // Creates and adds the assignment statement to the list of statements
    virtual void Execute();
};

#endif