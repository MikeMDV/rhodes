/*
 *             Author: Michael Marven
 *       Date Created: 12/04/13
 * Last Date Modified: 12/06/13
 *          File Name: LargestProduct.cpp
 *           Overview: The program allows the user to enter dimensions
 *                     for a table of random numbers, then calculates the
 *                     largest product of 4 numbers arranged according to 
 *                     Tetris shapes. The table is printed with the shape
 *                     highlighted and the details of the product and position
 *                     also printed. The shapes are below in the 0 degrees
 *                     orientation:
 *                     I ****  J ***   L ***   O **    S  **   T ***   Z **
 *                                 *     *       **      **       *       **
 *                     Starting position is the upper leftmost block of the
 *                     shape and calculation proceeds to right first, moving
 *                     down one row and to the left, then proceeding right, as
 *                     in a typewriter. Examples below:
 *                     o* - *o - ** - **    o* -  *o -  ** -  **
 *                     **   **   o*   *o   **    **    o*    *o
 *                     ** READ ME ** The program contains C++11 functions and
 *                                   must be compiled with the -std=c++0x flag
 *              Input: The user inputs the table dimensions as command line args
 *             Output: The table and greatest product are printed to the console
 *                     window
 *
 *
 */
 
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ctime>


const int MIN_ROWS_COLS    = 4; // Minimum number of rows or columns
const int RAND_UPPER_LIMIT = 99; // Add 1 to this value for inclusive range
const int MIN_COMM_ARGS    = 5; // Minimum number of command line arguments

// Adjust the for loop end values by these values to keep shapes in table
const int FOUR_BLOCK_ADJ   = 3; // Start position must not shift past total - 3
const int THREE_BLOCK_ADJ  = 2; // Start position must not shift past total – 2
const int TWO_BLOCK_ADJ    = 1; // Start position must not shift past total - 1


/*
 * Structure for the product of 4 integers in a table arranged in the shape of
 * Tetris pieces; The integers are in a larger table and the struct stores the
 * product of the integers, the position of each block, the shape, and the
 * orientation 
 */
struct Tetromino
{
    int         product;
    int         rowPosition1;
    int         colPosition1;
    int         rowPosition2;
    int         colPosition2;
    int         rowPosition3;
    int         colPosition3;
    int         rowPosition4;
    int         colPosition4;
    int         orientation;
    std::string shape;
};

/*
 * The validateCommArgsQuant function accepts an int as a parameter for the
 * quantity of command line arguments and validates that the value is correct
 */
void validateCommArgsQuant(int args);
                      
/*
 * The validateCommArgs function accepts the command line arguments as strings
 * and validates them
 */
void validateCommArgs(std::string arg1, std::string arg2,
                      std::string arg3, std::string arg4);

/*
 * The isPositiveInt function validates that a string input can be safely
 * converted to an integer and returns true if the input is a positive integer
 */
bool isPositiveInt(std::string input);

/*
 * The areValidRowColNums function validates that the numbers entered as
 * command line arguments are equal to or greater than the minimum value
 */
bool areValidRowColNums(int minimum, std::string arg2, std::string arg4);

/*
 * The printCommError function prints an error message explaining the correct
 * format for command line argument entry and exits the program
 */
void printCommError();

/*
 * The convertRowsArg function converts the correct rows command line argument
 * to an int
 */
int convertRowsArg(std::string arg1, std::string arg2,
                      std::string arg3, std::string arg4);

/*
 * The convertColsArg function converts the correct rows command line argument
 * to an int
 */
int convertColsArg(std::string arg1, std::string arg2,
                      std::string arg3, std::string arg4);
                      
/*
 * The createArray function creates a 2D array of random integer values in the
 * range from 0 to the max parameter
 */
int **createArray(int rows, int cols, int rangeMax);

/*
 * The findGreatestProduct function searches the generated table for the
 * greatest product of 4 numbers arranged in any of the Tetris shapes and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino findGreatestProduct(int rows, int cols, int **table);

/*
 * The greaterOfTwo function compares two Tetromino products and returns
 * the Tetromino with the greatest product or the first if they are equal
 */
Tetromino greaterOfTwo(Tetromino first, Tetromino second);

