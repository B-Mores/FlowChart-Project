#ifndef DECLARE_H
#define DECLARE_H

#include "Declare.h"
#include "Statement.h"


class Declare : public Statement
{
private:
   string var; 
    

    Connector* pOutConn;

    Point Inlet;
    Point Outlet;
    Point LeftCorner;

    virtual void UpdateStatementText();

public:
   
    Declare(Point Lcorner = { 0,0 }, string Variable = "");
    
    void setVariableName(const std::string& Variable);

    virtual void Draw(Output* pOut) const;

    virtual Point getInlet()const override;
    virtual Point getOutlet()const override;

    virtual void setOutConnector(Connector* pConn) override;
    virtual Connector* getOutConnector() override;

    virtual bool IsPointInside(Point P) override;
    virtual void SetCenter(Point P) override;
    virtual Statement* Clone() const override;


    virtual void Save(ofstream& OutFile)override;
    virtual void Load(ifstream& In)override;
    

   virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
       , int InitializedMax)  override;
    
   ;
   //  function to retrieve the variable name being declared.
   string GetVarName() const;
   virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
   virtual void Edit(ApplicationManager* pManager)override;
};

#endif