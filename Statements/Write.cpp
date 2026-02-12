#include "Write.h"
#include <sstream> 
#include<fstream>

using namespace std;


Write::Write(Point Lcorner, string variable)
{

    var = variable;

    UpdateStatementText();

    LeftCorner = Lcorner;

    pOutConn = NULL; //No connectors yet

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}



void Write::setVar(const string& Variable)
{
    var = Variable;
    UpdateStatementText();
}


void Write::Draw(Output* pOut) const
{
    pOut->DrawWriteStat(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);
}

Point Write::getInlet() const
{
    return Inlet;
}
Point Write::getOutlet() const
{
    return Outlet;
}

void Write::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* Write::getOutConnector()
{
    return pOutConn;
}

bool Write::IsPointInside(Point P)   // smallest Rectanglr contain all Parallelogram 
{
    if (P.x >= (LeftCorner.x- UI.ASSGN_WDTH*0.2)&& P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
    {
        return true;
    }
    return false;

}

void Write::UpdateStatementText()
{
    ostringstream T;
    T << "Write  " << var;
    Text = T.str();
}

Statement* Write::Clone() const {
    Write* s = new Write(*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConn = nullptr;    // لا ننسخ الـ connectors
    return s;
}
void Write::SetCenter(Point P)
{
    // حساب المركز الحالي
    Point oldCenter;
    oldCenter.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    oldCenter.y = LeftCorner.y + UI.ASSGN_HI / 2;

    // الفرق بين المركز الجديد والقديم
    int dx = P.x - oldCenter.x;
    int dy = P.y - oldCenter.y;

    // نقل LeftCorner
    LeftCorner.x += dx;
    LeftCorner.y += dy;

    // إعادة حساب Inlet و Outlet
    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}
void Write::Save(ofstream& OutFile)
{
    OutFile << "Write" << "\t"; 
    OutFile << ID << "\t";
    OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";
    OutFile << " Write \t" << var << "\t";
    OutFile << "\n";
}



void Write::Load(ifstream& In)
{
    int id, x, y;
    string write_keyword, varName; // write_keyword will hold "Write"

    //  Read 5 tokens exactly as saved: ID, X, Y, "Write", varName
    // The statement type ("Write") is read outside this function.
    In >> id >> x >> y >> write_keyword >> varName;

    // 1. Assign ID and Position
    ID = id;
    LeftCorner.x = x;
    LeftCorner.y = y;

    // 2. Assign Variable Name (ignoring the redundant 'write_keyword')
    var = varName;

    // 3. Update the graphical text and points
    UpdateStatementText();

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}

// In Write.cpp

bool Write::Validate(string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    string VarToWrite = var;
    int CurrentDeclaredCount = *DeclaredCountPtr;
    int CurrentInitializedCount = *InitializedCountPtr;

    //  Ensure the statement has exactly one output connector.
    if (pOutConn == NULL) 
    {
        return false; 
    }

    //  Variable must be DECLARED
    if (!SearchDeclared(VarToWrite, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarToWrite + " used without prior declaration.";
        return false;
    }

    //  The variable must be INITIALIZED
    if (!SearchDeclared(VarToWrite, InitializedVars, CurrentInitializedCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarToWrite + " is used before initialization.";
        return false;
    }

    // Write statements do not initialize new variables

    return true;
}
void Write::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    bool found;
    //  Get value of var using ApplicationManager
    double value = pManager->GetVariableValue(var, found);

    if (found)
    {
        pOut->AppendToOutputBar(var + " = " + to_string(value));
    }
    else
    {
 
        pOut->PrintMessage("Runtime Error (ID: " + to_string(ID) + "): Variable " + var + " is not initialized.");
    }
}
string Write::getVar() const {
    return var;
}

void Write::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    string varName;
    while (true)
    {
        pOut->PrintMessage("Enter variable or expression to Write:");
        varName = pIn->GetString(pOut);
        if (!varName.empty()) break;
        pOut->PrintMessage("Input cannot be empty!");
    }
    setVar(varName);

    UpdateStatementText();
}