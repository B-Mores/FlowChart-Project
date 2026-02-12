#include "OperationAssign.h"
#include <sstream> 
#include<fstream>


using namespace std;


OperationAssign::OperationAssign(Point Lcorner, string LeftHS,  string RightHS1, string RightHS2, char op)
{
    // Note: The LeftHS and RightHS should be validated inside (AddVariableAssign) action
    //       before passing it to the constructor of ValueAssign
 
    LHS = LeftHS;
    RHS1 = RightHS1;
    RHS2 = RightHS2;
    Operator = op;


    UpdateStatementText();

    LeftCorner = Lcorner;

    pOutConn = NULL; //No connectors yet

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}





void OperationAssign::setLHS(const string& L)
{
    LHS = L;
    UpdateStatementText();
}

void OperationAssign::setRHS1(const string& R1)
{
    RHS1 = R1;
    UpdateStatementText();
}
void OperationAssign::setRHS2(const string& R2)
{
    RHS2 = R2;
    UpdateStatementText();
}
void OperationAssign::setOPerator(char op)
{
    Operator = op;
    UpdateStatementText();
}

string OperationAssign::getLHS()
{
    return LHS;
}

string OperationAssign::getOp1()
{
    return RHS1;
}

char OperationAssign::getOperator()
{
    return Operator;
}




string OperationAssign::getOp2()
{
    return RHS2;
}

void OperationAssign::Draw(Output* pOut) const
{
    //Call Output::DrawAssign function to draw assignment statement  
    pOut->DrawAssign(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);

}

Point OperationAssign::getInlet() const
{
    return Inlet;
}
Point OperationAssign::getOutlet() const
{
    return Outlet;
}

void OperationAssign::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector*OperationAssign::getOutConnector()
{
    return pOutConn;
}

