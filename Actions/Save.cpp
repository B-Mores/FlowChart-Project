#include "Save.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Output.h"
#include "..\Statements\Statement.h"
#include "..\Connector.h" // Needed to save connectors
#include <fstream>

Save::Save(ApplicationManager* pAppManager) : Action(pAppManager)
{
    FileName = "";
}

void Save::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    // Read the FileName
    pOut->PrintMessage("Enter the FileName To Save the current flowchart in it (e.g, myFlowchart.txt):");
    FileName = pIn->GetString(pOut);

    pOut->ClearStatusBar();
}

void Save::Execute()
{
    ReadActionParameters();

    //Open the file for writing
    ofstream out;
    out.open(FileName);

    // Check if the file was opened successfully
    if (!out.is_open())
    {
        pManager->GetOutput()->PrintMessage("Error: Could not open file for saving. Operation cancelled.");
        return;
    }


    // Save the total count of statements
    int StatCount = pManager->GetStatCount(); 
    out << StatCount << endl;

    // Iterate through all statements and call the its Save function
    for (int i = 0; i < StatCount; i++)
    {
        Statement* pStat = pManager->GetStatementList()[i];
        pStat->Save(out); 
    }


    // Save the total count of connectors
    int ConnCount = pManager->GetConnCount(); 
    out << ConnCount << endl;

    // Iterate through all connectors and call its Save function
    for (int i = 0; i < ConnCount; i++)
    {
        Connector* pConn = pManager->GetConnectorList()[i];
        pConn->Save(out);
    }

    //Close the file and show success message
    out.close();
    pManager->GetOutput()->PrintMessage("Flowchart saved successfully to: " + FileName);
    Sleep(1200);
    pManager->GetOutput()->ClearStatusBar();
}