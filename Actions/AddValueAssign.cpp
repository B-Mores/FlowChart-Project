#include "AddValueAssign.h"
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddValueAssign::AddValueAssign(ApplicationManager* pAppManager) :Action(pAppManager)
{
    LHS = "";
    RHS = 0.0;
}



void AddValueAssign::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    //Read the (Position) parameter
    pOut->PrintMessage("Value Assignment Statement: Click to add the statement");

    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

    //DONE: Ask the user in the status bar to enter the LHS and set the data member
    pOut->PrintMessage("Value Assignment: Enter Left Hand Side (Variable Name):");
    LHS = pIn->GetVariable(pOut);

    //DONE: Ask the user in the status bar to enter the RHS and set the data member
    pOut->PrintMessage("Value Assignment: Enter Right Hand Side (Numeric Value):");
    RHS = pIn->GetValue(pOut);

    //Note: You should validate the LHS to be variable name and RHS to be a value
    //      Call the appropriate functions for this.
}

void AddValueAssign::Execute()
{
    ReadActionParameters();


    //Calculating left corner of assignement statement block
    Point Corner;
    Corner.x = Position.x - UI.ASSGN_WDTH / 2;
    Corner.y = Position.y;

    // The constructor will use the validated LHS and RHS members read in ReadActionParameters
    ValueAssign* pAssign = new ValueAssign(Corner, LHS, RHS);

    //DONE: should set the LHS and RHS of pAssign statement
    //      with the data members set and validated before in ReadActionParameters()
    // This is DONE by passing LHS and RHS directly into the constructor above.

    pManager->AddStatement(pAssign); // Adds the created statement to application manger's statement list
}