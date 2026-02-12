#ifndef ADD_READ_H
#define ADD_READ_H

#include "Action.h"
#include "..\Statements\Read.h" 


class AddRead : public Action
{
private:

    Point Position; // Position where the user clicks to add the stat.
    string Var; 
        

public:
    AddRead(ApplicationManager* pAppManager);

    // Reads assignment statement parameters (Position, VAr)
    virtual void ReadActionParameters();

    // Creates and adds the Read statement to the list of statements
    virtual void Execute();
};

#endif