bool OperationAssign::IsPointInside(Point P)
{
    if (P.x >= LeftCorner.x && P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
    {
        return true;
    }
    return false;

}


 
//This function should be called when LHS or RHS changes
void OperationAssign::UpdateStatementText()
{
    //Build the statement text: Left handside then equals then right handside
    ostringstream T;
    T << LHS << " = " << RHS1 << " " << Operator << " " << RHS2;
    Text = T.str();
}
Statement* OperationAssign::Clone() const {
    OperationAssign* s = new OperationAssign(*this);
    s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
    s->pOutConn = nullptr;    // لا ننسخ الـ connectors
    return s;
}
void OperationAssign::SetCenter(Point P) // غيّر اسم الكلاس هنا
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

void OperationAssign::Save(ofstream& OutFile)
{
    OutFile << "OP_ASSIDN" << "\t";
    OutFile << ID << "\t";
    OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";
    OutFile << LHS << "\t";
    OutFile << RHS1 << "\t";
    if (Operator == '+') OutFile << "ADD \t";
    else if (Operator == '-') OutFile << "SUB \t";
    else if (Operator == '*') OutFile << "MUL \t";
    else if (Operator == '/') OutFile << "DIV \t";
    OutFile << RHS2 << "\t";
    OutFile << endl;
}

void OperationAssign::Load(ifstream& In)
{
    int id, x, y;
    string left, r1, r2, op_keyword; // r1, r2 for RHS1, RHS2

    // Read the 8 tokens exactly as saved: ID, X, Y, LHS, RHS1, OP_KEYWORD, RHS2
    In >> id >> x >> y >> left >> r1 >> op_keyword >> r2;

    // 1. Assign ID and Position
    ID = id;
    LeftCorner.x = x;
    LeftCorner.y = y;

    // 2. Assign Expression details
    LHS = left;
    RHS1 = r1;
    RHS2 = r2;

    // 3. Decode the Operator from the keyword (e.g., "ADD" -> '+')
    if (op_keyword == "ADD") Operator = '+';
    else if (op_keyword == "SUB") Operator = '-';
    else if (op_keyword == "MUL") Operator = '*';
    else if (op_keyword == "DIV") Operator = '/';


    // 4. Update the graphical text and points
    UpdateStatementText();

    Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
    Inlet.y = LeftCorner.y;

    Outlet.x = Inlet.x;
    Outlet.y = LeftCorner.y + UI.ASSGN_HI;
}



bool OperationAssign::Validate(string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    string VarLHS = LHS;
    string VarRHS1 = RHS1;
    string VarRHS2 = RHS2;
    int CurrentDeclaredCount = *DeclaredCountPtr;
    int CurrentInitializedCount = *InitializedCountPtr;

    //  Ensure the statement has exactly one output connector.
    if (pOutConn == NULL)
    {
        return false;
    }

    //  LHS must be DECLARED.
    if (!SearchDeclared(VarLHS, DeclaredVars, CurrentDeclaredCount))
    {
        ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarLHS + " (LHS) used/assigned without prior declaration.";
        return false;
    }

    //  (RHS1): Must be DECLARED and INITIALIZED.
    if (!IsValue(VarRHS1))
    {
        if (!SearchDeclared(VarRHS1, DeclaredVars, CurrentDeclaredCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS1 + " in RHS used without prior declaration.";
            return false;
        }
        if (!SearchDeclared(VarRHS1, InitializedVars, CurrentInitializedCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS1 + " in RHS is used before initialization.";
            return false;
        }
    }

    //  (RHS2): Must be DECLARED and INITIALIZED if it is variable
    if (IsVariable(VarRHS2))
    {
        if (!SearchDeclared(VarRHS2, DeclaredVars, CurrentDeclaredCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS2 + " in RHS used without prior declaration.";
            return false;
        }
        if (!SearchDeclared(VarRHS2, InitializedVars, CurrentInitializedCount))
        {
            ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarRHS2 + " in RHS is used before initialization.";
            return false;
        }
    }

    //  Initialization Update (LHS): Mark the variable as initialized.
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

void OperationAssign::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    double v1, v2;
    bool found1 = false;
    bool found2 = false;

    // RHS1 VALUE
    if (isdigit(RHS1[0]) || (RHS1.length() > 1 && RHS1[0] == '-' && isdigit(RHS1[1]))) // number literal (handles negative numbers)
        v1 = stod(RHS1);
    else
    {

        v1 = pManager->GetVariableValue(RHS1, found1);
        if (!found1)
        {
            pOut->PrintMessage("Runtime Error: variable '" + RHS1 + "' used before initialization.");
            return;
        }
    }

    // RHS2 VALUE
    if (isdigit(RHS2[0]) || (RHS2.length() > 1 && RHS2[0] == '-' && isdigit(RHS2[1]))) // number literal (handles negative numbers)
        v2 = stod(RHS2);
    else
    {

        v2 = pManager->GetVariableValue(RHS2, found2);
        if (!found2)
        {
            pOut->PrintMessage("Runtime Error: variable '" + RHS2 + "' used before initialization.");
            return;
        }
    }

    // APPLY OPERATION
    double result = 0;

    switch (Operator)
    {
    case '+': result = v1 + v2; break;
    case '-': result = v1 - v2; break;
    case '*': result = v1 * v2; break;

    case '/':
        if (v2 == 0)
        {
            pOut->PrintMessage("Runtime Error: Division by zero.");
            return;
        }
        result = v1 / v2;
        break;

    default:
        pOut->PrintMessage("Runtime Error: Unknown operator in assignment.");
        return;
    }

    // STORE INTO LHS VARIABLE
    pManager->SetVariableValue(LHS, result);
}
 

void OperationAssign::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();

    // LHS
    string newLHS;
    while (true)
    {
        pOut->PrintMessage("Enter LHS variable:");
        newLHS = pIn->GetString(pOut);
        if (!newLHS.empty()) break;
        pOut->PrintMessage("LHS cannot be empty!");
    }
    setLHS(newLHS);

    // RHS1
    string newRHS1;
    while (true)
    {
        pOut->PrintMessage("Enter first operand (RHS1):");
        newRHS1 = pIn->GetString(pOut);
        if (!newRHS1.empty()) break;
        pOut->PrintMessage("Operand cannot be empty!");
    }
    setRHS1(newRHS1);

    // Operator
    char op;
    while (true)
    {
        pOut->PrintMessage("Enter operator (+, -, *, /):");
        string opStr = pIn->GetString(pOut);
        if (!opStr.empty() && (opStr[0] == '+' || opStr[0] == '-' || opStr[0] == '*' || opStr[0] == '/'))
        {
            op = opStr[0];
            break;
        }
        pOut->PrintMessage("Invalid operator!");
    }
    setOPerator(op);

    // RHS2
    string newRHS2;
    while (true)
    {
        pOut->PrintMessage("Enter second operand (RHS2):");
        newRHS2 = pIn->GetString(pOut);
        if (!newRHS2.empty()) break;
        pOut->PrintMessage("Operand cannot be empty!");
    }
    setRHS2(newRHS2);

    UpdateStatementText();
    
}
 