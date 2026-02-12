#ifndef OUPTUT_H
#define OUPTUT_H

#include "Input.h"

class Output	//The application manager should have a pointer to this class
{
private:
	window* pWind;	//Pointer to the Graphics Window
public:
	Output();

	window* CreateWind(int, int, int, int);
	Input* CreateInput(); //Creates a pointer to the Input object	

	void CreateStatusBar(); //Creates status bar

	//DONE: Complete the following 2 functions
	void CreateDesignToolBar();	//Tool bar of the design mode
	void CreateSimulationToolBar(); //Tool bar of the simulation mode

	void ClearStatusBar();	//Clears the status bar
	void ClearDrawArea();	//Clears the drawing area
	void ClearOutputBar(); //Clears the Output bar

	void PrintMessage(string msg);	//Prints a message on Status bar
	void DrawString(const int, const int, const string);  //Writes a string in the specified location

	// -- Statements Drawing Functions
	void DrawAssign(Point Left, int width, int height, string Text, bool Selected = false);


	//DONE: Add similar functions for drawing all other statements.
	//		e.g. DrawDeclareStat(.....), DrawCondtionalStat(......), DrawStart(......), DrawEnd(.......), ...etc
	//		Decide the parameters that should be passed to each of them


	void DrawReadStat(Point Left, int width, int height, string Text, bool Selected = false);
	void DrawWriteStat(Point Left, int width, int height, string Text, bool Selected = false);
	void DrawCondtionalStat(Point Left, int width, string Text, bool Selected = false);
	void DrawStart(Point Left, int width, int height, string Text, bool Selected = false);
	void DrawEnd(Point Left, int width, int height, string Text, bool Selected = false);
	void DrawDeclareStat(Point Left, int width, int height, string Text, bool Selected = false);
	void ClearToolbar();

	//DONE: Add DrawConnector function
	void DrawStraightConnector(Point Start, Point End, bool Selected = false);

	void PrintOnOutputBar(string msg);
	void AppendToOutputBar(string msg);

	~Output();
};

#endif