/*
 * The greatestO function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape O and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestO(int rows, int cols, int **table);

/*
 * The greatestI function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape I and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestI(int rows, int cols, int **table);

/*
 * The calcI0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape I in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcI0Degrees(int rows, int cols, int **table);

/*
 * The calcI90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape I in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcI90Degrees(int rows, int cols, int **table);

/*
 * The greatestS function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape S and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestS(int rows, int cols, int **table);

/*
 * The calcS0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape S in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcS0Degrees(int rows, int cols, int **table);

/*
 * The calcS90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape S in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcS90Degrees(int rows, int cols, int **table);

/*
 * The greatestZ function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape Z and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestZ(int rows, int cols, int **table);

/*
 * The calcZ0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape Z in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcZ0Degrees(int rows, int cols, int **table);

/*
 * The calcZ90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape Z in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcZ90Degrees(int rows, int cols, int **table);

/*
 * The greatestJ function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape J and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestJ(int rows, int cols, int **table);

/*
 * The calcJ0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape J in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcJ0Degrees(int rows, int cols, int **table);

/*
 * The calcJ90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape J in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcJ90Degrees(int rows, int cols, int **table);

/*
 * The calcJ180Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape J in the 180 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcJ180Degrees(int rows, int cols, int **table);

/*
 * The calcJ270Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape J in the 270 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcJ270Degrees(int rows, int cols, int **table);

/*
 * The greatestL function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape L and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestL(int rows, int cols, int **table);

/*
 * The calcL0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape L in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcL0Degrees(int rows, int cols, int **table);

/*
 * The calcL90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape L in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcL90Degrees(int rows, int cols, int **table);

/*
 * The calcL180Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape L in the 180 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcL180Degrees(int rows, int cols, int **table);

/*
 * The calcL270Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape L in the 270 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcL270Degrees(int rows, int cols, int **table);

/*
 * The greatestT function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape T and
 * returns a Tetromino struct containing the details of the shape
 */
Tetromino greatestT(int rows, int cols, int **table);

/*
 * The calcT0Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape T in the 0 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcT0Degrees(int rows, int cols, int **table);

/*
 * The calcT90Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape T in the 90 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcT90Degrees(int rows, int cols, int **table);

/*
 * The calcT180Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape T in the 180 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcT180Degrees(int rows, int cols, int **table);

/*
 * The calcT270Degrees function searches the generated table for the
 * greatest product of 4 numbers arranged in the Tetris shape T in the 270 deg
 * orientation and returns a Tetromino struct containing the details of the
 * shape
 */
Tetromino calcT270Degrees(int rows, int cols, int **table);

/*
 * The printTetDetails function prints the Tetromino product, position 
 * (rows, cols), shape, and orientation in degrees to the console window.
 */
void printTetDetails(Tetromino tet);

/*
 * The printTable function prints the 2D array with the Tetromino with the
 * greatest product highlighted
 */
void printTable(int **table, int rows, int cols, Tetromino tet);

