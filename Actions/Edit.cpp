#include "Edit.h"
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"


using namespace std;

Edit::Edit(ApplicationManager* pAppManager): Action(pAppManager)
{
    pStat = nullptr;
}

void Edit::ReadActionParameters()
{
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();
}

void Edit::Execute() {
    ReadActionParameters();

    Statement* pStat = pManager->GetSelectedStatement();

    if (!pStat) {
        pManager->GetOutput()->PrintMessage("No statement selected to edit!");
        return;  
    }

    // Call the statement's Edit method
    pStat->Edit(pManager);

    // After editing, deselect the statement
    pStat->SetSelected(false);
    pManager->SetSelectedStatement(nullptr);
}