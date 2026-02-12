#pragma once
#include "Action.h"
#include "..\ApplicationManager.h"
#include <map>
#include <string>

class Run : public Action
{
public:
    Run(ApplicationManager* pApp);
    virtual void ReadActionParameters();
    virtual void Execute();
};
