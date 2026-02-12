#include "Select.h"
#include "..\ApplicationManager.h"
#include "..\GUI\input.h"
#include "..\GUI\Output.h"
 

using namespace std;

Select::Select(ApplicationManager* pAppManager) :Action(pAppManager) //constructor
{
}
void Select::ReadActionParameters() {
    Input* pIn = pManager->GetInput();
    Output* pOut = pManager->GetOutput();

    //Read the (Position) parameter
    pOut->PrintMessage("Selector : Click on a statement or connector to select it");

    pIn->GetPointClicked(Position);
    pOut->ClearStatusBar();

}
 
  void Select::Execute() {

      ReadActionParameters();

      //  Check for statement first
      Statement* clickedStat = pManager->GetStatement(Position);

      //  If no statement check for connector
      Connector* clickedConn = nullptr;
      if (!clickedStat)
          clickedConn = pManager->GetConnector(Position);

      //Get currently selected statement (if any)
      Statement* selectedStat = pManager->GetSelectedStatement();
      Connector* selectedConn = pManager->GetSelectedConnector(); 

      //Handle statement click
      if (clickedStat)
      {
          // If clicked statement is already selected unselect it
          if (clickedStat == selectedStat)
          {
              clickedStat->SetSelected(false);
              pManager->SetSelectedStatement(nullptr);
          }
          else
          {
              // Unselect previous selection (statement or connector)
              if (selectedStat) selectedStat->SetSelected(false);
              if (selectedConn) selectedConn->SetSelected(false);

              clickedStat->SetSelected(true);
              pManager->SetSelectedStatement(clickedStat);
          }
      }
      //Handle connector click
      else if (clickedConn)
      {
          if (clickedConn == selectedConn)
          {
              clickedConn->SetSelected(false);
              pManager->SetSelectedConnector(nullptr);
          }
          else
          {
              if (selectedStat) selectedStat->SetSelected(false);
              if (selectedConn) selectedConn->SetSelected(false);

              clickedConn->SetSelected(true);
              pManager->SetSelectedConnector(clickedConn);
          }
      }
      //Click in empty area
      else
      {
          if (selectedStat) selectedStat->SetSelected(false);
          if (selectedConn) selectedConn->SetSelected(false);

          pManager->SetSelectedStatement(nullptr);
          pManager->SetSelectedConnector(nullptr);
      }
  
}