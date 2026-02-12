#pragma once
#ifndef CPP_H
#define CPP_H
#include "Action.h"
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include "..\Statements\Statement.h"
#include "..\GUI\UI_Info.h"

using namespace std;
class cppc : public Action
{ public:
   cppc(ApplicationManager* pAppManager); //constructor
    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif