#include "Start.h"
#include "fstream"

Start::Start(Point C)
{
    Center = C;
    width = UI.ASSGN_WDTH; // reuse default sizes
    height = UI.ASSGN_HI;
    Text = "Start";
    Selected = false;

    pOutConn = NULL;	//No connectors yet

    Outlet.x = Center.x;
    Outlet.y = Center.y + 22;

    Inlet.x = Center.x;
    Inlet.y = Center.y - 22;

}



void Start::UpdateStatementText()
{
    Text = "Start";
}

void Start::Draw(Output* pOut) const
{
    // Output::DrawStart expects center, width, height, text, selected
    pOut->DrawStart(Center, width, height, Text, Selected);
}

Point Start::getOutlet() const
{
    return Outlet;
}
Point Start::getInlet() const
{
    return  Inlet;
}


void Start::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* Start::getOutConnector()
{
    return pOutConn;
}


bool Start::IsPointInside(Point P) //  smallest rectangle contain the oval
{
    // X-coordinates
    const int x1 = Center.x - width / 2;   // most left
    const int x2 = Center.x + width / 2;   // most right 

    // Y-coordinates
    const int y1 = Center.y - height / 2;  // most top
    const int y2 = Center.y + height / 2;  // most bottom

    if (P.x >= x1 && P.x <= x2 && P.y >= y1 && P.y <= y2)
    {
        return true;
    }

    return false;

}

Statement* Start::Clone() const {
    Start* s = new Start(Point{ 0,0 });  // مؤقت
    s->Text = this->Text;
    s->Selected = false;
    s->pOutConn = nullptr;             // لا تنسخ المؤشرات
    return s;
}


void Start::SetCenter(Point P)
{
    // الآن هذا سيحدث المتغير الموروث من Statement
    Center = P;

    // ...
    Outlet.x = Center.x;
    Outlet.y = Center.y + 22;
}

void Start::Save(ofstream& OutFile)
{
    OutFile << "STRT" << "\t"; 
    OutFile << ID << "\t";
    OutFile << Center.x << "\t" << Center.y << "\t";
    OutFile << " Start \t";
    OutFile << "\n";
}




void Start::Load(ifstream& InFile)
{
    int id, x, y;
    string start_keyword; // start_keyword will hold "Start"

    //  Read 4 tokens exactly as saved: ID, X, Y, "Start"
    // The statement type ("STRT") is read outside this function.
    InFile >> id >> x >> y >> start_keyword;

    // 1. Assign ID and Position
    ID = id;
    Center.x = x;
    Center.y = y;

    // 2. Set fixed properties
    width = UI.ASSGN_WDTH;
    height = UI.ASSGN_HI;
    Text = "Start";

    // 3. Update Outlet point based on the new Center
    Outlet.x = Center.x;
    Outlet.y = Center.y + 22;
}

bool Start::Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    // 1. Check if the statement has exactly one output connector 
    if (pOutConn == NULL)
    {
        ErrorMsg = "Error: START Statement (ID: " + to_string(ID) + ") must have exactly one output connector.";
        return false;
    }
    return true; // Validation passed for this statement
}

void Start::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    // Start statement performs no variable operations 
}
void Start::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    pOut->PrintMessage("Start statement cannot be edited.");
}


