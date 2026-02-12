#ifndef OPERATION_ASSIGN_H
#define OPERATION_ASSIGN_H

#include "Statement.h"


class OperationAssign : public Statement
{
private:
    string LHS; //Left Handside of the assignment (name of a variable)
    string RHS1; //Right Handside (Variable or value)
    string RHS2; //Right Handside (Variable or value)
    char Operator;  // Arthmatic operator(+,-,*,/)

    Connector* pOutConn; //Operation Assignment Stat. has one Connector to next statement

    Point Inlet; //A point where connections enters this statement 
    //It's used as the (End) point of the (Input) connectors
    Point Outlet; //A point a connection leaves this statement
    //It's used as the (Start) point of the (Output) connector

    Point LeftCorner; //left corenr of the statement block.

    virtual void UpdateStatementText();

public:
   
    OperationAssign(Point Lcorner={0,0}, string LeftHS = "", string RightHS1 = "", string RightHS2 = "", char op = '+');



    void setLHS(const string& L);
    void setRHS1(const string& R1);
    void setRHS2(const string& R2);
    void setOPerator(char op);
    string getLHS();
    string getOp1();
 
    char  getOperator();
 
   string  getOp2();
    virtual void Draw(Output* pOut) const ;

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

    virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)override;
    virtual void Edit(ApplicationManager* pManager)override;
};

#endif