#ifndef PASTE_H
#define PASTE_H

#include "Action.h"

class Paste : public Action
{
private:
    Point P; // مكان اللصق
public:
    Paste(ApplicationManager* pApp);
    virtual void ReadActionParameters();
    virtual void Execute();
};

#endif
