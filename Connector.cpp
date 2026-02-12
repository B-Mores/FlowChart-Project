#include "Connector.h"
#include "Statements\Statement.h"
#include "GUI\Output.h" 
#include<fstream>
#include "Statements\Condition.h"



Connector::Connector(Statement* Src, Statement* Dst)
{
    //When a connector is created, it must have a source statement and a destination statement
    //There are NO FREE connectors in the flowchart
    SrcStat = Src;
    DstStat = Dst;
    CondPath = false;
    Selected = false;

}

void Connector::setSrcStat(Statement* Src)
{
    SrcStat = Src;
}

Statement* Connector::getSrcStat()
{
    return SrcStat;
}

void Connector::setDstStat(Statement* Dst)
{
    DstStat = Dst;
}

Statement* Connector::getDstStat()
{
    return DstStat;
}


void Connector::setStartPoint(Point P)
{
    Start = P;
}

Point Connector::getStartPoint()
{
    return Start;
}

void Connector::setEndPoint(Point P)
{
    End = P;
}

Point Connector::getEndPoint()
{
    return End;
}

void Connector::setCondPath(bool IsTrue)
{
    CondPath = IsTrue;
}

bool Connector::getCondPath()
{
    return CondPath;
}


void Connector::Draw(Output* pOut) 
{
    //  Calculate the Start (Outlet) and End (Inlet) points from the statements
    Start = SrcStat->getOutlet();
    End = DstStat->getInlet();

    pOut->DrawStraightConnector(Start, End,Selected);
}

bool Connector::IsOnPath(Point P)
{

    // Checks if a given point P is near the connector's path (for user click detection).
    // This function calculates the Bounding Box around the line segment and checks if P is inside.
    const int Tolerance = 8;
    Point StartPoint = SrcStat->getOutlet();
    Point EndPoint = DstStat->getInlet();

    //  Define the Bounding Box around the line segment 
    
    // X-bounds
    int MinX = min(StartPoint.x, EndPoint.x) - Tolerance;
    int MaxX = max(StartPoint.x, EndPoint.x) + Tolerance;

    // Y-bounds
    int MinY = min(StartPoint.y, EndPoint.y) - Tolerance;
    int MaxY = max(StartPoint.y, EndPoint.y) + Tolerance;

     //  Check if the point P is inside the Bounding Box 

    if (P.x >= MinX && P.x <= MaxX && P.y >= MinY && P.y <= MaxY)
    {
        // The point P is within the rectangular area surrounding the line.
        return true;
    }

    // Point P isnot on the connector path.
    return false;
}
 
void Connector::SetSelected(bool s) 
{
    Selected = s; 
}
bool Connector::IsSelected() const
{
    return Selected;
}


void Connector::Save(ofstream& OutFile)
{
    if (dynamic_cast<Condition*>(getSrcStat()))
    {
        OutFile << SrcStat->getID() << "\t";
        OutFile << DstStat->getID() << "\t";
     
        if (CondPath)
            OutFile << "1 \n";
        else
            OutFile << "2 \n";
    }
    else
    {
        OutFile << SrcStat->getID() << "\t";
        OutFile << DstStat->getID() << "\t";
        OutFile << "0";
        OutFile << endl;
    }
}

void Connector::Load(ifstream& InFile)
{


    // 1. Read the three tokens and store them in the  member variables(temporarily)
    InFile >> SrcID >> DstID >> PathType;

    // 2. Set the CondPath flag based on PathType:
    if (PathType == 1)
    {
        CondPath = true; // True path
    }
    else if (PathType == 2)
    {
        CondPath = false; // False path
    }
    else
    {
        CondPath = false; // Normal path (0)
    }

    // Note: The actual pointers (SrcStat, DstStat) will be set later 
    // in ApplicationManager::Load() after all statements are loaded.
}