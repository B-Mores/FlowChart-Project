#ifndef ADD_Write_H
#define ADD_Write_H

#include "Action.h"
#include "..\Statements\Write.h" 

class AddWrite:public Action
{
private:
	Point Position;  // Position where the user clicks to add the stat.
	string Var;

public:
	AddWrite(ApplicationManager* pAppManager);
	
	// Reads assignment statement parameters (Position, Var)
	virtual void ReadActionParameters();

	// Creates and adds the Write statement to the list of statements
	virtual void Execute();


};
#endif