int main(int argc, char *argv[])
{
	// Validate the command arguments
    validateCommArgsQuant(argc);
    validateCommArgs(argv[1], argv[2], argv[3], argv[4]);
    
    // Declare variables for rows and columns
    int rows = convertRowsArg(argv[1], argv[2], argv[3], argv[4]);
    int cols = convertColsArg(argv[1], argv[2], argv[3], argv[4]);
    
    // Declare a variable and generate the array
    int **randTable = createArray(rows, cols, RAND_UPPER_LIMIT);
    
    // Declare a Tetromino struct
    Tetromino maxProdTet;
    
    // Assign it the value of the Tetrmino with the greatest product
    maxProdTet = findGreatestProduct(rows, cols, randTable);
    
    // Print the table with the Tetrmino with max product highlighted
    printTable(randTable, rows, cols, maxProdTet);
    
    // Print the details of the Tetromino with the greatest product
    printTetDetails(maxProdTet);
    
    // Delete the dynamically created array
    for (int i = 0; i < rows; i++)
    {
        delete[] randTable[i];
    }
    delete[] randTable;
	
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: validateCommArgsQuant
 * 
 *    Entry: Input parameter is an int for the command line argument quantity
 *
 *     Exit: Validates the command line argument quantity
 *
 *  Purpose: Validate the command line argument quantity
 *
 *
 *   *   *   *   *   *   */
void validateCommArgsQuant(int args)
{
    // Validate that the quantity of command line arguments is correct
    if (args < MIN_COMM_ARGS)
    {
        printCommError();
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: validateCommArgs
 * 
 *    Entry: Input parameters are 4 strings for the command line arguments 
 *
 *     Exit: Validates the command line arguments
 *
 *  Purpose: Validate the command line arguments
 *
 *
 *   *   *   *   *   *   */
void validateCommArgs(std::string arg1, std::string arg2, 
                      std::string arg3, std::string arg4)
{
    // Check if command line arguments are valid
    if ((arg1 != "-rows" && arg1 != "-cols") || 
        (arg3 != "-rows" && arg3 != "-cols"))
    {
        printCommError();
    }
    else if (!isPositiveInt(arg2) || !isPositiveInt(arg4))
    {
        printCommError();
    }
    else if(!areValidRowColNums(MIN_ROWS_COLS, arg2, arg4))
    {
        printCommError();
    }
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
 * Function: areValidRowColNums
 * 
 *    Entry: The parameters are two strings and an int; The strings are the
 *           command line number entries to be checked and the int is the
 *           minimum value
 *
 *     Exit: Determines whether the numbers entered are valid values
 *
 *  Purpose: Validate that a number is within range      
 *
 *
 *   *   *   *   *   *   */
bool areValidRowColNums(int minimum, std::string arg2, std::string arg4)
{
    // Initialize boolean return variable and ints for string conversion
    bool areValid = true;
    int  arg2Int;
    int  arg4Int;
    
    // Convert the strings to ints
    arg2Int = std::stoi(arg2);
    arg4Int = std::stoi(arg4);
    
    // Check if the numbers entered are equal or greater than minimum value
    if (arg2Int >= minimum && arg4Int >= minimum)
    {
        return (areValid);
    }
    else
    {
        areValid = false;
        return (areValid);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: printCommError
 * 
 *    Entry: None
 *
 *     Exit: Prints an error message displaying the correct command line arg
 *           format and exits the program
 *
 *  Purpose: Print an error message and exit the program      
 *
 *
 *   *   *   *   *   *   */
void printCommError()
{
    // Print error message explaining correct command line format
    std::cout << "\nERROR: Command Line Argument Format Invalid\n\n"
              << "[-rows integer] [-cols integer]\n\n"
              << "Description: -rows and -cols flags must be provided and\n"
              << "followed by integers with values greater or equal to 4.\n\n"
              << "Example: ./prod -rows 6 -cols 6\n\n";
    
    std::exit(1);
}

/*   *   *   *   *   *   *
 * 
 * Function: convertRowsArg
 * 
 *    Entry: 4 strings for the command line arguments
 *
 *     Exit: Converts the correct rows command line argument to an int
 *
 *  Purpose: Converts the correct rows command line argument to an int     
 *
 *
 *   *   *   *   *   *   */
int convertRowsArg(std::string arg1, std::string arg2,
                      std::string arg3, std::string arg4)
{
    // Declare an int for output
    int output;
    
    // Covert the correct rows command line argument to an int
    if (arg1 == "-rows")
    {
        output = std::stoi(arg2);
    }
    else if (arg3 == "-rows")
    {
        output = std::stoi(arg4);
    }
    
    return (output);
}

/*   *   *   *   *   *   *
 * 
 * Function: convertColsArg
 * 
 *    Entry: 4 strings for the command line arguments
 *
 *     Exit: Converts the correct cols command line argument to an int
 *
 *  Purpose: Converts the correct cols command line argument to an int     
 *
 *
 *   *   *   *   *   *   */
int convertColsArg(std::string arg1, std::string arg2,
                      std::string arg3, std::string arg4)
{
    // Declare an int for output
    int output;
    
    // Covert the correct rows command line argument to an int
    if (arg1 == "-cols")
    {
        output = std::stoi(arg2);
    }
    else if (arg3 == "-cols")
    {
        output = std::stoi(arg4);
    }
    
    return (output);
}

/*   *   *   *   *   *   *
 * 
 * Function: createArray
 * 
 *    Entry: 3 ints for the rows, columns, and range maximum
 *
 *     Exit: Creates a 2D array of random integer values in the range specified
 *
 *  Purpose: Creates a 2D array of random integer values    
 *
 *
 *   *   *   *   *   *   */
int **createArray(int rows, int cols, int rangeMax)
{
    // Declare a pointer variable for the 2D array of ints
    int **table = new int*[rows];
    
    // Fill the initial array with arrays
    for (int i = 0; i < rows; i++)
    {
        table[i] = new int[cols];
    }
    
    // Use current time as a seed for the random number generator
    std::srand(std::time(0));
    
    // Fill the array with random integers
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Generate a random number from 0 to upper limit inclusive
            table[i][j] = rand() % (rangeMax + 1);
        }
    }
    
    return (table);
}

/*   *   *   *   *   *   *
 * 
 * Function: findGreatestProduct
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in any of the Tetris shapes and returns a Tetromino struct
 *           containing the details of the shape
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           a Tetris shape 
 *
 *
 *   *   *   *   *   *   */
Tetromino findGreatestProduct(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ""};
    Tetromino greatestProduct = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ""};
    
    // Search the array for the greatest product arranged in all the shapes
    greatestProduct = greatestO(rows, cols, table);
    
    temp = greatestI(rows, cols, table);
    greatestProduct = greaterOfTwo(temp, greatestProduct);
    
    temp = greatestS(rows, cols, table);
    greatestProduct = greaterOfTwo(temp, greatestProduct);
    
    temp = greatestZ(rows, cols, table);
    greatestProduct = greaterOfTwo(temp, greatestProduct);
    
    temp = greatestJ(rows, cols, table);
    greatestProduct = greaterOfTwo(temp, greatestProduct);
    
    temp = greatestL(rows, cols, table);
    greatestProduct = greaterOfTwo(temp, greatestProduct);
    
    temp = greatestT(rows, cols, table);
    
    return (greaterOfTwo(temp, greatestProduct));
}

/*   *   *   *   *   *   *
 * 
 * Function: greaterOfTwo
 * 
 *    Entry: 2 Tetromino structs as parameters
 *
 *     Exit: Compares the products of the two Tetromino structs as parameters
 *           and returns the Tetromino with the greatest product or returns the
 *           first if they are equal
 *
 *  Purpose: Compare the products of two Tetromino structs and return the
 *           greater Tetromino or return the first if they are equal
 *
 *
 *   *   *   *   *   *   */
Tetromino greaterOfTwo(Tetromino first, Tetromino second)
{
    // Compare the two products and return the greater
    if (first.product >= second.product)
    {
        return (first);
    }
    else
    {
        return (second);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestO
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris O shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris O shape - **
 *                                **
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestO(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "O"};
    Tetromino greatestO = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "O"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j + 1][i]
                            * table[j + 1][i + 1]);
            if (temp.product > greatestO.product)
            {
                greatestO.product = temp.product;
                greatestO.rowPosition1 = j;
                greatestO.colPosition1 = i;
                greatestO.rowPosition2 = j;
                greatestO.colPosition2 = i + 1;
                greatestO.rowPosition3 = j + 1;
                greatestO.colPosition3 = i;
                greatestO.rowPosition4 = j + 1;
                greatestO.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestO);  
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestI
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris I shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris I shape in 0 and 90 degree orientations- ****, *
 *                                                                     *
 *                                                                     *
 *                                                                     *
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestI(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "I"};
    Tetromino greatestI = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "I"};
    
    // Search the array for the greatest product of the I shapes in 0 and 90 deg
    temp = calcI0Degrees(rows, cols, table);
    greatestI = greaterOfTwo(temp, greatestI);
    
    temp = calcI90Degrees(rows, cols, table);
    greatestI = greaterOfTwo(temp, greatestI);
    
    return (greatestI);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcI0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris I shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris I shape in 0 degree orientation- ****
 *
 *
 *   *   *   *   *   *   */
Tetromino calcI0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "I"};
    Tetromino greatestI0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "I"};

    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - FOUR_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < rows; j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j][i + 2]
                            * table[j][i + 3]);                            
            if (temp.product > greatestI0Deg.product)
            {
                greatestI0Deg.product = temp.product;
                greatestI0Deg.rowPosition1 = j;
                greatestI0Deg.colPosition1 = i;
                greatestI0Deg.rowPosition2 = j;
                greatestI0Deg.colPosition2 = i + 1;
                greatestI0Deg.rowPosition3 = j;
                greatestI0Deg.colPosition3 = i + 2;
                greatestI0Deg.rowPosition4 = j;
                greatestI0Deg.colPosition4 = i + 3;
            }
        }
    }
    
    return (greatestI0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcI90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris I shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris I shape in 90 degree orientation- *
 *                                                        *
 *                                                        *
 *                                                        *
 *
 *
 *   *   *   *   *   *   */
Tetromino calcI90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "I"};
    Tetromino greatestI90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "I"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < (rows - FOUR_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 2][i]
                            * table[j + 3][i]);
            if (temp.product > greatestI90Deg.product)
            {
                greatestI90Deg.product = temp.product;
                greatestI90Deg.rowPosition1 = j;
                greatestI90Deg.colPosition1 = i;
                greatestI90Deg.rowPosition2 = j + 1;
                greatestI90Deg.colPosition2 = i;
                greatestI90Deg.rowPosition3 = j + 2;
                greatestI90Deg.colPosition3 = i;
                greatestI90Deg.rowPosition4 = j + 3;
                greatestI90Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestI90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestS
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris S shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris S shape in 0 and 90 degree orientations- **, *
 *                                                              **   **
 *                                                                    *
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestS(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "S"};
    Tetromino greatestS = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "S"};
    
    // Search the array for the greatest product of the S shapes in 0 and 90 deg
    temp = calcS0Degrees(rows, cols, table);
    greatestS = greaterOfTwo(temp, greatestS);
    
    temp = calcS90Degrees(rows, cols, table);
    greatestS = greaterOfTwo(temp, greatestS);
    
    return (greatestS);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcS0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris S shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris S shape in 0 degree orientation-  **
 *                                                       **
 *
 *   *   *   *   *   *   */
