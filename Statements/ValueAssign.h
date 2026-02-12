#ifndef VALUE_ASSIGN_H
#define VALUE_ASSIGN_H

#include "Statement.h"

//Value Assignment statement class
//The value assignment statement assigns a value to a variable
class ValueAssign : public Statement
{
private:
	string LHS;	//Left Handside of the assignment (name of a variable)
	double RHS;	//Right Handside (Value)
	
	Connector *pOutConn;	//Value Assignment Stat. has one Connector to next statement
	                        //Each statement type in flowchart has a predefined number of (output) connectors
	                        //For example, conditional statement always has 2 output connectors

	                        //Note: We don't need to keep track with input connectors
	                        //      Whenever we want to iterate on all statements of the flowchart
	                        //      we will begin with start statement then its output connector
	                        //      then the connector's destination statement and so on (follow the connectors)

	Point Inlet;	//A point where connections enters this statement 
	                //It's used as the (End) point of the (Input) connectors
	Point Outlet;	//A point a connection leaves this statement
	                //It's used as the (Start) point of the (Output) connector

	Point LeftCorner;	//left corenr of the statement block.

	virtual void UpdateStatementText();
	
public:
	ValueAssign(Point Lcorner = { 0,0 }, string LeftHS = "", double RightHS = 0);
	
	void setLHS(const string &L);
	void setRHS(double R);

	virtual void Draw(Output* pOut) const ;

	virtual Point getInlet()const override;
	virtual Point getOutlet()const override;

	virtual void setOutConnector(Connector* pConn) override;
	virtual Connector* getOutConnector() override;
	
	virtual bool IsPointInside(Point P) override;
	virtual void SetCenter(Point P) override;
	virtual Statement* Clone() const override;

	virtual void Save(ofstream& OutFile)override;
	virtual void Load(ifstream& In) override;

	virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
		, int InitializedMax)  override;
	virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
	virtual void Edit(ApplicationManager* pManager)override;
	string getLHS() const;
	double getRHS() const;
};

#endif