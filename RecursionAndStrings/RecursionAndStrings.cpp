/*
 *             Author: Michael Marven
 *       Date Created: 11/30/13
 * Last Date Modified: 11/30/13
 *          File Name: RecursionAndStrings.cpp
 *           Overview: The program allows the user to pick from a
 *                     selection of functions which reverses an string entered
 *                     or tells the user whether or not the entered string is a
 *                     palindrome
 *                     ** READ ME ** The program contains C++11 functions and
 *                                   must be compiled with the -std=c++0x flag
 *              Input: The user inputs function selections, and the string
 *             Output: The reversed string or a statement for the palindromes
 *                     are printed to the console window.
 *
 *
 */
 
#include <iostream>
#include <string>
#include <cstdlib>


const int MIN_FUNC_CHOICE = 1;
const int MAX_FUNC_CHOICE = 4;

/*
 * The reverse function accepts a string as a parameter, reverses the characters
 * through an iterative process, and returns the new string
 */
std::string reverse(std::string strInput);

/*
 * The reverseRecurs function accepts a string as a parameter, reverses the
 * characters through an recursive process, and returns the new string
 */
std::string reverseRecurs(std::string strInput);

/*
 * The getString function retrieves a string that the user enters
 */
std::string getString();

/*
 * The isPalindrome function accepts a string as a parameter and checks whether
 * or not it is a palindrome through an iterative process
 */
bool isPalindrome(std::string str);

/*
 * The isPalindromeRec function accepts a string as a parameter and checks
 * whether or not it is a palindrome through a recursive process
 */
bool isPalindromeRec(std::string str);

/*
 * The funcCaller function accepts a string as a parameter and allows the user
 * to select which function will process the string, then returns a string
 * for output
 */
std::string funcCaller(std::string input);

/*
 * The getUserInt function retrieves a number input by the user and
 * validates it
 */
int getUserInt();

/*
 * The isPositiveInt function validates that a string input can be safely
 * converted to an integer and returns true if the input is a positive integer
 */
bool isPositiveInt(std::string input);

/*
 * The checkValidInput function accepts three int values as parameters, and
 * returns the first argument if it is between (inclusive) the second and third
 * arguments, or a -1 if the number is not within the range of the second and
 * third numbers
 */
int checkValidInput(int check, int low, int high);

/*
 * The procAgain function determines whether the user would like to process
 * another string using getUserInput and alters the repeatProc variable 
 */
void procAgain(bool &repeatProc);

/*
 * The getYesOrNo function asks the user for a yes or no answer and returns the
 * result as a boolean value 
 */
bool getYesOrNo();

/*
 * The isYOrN function validates a string input as Y or N
 */
bool isYOrN(std::string input);

