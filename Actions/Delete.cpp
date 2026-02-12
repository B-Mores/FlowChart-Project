#include "Delete.h"
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"

using namespace std;

Delete::Delete(ApplicationManager* pAppManager):Action(pAppManager)
{
  pStat=NULL; 
  pCon=NULL; 
}
void Delete::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    // Get the currently selected statement or connector
    pStat = pManager->GetSelectedStatement();
    pCon = pManager->GetSelectedConnector();

    if (!pStat || !pCon)
    {
        pOut->PrintMessage("Delete : No statement or connector selected, Please select a statement or connector first");
    
    }
}

void Delete::Execute()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();
    ReadActionParameters();

    if (pCon)   //if connector selected
    {
        pManager->RemoveConnector(pCon); //delete connector
        pOut->PrintMessage("Connector deleted");
    }

    if (pStat) {
        // Delete ALL connectors linked to it
        pManager->DeleteConnectorsOfstat(pStat);

        // Delete the statement itself
        pManager->RemoveStatement(pStat);

        pOut->PrintMessage("Statement deleted");
    }
}

