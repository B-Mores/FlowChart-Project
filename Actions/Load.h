#ifndef LOAD_H
#define LOAD_H

#include "Action.h"
#include <string>

class Load : public Action
{
private:
    string FileName;

public:
    Load(ApplicationManager* pApp);

    virtual void ReadActionParameters() override;
    virtual void Execute() override;
};

#endif
