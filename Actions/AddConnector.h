#ifndef ADD_CONNECTOR_H
#define ADD_CONNECTOR_H 


#include "Action.h"
#include "..\Statements\Statement.h" // Needed to store pointers to source and destination statements

class AddConnector : public Action
{
private:
    Statement* pSrcStat; // Pointer to the Source Statement (where the arrow starts)
    Statement* pTDstStat; // Pointer to the Destination Statement (where the arrow ends) or to true statement in case of cond stat

    Statement* pFDstStat;   // Pointer to the false Destination Statement

public:
    // Constructor: Initializes members
    AddConnector(ApplicationManager* pAppManager);

    // Reads action parameters (User clicks on two statements)
    virtual void ReadActionParameters();

    // Executes the action (Creates and adds the Connector object)
    virtual void Execute();
};

#endif