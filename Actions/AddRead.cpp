#include "AddRead.h"



#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddRead::AddRead(ApplicationManager* pAppManager) :Action(pAppManager)
{
    Var = "";
}

void AddRead::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    //Read the (Position) parameter
    pOut->PrintMessage("Read Statement: Click to add the Statement");

    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

    //Read the variable name 
    pOut->PrintMessage("Read Assignment: Enter Variable name :");
    Var = pIn->GetVariable(pOut);

    
}

void AddRead::Execute()
{
    ReadActionParameters();


    //Calculating left corner of assignement statement block
    Point Corner;
    Corner.x = Position.x - UI.ASSGN_WDTH / 2;
    Corner.y = Position.y;

    // The constructor will use the validated Variable member read in ReadActionParameters
    Read* pRead = new Read(Corner, Var);

    pManager->AddStatement(pRead); // Adds the created statement to application manger's statement list
}