int main()
{
    // Initialize a variable for repeating the program
    bool repeatProc = true;
    
    // A loop to allow the user to calculate again
    while (repeatProc)
    {
        // Initialize a string for user input
        std::string userString = "";
        
        // Retrieve the users string
        userString = getString();
        
        /* 
         * Allow the user to choose the function, process the string, then print
         * the new string
         */
        std::cout << funcCaller(userString) << "\n";
        
        // Ask the user whether he would like to process another string
        procAgain(repeatProc);
    }
    
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: reverse
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Reverses the order of the characters in the input string, then 
 *           returns the new string
 *
 *  Purpose: Reverse the characters in a string through an iterative process
 *
 *
 *   *   *   *   *   *   */
std::string reverse(std::string strInput)
{
    // Initialize a string for the output
    std::string output;
    
    // Reverse the characters in the input string with the for loop
    for (int i = strInput.length() - 1; i > -1; i--)
    {
        output = output + strInput.substr(i, 1);
    }
    
    return (output);
}

/*   *   *   *   *   *   *
 * 
 * Function: reverseRecurs
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Reverses the order of the characters in the input string, then 
 *           returns the new string
 *
 *  Purpose: Reverse the characters in a string through a recursive process
 *
 *
 *   *   *   *   *   *   */
std::string reverseRecurs(std::string strInput)
{
    /* 
     * The if else statement checks if the string length is 1 or the string is
     * empty, and returns that string if true; Otherwise it concatenates the
     * last character of the current string to a recursive call with the
     * remainder of the string as the parameter     
     */
    if (strInput.length() == 1 || strInput.length() == 0)
    {
        return (strInput);
    }
    else
    {
        return (strInput.substr((strInput.length() - 1), 1) + 
                reverseRecurs(strInput.substr(0, strInput.length() - 1)));
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: getString
 * 
 *    Entry: None
 *
 *     Exit: Retrieves a string that the user enters
 *
 *  Purpose: Retrieve a string that the user enters
 *
 *
 *   *   *   *   *   *   */
std::string getString()
{
   // Initialize a string to receive the user input
   std::string input;
    
   // Print a message asking the user for a string
   std::cout << "Please enter a string to be processed.\n";

   // Retrieve the input
   std::cin.ignore();
   std::getline(std::cin, input);
    
   return (input);
}
 
/*   *   *   *   *   *   *
 * 
 * Function: isPalindrome
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Checks whether or not the string is a palindrome and returns a 
 *           boolean value
 *
 *  Purpose: Check whether or not a string is a palindrome through an iterative
 *           process
 *
 *
 *   *   *   *   *   *   */
bool isPalindrome(std::string str)
{
    // Declare a variable for output and to track the last character position
    bool isAPalindrome = true;
    int  lastCharPos   = str.length() - 1;
    
    if (str.length() == 1 || str.length() == 0)
    {
        return (isAPalindrome);
    }
    else
    {
        /* 
         * The for loop iterates through the string checking pairs of letters 
         * for equality, beginning with the outermost pair and ending in the
         * middle
         */
        for (int i = 0; i <= ((str.length() / 2) + (str.length() % 2)); i++)
        {
            if (str.at(i) == str.at(lastCharPos))
            {
                lastCharPos--;
            }
            else
            {
                isAPalindrome = false;
                i = ((str.length() / 2) + (str.length() % 2));
            }
        }
        
        return (isAPalindrome);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: isPalindromeRec
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Checks whether or not the string is a palindrome and returns a 
 *           boolean value
 *
 *  Purpose: Check whether or not a string is a palindrome through a recursive
 *           process
 *
 *
 *   *   *   *   *   *   */
bool isPalindromeRec(std::string str)
{
    /*
     * The if else statements first check if the string is 0 or 1 length, and
     * return true if so, since a single character or empty string is the same
     * from front to back; If not, then the test for equality of the first and
     * last characters is performed and false is returned if they are not equal;
     * If the two characters are equal, the function is recursively called on
     * the remainder of the string
     */
    
    if (str.length() == ((str.length() / 2) + (str.length() % 2)))
    {
        return (true);
    }
    else if (str.at(0) != str.at(str.length() - 1))
    {
        return (false);
    }
    else
    {
        return (isPalindromeRec(str.substr(1, (str.length() - 2))));
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: funcCaller
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Asks the user to select a function to process the string, then
 *           returns a string with the results
 *
 *  Purpose: Allow the user to select a function to process a string
 *
 *
 *   *   *   *   *   *   */
std::string funcCaller(std::string input)
{
    // Declare a variable for the function choice
    int         choice;
    std::string output;
    
    // Print a message describing the options and retrieve the user's choice
    std::cout << "This program will process the string you entered.\n";
    std::cout << "The available processes are:\n\n"
              << "[1] Reverse the characters in the string through iteration\n"
              << "[2] Reverse the characters in the string through recursion\n"
              << "[3] Check if the string is a palindrome through iteration\n"
              << "[4] Check if the string is a palindrome through recursion\n"
              << "Please pick a process (1 - 4): ";
    choice = getUserInt();
    
    // Validate that choice is within the correct range
	while ((checkValidInput(choice, MIN_FUNC_CHOICE, MAX_FUNC_CHOICE)) == -1)
	{
		std::cout << "Please enter a number from " << MIN_FUNC_CHOICE << " to "
		          << MAX_FUNC_CHOICE << ". ";
		choice = getUserInt();
	}
    
    // The switch block calls the correct function
	switch (choice)
	{
		case 1: output = reverse(input);
			    break;
		case 2: output = reverseRecurs(input);
				break;
		case 3: if (isPalindrome(input))
                {
                    output = "The string entered is a palindrome.";
                }
                else
                {
                    output = "The string entered is not a palindrome.";
                }
				break;
		case 4: if (isPalindromeRec(input))
                {
                    output = "The string entered is a palindrome.";
                }
                else
                {
                    output = "The string entered is not a palindrome.";
                }
				break;
	}
    
    return (output);
}

/*   *   *   *   *   *   *
 * 
 * Function: getUserInt
 * 
 *    Entry: None
 *
 *     Exit: Retrieves the numbers input by the user and validates that the 
 *           numbers are positive integers using isPositiveInt
 *
 *  Purpose: Retrieves the numbers entered by the user
 *
 *
 *   *   *   *   *   *   */
int getUserInt()
{
    // Initialize string object and variables
    std::string userInput;
    int         validatedInt;
    bool        inputIsPositiveInt = false;
    
    // Loop to accept input and determine if input is a positive integer
    while(!inputIsPositiveInt)
    {
        // Retrieve the string entered
        std::cin >> userInput;
        
        // Pass the string to isPositiveInt function for validation
        inputIsPositiveInt = isPositiveInt(userInput);
        
        // Ask for new input if the previous entry is invalid
        if (!inputIsPositiveInt)
        {
            std::cout << "\nPlease enter a positive integer. ";
        }
    }
    
    // Convert the string entered into an integer
    validatedInt = std::stoi(userInput);
    
    return (validatedInt);
}

/*   *   *   *   *   *   *
 * 
 * Function: isPositiveInt
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Validates the string input is a positive integer
 *
 *  Purpose: Validates the string input is a positive integer
 *
 *
 *   *   *   *   *   *   */
bool isPositiveInt(std::string input)
{
    // Initialize boolean return variable
    bool isValid = true;
    
    // This loop checks each character in the string
    for (int i = 0; i < input.size(); i++)
    {
        // determine if the character entered is a number
        if (input.at(i) < '0' || input.at(i) > '9')
        {
            isValid = false;
            i = input.size();
        }  
    }

    return (isValid);
}

/*   *   *   *   *   *   *
 * 
 * Function: checkValidInput
 * 
 *    Entry: The parameters are three int values; The first is a value to be
 *           checked whether it is within the range of the other two values
 *
 *     Exit: Determines whether the number to be checked, the first parameter,
 *           is within the range of the other two parameters; The function 
 *           returns the checked int if it is within range and -1 otherwise
 *
 *  Purpose: Validate that a number is within range      
 *
 *
 *   *   *   *   *   *   */
int checkValidInput(int check, int low, int high)
{
    // Check if number is within the range and return the appropriate value
    if (check >= low && check <= high)
    {
        return check;
    }
    else
    {
        return (-1);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: procAgain
 * 
 *    Entry: The parameter is the boolean variable indicating whether the
 *           program should be repeated and it is altered by the function
 *
 *     Exit: The function asks the user whether he would like to process
 *           another string using the getYesOrNo function; The repeatProc
 *           parameter is altered by the function
 *
 *  Purpose: Asks the user whether he would like to repeat the calculation         
 *
 *
 *   *   *   *   *   *   */
void procAgain(bool &repeatProc)
{
	// Ask the user if he would like to perform another process
	std::cout << "Would you like to perform another process? (Y or N) ";
	
	// Get the user's answer and assign true or false to repeatProc
	repeatProc = getYesOrNo();
	std::cout << "\n\n";
}

/*   *   *   *   *   *   *
 * 
 * Function: getYesOrNo
 * 
 *    Entry: None
 *
 *     Exit: Retrieves the input from the user and uses isYOrN to determine
 *           whether the user entered a Y for Yes or an N for No; Returns true
 *           if the user entered Y for Yes and false if the user entered N for
 *           No
 *
 *  Purpose: Ask the user for a Yes or No answer      
 *
 *
 *   *   *   *   *   *   */
bool getYesOrNo()
{
	// Initialize string object and variables
    std::string userInput;
    bool        userEnteredYes;
    bool        inputIsYOrN = false;
	
	// Loop to accept input and determine if input is Y or N
	while (!inputIsYOrN)
	{
		// Retrieve the string entered
        std::cin >> userInput;
		
		// Pass the string to isYOrN function for validation
		inputIsYOrN = isYOrN(userInput);
		
		// Ask for new input if the previous entry is invalid
        if (!inputIsYOrN)
        {
            std::cout << "\nPlease enter Y or N. ";
        }
	}
	
	// Set userEnteredYes as determined by the string entered
	if (userInput == "Y" || userInput == "y")
	{
		userEnteredYes = true;
	}
	else
	{
		userEnteredYes = false;
	}
	
	return (userEnteredYes);
}

/*   *   *   *   *   *   *
 * 
 * Function: isYOrN
 * 
 *    Entry: The parameter is the string input by the user
 *
 *     Exit: The function validates whether the string input is a Y or N
 *
 *  Purpose: Validates whether the string input is a Y or N        
 *
 *
 *   *   *   *   *   *   */
bool isYOrN(std::string input)
{
	// Initialize boolean return variable
    bool isValid = false;
	
	// Determine if the user entered a Y or N
	if ((input == "Y" || input == "y") || (input == "N" || input == "n"))
	{
		isValid = true;
	}
	
	return (isValid);
}