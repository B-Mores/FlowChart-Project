
#include "Load.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Output.h"
#include "..\GUI\Input.h"
#include "..\Statements\Statement.h"
#include "..\Statements\ValueAssign.h" // Needed for creating statements
#include "..\Statements\VariableAssign.h" // Needed for creating statements 
#include "..\Statements\OperationAssign.h" // Needed for creating statements 
#include "..\Statements\Read.h"        // Needed for creating statements
#include "..\Statements\Write.h"       // Needed for creating statements
#include "..\Statements\Condition.h"   // Needed for creating statements
#include "..\Statements\Start.h"       // Needed for creating statements
#include "..\Statements\End.h"   
#include "..\Statements\Declare.h" // Needed for creating statements
#include "..\Connector.h"              // Needed for creating connectors
#include <fstream>
#include <sstream>
#include <string>



// Constructor
Load::Load(ApplicationManager* pAppManager) : Action(pAppManager)
{
    FileName = "";
}

// Reads the filename from the user
void Load::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    pOut->PrintMessage("Load Flowchart: Enter the filename to load from (e.g., myFlowchart.txt):");
    FileName = pIn->GetString(pOut);

    pOut->ClearStatusBar();
}

void Load::Execute()
{
    ReadActionParameters();

    // 1. Open the file for reading
    ifstream InFile;
    InFile.open(FileName);

    if (!InFile.is_open())
    {
        pManager->GetOutput()->PrintMessage("Error: Could not open file '" + FileName + "'. Operation cancelled.");
        return;
    }

    // 2. Prepare the Application Manager
    // Clear the current flowchart before loading a new one
    pManager->ClearFlowchart(); //  this function exists to delete current StatList and ConnList

    // --- First Pass: Load All Statements ---

    int StatCount;
    InFile >> StatCount;

    // We use a temporary string to read the statement type keyword
    string StmtType;
    Statement* pNewStat = NULL;

    for (int i = 0; i < StatCount; i++)
    {
        InFile >> StmtType; // Read the statement type (e.g., "VAL_ASSIGN", "CONDITION")

        // 3. Create the appropriate statement object based on its type
        if (StmtType == "VAL_ASSIGN")
        {
            pNewStat = new ValueAssign();
        }
        else if (StmtType == "Read" || StmtType == "READ")

        {
            pNewStat = new Read();
        }
        else if (StmtType == "COND" || StmtType == "CONDITION")
        {
            pNewStat = new Condition();
        }
        else if (StmtType == "STRT")
        {
            pNewStat = new Start();
        }
        else if (StmtType == "END")
        {
            pNewStat = new End();
        }
        else if (StmtType == "WRITE" || StmtType == "Write")
        {
            pNewStat = new Write();
        }
        else if (StmtType == "VAR_ASSIGN")
        {
            pNewStat = new VariableAssign();
        }
        else if (StmtType == "OP_ASSIDN")
        {
            pNewStat = new OperationAssign();
        }
        else if (StmtType == "DECLARE")
        {
            pNewStat = new Declare(); // <<-- Global scope
        }

        /*else if (StmtType == "Connector")
        {
            pNewStat = new Connector();
        }*/
        // ... Add checks for all other statement types (VAR_ASSIGN, OP_ASSIGN, WRITE, etc.)

        // 4. Load the statement's specific data polymorphically
        if (pNewStat)
        {
            pNewStat->Load(InFile); // Calls the correct Load implementation (e.g., ValueAssign::Load)
            pManager->AddStatement(pNewStat);
        }
    }

    // --- Second Pass: Load All Connectors ---

    int ConnCount;
    InFile >> ConnCount;

    int SrcID, DstID, CondPathCode;

    for (int i = 0; i < ConnCount; i++)
    {
        // 5. Read Connector details (Source ID, Destination ID, Path Type)
        InFile >> SrcID >> DstID >> CondPathCode;

        // 6. Retrieve the actual statement pointers from their IDs (Assume pManager->GetStatementByID(ID) exists)
        Statement* pSrc = pManager->GetStatementByID(SrcID);
        Statement* pDst = pManager->GetStatementByID(DstID);

        if (pSrc && pDst)
        {
            // 7. Create the Connector object
Connector* pConn = new Connector(pSrc, pDst);

// 8. Set the CondPath based on the code we saved (0=Normal, 1=True, 2=False)
if (CondPathCode == 1)
{
    pConn->setCondPath(true);
    // Link the connector to the source statement's True slot (for Condition)
    dynamic_cast<Condition*>(pSrc)->setOutConnectorTrue(pConn);
}
else if (CondPathCode == 2)
{
    pConn->setCondPath(false);
    // Link the connector to the source statement's False slot (for Condition)
    dynamic_cast<Condition*>(pSrc)->setOutConnectorFalse(pConn);
}
else
{
    // Link the connector to the source statement's single output slot
    pSrc->setOutConnector(pConn);
}

// 9. Add the connector to the manager's list
pManager->AddConnector1(pConn);
        }
    }

    // 10. Finalize
    InFile.close();
    pManager->GetOutput()->PrintMessage("Flowchart loaded successfully from: " + FileName);
    pManager->UpdateInterface();

    Sleep(1200); // after 1.2 sec will vanish the mess
    pManager->GetOutput()->ClearStatusBar();

}