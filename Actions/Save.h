#ifndef SAVE_H
#define SAVE_H

#include "Action.h" 
#include <string>   

// Action to save the current flowchart to a file
class Save : public Action
{
private:
    string FileName; // To store the filename entered by the user

public:
    Save(ApplicationManager* pAppManager);

    // Reads the filename from the user 
    virtual void ReadActionParameters();
    // Creates the file, and saves the flowchart
    virtual void Execute();
};

#endif