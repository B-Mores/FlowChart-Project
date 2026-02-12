#ifndef ADD_START_H
#define ADD_START_H

#include "Action.h"
#include "..\Statements\Start.h"

// Action to add a Start statement (ellipse labeled "Start") at click point
class AddStart : public Action
{
private:
    Point Position; // click position

public:
    AddStart(ApplicationManager* pAppManager);

    // Get click position (no other inputs)
    virtual void ReadActionParameters();

    // Create Start statement and add to manager
    virtual void Execute();
};

#endif