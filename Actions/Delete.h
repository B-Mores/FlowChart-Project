#pragma once
#ifndef DEL_H
#define DEL_H


#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\GUI\UI_Info.h"


class Delete :public Action
{
	Statement* pStat; // statement to delete
	Connector* pCon; // connector to delete
public:
	Delete(ApplicationManager* pAppManager); //constructor
	virtual void ReadActionParameters();
	virtual void Execute();

};

#endif