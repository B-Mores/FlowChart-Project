#pragma once
#ifndef SELECT_H
#define SELECT_H

#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\GUI\UI_Info.h"

class Select : public Action
{
    Point Position;  // store the clicked point
public:
    Select(ApplicationManager* pAppManager); //constructor
    virtual void ReadActionParameters();
    virtual void Execute();

};
#endif
