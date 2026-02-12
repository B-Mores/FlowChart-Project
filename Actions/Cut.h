#ifndef CUT_H
#define CUT_H

#include "Action.h"
#include "..\Statements\Statement.h"

class Cut : public Action
{
private:
    Statement* pStat; // statement to cut
public:
    Cut(ApplicationManager* pApp);
    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif
