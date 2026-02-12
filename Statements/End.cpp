#include "End.h"
#include "..\GUI\UI_Info.h"
#include"fstream"


End::End(Point C)
{
    Center = C;
    width = UI.ASSGN_WDTH;
    height = UI.ASSGN_HI;
    Text = "End";
    Selected = false;

    pOutConn = NULL;	//No connectors yet

    Inlet.x = Center.x;
    Inlet.y = Center.y - 22;

    Outlet.x = Center.x;
    Outlet.y = Center.y + 22;
}



void End::UpdateStatementText()
{
    // End text is fixed
    Text = "End";
}

void End::Draw(Output* pOut) const
{
    pOut->DrawEnd(Center, width, height, Text, Selected);
}
Point End::getInlet() const
{
    return Inlet;
}
Point End::getOutlet() const
{
    return Outlet;
}
void End::setOutConnector(Connector* pConn)
{
    pOutConn = pConn;
}

Connector* End::getOutConnector()
{
    return NULL;
}

bool End::IsPointInside(Point P) //  smallest rectangle contain the oval
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
Statement* End::Clone() const {
    End* s = new End(Point{ 0,0 });  // مؤقت
    s->Text = this->Text;
    s->Selected = false;
    s->pOutConn = nullptr;             // لا تنسخ المؤشرات
    return s;
}

void End::SetCenter(Point P)
{
    // 1. تحديث المركز الموروث (Center)
    Center = P;

    // 2. تحديث نقطة الوصل (Inlet) - النهاية لديها مدخل واحد فقط
    Inlet.x = Center.x;
    Inlet.y = Center.y - 22; // أو حسب ارتفاع البيان

    // 3. لا يوجد مخرج
}
void End::Save(ofstream& OutFile)
{
    OutFile << "END" << "\t"; 
    OutFile << ID << "\t";
    OutFile << Center.x << "\t" << Center.y << "\t";
    OutFile << " End \t";
    OutFile << "\n";
}

void End::Load(ifstream& In)
{
    int id, x, y;
    string end_keyword; // end_keyword will hold "End"

    //  Read 4 tokens exactly as saved: ID, X, Y, "End"
    // The statement type ("END") is read outside this function.
    In >> id >> x >> y >> end_keyword;

    // 1. Assign ID and Position
    ID = id;
    Center.x = x;
    Center.y = y;

    // 2. Set fixed properties
    width = UI.ASSGN_WDTH;
    height = UI.ASSGN_HI;
    Text = "End";

    // 3. Update Inlet point based on the new Center
    Inlet.x = Center.x;
    Inlet.y = Center.y - 22;
}

bool End::Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    // The End statement mustnot have any output connector.
    if (getOutConnector() != NULL)
    {
        ErrorMsg = "Error: END Statement (ID: " + to_string(ID) + ") must not have any output connectors.";
        return false;
    }
    return true; // Validation passed for this statement
}
 
void End::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
    // End statement does not perform any variable modification 
}
void End::Edit(ApplicationManager* pManager)
{
    Output* pOut = pManager->GetOutput();
    pOut->PrintMessage("End statement cannot be edited.");
}



  
    



