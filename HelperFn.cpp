#include "HelperFn.h"

bool IsValue(string input)
{

	if (input == "")   //empty string 
		return false;

	int dotcounter = 0;  //checks number of dots in the number 

	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == '-' && i == 0)
			continue;                          //minus sign allowed only at start of digit

		if (input[i] == '.' && dotcounter == 0)
		{
			dotcounter++;
			continue;
		}
		if (input[i] == '.' && dotcounter != 0)
			return false;                        //if there is more than one dot that can't be a double value

		if (input[i] < '0' || input[i] > '9')  //check if char is a digit
			return false;
	}

	if (input == "-" || input == "." || input == "-.")  //checks special cases that may pass the loop above
		return false;

	return true; //then it is a double value
}

bool IsVariable(string input)
{
	if (input == "")  // empty string can't be a variable
		return false;

	if (!((input[0] >= 'A' && input[0] <= 'Z') || (input[0] >= 'a' && input[0] <= 'z') || input[0] == '_'))
		return false;                                  // First character check if it is (A-Z) or (a-z) or "_"


	for (int i = 1; i < input.length(); i++) // Remaining characters can be letters, digits, or underscore
	{
		if (!((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z') || (input[i] >= '0' && input[i] <= '9') || input[i] == '_'))
		{
			return false;  // any character that doesn't follow that criteria is invalid character
		}
	}

	return true; // valid variable
}

OpType ValueOrVariable(string input)
{
	if (IsValue(input))       //check if value
		return VALUE_OP;

	if (IsVariable(input))       //check if variable 
		return VARIABLE_OP;

	return INVALID_OP; //if not value or variable 

}
