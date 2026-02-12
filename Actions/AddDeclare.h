#ifndef ADD_DECLARE_H
#define ADD_DECLARE_H

#include "Action.h"
#include "..\Statements\Declare.h"

// Add Declare Statement Action
class AddDeclare : public Action
{
private:
    Point Position;
    string VariableName; 

public:
    AddDeclare(ApplicationManager* pAppManager);

   
    virtual void ReadActionParameters();

    virtual void Execute();
};

#endif