
#include "AddEnd.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

AddEnd::AddEnd(ApplicationManager* pAppManager) : Action(pAppManager)
{
}

void AddEnd::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    pOut->PrintMessage("Add End: Click to place the End statement");
    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();
}

void AddEnd::Execute()
{
    ReadActionParameters();

    End* pEnd = new End(Position);
    pManager->AddStatement(pEnd);
}