Tetromino calcS0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "S"};
    Tetromino greatestS0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "S"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 1; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j + 1][i - 1]
                            * table[j + 1][i]);
            if (temp.product > greatestS0Deg.product)
            {
                greatestS0Deg.product = temp.product;
                greatestS0Deg.rowPosition1 = j;
                greatestS0Deg.colPosition1 = i;
                greatestS0Deg.rowPosition2 = j;
                greatestS0Deg.colPosition2 = i + 1;
                greatestS0Deg.rowPosition3 = j + 1;
                greatestS0Deg.colPosition3 = i - 1;
                greatestS0Deg.rowPosition4 = j + 1;
                greatestS0Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestS0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcS90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris S shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris S shape in 90 degree orientation-  *
 *                                                         **
 *                                                          *
 *
 *   *   *   *   *   *   */
Tetromino calcS90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "S"};
    Tetromino greatestS90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "S"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 1][i + 1]
                            * table[j + 2][i + 1]);
            if (temp.product > greatestS90Deg.product)
            {
                greatestS90Deg.product = temp.product;
                greatestS90Deg.rowPosition1 = j;
                greatestS90Deg.colPosition1 = i;
                greatestS90Deg.rowPosition2 = j + 1;
                greatestS90Deg.colPosition2 = i;
                greatestS90Deg.rowPosition3 = j + 1;
                greatestS90Deg.colPosition3 = i + 1;
                greatestS90Deg.rowPosition4 = j + 2;
                greatestS90Deg.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestS90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestZ
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris Z shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris Z shape in 0 and 90 degree orientations- ** ,  *
 *                                                                **  **
 *                                                                    *
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestZ(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Z"};
    Tetromino greatestZ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Z"};
    
    // Search the array for the greatest product of the Z shapes in 0 and 90 deg
    temp = calcZ0Degrees(rows, cols, table);
    greatestZ = greaterOfTwo(temp, greatestZ);
    
    temp = calcZ90Degrees(rows, cols, table);
    greatestZ = greaterOfTwo(temp, greatestZ);
    
    return (greatestZ);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcZ0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris Z shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris Z shape in 0 degree orientation- **
 *                                                        **
 *
 *   *   *   *   *   *   */
Tetromino calcZ0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Z"};
    Tetromino greatestZ0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Z"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - THREE_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j + 1][i + 1]
                            * table[j + 1][i + 2]);
            if (temp.product > greatestZ0Deg.product)
            {
                greatestZ0Deg.product = temp.product;
                greatestZ0Deg.rowPosition1 = j;
                greatestZ0Deg.colPosition1 = i;
                greatestZ0Deg.rowPosition2 = j;
                greatestZ0Deg.colPosition2 = i + 1;
                greatestZ0Deg.rowPosition3 = j + 1;
                greatestZ0Deg.colPosition3 = i + 1;
                greatestZ0Deg.rowPosition4 = j + 1;
                greatestZ0Deg.colPosition4 = i + 2;
            }
        }
    }
    
    return (greatestZ0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcZ90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris Z shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris Z shape in 90 degree orientation-   *
 *                                                         **
 *                                                         *
 *
 *   *   *   *   *   *   */
Tetromino calcZ90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "Z"};
    Tetromino greatestZ90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "Z"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 1; i < cols; i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i - 1] * table[j + 1][i]
                            * table[j + 2][i - 1]);
            if (temp.product > greatestZ90Deg.product)
            {
                greatestZ90Deg.product = temp.product;
                greatestZ90Deg.rowPosition1 = j;
                greatestZ90Deg.colPosition1 = i;
                greatestZ90Deg.rowPosition2 = j + 1;
                greatestZ90Deg.colPosition2 = i - 1;
                greatestZ90Deg.rowPosition3 = j + 1;
                greatestZ90Deg.colPosition3 = i;
                greatestZ90Deg.rowPosition4 = j + 2;
                greatestZ90Deg.colPosition4 = i - 1;
            }
        }
    }
    
    return (greatestZ90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestJ
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris J shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris J shape in 0, 90, 180, and 270 degree orientations -
 *            ***,  *, *  , **
 *              *   *  ***  *
 *                 **       *
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestJ(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "J"};
    Tetromino greatestJ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "J"};
    
    // Search the array for the greatest product of the J shapes in 0 - 270 deg
    temp = calcJ0Degrees(rows, cols, table);
    greatestJ = greaterOfTwo(temp, greatestJ);
    
    temp = calcJ90Degrees(rows, cols, table);
    greatestJ = greaterOfTwo(temp, greatestJ);
    
    temp = calcJ180Degrees(rows, cols, table);
    greatestJ = greaterOfTwo(temp, greatestJ);
    
    temp = calcJ270Degrees(rows, cols, table);
    greatestJ = greaterOfTwo(temp, greatestJ);
    
    return (greatestJ);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcJ0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris J shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris J shape in 0 degree orientation- ***
 *                                                         *
 *
 *   *   *   *   *   *   */
