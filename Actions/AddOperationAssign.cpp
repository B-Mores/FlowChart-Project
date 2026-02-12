#include "AddOperationAssign.h"

// Original Includes from your project:
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
#include "..\GUI\UI_Info.h"
#include "..\HelperFn.h"

#include <sstream>
using namespace std;

// constructor: sets the ApplicationManager pointer inside this action
AddOperationAssign::AddOperationAssign(ApplicationManager* pAppManager) :Action(pAppManager)
{
    LHS = "";
    RHS1 = "";
    RHS2 = "";
    Operator = '+';
}


void AddOperationAssign::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    // 1. Read the (Position) parameter
    pOut->PrintMessage("Operation Assignment Statement: Click to add the statement");
    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

    // 2. Read LHS (The assigned variable)
    pOut->PrintMessage("Operation Assignment: Enter Left Hand Side (Variable Name):");
    LHS = pIn->GetVariable(pOut);

    // 3. Read RHS1 (First Right Hand Side Operand) and validate
    pOut->PrintMessage("Operation Assignment: Enter First Right Hand Side (Variable or Value):");
    RHS1 = pIn->GetString(pOut);

    // Validation loop for RHS1: must be a Variable or a Value (using HelperFn)
    while (ValueOrVariable(RHS1) == INVALID_OP)
    {
        pOut->PrintMessage("Invalid input for RHS1. Must be a variable name or a numeric value. Try again:");
        RHS1 = pIn->GetString(pOut);
    }

    // 4. Read the Arithmetic Operator
    pOut->PrintMessage("Operation Assignment: Enter arithmetic operator (+, -, *, /):");
    Operator = pIn->GetArithOperator(pOut);

    // 5. Read RHS2 (Second Right Hand Side Operand) and validate
    pOut->PrintMessage("Operation Assignment: Enter Second Right Hand Side (Variable or Value):");
    RHS2 = pIn->GetString(pOut);

    // Validation loop for RHS2: must be a Variable or a Value (using HelperFn)
    while (ValueOrVariable(RHS2) == INVALID_OP)
    {
        pOut->PrintMessage("Invalid input for RHS2. Must be a variable name or a numeric value. Try again:");
        RHS2 = pIn->GetString(pOut);
    }

    pOut->ClearStatusBar();
}

void AddOperationAssign::Execute()
{
    // 1. Read the required parameters (Position, LHS, RHS) from the user
    ReadActionParameters();

    // 2. Calculate the Left Corner of the assignment statement block
    Point Corner;
    // The position is usually the center, so calculate the top-left corner
    Corner.x = Position.x - UI.ASSGN_WDTH / 2;
    Corner.y = Position.y;

    // 3. Create a new VariableAssign object using the collected parameters
    OperationAssign* pAssign = new OperationAssign(Corner, LHS, RHS1, RHS2, Operator);

    // 4. Add the created statement to the Application Manager's statement list
    pManager->AddStatement(pAssign);
}