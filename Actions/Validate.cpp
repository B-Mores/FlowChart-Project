#include "Validate.h"
#include "..\ApplicationManager.h"
#include "..\Statements\Statement.h"
#include "..\Statements\Start.h"
#include "..\Statements\End.h"
#include "..\Statements\Condition.h"
#include "..\Statements\Declare.h"
#include "..\GUI\Output.h"
#include <string>

using namespace std;

Validate::Validate(ApplicationManager* pAppManager) : Action(pAppManager) {}
// just to implement pure vurtual fn in base class(action)
void Validate::ReadActionParameters() {}

// helper fn to check on single statement
bool Validate::TraverseAndValidateSequential(Statement* CurrentStat, string& ErrorMsg,
    string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
    string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
{
    //  Reached NULL (End of a path)
    if (!CurrentStat)
    {
        return true;
    }
    if (CurrentStat->IsVisited()) return true; CurrentStat->SetVisited(true);
    // The Statement will perform local validation and update the appropriate lists.
    if (!CurrentStat->Validate(ErrorMsg,DeclaredVars, DeclaredCountPtr, DeclaredMax,
        InitializedVars, InitializedCountPtr, InitializedMax))
    {
        // Validation failed
        return false;
    }

    //  Recursive Traversal (Follow the paths)
    Condition* pCond = dynamic_cast<Condition*>(CurrentStat);
    bool TraversalResult = true;

    if (pCond)
    {
        // Condition statements have two paths
        Connector* pConnT = pCond->getOutConnectorTrue();
        Connector* pConnF = pCond->getOutConnectorFalse();

        // 1. Traverse the True path
        if (pConnT) {
            if (!TraverseAndValidateSequential(pConnT->getDstStat(), ErrorMsg,
                DeclaredVars, DeclaredCountPtr, DeclaredMax,
                InitializedVars, InitializedCountPtr, InitializedMax))
                TraversalResult = false;
        }
        // 2. Traverse the False path (only if the True path succeeded so far)
        if (TraversalResult)
        {
            if (pConnF) {
                if (!TraverseAndValidateSequential(pConnF->getDstStat(), ErrorMsg,
                    DeclaredVars, DeclaredCountPtr, DeclaredMax,
                    InitializedVars, InitializedCountPtr, InitializedMax))
                    TraversalResult = false;
            }
        }
    }
    else // Statements with a single output (Assign, Read, Write, Start, Declare, End)
    {
        Connector* pConn = CurrentStat->getOutConnector();
        if (pConn) {
            // Traverse the single path
            TraversalResult = TraverseAndValidateSequential(pConn->getDstStat(), ErrorMsg,
                DeclaredVars, DeclaredCountPtr, DeclaredMax,
                InitializedVars, InitializedCountPtr, InitializedMax);
        }
    }
    // Only if the current statement and ALL its children were successfully validated:
    if (TraversalResult)
    {
        CurrentStat->SetVisited(true); // Mark as fully visited (Rule E)
    }

    return TraversalResult;
}

void Validate::Execute()
{
    Output* pOut = pManager->GetOutput();
    Statement** pStatList = pManager->GetStatementList();
    int StatCount = pManager->GetStatCount();
    string ErrorMsg = "";

    // first check (Start/End Count) 
    int StartCount = 0;
    int EndCount = 0;
    Statement* pStart = NULL;

    for (int i = 0; i < StatCount; i++)
    {
        if (dynamic_cast<Start*>(pStatList[i]))
        {
            StartCount++;
            pStart = pStatList[i];
        }
        else if (dynamic_cast<End*>(pStatList[i]))
        {
            EndCount++;
        }
    }
    if (StartCount != 1)
    {
        pOut->PrintMessage("Validation Failed: Flowchart must contain exactly ONE Start statement ");
        return;
    }
    if (EndCount != 1)
    {
        pOut->PrintMessage("Validation Failed: Flowchart must contain exactly ONE End statement ");
        return;
    }

  
    //  second Check (Number of Outgoing Connectors)
    for (int i = 0; i < StatCount; i++)
    {
        Statement* pCurrentStat = pStatList[i];

        int ActualConnectors = pManager->CountOutgoingConnectors(pCurrentStat);
        int ExpectedConnectors = 0;

        if (dynamic_cast<Condition*>(pCurrentStat))
        {
            ExpectedConnectors = 2;
        }
        else if (dynamic_cast<End*>(pCurrentStat))
        {
            ExpectedConnectors = 0;
        }
        else
        {
            // Assign, Read, Write, Start, Declare must have exactly 1
            ExpectedConnectors = 1;
        }

        if (ActualConnectors != ExpectedConnectors)
        {
            ErrorMsg = "Validation Failed : Statement ID " + to_string(pCurrentStat->getID()) + " has an incorrect number of outgoing connectors (" +
                to_string(ActualConnectors) + " found, " + to_string(ExpectedConnectors) + " expected).";

            pOut->PrintMessage(ErrorMsg);
            pManager->ResetAllStatementVisits();
            return;
        }
    }

    
    // 3rd Sequential Traversal ( Declaration/Initialization, Cycle Check)
    static const int MaxVariables = 200;
    // List 1: Tracks only variables that have been DECLARED
    string DeclaredVariables[MaxVariables];
    int DeclaredVarCount = 0;

    // List 2: Tracks only variables that have been INITIALIZED 
    string InitializedVariables[MaxVariables];
    int InitializedVarCount = 0;

    // Start the recursive traversal and validation from the Start statement
    bool TraversalSuccess = TraverseAndValidateSequential(pStart, ErrorMsg,
        DeclaredVariables, &DeclaredVarCount, MaxVariables,
        InitializedVariables, &InitializedVarCount, MaxVariables);

    
    // 4th Final Reachability Check 
    if (TraversalSuccess)
    {
        //  Check for Unreachable Statements 
        for (int i = 0; i < StatCount; i++)
        {
            if (!pStatList[i]->IsVisited())
            {
                ErrorMsg = "Validation Failed : Statement ID " + to_string(pStatList[i]->getID()) + " is unreachable";
                pOut->PrintMessage(ErrorMsg);
                pManager->ResetAllStatementVisits();
                return;
            }
        }
    }

    // Final Result
    if (TraversalSuccess)
    {
        pOut->PrintMessage("Flowchart Validation Succeeded!");
    }
    else
    {
        pOut->PrintMessage("Validation Failed: " + ErrorMsg);
    }

    pManager->ResetAllStatementVisits();
}


bool Validate::TraverseAndValidate(string& ErrorMsg)
{
    Statement** pStatList = pManager->GetStatementList();
    int StatCount = pManager->GetStatCount();
    pManager->ResetAllStatementVisits();

    // Find the Start statement
    Statement* pStart = nullptr;
    for (int i = 0; i < StatCount; i++)
    {
        if (dynamic_cast<Start*>(pStatList[i]))
        {
            pStart = pStatList[i];
            break;
        }
    }
    if (!pStart) { ErrorMsg = "No Start statement found"; return false; }

    static const int MaxVariables = 200;
    string DeclaredVariables[MaxVariables]; int DeclaredVarCount = 0;
    string InitializedVariables[MaxVariables]; int InitializedVarCount = 0;

    bool TraversalSuccess = TraverseAndValidateSequential(pStart, ErrorMsg,
        DeclaredVariables, &DeclaredVarCount, MaxVariables,
        InitializedVariables, &InitializedVarCount, MaxVariables);

    // Check for unreachable statements
    if (TraversalSuccess)
    {
        for (int i = 0; i < StatCount; i++)
        {
            if (!pStatList[i]->IsVisited())
            {
                ErrorMsg = "Statement ID " + to_string(pStatList[i]->getID()) + " is unreachable";
                TraversalSuccess = false;
                break;
            }
        }
    }

    pManager->ResetAllStatementVisits();
    return TraversalSuccess;
}