Tetromino calcJ0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "J"};
    Tetromino greatestJ0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "J"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - THREE_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j][i + 2]
                            * table[j + 1][i + 2]);
            if (temp.product > greatestJ0Deg.product)
            {
                greatestJ0Deg.product = temp.product;
                greatestJ0Deg.rowPosition1 = j;
                greatestJ0Deg.colPosition1 = i;
                greatestJ0Deg.rowPosition2 = j;
                greatestJ0Deg.colPosition2 = i + 1;
                greatestJ0Deg.rowPosition3 = j;
                greatestJ0Deg.colPosition3 = i + 2;
                greatestJ0Deg.rowPosition4 = j + 1;
                greatestJ0Deg.colPosition4 = i + 2;
            }
        }
    }
    
    return (greatestJ0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcJ90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris J shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris J shape in 90 degree orientation-   *
 *                                                          *
 *                                                         **
 *
 *   *   *   *   *   *   */
Tetromino calcJ90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "J"};
    Tetromino greatestJ90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "J"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 1; i < cols; i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 2][i - 1]
                            * table[j + 2][i]);
            if (temp.product > greatestJ90Deg.product)
            {
                greatestJ90Deg.product = temp.product;
                greatestJ90Deg.rowPosition1 = j;
                greatestJ90Deg.colPosition1 = i;
                greatestJ90Deg.rowPosition2 = j + 1;
                greatestJ90Deg.colPosition2 = i;
                greatestJ90Deg.rowPosition3 = j + 2;
                greatestJ90Deg.colPosition3 = i - 1;
                greatestJ90Deg.rowPosition4 = j + 2;
                greatestJ90Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestJ90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcJ180Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris J shape in the 180 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris J shape in 180 degree orientation- *
 *                                                         ***
 *
 *   *   *   *   *   *   */
Tetromino calcJ180Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "J"};
    Tetromino greatestJ180Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "J"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - THREE_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 1][i + 1]
                            * table[j + 1][i + 2]);
            if (temp.product > greatestJ180Deg.product)
            {
                greatestJ180Deg.product = temp.product;
                greatestJ180Deg.rowPosition1 = j;
                greatestJ180Deg.colPosition1 = i;
                greatestJ180Deg.rowPosition2 = j + 1;
                greatestJ180Deg.colPosition2 = i;
                greatestJ180Deg.rowPosition3 = j + 1;
                greatestJ180Deg.colPosition3 = i + 1;
                greatestJ180Deg.rowPosition4 = j + 1;
                greatestJ180Deg.colPosition4 = i + 2;
            }
        }
    }
    
    return (greatestJ180Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcJ270Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris J shape in the 270 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris J shape in 270 degree orientation-  **
 *                                                          *
 *                                                          *
 *
 *   *   *   *   *   *   */
Tetromino calcJ270Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "J"};
    Tetromino greatestJ270Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "J"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j + 1][i]
                            * table[j + 2][i]);
            if (temp.product > greatestJ270Deg.product)
            {
                greatestJ270Deg.product = temp.product;
                greatestJ270Deg.rowPosition1 = j;
                greatestJ270Deg.colPosition1 = i;
                greatestJ270Deg.rowPosition2 = j;
                greatestJ270Deg.colPosition2 = i + 1;
                greatestJ270Deg.rowPosition3 = j + 1;
                greatestJ270Deg.colPosition3 = i;
                greatestJ270Deg.rowPosition4 = j + 2;
                greatestJ270Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestJ270Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: greatestL
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris L shape and returns a Tetromino struct
 *           containing the details of the shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris L shape in 0, 90, 180, and 270 degree orientations -
 *            ***, **,   *, *
 *            *     *  ***  *
 *                  *       **
 *
 *
 *   *   *   *   *   *   */
Tetromino greatestL(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "L"};
    Tetromino greatestL = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "L"};
    
    // Search the array for the greatest product of the L shapes in 0 - 270 deg
    temp = calcL0Degrees(rows, cols, table);
    greatestL = greaterOfTwo(temp, greatestL);
    
    temp = calcL90Degrees(rows, cols, table);
    greatestL = greaterOfTwo(temp, greatestL);
    
    temp = calcL180Degrees(rows, cols, table);
    greatestL = greaterOfTwo(temp, greatestL);
    
    temp = calcL270Degrees(rows, cols, table);
    greatestL = greaterOfTwo(temp, greatestL);
    
    return (greatestL);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcL0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris L shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris L shape in 0 degree orientation- ***
 *                                                       *
 *
 *   *   *   *   *   *   */
