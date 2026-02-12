#include "Declare.h"
#include <sstream>
#include<fstream>

using namespace std;


Declare::Declare(Point Lcorner, string Variable) 
{
    var = Variable;

    UpdateStatementText();

    LeftCorner = Lcorner;
    pOutConn = NULL;

  
    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}



void Declare::setVariableName(const string& Variable)
{
    var = Variable;
    UpdateStatementText();
}


void Declare::Draw(Output* pOut) const
{
    pOut->DrawAssign(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);
}

Point Declare::getInlet() const
{
    return Inlet;
}
Point Declare::getOutlet() const
{
    return Outlet;
}

void Declare::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* Declare::getOutConnector()
{
    return pOutConn;
}


bool Declare::IsPointInside(Point P)
{ 
        if (P.x >= LeftCorner.x && P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
        {
            return true;
        }
        return false;
    
}

void Declare::UpdateStatementText()
{
    ostringstream T;
    T << "double  " <<  var;
    Text = T.str();
}
Statement* Declare::Clone() const {
    Declare * s = new Declare (*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConn = nullptr;    // لا ننسخ الـ connectors
    return s;
}
void Declare::SetCenter(Point P)
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
void Declare::Save(ofstream& OutFile)
{
    OutFile << "DECLARE" << "\t"; 
    OutFile << ID << "\t";
    OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";
    OutFile << " double \t" << var << "\t";
    OutFile << "\n";
}




void Declare::Load(ifstream& In)
{
    int id, x, y;
    string type_keyword, varName; // type_keyword will hold "double"

    // Read the 5 tokens exactly as saved: ID, X, Y, "double", varName
    // The statement type ("DECLARE") is read outside this function.
    In >> id >> x >> y >> type_keyword >> varName;

    // 1. Assign ID and Position
    ID = id;
    LeftCorner.x = x;
    LeftCorner.y = y;

    // 2. Assign Variable Name (ignoring the redundant 'type_keyword' which holds "double")
    var = varName;

    // 3. Update the graphical text and points
    UpdateStatementText();

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}







bool Declare::Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    string VarToDeclare = GetVarName();
    int CurrentDeclaredCount = *DeclaredCountPtr;

    // Ensure the statement has exactly one output connector
    if (getOutConnector() == NULL)
    {
        ErrorMsg = "Declare Statement (" + Text + ", ID: " + to_string(ID) + ") must have one outgoing connector.";
        return false;
    }

    //   (Must not be in DeclaredVars list yet)
    if (SearchDeclared(VarToDeclare, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarToDeclare + " is already declared.";
        return false;
    }

    // Declaration Update: Add the variable to the DeclaredVars list.
    if (*DeclaredCountPtr < DeclaredMax)
    {
        DeclaredVars[(*DeclaredCountPtr)++] = VarToDeclare;
    }
    else 
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Maximum declared variable limit (" + to_string(DeclaredMax) + ") reached.";
        return false;
    }

    //  Initialization: Declare does not initialize a value, so InitializedVars list is not modified.
    return true;
}

string Declare::GetVarName() const
{
    //  VarName is a member of Declare statement
    return var;
}

void Declare::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    bool found;
    pManager->GetVariableValue(var, found);

    if (!found)
    {
        // SetVariableValue will create the variable if it doesn't exist
        pManager->SetVariableValue(var, 0.0);
    }

}

void Declare::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    string varName;
    while (true)
    {
        pOut->PrintMessage("Enter variable name to declare:");
        varName = pIn->GetString(pOut);
        if (!varName.empty()) break;
        pOut->PrintMessage("Variable name cannot be empty!");
    }
    setVariableName(varName);

    UpdateStatementText();
 
}


