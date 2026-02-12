#ifndef INPUT_H
#define INPUT_H

#include "..\DEFS.h"
#include "UI_Info.h"
#include "..\HelperFn.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include <string>
using namespace std;

class Output;

class Input
{
private:
	window* pWind; // Pointer to the graphics window أنا عاملها

public:
	Input(window* pW);  // Constructor أنا عاملها

	// Basic input functions
	void GetPointClicked(Point& P) const;      // Get mouse click coordinates أنا عاملها
	string GetString(Output* pO) const;        // Get a string input from the user أنا عاملها


	double GetValue(Output* pO) const;         // Get a numeric value (double) أنا عاملها

	string GetVariable(Output* pO) const;      // Get a valid variable name أنا مكملها
	char GetArithOperator(Output* pO) const;   // Get an arithmetic operator (+,-,*,/) أنا عاملها
	string GetCompOperator(Output* pO) const;  // Get a comparison operator (==, !=, <, <=, >, >=) أنا عاملها

	ActionType GetUserAction() const;  // Map user click to action أنا عاملها

	~Input();  // Destructor أنا عاملها

};

#endif