Tetromino calcL0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "L"};
    Tetromino greatestL0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "L"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - THREE_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j][i + 2]
                            * table[j + 1][i]);
            if (temp.product > greatestL0Deg.product)
            {
                greatestL0Deg.product = temp.product;
                greatestL0Deg.rowPosition1 = j;
                greatestL0Deg.colPosition1 = i;
                greatestL0Deg.rowPosition2 = j;
                greatestL0Deg.colPosition2 = i + 1;
                greatestL0Deg.rowPosition3 = j;
                greatestL0Deg.colPosition3 = i + 2;
                greatestL0Deg.rowPosition4 = j + 1;
                greatestL0Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestL0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcL90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris L shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris L shape in 90 degree orientation-  **
 *                                                          *
 *                                                          *
 *
 *   *   *   *   *   *   */
Tetromino calcL90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "L"};
    Tetromino greatestL90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "L"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j + 1][i + 1]
                            * table[j + 2][i + 1]);
            if (temp.product > greatestL90Deg.product)
            {
                greatestL90Deg.product = temp.product;
                greatestL90Deg.rowPosition1 = j;
                greatestL90Deg.colPosition1 = i;
                greatestL90Deg.rowPosition2 = j;
                greatestL90Deg.colPosition2 = i + 1;
                greatestL90Deg.rowPosition3 = j + 1;
                greatestL90Deg.colPosition3 = i + 1;
                greatestL90Deg.rowPosition4 = j + 2;
                greatestL90Deg.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestL90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcL180Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris L shape in the 180 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris L shape in 180 degree orientation-   *
 *                                                         ***
 *
 *   *   *   *   *   *   */
Tetromino calcL180Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "L"};
    Tetromino greatestL180Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "L"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 2; i < cols; i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i - 2] 
                            * table[j + 1][i - 1] * table[j + 1][i]);
            if (temp.product > greatestL180Deg.product)
            {
                greatestL180Deg.product = temp.product;
                greatestL180Deg.rowPosition1 = j;
                greatestL180Deg.colPosition1 = i;
                greatestL180Deg.rowPosition2 = j + 1;
                greatestL180Deg.colPosition2 = i - 2;
                greatestL180Deg.rowPosition3 = j + 1;
                greatestL180Deg.colPosition3 = i - 1;
                greatestL180Deg.rowPosition4 = j + 1;
                greatestL180Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestL180Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcL270Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris L shape in the 270 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris L shape in 270 degree orientation-  *
 *                                                          *
 *                                                          **
 *
 *   *   *   *   *   *   */
Tetromino calcL270Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "L"};
    Tetromino greatestL270Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "L"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 2][i]
                            * table[j + 2][i + 1]);
            if (temp.product > greatestL270Deg.product)
            {
                greatestL270Deg.product = temp.product;
                greatestL270Deg.rowPosition1 = j;
                greatestL270Deg.colPosition1 = i;
                greatestL270Deg.rowPosition2 = j + 1;
                greatestL270Deg.colPosition2 = i;
                greatestL270Deg.rowPosition3 = j + 2;
                greatestL270Deg.colPosition3 = i;
                greatestL270Deg.rowPosition4 = j + 2;
                greatestL270Deg.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestL270Deg);  
}

