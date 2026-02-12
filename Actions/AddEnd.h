#ifndef ADD_END_H
#define ADD_END_H

#include "Action.h"
#include "..\Statements\End.h"

class AddEnd : public Action
{
private:
    Point Position;

public:
    AddEnd(ApplicationManager* pAppManager);

    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif