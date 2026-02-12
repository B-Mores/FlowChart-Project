#include "ApplicationManager.h"
#include "GUI\Input.h"
#include "GUI\Output.h"
#include "Actions\AddValueAssign.h"
#include "Actions\AddVariableAssign.h"
#include "Actions\AddOperationAssign.h"
#include "Actions\AddRead.h"
#include "Actions\AddWrite.h"
#include "Actions\AddStart.h"
#include "Actions\AddEnd.h"
#include "Actions\AddDeclare.h"
#include "Actions\AddCondition.h"
#include"Actions\AddConnector.h"
#include"Actions\Select.h"
#include "Actions\Edit.h"
#include "Actions\Delete.h"
#include "Actions\Cut.h"
#include "Actions\Copy.h"
#include "Actions\Paste.h"
#include "Actions\Save.h"
#include "Actions\Load.h"
#include "Actions\Validate.h"
#include "Actions\Run.h"
#include "Actions\cppc.h"
ApplicationManager::ApplicationManager()
{
	//Create Input and output
	pOut = new Output;
	pIn = pOut->CreateInput();
	
	StatCount = 0;
	ConnCount = 0;
	pSelectedStat = NULL;	//no Statement is selected yet
	pSelectedConn=NULL;      //no connector is selected yet
	pClipboard = NULL;
	SimVarCount = 0; //Initialize the simulation variable counter
	//Create an array of Statement pointers and set them to NULL		
	for(int i=0; i<MaxCount; i++)
	{
		StatList[i] = NULL;	
		ConnList[i] = NULL;
	}

}


//==================================================================================//
//								Actions Related Functions							//
//==================================================================================//


ActionType ApplicationManager::GetUserAction() const
{
	//Ask the input to get the action from the user.
	return pIn->GetUserAction();		
}
////////////////////////////////////////////////////////////////////////////////////
//Creates an action and executes it
void ApplicationManager::ExecuteAction(ActionType ActType) 
{
	Action* pAct = NULL;
	
	//According to ActioType, create the corresponding action object
	switch (ActType)
	{
		case ADD_VALUE_ASSIGN:
			pAct = new AddValueAssign(this);
			break;
	
		case ADD_VAR_ASSIGN:
			pAct = new AddVariableAssign(this);
			break;


		case ADD_OPER_ASSIGN:
			pAct = new AddOperationAssign(this);
			break;

		case ADD_READ:
			pAct = new AddRead(this);
			break;

		case ADD_WRITE:
			pAct = new AddWrite(this);
			break;

		case ADD_START:
			pAct = new AddStart(this);
			break;

		case ADD_END:
			pAct = new AddEnd(this);
			break;

		case ADD_DECLARE_VARIABLE:
			pAct = new AddDeclare(this);
			break;

			
		case ADD_CONDITION:
			pAct = new AddCondition(this);
			break;

		case ADD_CONNECTOR:
			pAct = new AddConnector(this);
			break;

		case SELECT:
			pAct = new Select(this);
			break;

		case EDIT_STAT:
			pAct = new Edit(this);
			break;
			
		case DEL:
			pAct = new Delete(this);
			break;
		case COPY:
			pAct = new Copy(this);
			break;
		case CUT:
			pAct = new Cut(this);
			break;
		case PASTE:
			pAct = new Paste(this);
			break;
		case SAVE:
			pAct = new Save(this);
			break;
		case LOAD:
			pAct = new Load(this);
			break;

		case SWITCH_SIM_MODE:
			pOut->CreateSimulationToolBar(); 
			break;

		case VALIDIATE:
			pAct = new Validate(this);
			break;
	
		case RUN:
			pAct = new Run(this);
			break;

		case CPP:
			pAct = new cppc(this);
			break;
		case SWITCH_DSN_MODE:
			pOut->CreateDesignToolBar();
			break;

		case EXIT:
			///create Exit Action here
			
			break;
	
		case STATUS:
			return;
	}
	
	//Execute the created action
	if(pAct != NULL)
	{
		pAct->Execute();//Execute
		delete pAct;	//Action is not needed any more ==> delete it
	}
}



//==================================================================================//
//						Statements Management Functions								//
//==================================================================================//


//Add a statement to the list of statements
void ApplicationManager::AddStatement(Statement *pStat)
{
	if(StatCount < MaxCount)
		StatList[StatCount++] = pStat;
	
}

////////////////////////////////////////////////////////////////////////////////////
Statement *ApplicationManager::GetStatement(Point P) 
{
	for (int i = 0; i < StatCount; i++)
	{
		if (StatList[i]->IsPointInside(P) && StatList[i] != nullptr)
		{
			//Return a pointer to the statement.
			return StatList[i];
		}
	}
		// No statement found at point P.
	return NULL;
}
void ApplicationManager::AddConnector1(Connector* pConn)
{
	if (ConnCount < MaxCount)
		ConnList[ConnCount++] = pConn;

}
Connector* ApplicationManager::GetConnector(Point P) const
{	
	for (int i = 0;i < ConnCount; i++)
	{
		if (ConnList[i]->IsOnPath(P))
		{
			//  Return a pointer to the connector.
			return ConnList[i];
		}
	}
	// No connector found at point P.
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////////
//Returns the selected statement
Statement *ApplicationManager::GetSelectedStatement() const
{	return pSelectedStat;	}

////////////////////////////////////////////////////////////////////////////////////
//Set the statement selected by the user
void ApplicationManager::SetSelectedStatement(Statement *pStat)
{	pSelectedStat = pStat;	}

Connector* ApplicationManager::GetSelectedConnector() const  //Returns the selected connector
{ return pSelectedConn; }

void ApplicationManager::SetSelectedConnector(Connector* c) //Set the connector selected by the user
{ pSelectedConn = c; }
////////////////////////////////////////////////////////////////////////////////////
//Returns the Clipboard
Statement *ApplicationManager::GetClipboard() const
{	return pClipboard;	}

////////////////////////////////////////////////////////////////////////////////////
//Set the Clipboard
void ApplicationManager::SetClipboard(Statement *pStat)
{	pClipboard = pStat;	}


//==================================================================================//
//							Interface Management Functions							//
//==================================================================================//


//Draw all figures on the user interface
void ApplicationManager::UpdateInterface() const
{
	pOut->ClearDrawArea();

	//Draw all statements
	for(int i=0; i<StatCount; i++)
		StatList[i]->Draw(pOut);
	
	//Draw all connections
	for(int i=0; i<ConnCount; i++)
		ConnList[i]->Draw(pOut);

}
void ApplicationManager::RemoveStatement(Statement* pStat) //to delete statement
{
	for (int i = 0; i < StatCount; i++)
	{
		if (StatList[i] == pStat)
		{
			delete StatList[i];

			// shift the array after delete
			for (int j = i; j < StatCount - 1; j++)
				StatList[j] = StatList[j + 1];
			StatCount--;
			pSelectedStat = nullptr;
		}
	}
}
void ApplicationManager::RemoveConnector(Connector* pConn)  //to delete connector
{
	for (int i = 0; i < ConnCount; i++)
	{
		if (ConnList[i] == pConn)
		{
			delete ConnList[i];

			// Shift the array after deleting
			for (int j = i; j < ConnCount - 1; j++)
				ConnList[j] = ConnList[j + 1];

			ConnCount--;
			pSelectedConn = nullptr;
			break;
		}
	}
}
void ApplicationManager::DeleteConnectorsOfstat(Statement* pStat)   //to delete connectors connected to statement
{
	for (int i = 0; i < ConnCount; i++)
	{
		if (ConnList[i]->getSrcStat() == pStat || ConnList[i]->getDstStat() == pStat)
		{
			RemoveConnector(ConnList[i]);
			i--; //to stay at same index to check the new element that moved here
		}
	}

}

int ApplicationManager::GetStatCount()
{
	return StatCount;
}
int ApplicationManager::GetConnCount()
{
	return ConnCount;
}
Statement** ApplicationManager::GetStatementList()
{
	return StatList;

}
Connector** ApplicationManager::GetConnectorList()
{
	return ConnList;
}

void ApplicationManager::ClearFlowchart()
{
	// 1. Delete all Statement objects and clear the list
	for (int i = 0; i < StatCount; i++)
	{
		if (StatList[i] != NULL)
		{
			// Call the virtual destructor for the statement object
			delete StatList[i];
			StatList[i] = NULL;
		}
	}

	// 2. Reset the statement count
	StatCount = 0;

	// 3. Delete all Connector objects and clear the list
	for (int i = 0; i < ConnCount; i++)
	{
		if (ConnList[i] != NULL)
		{
			delete ConnList[i]; // Delete the connector object
			ConnList[i] = NULL;
		}
	}

	// 4. Reset the connector count
	ConnCount = 0;
}

// Function to search for a statement using its unique ID
Statement* ApplicationManager::GetStatementByID(int ID)
{
	// Get the list of statements and their count 
	Statement** pStatList = GetStatementList();
	int StatCount = GetStatCount();

	// Loop through all stored statements
	for (int i = 0; i < StatCount; i++)
	{
		// Check if the pointer is not NULL
		if (pStatList[i] != NULL)
		{
			// Compare the statement's ID with the required ID
			if (pStatList[i]->getID() == ID)
			{
				// Statement found, return its pointer
				return pStatList[i];
			}
		}
	}

	// No statement matching the ID was found
	return NULL;
}


void ApplicationManager::ResetAllStatementVisits()
{
	// Get the list of statements and their count
	Statement** pStatList = GetStatementList();
	int StatCount = GetStatCount();

	// Loop through all statements and set the 'visited' flag to false
	for (int i = 0; i < StatCount; i++)
	{
		if (pStatList[i] != NULL)
		{
			// Call the Statement's method to reset the flag
			pStatList[i]->SetVisited(false);
		}
	}
}

int ApplicationManager::CountOutgoingConnectors(const Statement* pSrcStat) const
{
		int count = 0;
		for (int i = 0; i < ConnCount; i++) 
		{
			// Check if the source statement of the connector matches the given statement
			if (ConnList[i]->getSrcStat() == pSrcStat)
			{
				count++;
			}
		}
		return count;
}
Statement* ApplicationManager::GetStartStatement()
{
	for (int i = 0; i < StatCount; i++)
	{
		if (dynamic_cast<Start*>(StatList[i]) != nullptr)
			return StatList[i];
	}
	return nullptr; // No start found
}

 
//Return a pointer to the input
Input *ApplicationManager::GetInput() const
{	return pIn; }
//Return a pointer to the output
Output *ApplicationManager::GetOutput() const
{	return pOut; }
 


bool ApplicationManager::ValidateFlowchart(string& ErrorMsg)
{
	Validate validator(this);
	return validator.TraverseAndValidate(ErrorMsg); // wrapper function that returns true/false
}
int ApplicationManager::SearchVariableIndex(const string& varName) const
{
	// Check SimVarCount
	for (int i = 0; i < SimVarCount; ++i)
	{
		if (SimVariableNames[i] == varName)
		{
			return i; // Found at index i
		}
	}
	return -1; // Not found
}


//  Gets the value of a variable using arrays
double ApplicationManager::GetVariableValue(const string& Name, bool& found) const
{
	int index = SearchVariableIndex(Name);

	if (index != -1)
	{
		found = true;
		// Access the value using the found index
		return SimVariableValues[index];
	}
	else
	{
		found = false;
		return 0.0;
	}
}

// Sets the value of an existing variable, or adds it if it doesn't exist.
void ApplicationManager::SetVariableValue(const string& Name, double Value)
{
	int index = SearchVariableIndex(Name);

	if (index != -1)
	{
		// Variable found: update its value
		SimVariableValues[index] = Value;
	}
	else
	{
		// Variable not found (Add it)
		if (SimVarCount < MaxVariables)
		{
			SimVariableNames[SimVarCount] = Name;
			SimVariableValues[SimVarCount] = Value;
			SimVarCount++;
		}
	}
}
 
//Clears all stored variables before a new run
void ApplicationManager::ClearVariables()
{
	//   reset the counter.
	SimVarCount = 0;
}
//Destructor
ApplicationManager::~ApplicationManager()
{
	for(int i=0; i<StatCount; i++)
		delete StatList[i];
	for(int i=0; i<StatCount; i++)
		delete ConnList[i];
	delete pIn;
	delete pOut;
	
}




 