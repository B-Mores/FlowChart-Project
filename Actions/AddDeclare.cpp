#include "AddDeclare.h"

#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include "..\GUI\UI_Info.h"

#include <sstream>
using namespace std;

// constructor: sets the ApplicationManager pointer inside this action
AddDeclare::AddDeclare(ApplicationManager* pAppManager) :Action(pAppManager)
{
    VariableName = ""; 
}

void AddDeclare::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    // 1. Read the (Position) parameter
    pOut->PrintMessage("Declare Statement: Click to add the statement");
    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

    // 2. Read the Variable Name 
    pOut->PrintMessage("Declare Statement: Enter Variable Name (double type):");
    VariableName = pIn->GetVariable(pOut);

    pOut->ClearStatusBar();
}

void AddDeclare::Execute()
{
    // 1. Read the required parameters (Position, VariableName) from the user
    ReadActionParameters();

    // 2. Calculate the Left Corner of the statement block
    Point Corner;
    Corner.x = Position.x - UI.ASSGN_WDTH / 2;
    Corner.y = Position.y;

    // 3. Create a new Declare object
    Declare* pDeclare = new Declare(Corner, VariableName);

    // 4. Add the created statement to the Application Manager's statement list
    pManager->AddStatement(pDeclare);
}