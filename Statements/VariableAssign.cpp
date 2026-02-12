#include "VariableAssign.h"
#include <sstream> 
#include <fstream>

using namespace std;


VariableAssign::VariableAssign(Point Lcorner, string LeftHS, string RightHS) 
{
    // Note: The LeftHS and RightHS should be validated inside (AddVariableAssign) action
    //       before passing it to the constructor of ValueAssign
    LHS = LeftHS;
    RHS = RightHS;

    UpdateStatementText();

    LeftCorner = Lcorner;

    pOutConn = NULL; //No connectors yet

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}





void VariableAssign::setLHS(const string& L)
{
    LHS = L;
    UpdateStatementText();
}

void VariableAssign::setRHS(const string& R)
{
    RHS = R;
    UpdateStatementText();
}


void VariableAssign::Draw(Output* pOut) const
{
    //Call Output::DrawAssign function to draw assignment statement  
    pOut->DrawAssign(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);

}

Point VariableAssign::getInlet() const
{
    return Inlet;
}
Point VariableAssign::getOutlet() const
{
    return Outlet;
}

void VariableAssign::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* VariableAssign::getOutConnector()
{
    return pOutConn;
}


bool VariableAssign::IsPointInside(Point P)
{
    if (P.x >= LeftCorner.x && P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
    {
        return true;
    }
    return false;

}

//This function should be called when LHS or RHS changes
void VariableAssign::UpdateStatementText()
{
    //Build the statement text: Left handside then equals then right handside
    ostringstream T;
    T << LHS << " = " << RHS;
    Text = T.str();
}
Statement* VariableAssign::Clone() const {
    VariableAssign* s = new VariableAssign(*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConn = nullptr;    // لا ننسخ الـ connectors
    return s;
}

void VariableAssign::SetCenter(Point P) // غيّر اسم الكلاس هنا
{
    // 1. تحديث المركز الموروث (Center)
    Center = P;

    // 2. حساب الزاوية اليسرى العلوية (LeftCorner)
    // LeftCorner = Center - نصف العرض/نصف الارتفاع
    LeftCorner.x = Center.x - UI.ASSGN_WDTH / 2;
    LeftCorner.y = Center.y - UI.ASSGN_HI / 2;

    // 3. تحديث نقاط الوصل (Inlet/Outlet) بناءً على الموقع الجديد
    Inlet.x = Center.x;
    Inlet.y = LeftCorner.y;

    Outlet.x = Center.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}

void VariableAssign::Save(ofstream& OutFile)
{

    // 1. Write the Statement Type
    OutFile << "VAR_ASSIGN" << "\t"; // The unique keyword for Variable Assignment statement

    // 2. Write the Statement ID 
    OutFile << ID << "\t";

    // 3. Write the Statement Position (Top Left Point coordinates)
    OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";

    // 4. Write the Expression (LHS and RHS)
    // The format should be: LHS equal RHS
    OutFile << LHS << "\t" << "EQUAL \t" << RHS << "\t";

    // 5. End the line
    OutFile << "\n";

}

void VariableAssign::Load(ifstream& In)
{
    int id, x, y;
    string left, right, equal_keyword; // Must read the "EQUAL" keyword

    //  Read 7 tokens exactly as saved: ID, X, Y, LHS, "EQUAL", RHS.
    // The statement type ("VAR_ASSIGN") is read outside this function.
    In >> id >> x >> y >> left >> equal_keyword >> right;

    // 1. Assign ID
    ID = id;

    // 2. Assign Position
    LeftCorner.x = x;
    LeftCorner.y = y;

    // 3. Assign Expression details
    LHS = left;
    // We ignore 'equal_keyword'
    RHS = right;

    // 4. Update the graphical text for drawing
    UpdateStatementText();

    // 5. Update Inlet/Outlet points based on the new LeftCorner
    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}

string VariableAssign::getLHS() const { return LHS; }
string VariableAssign::getRHS() const { return RHS; }


bool VariableAssign::Validate(string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    string VarLHS = LHS;
    string VarRHS = RHS;
    int CurrentDeclaredCount = *DeclaredCountPtr;
    int CurrentInitializedCount = *InitializedCountPtr;

    //  Ensure the statement has exactly one output connector.
    if (pOutConn == NULL) 
    { 
        return false;
    }

    // Check (LHS): LHS must be DECLARED.
    if (!SearchDeclared(VarLHS, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarLHS + " (LHS) used/assigned without prior declaration.";
        return false;
    }

    //  Check (RHS): RHS must be DECLARED.
    if (!SearchDeclared(VarRHS, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS + " (RHS) used without prior declaration.";
        return false;
    }

    // 4. Initialization Check : RHS must be INITIALIZED.
    if (!SearchDeclared(VarRHS, InitializedVars, CurrentInitializedCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS + " (RHS) is used before initialization.";
        return false;
    }

    // 5. Initialization Update (LHS): Mark the variable as initialized.
    if (!SearchDeclared(VarLHS, InitializedVars, CurrentInitializedCount))
    {
        if (*InitializedCountPtr < InitializedMax)
        {
            InitializedVars[(*InitializedCountPtr)++] = VarLHS;
        }
        else {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Cannot initialize variable " + VarLHS + ". Initialization limit reached.";
            return false;
        }
    }

    return true;
}


void VariableAssign::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    bool found;
    // Get value from RHS using ApplicationManager
    double valueRHS = pManager->GetVariableValue(RHS, found);

    if (found)
    {
        //  Set value to LHS using ApplicationManager
        pManager->SetVariableValue(LHS, valueRHS);
    }
    else
    {
        pOut->PrintMessage("Runtime Error (ID: " + to_string(ID) + "): Variable " + RHS + " not initialized.");
    }
}
void VariableAssign::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    string newLHS;
    while (true)
    {
        pOut->PrintMessage("Enter variable name:");
        newLHS = pIn->GetString(pOut);
        if (!newLHS.empty()) break;
        pOut->PrintMessage("Variable name cannot be empty!");
    }
    setLHS(newLHS);

    string newRHS;
    while (true)
    {
        pOut->PrintMessage("Enter value or variable:");
        newRHS = pIn->GetString(pOut);
        if (!newRHS.empty()) break;
        pOut->PrintMessage("RHS cannot be empty!");
    }
    setRHS(newRHS);

    UpdateStatementText();
}


