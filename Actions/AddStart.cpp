#include "AddStart.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

AddStart::AddStart(ApplicationManager* pAppManager) : Action(pAppManager)
{
}

void AddStart::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    pOut->PrintMessage("Add Start: Click to place the Start statement");
    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();
}

void AddStart::Execute()
{
    ReadActionParameters();

    // Create Start statement and add it to the application manager
    Start* pStart = new Start(Position);
    pManager->AddStatement(pStart);
}