#ifndef ADD_OPERATION_ASSIGN_H
#define ADD_OPERATION_ASSIGN_H

#include "Action.h"
#include "..\Statements\OperationAssign.h"

// Add Variable Assignment Statement Action
class AddOperationAssign : public Action
{
private:
    // Action parameters
    Point Position; // Position where the user clicks to add the stat.
    string LHS; // Left Hand Side: The variable being assigned to
    string RHS1; //   variable name or value
    string RHS2; //   variable name or value
    char Operator;

public:
    AddOperationAssign(ApplicationManager* pAppManager);

    // Reads assignment statement parameters (Position, LHS, RHS)
    virtual void ReadActionParameters();

    // Creates and adds the assignment statement to the list of statements
    virtual void Execute();
};

#endif