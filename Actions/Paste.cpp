#include "Paste.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"
#include "..\Statements\Statement.h"

Paste::Paste(ApplicationManager* pApp) : Action(pApp) {}



void Paste::ReadActionParameters()
{

    pManager->GetOutput()->PrintMessage("Click where to paste the statement");
    pManager->GetInput()->GetPointClicked(P); 
}

void Paste::Execute()
{

    ReadActionParameters();

   //make sure there is somthing copied 
    Statement* c = pManager->GetClipboard();
    if (!c)
    {
        pManager->GetOutput()->PrintMessage("Clipboard is empty!");
        return;
    }

    // copy the state
    Statement* newStat = c->Clone();

  
    newStat->SetCenter(P);


    pManager->AddStatement(newStat);

 
    pManager->UpdateInterface();

    pManager->GetOutput()->PrintMessage("Statement pasted!");
}