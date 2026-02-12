	#include "Output.h"


	Output::Output()
	{
		//Initialize user interface parameters
		UI.width = 1200;
		UI.height = 620;
		UI.wx = 15;
		UI.wy = 15;

		UI.AppMode = DESIGN;	//Design Mode is the default mode

		UI.StatusBarHeight = 50;
		UI.ToolBarHeight = 50;
		UI.MenuItemWidth = (float)UI.width / DSN_ITM_CNT; //to accommodate all icons in a suitable size
		UI.DrawingAreaWidth = 0.75 * UI.width;

		UI.DrawColor = BLUE;
		UI.HighlightColor = RED;
		UI.MsgColor = RED;

		UI.ASSGN_WDTH = 150;
		UI.ASSGN_HI = 50;

		//Create the output window
		pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
		//Change the title
		pWind->ChangeTitle("Programming Techniques Project");

		pWind->SetPen(RED, 3);

		CreateDesignToolBar();
		CreateStatusBar();
		ClearDrawArea();
		ClearOutputBar();
	}


	Input* Output::CreateInput()
	{
		Input* pIn = new Input(pWind);
		return pIn;
	}

	//======================================================================================//
	//								Interface Functions										//
	//======================================================================================//

	window* Output::CreateWind(int wd, int h, int x, int y)
	{
		return new window(wd, h, x, y);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::CreateStatusBar()
	{
		pWind->DrawLine(0, UI.height - UI.StatusBarHeight, UI.width, UI.height - UI.StatusBarHeight);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//DONE: Complete this function
	void Output::CreateDesignToolBar() //Draws the Design Menu
	{
		UI.AppMode = DESIGN;	//Design Mode

		//fill the tool bar 

		//You can draw the tool bar icons in any way you want.
		//Below is one possible way

		//First prepare List of images for each menu item
		//To control the order of these images in the menu, 
		//reoder them in Defs.h ==> enum DrawMenuItem
		string MenuItemImages[DSN_ITM_CNT];



		//DONE Prepare images for each menu item and add it to the list


		MenuItemImages[ITM_START] = "images\\start.jpg";
		MenuItemImages[ITM_END] = "images\\end.jpg";
		MenuItemImages[ITM_READ] = "images\\read.jpg";
		MenuItemImages[ITM_WRITE] = "images\\write.jpg";
		MenuItemImages[ITM_DECLARE] = "images\\declare.jpg";
		MenuItemImages[ITM_VALUE_ASSIGN] = "images\\value_assign.jpg";
		MenuItemImages[ITM_VAR_ASSIGN] = "images\\var_assign.jpg";
		MenuItemImages[ITM_OPER_ASSIGN] = "images\\oper_assign.jpg";
		MenuItemImages[ITM_COND] = "images\\cond.jpg";
		MenuItemImages[ITM_Connector] = "images\\connector.jpg";
		MenuItemImages[ITM_SELECT] = "images\\select.jpg";
		MenuItemImages[ITM_EDIT] = "images\\edit.jpg";
		MenuItemImages[ITM_COPY] = "images\\copy.jpg";
		MenuItemImages[ITM_CUT] = "images\\cut.jpg";
		MenuItemImages[ITM_PASTE] = "images\\paste.jpg";
		MenuItemImages[ITM_DELETE] = "images\\delete.jpg";
		MenuItemImages[ITM_SAVE] = "images\\save.jpg";
		MenuItemImages[ITM_LOAD] = "images\\load.jpg";
		MenuItemImages[DesignMenuItem::ITM_SIM_MODE] = "images\\SIM_mode.jpg";

		MenuItemImages[ITM_EXIT_DSN] = "images\\Exit.jpg";
		//Draw menu item one image at a time
		for (int i = 0; i < DSN_ITM_CNT; i++)
			pWind->DrawImage(MenuItemImages[i], i * UI.MenuItemWidth, 0, UI.MenuItemWidth, UI.ToolBarHeight);

		//Draw a line under the toolbar
		pWind->SetPen(RED, 2);
		pWind->DrawLine(0, UI.ToolBarHeight, UI.width, UI.ToolBarHeight);

	}
	//////////////////////////////////////////////////////////////////////////////////////////
	//TODO: Complete this function
	void Output::CreateSimulationToolBar() //Draws the Simulation Menu
	{

		UI.AppMode = SIMULATION;	//Simulation Mode
		///DONE: add code to create the simulation tool bar

		ClearToolbar();//Clear old toolbar first
		string MenuItemImages[SIM_ITM_CNT];

		MenuItemImages[ITM_VALIDATE] = "images\\validate.jpg";
		MenuItemImages[ITM_RUN] = "images\\run.jpg";
		MenuItemImages[ITM_CPP] = "images\\cpp.jpg";
		MenuItemImages[ITM_DSN_MODE] = "images\\DSN_mode.jpg";
		MenuItemImages[ITM_EXIT_SIM] = "images\\Exit.jpg";
		for (int i = 0; i < SIM_ITM_CNT; i++)
			pWind->DrawImage(MenuItemImages[i], i * UI.MenuItemWidth, 0, UI.MenuItemWidth, UI.ToolBarHeight);

		//Draw line under toolbar
		pWind->SetPen(RED, 2);
		pWind->DrawLine(0, UI.ToolBarHeight, UI.width, UI.ToolBarHeight);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::ClearStatusBar()
	{
		//Clear Status bar by drawing a filled white rectangle
		pWind->SetPen(RED, 2);
		pWind->SetBrush(WHITE);
		pWind->DrawRectangle(0, UI.height - UI.StatusBarHeight, UI.width, UI.height);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::ClearDrawArea()
	{
		pWind->SetPen(RED, 2);
		pWind->SetBrush(WHITE);
		pWind->DrawRectangle(0, UI.ToolBarHeight, UI.DrawingAreaWidth, UI.height - UI.StatusBarHeight);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::ClearOutputBar()
	{
		//Create output bar by drawing a filled rectangle
		pWind->SetPen(RED, 2);
		pWind->SetBrush(LIGHTBLUE);
		pWind->DrawRectangle(UI.DrawingAreaWidth, UI.ToolBarHeight, UI.width, UI.height - UI.StatusBarHeight);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::PrintMessage(string msg)	//Prints a message on status bar
	{
		ClearStatusBar();	//First clear the status bar

		pWind->SetPen(UI.MsgColor, 50);
		pWind->SetFont(20, BOLD, BY_NAME, "Arial");
		pWind->DrawString(10, UI.height - (int)(UI.StatusBarHeight / 1.5), msg);
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Output::DrawString(const int iX, const int iY, const string Text)
	{
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(iX, iY, Text);
	}
	//////////////////////////////////////////////////////////////////////////////////////////

	//======================================================================================//
	//								Statements Drawing Functions								//
	//======================================================================================//




	//Draw assignment statement and write the "Text" on it
	void Output::DrawAssign(Point Left, int width, int height, string Text, bool Selected)
	{
		if (Selected)	//if stat is selected, it should be highlighted
			pWind->SetPen(UI.HighlightColor, 3);	//use highlighting color
		else
			pWind->SetPen(UI.DrawColor, 3);	//use normal color

		//Draw the statement block rectangle
		pWind->DrawRectangle(Left.x, Left.y, Left.x + width, Left.y + height);

		//Write statement text
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Left.x + width / 4, Left.y + height / 4, Text);
	}

	//DONE: Add similar functions for drawing all other statements.
	//		e.g. DrawDeclareStat(.....), DrawCondtionalStat(......), DrawStart(......), DrawEnd(.......), ...etc
	//		Decide the parameters that should be passed to each of them

	void Output::DrawReadStat(Point Left , int width, int height, string Text, bool Selected)
	{
		if (Selected)
			pWind->SetPen(UI.HighlightColor, 3);
		else
			pWind->SetPen(UI.DrawColor, 3);

		int X[4], Y[4];
		int offset = width * 0.2; // horizontal shift for left-leaning

		// Define vertices for left-leaning parallelogram
		X[0] = Left.x;              Y[0] = Left.y;           // Top-left
		X[1] = Left.x + width;      Y[1] = Left.y;           // Top-right
		X[2] = Left.x + width - offset;               Y[2] = Left.y + height;  // Bottom-right
		X[3] = Left.x - offset;                       Y[3] = Left.y + height;  // Bottom-left

		// Draw the parallelogram(statement of read and write)
		pWind->DrawPolygon(X, Y, 4);

		// Draw the statement text
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Left.x + width / 4, Left.y + height / 3, Text);
	}
	void Output::DrawWriteStat(Point Left, int width, int height, string Text, bool Selected)
	{
		if (Selected)
			pWind->SetPen(UI.HighlightColor, 3);
		else
			pWind->SetPen(UI.DrawColor, 3);

		int X[4], Y[4];
		int offset = width * 0.2; // horizontal shift for left-leaning

		// Define vertices for left-leaning parallelogram
		X[0] = Left.x;              Y[0] = Left.y;           // Top-left
		X[1] = Left.x + width;      Y[1] = Left.y;           // Top-right
		X[2] = Left.x + width - offset;               Y[2] = Left.y + height;  // Bottom-right
		X[3] = Left.x - offset;                       Y[3] = Left.y + height;  // Bottom-left

		// Draw the parallelogram(statement of read and write)
		pWind->DrawPolygon(X, Y, 4);

		// Draw the statement text
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Left.x + width / 4, Left.y + height / 3, Text);
	}
	void Output::DrawCondtionalStat(Point Top, int width, string Text, bool Selected)
	{
		if (Selected)
			pWind->SetPen(UI.HighlightColor, 3);
		else
			pWind->SetPen(UI.DrawColor, 3);

		int X[4], Y[4];

		// Diamond vertices
		X[0] = Top.x;               Y[0] = Top.y;               // Top
		X[1] = Top.x + width / 2;   Y[1] = Top.y + width / 2;  // Right
		X[2] = Top.x;               Y[2] = Top.y + width;      // Bottom
		X[3] = Top.x - width / 2;   Y[3] = Top.y + width / 2;  // Left

		// Draw diamond
		pWind->DrawPolygon(X, Y, 4);

		// Print condition text
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Top.x - width / 4, Top.y + width / 2, Text);
	}

	void Output::DrawStart(Point Center, int width, int height, string Text, bool Selected)
	{
		if (Selected)
			pWind->SetPen(UI.HighlightColor, 3);
		else
			pWind->SetPen(UI.DrawColor, 3);

		// Compute bounding box coordinates for the ellipse
		int x1 = Center.x - width / 2;   // Left
		int y1 = Center.y - height / 2;  // Top
		int x2 = Center.x + width / 2;   // Right
		int y2 = Center.y + height / 2;  // Bottom

		// Draw the ellipse (Start or End statement)
		pWind->DrawEllipse(x1, y1, x2, y2, FRAME);

		// Draw the text inside the ellipse, approximately centered
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Center.x - width / 4, Center.y - height / 8, Text);
	}
	void Output::DrawEnd(Point Center, int width, int height, string Text, bool Selected)
	{
		if (Selected)
			pWind->SetPen(UI.HighlightColor, 3);
		else
			pWind->SetPen(UI.DrawColor, 3);

		// Compute bounding box coordinates for the ellipse
		int x1 = Center.x - width / 2;   // Left
		int y1 = Center.y - height / 2;  // Top
		int x2 = Center.x + width / 2;   // Right
		int y2 = Center.y + height / 2;  // Bottom

		// Draw the ellipse (Start or End statement)
		pWind->DrawEllipse(x1, y1, x2, y2, FRAME);

		// Draw the text inside the ellipse, approximately centered
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Center.x - width / 4, Center.y - height / 8, Text);
	}

	void Output::DrawDeclareStat(Point Left, int width, int height, string Text, bool Selected)
	{
		if (Selected)	//if stat is selected, it should be highlighted
			pWind->SetPen(UI.HighlightColor, 3);	//use highlighting color
		else
			pWind->SetPen(UI.DrawColor, 3);	//use normal color

		//Draw the statement block rectangle
		pWind->DrawRectangle(Left.x, Left.y, Left.x + width, Left.y + height);

		//Write statement text
		pWind->SetPen(BLACK, 2);
		pWind->DrawString(Left.x + width / 4, Left.y + height / 4, Text);
	}


	//DONE: Add DrawConnector function

	void Output::DrawStraightConnector(Point Start, Point End, bool Selected)
	{
		if(Selected)
			pWind->SetPen(UI.HighlightColor, 4);
	else
		pWind->SetPen(UI.DrawColor, 3);

		if (End.y > Start.y)
		{
			// نقطة الانكسار في منتصف المسافة الرأسية
			int midY = Start.y + (End.y - Start.y) / 2;

			// رسم 3 خطوط لتكوين شكل حرف Z أو S
			pWind->DrawLine(Start.x, Start.y, Start.x, midY); // خط رأسي من البداية للمنتصف
			pWind->DrawLine(Start.x, midY, End.x, midY);     // خط أفقي يربط بين الـ x للبداية والـ x للنهاية
			pWind->DrawLine(End.x, midY, End.x, End.y);     // خط رأسي يوصل للنهاية

			// رسم رأس السهم عند النهاية
			int arrowSize = 10;
			int X[3] = { End.x, End.x - 5, End.x + 5 };
			int Y[3] = { End.y, End.y - arrowSize, End.y - arrowSize };
			pWind->DrawPolygon(X, Y, 3);
		}
		else if (End.y < Start.y && End.x>Start.x)
		{
			pWind->DrawLine(Start.x, Start.y, Start.x, Start.y + 20);
			pWind->DrawLine(Start.x, Start.y + 20, Start.x + 140, Start.y + 20);
			pWind->DrawLine(Start.x + 140, Start.y + 20, Start.x + 140, End.y - 30);
			pWind->DrawLine(Start.x + 140, End.y - 30, End.x, End.y - 30);
			pWind->DrawLine(End.x, End.y - 30, End.x, End.y);
			int arrowSize = 10;
			int X[3] = { End.x, End.x - 5, End.x + 5 };
			int Y[3] = { End.y, End.y - arrowSize, End.y - arrowSize };
			pWind->DrawPolygon(X, Y, 3);
		}
		else if (End.y < Start.y && End.x < Start.x)
		{
			pWind->DrawLine(Start.x, Start.y, Start.x, Start.y + 20);
			pWind->DrawLine(Start.x, Start.y + 20, Start.x - 140, Start.y + 20);
			pWind->DrawLine(Start.x - 140, Start.y + 20, Start.x - 140, End.y - 30);
			pWind->DrawLine(Start.x - 140, End.y - 30, End.x, End.y - 30);
			pWind->DrawLine(End.x, End.y - 30, End.x, End.y);
			int arrowSize = 10;
			int X[3] = { End.x, End.x - 5, End.x + 5 };
			int Y[3] = { End.y, End.y - arrowSize, End.y - arrowSize };
			pWind->DrawPolygon(X, Y, 3);


		}

	}





	void Output::ClearToolbar()
	{
		pWind->SetPen(RED, 2);
		pWind->SetBrush(WHITE);
		pWind->DrawRectangle(0, 0, UI.width, UI.ToolBarHeight);
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	Output::~Output()
	{
		delete pWind;
	}
	void Output::PrintOnOutputBar(string msg)
	{
		// Clear output bar first
		ClearOutputBar();

		pWind->SetPen(BLACK, 2);
		pWind->SetFont(18, BOLD, BY_NAME, "Arial");

		// Draw text starting near the top-left of the output bar
		pWind->DrawString(UI.DrawingAreaWidth + 10, UI.ToolBarHeight + 20, msg);
	}

	void Output::AppendToOutputBar(string msg)
	{
		static int offsetY = 0;

		// If first time → clear output bar
		if (offsetY == 0)
			ClearOutputBar();

		pWind->SetPen(BLACK, 2);
		pWind->SetFont(18, BOLD, BY_NAME, "Arial");

		// Draw next line
		pWind->DrawString(UI.DrawingAreaWidth + 10, UI.ToolBarHeight + 5 + offsetY, msg);


		offsetY += 25; // Move down for next line

		// Reset if bar full
		if (UI.ToolBarHeight + 20 + offsetY > UI.height - UI.StatusBarHeight - 20)
			offsetY = 0;
	}
