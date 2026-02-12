#ifndef WRITE_H
#define WRITE_H

#include "Statement.h"


class Write : public Statement
{
private:
	string var;

	Connector* pOutConn;	//write Stat. has one Connector to next statement

	Point Inlet;	//A point where connections enters this statement 
	//It's used as the (End) point of the (Input) connectors
	Point Outlet;	//A point a connection leaves this statement
	//It's used as the (Start) point of the (Output) connector

	Point LeftCorner;	//left corenr of the statement block.

	virtual void UpdateStatementText();

public:
	
	Write(Point Lcorner = { 0,0 }, string variable = "");

	void setVar(const string& Variable);

	virtual void Draw(Output* pOut) const;

	virtual Point getInlet()const override;
	virtual Point getOutlet()const override;

	virtual void setOutConnector(Connector* pConn)override;
	virtual Connector* getOutConnector() override;

	virtual bool IsPointInside(Point P)override;
	virtual void SetCenter(Point P) override;
	virtual Statement* Clone() const override;

	virtual void Save(ofstream& OutFile)override;
	virtual void Load(ifstream& In)override ;

	virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
		, int InitializedMax)  override;
	virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
	virtual void Edit(ApplicationManager* pManager)override;
	string getVar() const;
};

#endif