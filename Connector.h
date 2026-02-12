#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "GUI\UI_Info.h"
#include "GUI\output.h"
#include"fstream"

class Statement;

class Connector	//a connector that connects two statements (Source & Destination)
{
private:
	Statement *SrcStat;	//The source statement of the connector
	Statement *DstStat;	//The destination statement of the connector
	Point Start;	//Start point of the connector
	Point End;		//End point of the connector
	bool CondPath; // to know the correct direct for condition statement

protected:
	bool Selected; //used to check selection
public:

	// temporarily Members to hold IDs during Load/Save process
	int SrcID;
	int DstID;
	int PathType;   // Holds 0 (Normal), 1 (True), or 2 (False)
	int getSrcID() const { return SrcID; }
	int getDstID() const { return DstID; }
	int getPathType() const { return PathType; }


	Connector(Statement* Src, Statement* Dst);

	void setSrcStat(Statement *Src);
	Statement*	getSrcStat();	
	void setDstStat(Statement *Dst);
	Statement*	getDstStat();

	void setStartPoint(Point P);
	Point getStartPoint();

	void setEndPoint(Point P);
	Point getEndPoint();

	void setCondPath(bool IsTrue);
	bool getCondPath();

	void Draw(Output* pOut) ;
	
	bool IsOnPath(Point P);

	void SetSelected(bool s);
	bool IsSelected() const;


	void Save(ofstream& OutFile);
	void Load(ifstream& InFile);

};

#endif


