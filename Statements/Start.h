#ifndef START_H
#define START_H

#include "Statement.h"

class Start : public Statement
{
private:
    
    int width, height;

    virtual void UpdateStatementText();

    Connector* pOutConn;
    Point Outlet;
    Point Inlet;
   

public:
    Start(Point C = { 0,0 });
   

    // Draw the start statement (ellipse with "Start")
    virtual void Draw(Output* pOut) const;

    virtual Point getOutlet()const override;
    virtual Point getInlet()const override;

    virtual void setOutConnector(Connector* pConn) override;
    virtual Connector* getOutConnector() override;


    virtual bool IsPointInside(Point P) override;
    virtual Statement* Clone() const override;
    virtual void SetCenter(Point P) override;
    virtual void Save(ofstream& OutFile)override;
    virtual void Load(ifstream& In) override;

    virtual bool Validate(string& ErrorMsg, string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax, string InitializedVars[], int* InitializedCountPtr
        , int InitializedMax)  override;

    virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
    virtual void Edit(ApplicationManager* pManager)override;

};

#endif