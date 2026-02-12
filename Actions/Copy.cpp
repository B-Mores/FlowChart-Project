#include "Copy.h"
#include "..\ApplicationManager.h"
#include "..\GUI\Input.h"
#include "..\GUI\Output.h"

Copy::Copy(ApplicationManager* pApp) : Action(pApp) {}

void Copy::ReadActionParameters()
{
   //click point to paste on it
    Point click;
    pManager->GetInput()->GetPointClicked(click);
    Statement* s = pManager->GetStatement(click);

    if (s)
        pManager->SetSelectedStatement(s);
    else
        pManager->SetSelectedStatement(nullptr);
}

void Copy::Execute()
{
    Statement* s = pManager->GetSelectedStatement();
    if (s)
    {
      //avoid memory leaking 
        if (pManager->GetClipboard())
            delete pManager->GetClipboard();

        pManager->SetClipboard(s->Clone());
        pManager->GetOutput()->PrintMessage("Copied!");
    }
    else
        pManager->GetOutput()->PrintMessage("No statement selected!");
}
