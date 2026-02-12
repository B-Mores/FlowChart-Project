
#ifndef END_H
#define END_H

#include "Statement.h"

class End : public Statement
{
private:
    Point Center;
    int width, height;

    Connector* pOutConn;
    Point Inlet;
    Point Outlet;

    virtual void UpdateStatementText();

public:
    End(Point C = { 0,0 });
 

    virtual void Draw(Output* pOut) const;

    virtual Point getInlet()const override;
    virtual Point getOutlet()const override; 
    virtual void setOutConnector(Connector* pConn)override;
    virtual Connector* getOutConnector() override;


    virtual bool IsPointInside(Point P)override;
    virtual void SetCenter(Point P) override;
    virtual Statement* Clone() const override;

    virtual void Save(ofstream& OutFile)override;
    virtual void Load(ifstream& In) override;

    virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
        , int InitializedMax)  override;
    virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
    virtual void Edit(ApplicationManager* pManager)override;
};


#endif
