#ifndef VALIDATE_ACTION_H
#define VALIDATE_ACTION_H

#include "Action.h"
#include <string>


class Statement;


class Validate : public Action
{
private:
    string ErrorMsg; 

public:
    Validate(ApplicationManager* pAppManager);

    // Reads parameters to implement pure virtual fn only
    virtual void ReadActionParameters();

    // Executes the validation process
    virtual void Execute();

    // This function performs the recursive traversal and validation.
    bool TraverseAndValidateSequential(Statement* CurrentStat, string& ErrorMsg,
        string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
        string InitializedVars[], int* InitializedCountPtr, int InitializedMax);

    bool TraverseAndValidate(string& ErrorMsg);
};


#endif