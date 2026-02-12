#include "AddWrite.h"



#include "..\ApplicationManager.h"

#include "..\GUI\input.h"
#include "..\GUI\Output.h"

#include <sstream>

using namespace std;

//constructor: set the ApplicationManager pointer inside this action
AddWrite::AddWrite(ApplicationManager* pAppManager) :Action(pAppManager)
{
    Var = "";
}



void AddWrite::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    //Read the (Position) parameter
    pOut->PrintMessage("Write Statement: Click to add  the Statement");

    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

    //Read the variable name 
    pOut->PrintMessage("Write Assignment: Enter Variable name :");
    Var = pIn->GetVariable(pOut);


}

void AddWrite::Execute()
{
    ReadActionParameters();


    //Calculating left corner of assignement statement block
    Point Corner;
    Corner.x = Position.x - UI.ASSGN_WDTH / 2;
    Corner.y = Position.y;

    // The constructor will use the validated Variable member read in ReadActionParameters
    Write* pWrite = new Write(Corner, Var);



    pManager->AddStatement(pWrite); // Adds the created statement to application manger's statement list
}