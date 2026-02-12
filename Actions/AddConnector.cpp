#include "AddConnector.h"

#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include "..\Statements\Statement.h" // Crucial for using Statement::setOutConnector()
#include "..\Statements\Condition.h"
#include "..\Connector.h" 


// Constructor: Initializes members
AddConnector::AddConnector(ApplicationManager* pAppManager) :Action(pAppManager)
{
    pSrcStat = NULL;
    pTDstStat = NULL;
    pFDstStat = NULL;
}

// Reads the source and destination statements from user clicks.
void AddConnector::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();
    Point P;

    // 1. Read the Source Statement point (where the arrow starts)
    pOut->PrintMessage("Add Connector: Click on the SOURCE Statement.");
    pIn->GetPointClicked(P);

    // Attempt to retrieve the statement at point P using ApplicationManager's helper function
    pSrcStat = pManager->GetStatement(P);

    if (pSrcStat == NULL)
    {
        pOut->PrintMessage("Error: No statement found at the source point. Operation cancelled.");
        return;
    }
    if (dynamic_cast<Condition*>(pSrcStat))
    {
        pOut->PrintMessage("Source selected. Now click on the TRUE DESTINATION Statement.");
        pIn->GetPointClicked(P);
        pTDstStat = pManager->GetStatement(P);
        pOut->PrintMessage("Source selected. Now click on the FALSE DESTINATION Statement.");
        pIn->GetPointClicked(P);
        pFDstStat = pManager->GetStatement(P);

        if (pTDstStat == NULL|| pFDstStat == NULL)
        {
            pOut->PrintMessage("Error: No statement found at the destination point. Operation cancelled.");
            Sleep(1000);
            // Ensure Execute() doesn't run if destination is invalid
            pSrcStat = NULL;
            pTDstStat == NULL;
            pFDstStat == NULL;
        }
        pOut->ClearStatusBar();
    }


    else 
    {

        // 2. Read the Destination Statement point (where the arrow ends)
        pOut->PrintMessage("Source selected. Now click on the DESTINATION Statement.");
        pIn->GetPointClicked(P);


        // Attempt to retrieve the statement at point P
        pTDstStat = pManager->GetStatement(P);

        if (pTDstStat == NULL)
        {
            pOut->PrintMessage("Error: No statement found at the destination point. Operation cancelled.");
            Sleep(1000);
            // Ensure Execute() doesn't run if destination is invalid
            pSrcStat = NULL;
        }
        //  Ensure destination is not a Start statement

        pOut->ClearStatusBar();
    }
}



void AddConnector::Execute()
{
    // 1. Read the required parameters (pSrcStat and pDstStat) from the user
    ReadActionParameters();

    // 2. Check if the source is a Conditional statement
    Condition* pCond = dynamic_cast<Condition*>(pSrcStat);

    if (pCond)
    {
        // Check for validity of both True and False destinations
        if (pSrcStat != NULL && pTDstStat != NULL && pFDstStat != NULL)
        {
            // 3. Create TRUE Connector
            Connector* pTConn = new Connector(pSrcStat, pTDstStat);
            pTConn->setCondPath(true); 

            // 4. Create FALSE Connector
            Connector* pFConn = new Connector(pSrcStat, pFDstStat);
            pFConn->setCondPath(false); 

            // 5. Add and link connectors
            pManager->AddConnector1(pTConn);
            pManager->AddConnector1(pFConn);

            pCond->setOutConnectorTrue(pTConn);
            pCond->setOutConnectorFalse(pFConn);
        }
    }
    else
        if (pSrcStat != NULL && pTDstStat != NULL)
        {
            // For non-conditional statements, pTDstStat holds the single destination

            // 3. Create a new Connector object, passing the Source and Destination
            Connector* pConn = new Connector(pSrcStat, pTDstStat);

            // 4. Add the newly created Connector to the Application Manager's list
            pManager->AddConnector1(pConn);

            // 5. Tell the Source Statement which connector it owns
            pSrcStat->setOutConnector(pConn);
        }
}