#include "Read.h"
#include <sstream> 
#include<fstream>

using namespace std;


Read::Read(Point Lcorner, string variable)
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

void Read::setVar(const string& Variable)
{
    var = Variable;
    UpdateStatementText();
}

void Read::Draw(Output* pOut) const
{
    pOut->DrawReadStat(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);
}

Point Read::getInlet() const
{
    return Inlet;
}
Point Read::getOutlet() const
{
    return Outlet;
}

void Read::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* Read::getOutConnector()
{
    return pOutConn;
}

bool Read::IsPointInside(Point P) // smallest Rectangle contain all Parallelogram 
{
    if (P.x >= (LeftCorner.x - UI.ASSGN_WDTH * 0.2) && P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
    {
        return true;
    }
    return false;

}

void Read::UpdateStatementText()
{
    ostringstream T;
    T << "Read  " << var;
    Text = T.str();
}
Statement* Read::Clone() const {
    Read* s = new Read(*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConn = nullptr;    // لا ننسخ الـ connectors
    return s;
}
void Read::SetCenter(Point P)
{
    // نحدّث مركز الشكل
    Point oldCenter;
    oldCenter.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    oldCenter.y = LeftCorner.y + UI.ASSGN_HI / 2;

    // نحسب الفرق بين النقطة الجديدة والقديمة
    int dx = P.x - oldCenter.x;
    int dy = P.y - oldCenter.y;

    // ننقل LeftCorner بناءً على الفرق
    LeftCorner.x += dx;
    LeftCorner.y += dy;

    // نعيد حساب Inlet و Outlet
    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}
void Read::Save(ofstream& OutFile)
{
    OutFile << "Read" << "\t"; 
    OutFile << ID << "\t";
    OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";
    OutFile << " Read \t" << var << "\t";
    OutFile << "\n";
}



void Read::Load(ifstream& InFile)
{
    string varName;

    InFile >> ID >> LeftCorner.x >> LeftCorner.y >> Text >> varName;
    var = varName;
    // إعادة حساب inlet/outlet
    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y - 20;

    Outlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI + 20;
    UpdateStatementText();

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}




bool Read::Validate(string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    string VarToRead = var;
    int CurrentDeclaredCount = *DeclaredCountPtr;
    int CurrentInitializedCount = *InitializedCountPtr;

    //  Ensure the statement has exactly one output connector
    if (pOutConn == NULL)
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Read statement must have exactly one output connector.";
        return false;
    }

    //  (LHS): Variable must be DECLARED
    if (!SearchDeclared(VarToRead, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarToRead + " used/assigned without prior declaration.";
        return false;
    }

    // 3. Initialization Update (LHS):  add the variable to InitializedVars list
    if (!SearchDeclared(VarToRead, InitializedVars, CurrentInitializedCount))
    {
        if (*InitializedCountPtr < InitializedMax)
        {
            InitializedVars[(*InitializedCountPtr)++] = VarToRead;
        }
        else {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Cannot initialize variable " + VarToRead + ". Initialization limit reached.";
            return false;
        }
    }
    // If the variable is already initialized, we don't return an error, just skip adding.

    return true;
}

void Read::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    pOut->PrintMessage("Enter value for " + var);
    double val = pIn->GetValue(pOut); 

 
    pManager->SetVariableValue(var, val);
}
string Read::getVar() const
{
    return var;
}

void Read::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    string varName;
    while (true)
    {
        pOut->PrintMessage("Enter variable name to Read:");
        varName = pIn->GetString(pOut);
        if (!varName.empty()) break;
        pOut->PrintMessage("Variable name cannot be empty!");
    }
    setVar(varName);

    UpdateStatementText();
    
}
