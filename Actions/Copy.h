#ifndef COPY_H
#define COPY_H

#include "Action.h"

class Copy : public Action
{
public:
    Copy(ApplicationManager* pApp);
    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif
