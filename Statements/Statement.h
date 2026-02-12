#ifndef STATEMENT_H
#define STATEMENT_H

#include "..\defs.h"
#include "..\Connector.h"
//class Output;
#include "..\GUI\Output.h"
#include <vector>
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
 
class ApplicationManager; // Forward declaration
//Base class for all Statements
class Statement
{
protected:
	static int NextID;			
	int ID;         //Each Statement has an ID --> must be unique
	string Text;	//Statement text (e.g.  "X = 5" OR "salary > 3000" and so on)
	bool Selected;	//true if the statement is selected on the folwchart
	Point Center;
	

	bool visited;


	virtual void UpdateStatementText() = 0;	//is called when any part of the stat. is edited	

	/// Add more parameters if needed.

public:
	Statement();
	void SetSelected(bool s);
	bool IsSelected() const;



	int getID(); //return the number of statement
	void SetText(string& newText); //used in edit to change Text

	virtual void Draw(Output* pOut) const  = 0 ;	//Draw the statement
	
	

	///TODO:The following functions should be supported by the Statement class
	///		It should then be overridden by each derived Statement
	///		Decide the parameters that you should pass to each function and its return type

	virtual void Save(ofstream &OutFile) = 0;	//Save the Statement parameters to a file
	virtual void Load(ifstream &Infile) = 0;	//Load the Statement parameters from a file
	 
	virtual void Edit(ApplicationManager* pManager)=0 ;		//Edit the Statement parameter

	virtual void Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn);	//Execute the statement in the simulation mode

 


	///DONE: Add more functions if needed
	virtual Point getInlet()const = 0;
	virtual Point getOutlet()const = 0;

	virtual void setOutConnector(Connector* pConn) = 0;
	virtual Connector* getOutConnector() = 0;

	virtual bool IsPointInside(Point P) = 0;
	virtual Statement* Clone() const = 0;

	virtual void SetCenter(Point P) = 0;



	void SetVisited(bool v) { visited = v; }  //for validate
	bool IsVisited() const { return visited; }  //for validate
	bool SearchDeclared(const string& VarName, const string DeclaredVars[], int VarCount) ; //for validate
	virtual bool Validate(string& ErrorMsg,string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,string InitializedVars[], int* InitializedCountPtr, int InitializedMax) = 0;
	

	virtual ~Statement() {}

};

#endif