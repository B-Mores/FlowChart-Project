#ifndef CONDITION_H
#define CONDITION_H


#include "Statement.h"
#include "..\Connector.h" 

class Condition : public Statement
{
private:
	Point Top; // top vertex of diamond
	int width;
	string LHS, Comp, RHS;

	
	Connector* pOutConnTrue;
	Connector* pOutConnFalse;

	Point Inlet;
	Point Outlet;

	virtual void UpdateStatementText();

public:
	
	Condition(Point = { 0,0 }, const string& lhs = "", const string& comp = "", const string& rhs = "");

	virtual void Draw(Output* pOut) const;

	virtual Point getInlet()const override;
	virtual Point getOutlet()const override;

	// (Pure virtual overrides - Mapped to TRUE path for simplicity)
	virtual void setOutConnector(Connector* pConn) override;
	virtual Connector* getOutConnector()override ;



	//  for the TRUE path
	void setOutConnectorTrue(Connector* pConn) ;
	Connector* getOutConnectorTrue() const;

	//   for the FALSE path
	void setOutConnectorFalse(Connector* pConn);
	Connector* getOutConnectorFalse() const;


	virtual bool IsPointInside(Point P)override;
	virtual void SetCenter(Point P) override;
	virtual Statement* Clone() const override;

	virtual void Save(ofstream& OutFile)override;
	virtual void Load(ifstream& In) override;

	virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
		, int InitializedMax)  override;

	virtual void Simulate (ApplicationManager* pManager, Output* pOut, Input* pIn)override;
	virtual void Edit(ApplicationManager* pManager)override;
	string getLHS() const { return LHS; }
	string getRHS() const { return RHS; }
	string getComp() const { return Comp; }
};

#endif