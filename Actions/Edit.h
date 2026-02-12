#pragma once
#ifndef EDIT_STAT_H
#define EDIT_STAT_H

#include "Action.h"
#include "..\Statements\Statement.h"
#include "..\GUI\UI_Info.h"


class Edit :public Action
{
	Statement* pStat; // statement to edit

public:
	Edit(ApplicationManager* pAppManager); //constructor
	virtual void ReadActionParameters();
	virtual void Execute();

};

#endif