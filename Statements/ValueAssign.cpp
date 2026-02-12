	#include "ValueAssign.h"
	#include <sstream>
	#include <fstream>// use it in save fn
	#include <string>
	#include <cstdlib>

	using namespace std;

	ValueAssign::ValueAssign(Point Lcorner, string LeftHS, double RightHS)
	{
		// Note: The LeftHS and RightHS should be validated inside (AddValueAssign) action
		//       before passing it to the constructor of ValueAssign
		LHS = LeftHS;
		RHS = RightHS;

		UpdateStatementText();

		LeftCorner = Lcorner;

		pOutConn = NULL;	//No connectors yet

		Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
		Inlet.y = LeftCorner.y;

		Outlet.x = Inlet.x;
		Outlet.y = LeftCorner.y + UI.ASSGN_HI;
	}


	void ValueAssign::setLHS(const string& L)
	{
		LHS = L;
		UpdateStatementText();
	}

	void ValueAssign::setRHS(double R)
	{
		RHS = R;
		UpdateStatementText();
	}


	void ValueAssign::Draw(Output* pOut) const
	{
		//Call Output::DrawAssign function to draw assignment statement 	
		pOut->DrawAssign(LeftCorner, UI.ASSGN_WDTH, UI.ASSGN_HI, Text, Selected);

	}

	Point ValueAssign::getInlet() const
	{
		return Inlet;
	}
	Point ValueAssign::getOutlet() const
	{
		return Outlet;
	}


	void ValueAssign::setOutConnector(Connector* pConn)
	{
		pOutConn = pConn;
	}

	Connector* ValueAssign::getOutConnector()
	{
		return pOutConn;
	}

	bool ValueAssign::IsPointInside(Point P)
	{
		if (P.x >= LeftCorner.x && P.x <= LeftCorner.x + UI.ASSGN_WDTH && P.y >= LeftCorner.y && P.y <= LeftCorner.y + UI.ASSGN_HI)
		{
			return true;
		}
		return false;

	}



	//This function should be called when LHS or RHS changes
	void ValueAssign::UpdateStatementText()
	{
		//Build the statement text: Left handside then equals then right handside
		ostringstream T;
		T << LHS << " = " << RHS;
		Text = T.str();
	}

	// ** دالة Clone **
	Statement* ValueAssign::Clone() const {
		ValueAssign* s = new ValueAssign(*this);
		s->Selected = false;      // يجعل النسخة الملصوقة غير محددة
		s->pOutConn = nullptr;    // لا ننسخ الـ connectors
		return s;
	}

	void ValueAssign::SetCenter(Point P) // غيّر اسم الكلاس هنا
	{
		// 1. تحديث المركز الموروث (Center)
		Center = P;

		// 2. حساب الزاوية اليسرى العلوية (LeftCorner)
		// LeftCorner = Center - نصف العرض/نصف الارتفاع
		LeftCorner.x = Center.x - UI.ASSGN_WDTH / 2;
		LeftCorner.y = Center.y - UI.ASSGN_HI / 2;

		// 3. تحديث نقاط الوصل (Inlet/Outlet) بناءً على الموقع الجديد
		Inlet.x = Center.x;
		Inlet.y = LeftCorner.y;

		Outlet.x = Center.x;
		Outlet.y = LeftCorner.y + UI.ASSGN_HI;
	}

	void ValueAssign::Save(ofstream& OutFile)
	{

		// 1. Write the Statement Type
		OutFile << "VAL_ASSIGN" << "\t"; // The unique keyword for Value Assignment statement
		// 2. Write the Statement ID 
		OutFile << ID << "\t";
		// 3. Write the Statement Position (Top Left Point coordinates)
		OutFile << LeftCorner.x << "\t" << LeftCorner.y << "\t";
		// 4. Write the Expression (LHS and RHS)
		OutFile << LHS << "\t" << "EQUAL\t" << RHS << "\t";
		// 5. End the line
		OutFile << "\n";

	}





	void ValueAssign::Load(ifstream& In)
	{
		int id, x, y;
		string left, equal_keyword; // Must read the "EQUAL" keyword
		double right;

		//  Read 7 tokens exactly as saved by ValueAssign::Save.
		// The tokens are: ID, X, Y, LHS, "EQUAL", RHS.
		// The statement type ("VAL_ASSIGN") is read outside this function.
		In >> id >> x >> y >> left >> equal_keyword >> right;

		// 1. Assign ID
		ID = id;

		// 2. Assign Position
		LeftCorner.x = x;
		LeftCorner.y = y;

		// 3. Assign Expression details
		LHS = left;
		RHS = right;

		// 4. Update the graphical text for drawing
		UpdateStatementText();

		// 5. Update Inlet/Outlet points based on the new LeftCorner
		Inlet.x = LeftCorner.x + UI.ASSGN_WDTH / 2;
		Inlet.y = LeftCorner.y;

		Outlet.x = Inlet.x;
		Outlet.y = LeftCorner.y + UI.ASSGN_HI;
	}



	bool ValueAssign::Validate(string& ErrorMsg,
		string DeclaredVars[], int* DeclaredCountPtr, int DeclaredMax,
		string InitializedVars[], int* InitializedCountPtr, int InitializedMax)
	{
		string VarLHS = LHS;
		int CurrentDeclaredCount = *DeclaredCountPtr;
		int CurrentInitializedCount = *InitializedCountPtr;

		// Ensure the statement has exactly one output connector.
		if (pOutConn == NULL)
		{
			ErrorMsg = "Error (ID: " + to_string(ID) + "): Value Assignment statement must have exactly one output connector.";
			return false;
		}

		//  Check (LHS): Variable must be DECLARED.
		if (!SearchDeclared(VarLHS, DeclaredVars, CurrentDeclaredCount))
		{
			ErrorMsg = "Error (ID: " + to_string(ID) + "): Variable " + VarLHS + " used/assigned without prior declaration.";
			return false;
		}

		// 3. Initialization Update (LHS): Mark the variable as initialized.
		if (!SearchDeclared(VarLHS, InitializedVars, CurrentInitializedCount))
		{
			if (*InitializedCountPtr < InitializedMax)
			{
				InitializedVars[(*InitializedCountPtr)++] = VarLHS;
			}
			else 
			{
				ErrorMsg = "Error (ID: " + to_string(ID) + "): Cannot initialize variable " + VarLHS + ". Initialization limit reached.";
				return false;
			}
		}

		return true;
	}
	void ValueAssign::Simulate(ApplicationManager* pManager, Output* pOut, Input* pIn)
	{
		pManager->SetVariableValue(LHS, RHS);
	}

	string ValueAssign::getLHS() const {
		return LHS;
	}

	double ValueAssign::getRHS() const {
		return RHS;
	}

	void ValueAssign::Edit(ApplicationManager* pManager)
	{
		Output* pOut = pManager->GetOutput();
		Input* pIn = pManager->GetInput();

		string newLHS;
		while (true)
		{
			pOut->PrintMessage("Enter variable name :");
			newLHS = pIn->GetString(pOut);
			if (IsVariable(newLHS)) break;
	
		}
		setLHS(newLHS);

		double newRHS;
		while (true)
		{
			pOut->PrintMessage("Enter numeric value :");
			newRHS = pIn->GetValue(pOut);
			break; // GetValue already ensures a valid number
		}
		setRHS(newRHS);

		UpdateStatementText();
 
	}
