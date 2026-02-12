#include "Condition.h"
#include <sstream> 
#include <cmath> // Added: For abs() function used in geometric check
#include<fstream>



Condition::Condition(Point T, const string& lhs, const string& comp, const string& rhs)
{
    Top = T;
    width = 90;
    LHS = lhs;
    Comp = comp;
    RHS = rhs;
    Selected = false;
    UpdateStatementText();

    Inlet.x = Top.x;
    Inlet.y = Top.y;

    Outlet.x = Inlet.x;
    Outlet.y = Inlet.y + width;

    pOutConnTrue = NULL;
    pOutConnFalse = NULL;
}

void Condition::UpdateStatementText()
{
    Text = LHS + " " + Comp + " " + RHS;
}

void Condition::Draw(Output* pOut) const
{
    pOut->DrawCondtionalStat(Top, width, Text, Selected);
}

Point Condition::getInlet() const
{
    // Inlet is the top vertex of the diamond
    return Inlet;
}

Point Condition::getOutlet() const
{
    // Outlet is the bottom vertex of the diamond
    return Outlet;
}

//  implement pure virtual fn
void Condition::setOutConnector(Connector* pConn)
{
}
// implement pure virtual fn
Connector* Condition::getOutConnector() 
{
    return NULL;
}
void Condition::setOutConnectorTrue(Connector* pConn)
{
    pOutConnTrue = pConn;
}
Connector* Condition::getOutConnectorTrue() const
{
    return pOutConnTrue;
}
void Condition::setOutConnectorFalse(Connector* pConn)
{
    pOutConnFalse = pConn;
}
Connector* Condition::getOutConnectorFalse() const
{
    return pOutConnFalse;
}
bool Condition::IsPointInside(Point P)  //  smallest rectangle contain the diamond
{
    Point Right;
    Point Left;
    Point Bottom;
    Right.x = Top.x + width / 2;   Right.y = Top.y + width / 2;  // Right
    Bottom.x = Top.x;              Bottom.y = Top.y + width;      // Bottom
    Left.x = Top.x - width / 2;     Left.y = Top.y + width / 2;  // Left
    if (P.x >= Left.x && P.x <= Right.x && P.y >= Top.y && P.y <= Bottom.y)
    {
        return true;
    }
    return false;
}
Statement* Condition::Clone() const {
    Condition* s = new Condition(*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConnTrue = nullptr;    // لا ننسخ الـ connectors
    s->pOutConnFalse = nullptr;    // لا ننسخ الـ connectors
    return s;
}


 
void Condition::SetCenter(Point P)
{
    // حساب المركز الحالي للـ Conditional
    Point oldCenter;
    oldCenter.x = Top.x;
    oldCenter.y = Top.y + width / 2; // مركز المعين = منتصف الارتفاع

    // الفرق بين المركز الجديد والقديم
    int dx = P.x - oldCenter.x;
    int dy = P.y - oldCenter.y;

    // نقل Top
    Top.x += dx;
    Top.y += dy;

    // إعادة حساب Inlet و Outlet
    Inlet.x = Top.x;
    Inlet.y = Top.y;

    Outlet.x = Top.x;
    Outlet.y = Top.y + width;
}
void Condition::Save(ofstream& OutFile)
{
    OutFile << "COND" << "\t";
    OutFile << ID << "\t";
    OutFile << Top.x << "\t" << Top.y << "\t";
    OutFile << LHS << "\t";
    if (Comp == "==") OutFile << "EQULTO" << " \t";
    if (Comp == "!=") OutFile << "NOTEQULTO" << " \t";
    if (Comp == ">") OutFile << "GRT" << " \t";
    if (Comp == "<") OutFile << "SML" << " \t";
    if (Comp == "<=") OutFile << "SMALLOREQUL" << " \t";
    if (Comp == ">=") OutFile << "GRTOREQUL" << " \t";
    OutFile << RHS <<endl;
}


void Condition::Load(ifstream& In)
{
    int id, x, y;
    string lhs, rhs, op_keyword; // op_keyword holds the saved string (e.g., "EQULTO")

    // Read the 6 tokens exactly as saved: ID, X, Y, LHS, OP_KEYWORD, RHS
    // The statement type ("COND") is read outside this function.
    In >> id >> x >> y >> lhs >> op_keyword >> rhs;

    // 1. Assign ID and Position
    ID = id;
    Top.x = x;
    Top.y = y;

    // 2. Assign Expression details
    LHS = lhs;
    RHS = rhs;

    // 3. Decode the actual Comparison Operator from the keyword
    //  Convert the saved keyword (e.g., "EQULTO") back to the symbol (e.g., "==")
    if (op_keyword == "EQULTO") Comp = "==";
    else if (op_keyword == "NOTEQULTO") Comp = "!=";
    else if (op_keyword == "GRT") Comp = ">";
    else if (op_keyword == "SML") Comp = "<";
    else if (op_keyword == "SMALLOREQUL") Comp = "<=";
    else if (op_keyword == "GRTOREQUL") Comp = ">=";
    // Default or Error case handling might be needed here.

    // 4. Update the graphical text and points
    UpdateStatementText();

    Inlet.x = Top.x;
    Inlet.y = Top.y;

    Outlet.x = Inlet.x;
    Outlet.y = Inlet.y + width;

    pOutConnFalse = NULL; // Must be initialized to NULL
}


bool Condition::Validate(string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    int CurrentDeclaredCount = *DeclaredCountPtr;
    int CurrentInitializedCount = *InitializedCountPtr;

    //  Ensure the statement has two output connectors.
    if (pOutConnTrue == NULL || pOutConnFalse == NULL)
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Conditional Statement must have two output connectors (True and False).";
        return false;
    }

    //  LHS must be DECLARED.
    if (!SearchDeclared(LHS, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + LHS + " (LHS) used without prior declaration.";
        return false;
    }

    //  LHS must be INITIALIZED.
    if (!SearchDeclared(LHS, InitializedVars, CurrentInitializedCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + LHS + " (LHS) is used before initialization in the Condition.";
        return false;
    }

    // RHS must DECLARED and INITIALIZED if it is avariable 
    if (ValueOrVariable(RHS) == VARIABLE_OP)
    {
        if (!SearchDeclared(RHS, DeclaredVars, CurrentDeclaredCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + RHS + " (RHS) used without prior declaration.";
            return false;
        }
        if (!SearchDeclared(RHS, InitializedVars, CurrentInitializedCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + RHS + " (RHS) is used before initialization in the Condition.";
            return false;
        }
    }

    return true;
}
void Condition::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    // EMPTY because Run.cpp handles condition evaluation.
 
}
void Condition::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    // Edit LHS
    string newLHS;
    while (true)
    {
        pOut->PrintMessage("Enter LHS variable for the condition:");
        newLHS = pIn->GetString(pOut);
        if (!newLHS.empty()) break;
        pOut->PrintMessage("LHS cannot be empty!");
    }
    LHS = newLHS;

    // Edit comparator
    string comp;
    while (true)
    {
        pOut->PrintMessage("Enter comparator (>, <, ==, !=, >=, <=):");
        comp = pIn->GetString(pOut);
        if (comp == ">" || comp == "<" || comp == "==" || comp == "!=" || comp == ">=" || comp == "<=")
            break;
        pOut->PrintMessage("Invalid comparator!");
    }
    Comp = comp;

    // Edit RHS
    string newRHS;
    while (true)
    {
        pOut->PrintMessage("Enter RHS (number or variable):");
        newRHS = pIn->GetString(pOut);
        if (!newRHS.empty()) break;
        pOut->PrintMessage("RHS cannot be empty!");
    }
    RHS = newRHS;

    UpdateStatementText();
    pOut->ClearStatusBar();
}
