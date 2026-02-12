#include "Run.h"
#include "..\Statements\Statement.h"
#include "..\Statements\Condition.h"
#include "..\Statements\Write.h"
#include "..\Statements\Read.h"
#include "..\Statements\ValueAssign.h"
#include "..\Statements\VariableAssign.h"
#include "..\GUI\Output.h"
#include "..\GUI\Input.h"
 
#include <string>

using namespace std;

Run::Run(ApplicationManager* pApp) : Action(pApp) {}

void Run::ReadActionParameters() {}

void Run::Execute()
{
    Output* pOut = pManager->GetOutput();
    Input* pIn = pManager->GetInput();
  string validationError;

    // Clear Output Window First  
    pOut->ClearOutputBar();

    // Validation message
    if (!pManager->ValidateFlowchart(validationError))
    {
        pOut->PrintMessage("Validation Failed: " + validationError);
        return;
    }

    Statement* pCurrent = pManager->GetStartStatement();
    if (!pCurrent)
    {
        pOut->PrintMessage("Error: No Start statement found!"); //another check 
        return;
    }

    pManager->ClearVariables();

    pManager->ResetAllStatementVisits();
  
    while (pCurrent)
    {
        
        //Condition is handled here
        if (Condition* cond = dynamic_cast<Condition*>(pCurrent))
        {
            double leftVal = 0, rightVal = 0;
             string lhsVar = cond->getLHS();
              string rhsVarOrVal = cond->getRHS();
            bool foundLHS = false;
            bool foundRHS = false;

          
            leftVal = pManager->GetVariableValue(lhsVar, foundLHS);
            if (!foundLHS)
            {
                pOut->PrintMessage("Runtime Error: Variable '" + lhsVar + "' not initialized.");
                return;
            }

            rightVal = pManager->GetVariableValue(rhsVarOrVal, foundRHS);
            if (!foundRHS)
            {
            
                try
                {
                    rightVal = stod(rhsVarOrVal);
                }
                catch (...)
                {
                    pOut->PrintMessage("Runtime Error: RHS '" + rhsVarOrVal + "' is invalid.");
                    return;
                }
            }

            bool condResult = false;
            string comp = cond->getComp();

            if (comp == "==") condResult = (leftVal == rightVal);
            else if (comp == "!=") condResult = (leftVal != rightVal);
            else if (comp == "<") condResult = (leftVal < rightVal);
            else if (comp == "<=") condResult = (leftVal <= rightVal);
            else if (comp == ">") condResult = (leftVal > rightVal);
            else if (comp == ">=") condResult = (leftVal >= rightVal);

            // Follow correct branch
            pCurrent = condResult
            ? (cond->getOutConnectorTrue() ? cond->getOutConnectorTrue()->getDstStat() : nullptr)
                : (cond->getOutConnectorFalse() ? cond->getOutConnectorFalse()->getDstStat() : nullptr);

            continue;
        }

      
        pCurrent->Simulate(pManager, pOut, pIn);


        Connector* nextConn = pCurrent->getOutConnector();
        pCurrent = nextConn ? nextConn->getDstStat() : nullptr;
    }

    pOut->PrintMessage("Run Completed Successfuly");
}
