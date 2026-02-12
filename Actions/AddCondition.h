#ifndef ADD_CONDITION_H
#define ADD_CONDITION_H 


#include "Action.h"
#include "..\Statements\Condition.h"
#include "..\Statements\Statement.h" // Needed for pNextTrueStat/pNextFalseStat pointers


// Add Condition Action
class AddCondition : public Action
{
private:
    Point Position; // placement (top vertex)
    string LHS, Comp, RHS;

    // Added: Pointers to the destination statements for the two paths
   

public:
    AddCondition(ApplicationManager* pAppManager);

    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif