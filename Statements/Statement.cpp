#include "Statement.h"

int Statement::NextID = 0;
Statement::Statement()	
{ 
	ID = NextID++;
	Text = "";
	Selected = false;		
}

void Statement::SetSelected(bool s)
{	Selected = s; }

bool Statement::IsSelected() const
{	return Selected; }

int Statement::getID()
{
	return ID;
}

void Statement::SetText(string& newText)
{
	Text = newText;
}

bool Statement::SearchDeclared(const string& VarName, const string DeclaredVars[], int VarCount) 
{
	for (int i = 0; i < VarCount; i++)
	{
		if (DeclaredVars[i] == VarName)
			return true;
	}
	return false;
}
void Statement::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
{
}

 