Tetromino greatestT(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "T"};
    Tetromino greatestT = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "T"};
    
    // Search the array for the greatest product of the T shapes in 0 - 270 deg
    temp = calcT0Degrees(rows, cols, table);
    greatestT = greaterOfTwo(temp, greatestT);
    
    temp = calcT90Degrees(rows, cols, table);
    greatestT = greaterOfTwo(temp, greatestT);
    
    temp = calcT180Degrees(rows, cols, table);
    greatestT = greaterOfTwo(temp, greatestT);
    
    temp = calcT270Degrees(rows, cols, table);
    greatestT= greaterOfTwo(temp, greatestT);
    
    return (greatestT);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcT0Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris T shape in the 0 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris T shape in 0 degree orientation- ***
 *                                                        *
 *
 *   *   *   *   *   *   */
Tetromino calcT0Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp          = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "T"};
    Tetromino greatestT0Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "T"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - THREE_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j][i + 1] * table[j][i + 2]
                            * table[j + 1][i + 1]);
            if (temp.product > greatestT0Deg.product)
            {
                greatestT0Deg.product = temp.product;
                greatestT0Deg.rowPosition1 = j;
                greatestT0Deg.colPosition1 = i;
                greatestT0Deg.rowPosition2 = j;
                greatestT0Deg.colPosition2 = i + 1;
                greatestT0Deg.rowPosition3 = j;
                greatestT0Deg.colPosition3 = i + 2;
                greatestT0Deg.rowPosition4 = j + 1;
                greatestT0Deg.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestT0Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcT90Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris T shape in the 90 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris T shape in 90 degree orientation-   *
 *                                                         **
 *                                                          *
 *
 *   *   *   *   *   *   */
Tetromino calcT90Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "T"};
    Tetromino greatestT90Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 90, "T"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 1; i < cols; i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i - 1] * table[j + 1][i]
                            * table[j + 2][i]);
            if (temp.product > greatestT90Deg.product)
            {
                greatestT90Deg.product = temp.product;
                greatestT90Deg.rowPosition1 = j;
                greatestT90Deg.colPosition1 = i;
                greatestT90Deg.rowPosition2 = j + 1;
                greatestT90Deg.colPosition2 = i - 1;
                greatestT90Deg.rowPosition3 = j + 1;
                greatestT90Deg.colPosition3 = i;
                greatestT90Deg.rowPosition4 = j + 2;
                greatestT90Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestT90Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcT180Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris T shape in the 180 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris T shape in 180 degree orientation-  *
 *                                                         ***
 *
 *   *   *   *   *   *   */
Tetromino calcT180Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "T"};
    Tetromino greatestT180Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 180, "T"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 1; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - TWO_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i - 1] * table[j + 1][i]
                            * table[j + 1][i + 1]);
            if (temp.product > greatestT180Deg.product)
            {
                greatestT180Deg.product = temp.product;
                greatestT180Deg.rowPosition1 = j;
                greatestT180Deg.colPosition1 = i;
                greatestT180Deg.rowPosition2 = j + 1;
                greatestT180Deg.colPosition2 = i - 1;
                greatestT180Deg.rowPosition3 = j + 1;
                greatestT180Deg.colPosition3 = i;
                greatestT180Deg.rowPosition4 = j + 1;
                greatestT180Deg.colPosition4 = i + 1;
            }
        }
    }
    
    return (greatestT180Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: calcT270Degrees
 * 
 *    Entry: 3 ints for the rows, columns, and table array
 *
 *     Exit: Searches the 2D array for the greatest product of 4 numbers
 *           arranged in the Tetris T shape in the 270 degree orientation and
 *           returns a Tetromino struct containing the details of the
 *           shape                
 *
 *  Purpose: Search the array for the greatest product of 4 numbers arranged in
 *           the Tetris T shape in 270 degree orientation-  *
 *                                                          **
 *                                                          *
 *
 *   *   *   *   *   *   */
Tetromino calcT270Degrees(int rows, int cols, int **table)
{
    // Initialize two Tetromino structs, one for temp values and one for return
    Tetromino temp            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "T"};
    Tetromino greatestT270Deg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 270, "T"};
    
    // Cycle starting position through the array and calculate product
    for (int i = 0; i < (cols - TWO_BLOCK_ADJ); i++)
    {
        for (int j = 0; j < (rows - THREE_BLOCK_ADJ); j++)
        {
            temp.product = (table[j][i] * table[j + 1][i] * table[j + 1][i + 1]
                            * table[j + 2][i]);
            if (temp.product > greatestT270Deg.product)
            {
                greatestT270Deg.product = temp.product;
                greatestT270Deg.rowPosition1 = j;
                greatestT270Deg.colPosition1 = i;
                greatestT270Deg.rowPosition2 = j + 1;
                greatestT270Deg.colPosition2 = i;
                greatestT270Deg.rowPosition3 = j + 1;
                greatestT270Deg.colPosition3 = i + 1;
                greatestT270Deg.rowPosition4 = j + 2;
                greatestT270Deg.colPosition4 = i;
            }
        }
    }
    
    return (greatestT270Deg);  
}

