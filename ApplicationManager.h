#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include "DEFS.h"
#include <vector>
#include "Statements\Statement.h"
 
class Input;
class Output;

 
//Main class that manages everything in the application.
class ApplicationManager
{
	enum { MaxCount = 200 };	//Max no of statements/connectors in a single flowchart

private:
	int StatCount;		//Actual number of statements
	int ConnCount;		//Actual number of connectors
	Statement* StatList[MaxCount];	//List of all statements (Array of pointers)
	Connector* ConnList[MaxCount];	//List of all connectors (Array of pointers)
	 
	Statement *pSelectedStat; //a pointer to the last selected statement
	                          //you can set and get this pointer

	Connector* pSelectedConn; //a pointer to the last selected connector
	                          //you can set and get this pointer

	Statement *pClipboard;    //a pointer to the last copied/cut statement
	                          //you can set and get this pointer

	static const int MaxVariables = 100; // Define max capacity (must match validation)
	string SimVariableNames[MaxVariables]; // Array to store variable names
	double SimVariableValues[MaxVariables];     // Array to store variable values (parallel array)
	int SimVarCount;                            // Actual count of variables in use

 
	//Pointers to Input and Output classes
	Input *pIn;
	Output *pOut;

public:	
	ApplicationManager(); 
	~ApplicationManager();
	
	// == Actions Related Functions ==
	ActionType GetUserAction() const; //Reads the input command from the user 
	                                  //and returns the corresponding action type
	void ExecuteAction(ActionType) ;  //Creates an action and executes it
	
	// == Statements/Connector Management Functions ==
	void AddStatement(Statement* pStat);    //Adds a new Statement to the Flowchart
	Statement *GetStatement(Point P) ;	//Searches for a statement where point P belongs
	                                        //DONE: Complete its implementation 
	                                        //      WITHOUT breaking class responsibilities

	void AddConnector1(Connector* pConn);    //Adds a new Connector to the Flowchart
	Connector *GetConnector(Point P) const;	//search for a Connector where point P belongs

	// Note: you should use the following 4 functions 
	//       in order not to break class responsibilities (especially in copy, cut and paste)
	Statement *GetSelectedStatement() const;	 //Returns the selected Statement
	void SetSelectedStatement(Statement *pStat); //Set the Statement selected by the user
	Connector* GetSelectedConnector() const;  //Returns the selected connector
	void SetSelectedConnector(Connector* c); //Set the connector selected by the user
	
	Statement *GetClipboard() const;	         //Returns the Clipboard
	void SetClipboard(Statement *pStat);         //Set the Clipboard

	// == Interface Management Functions ==
	Input *GetInput() const;        //Return pointer to the input
	Output *GetOutput() const;      //Return pointer to the output
	void UpdateInterface() const;	//Redraws all the drawing window
	
	void RemoveStatement(Statement* pStat); //to delete statement
	void RemoveConnector(Connector* pConn); //to delete connector
	void DeleteConnectorsOfstat(Statement* pStat);  //to delete connectors connected to statement

	int GetStatCount(); // used to know number of statement used in save::Execute
	int GetConnCount();  // used to know number of Connectors used in save::Execute
	Statement** GetStatementList(); // return a array of Statement*
	Connector** GetConnectorList(); // return a array of Connector*
 
	void ClearFlowchart();    //for load
	Statement* GetStatementByID(int ID);// for load
	

	void ResetAllStatementVisits(); //for validate
	int CountOutgoingConnectors(const Statement* pSrcStat) const; //for validate

	Statement* ApplicationManager::GetStartStatement();
	bool ValidateFlowchart(string& ErrorMsg);
	int SearchVariableIndex(const string& varName) const;
	double GetVariableValue(const string& Name, bool& found) const;
	void SetVariableValue(const string& Name, double Value);
	void ClearVariables();
 
};

#endif