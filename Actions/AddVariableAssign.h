#ifndef ADD_VARIABLE_ASSIGN_H
#define ADD_VARIABLE_ASSIGN_H

#include "Action.h"
#include "..\Statements\VariableAssign.h"

// Add Variable Assignment Statement Action
class AddVariableAssign : public Action
{
private:
    // Action parameters
    Point Position; // Position where the user clicks to add the stat.
    string LHS; // Left Hand Side: The variable being assigned to
    string RHS; // Right Hand Side: The variable name 

public:
    AddVariableAssign(ApplicationManager* pAppManager);

    // Reads assignment statement parameters (Position, LHS, RHS)
    virtual void ReadActionParameters();

    // Creates and adds the assignment statement to the list of statements
    virtual void Execute();
};

#endif