/*   *   *   *   *   *   *
 * 
 * Function: printTetDetails
 * 
 *    Entry: A Tetromino as a parameter
 *
 *     Exit: Prints the Tetromino product, position (rows, cols), shape, and 
 *           orientation in degrees to the console window.               
 *
 *  Purpose: Print the Tetromino dteails to the console window
 *
 *
 *   *   *   *   *   *   */
void printTetDetails(Tetromino tetMax)
{
    // Print the details of the Tetromino to the console window
    std::cout << "\nMax Prod: " << tetMax.product << " Position: Row "
              << tetMax.rowPosition1 << ", Col " << tetMax.colPosition1
              << " Shape: " << tetMax.shape << ", ";
    if (tetMax.orientation == 0 || tetMax.orientation == 180)
    {
        std::cout << "horizontal - " << tetMax.orientation << " deg.\n\n";
    }
    else
    {
        std::cout << "vertical - " << tetMax.orientation << " deg.\n\n";
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: printTable
 * 
 *    Entry: A Tetromino, the int array, and rows and cols for parameters
 *
 *     Exit: Prints the int array table with the Tetromino shape highlighted 
 *           by printing an asterisk to the left of the random number
 *
 *  Purpose: Print the table with the Tetromino highlighted to the console
 *           window
 *
 *
 *   *   *   *   *   *   */
void printTable(int **table, int rows, int cols, Tetromino tet)
{
    // Print the array table and highlight the four spaces for the Tetrmino
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if ((tet.rowPosition1 == i && tet.colPosition1 == j)
                || (tet.rowPosition2 == i && tet.colPosition2 == j)
                || (tet.rowPosition3 == i && tet.colPosition3 == j)
                || (tet.rowPosition4 == i && tet.colPosition4 == j))
            {
                std::printf("*%*.2d", -4, table[i][j]);
            }
            else
            {
                std::printf(" %*.2d", -4, table[i][j]);
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}