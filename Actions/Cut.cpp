#include "Cut.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

Cut::Cut(ApplicationManager* pApp) : Action(pApp) 
{
    pStat = NULL;
}

void Cut::ReadActionParameters()
{
    Point click;
    pManager->GetInput()->GetPointClicked(click);
    pStat = pManager->GetStatement(click);

    if (pStat)
        pManager->SetSelectedStatement(pStat);
    else
        pManager->SetSelectedStatement(nullptr);
}

void Cut::Execute()
{
    pStat = pManager->GetSelectedStatement();
    if (pStat)
    {
        if (pManager->GetClipboard())
            delete pManager->GetClipboard();

        pManager->SetClipboard(pStat->Clone());
        pManager->GetOutput()->PrintMessage("Cut Done");

        // Delete ALL connectors linked to it
        pManager->DeleteConnectorsOfstat(pStat);

        // Delete the statement itself
        pManager->RemoveStatement(pStat);
    }
    else
        pManager->GetOutput()->PrintMessage("No statement selected!");
}
