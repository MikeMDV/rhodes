/*
 *             Author: Michael Marven
 *       Date Created: 03/20/13
 * Last Date Modified: 03/23/13
 *          File Name: DragRace.cpp
 *           Overview: This program allows a user to enter data for a car and
 *                     then choose one of 8 opponents to race. This program
 *                     requires the header file, rcar.h, and the implementation
 *                     file, rcar.cpp, which contain the RaceCar class. A 
 *                     Makefile is provided.
 *              Input: The user inputs weight and horsepower data through the
 *                     console window for a car to drag race against cars that
 *                     are input into the program through text file input. The
 *                     text file name is input as a command line argument        
 *             Output: Instructions for the race and the results of the races
 *                     are printed to the console window
 *                     
 *
 */
 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include "rcar.h"

using RCarMarven::RaceCar;

const int MIN_WEIGHT = 1000;
const int MAX_WEIGHT = 5000;
const int MIN_HP     = 34;
const int MAX_HP     = 730;


/*
 * The getPlayerCarWeight function retrieves the weight data for 
 * the player's car
 */
int getPlayerCarWeight();

/*
 * The getPlayerCarHP function retrieves the horsepower data for 
 * the player's car
 */
int getPlayerCarHP();

/*
 * The getUserInput function retrieves the numbers input by the user and
 * validates them
 */
int getUserInt();

/*
 * The isNumber function validates that a string input can be safely
 * converted to a number and returns true if the input is a number
 */
bool isNumber(std::string input);

/*
 * The printInstruct function prints some text for instructions to the console
 * window 
 */
void printInstruct();

/*
 * The dragRace function allows the player to drag race opponents chosen from
 * one of 8 different cars
 */
void dragRace(RaceCar &playerCar, std::vector<RaceCar> &opponentCars);

/*
 * The playAgain function determines whether the user would like to play again
 * using getUserInput and alters the repeatPlay variable 
 */
void playAgain(bool &repeatPlay);

/*
 * The getYesOrNo function asks the user for a yes or no answer and returns the
 * result as a boolean value 
 */
bool getYesOrNo();

/*
 * The isYOrN function validates a string input as Y or N
 */
bool isYOrN(std::string input);

int main(int argc, char *argv[])
{
    // Open file with opponent car data and check for failure 
    std::ifstream inStream(argv[1]);
    if (inStream.fail())
    {
        std::cout << "Input file opening failed.\n"
                  << "Command line argument usage: ./program [filename]\n"
                  << "Please ensure the text file is in the same directory as "
                  << "this program.\n";
        exit(1);
    }
    
    // Declare variables for creating opponent RaceCar objects
    std::string          tempName;
    int                  tempWeight;
    int                  tempHP;
    std::vector<RaceCar> opponentCars;
    
    /*
     * I hard coded the loop to fill the vector with 8 opponents but in a future
     * version I may change this so the amount of opponents can be more flexible
     */
    
    // Loop to construct opponent cars
    for (int i = 0; i < 8; i++)
    {
        inStream >> tempName >> tempWeight >> tempHP;
        RaceCar tempOpponent(tempName, tempWeight, tempHP);
        opponentCars.push_back(tempOpponent);
    }
    
    // Close input stream
    inStream.close();
    
    // Print instructions for player
    printInstruct();
    
    // Initialize player's RaceCar object
    RaceCar player("Player's Car", getPlayerCarWeight(), getPlayerCarHP());
    
    // Allow the user to race the opponents
    dragRace(player, opponentCars);
	
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: getPlayerCarWeight
 * 
 *    Entry: none 
 *
 *     Exit: Retrieves the weight data for the player's car from a number
 *           input by the user in the console window; Returns an int with the
 *           value entered
 *
 *  Purpose: Retrieve the weight and horsepower data for the player's car
 *
 *
 *   *   *   *   *   *   */
int getPlayerCarWeight()
{
    // Declare variables for the user input;
    int  weight;
    bool weightIsValid = false;
    
    // Ask the user for the data
    while (!weightIsValid)
    {
        std::cout << "Please enter the weight in pounds of your car ("
                  << MIN_WEIGHT << " - " << MAX_WEIGHT << "):\n";
        weight = getUserInt();
        
        // Determine if the weight entered is within range
        if (weight < MIN_WEIGHT || weight > MAX_WEIGHT)
        {
            std::cout << "\n" << "Please enter a number from " << MIN_WEIGHT
                      << " to " << MAX_WEIGHT << "." << "\n\n";
        }
        else
        {
            weightIsValid = true;
        }
    }
    
    return (weight);
}

/*   *   *   *   *   *   *
 * 
 * Function: getPlayerCarHP
 * 
 *    Entry: none 
 *
 *     Exit: Retrieves the horsepower data for the player's car from a number
 *           input by the user in the console window; Returns an int with the
 *           value entered
 *
 *  Purpose: Retrieve the horsepower data for the player's car
 *
 *
 *   *   *   *   *   *   */
int getPlayerCarHP()
{
    // Declare variables for the user input;
    int  horsepower;
    bool hpIsValid = false;
    
    while (!hpIsValid)
    {
        std::cout << "Please enter the horsepower of your car (" << MIN_HP
                  << " - " << MAX_HP << "):\n";
        horsepower = getUserInt();
        
        // Determine if the weight entered is within range
        if (horsepower < MIN_HP || horsepower > MAX_HP)
        {
            std::cout << "\n" << "Please enter a number from " << MIN_HP
                      << " to " << MAX_HP << "." << "\n\n";
        }
        else
        {
            hpIsValid = true;
        }
        
    }
    
    return (horsepower);
}

/*   *   *   *   *   *   *
 * 
 * Function: getUserInt
 * 
 *    Entry: None
 *
 *     Exit: Retrieves the numbers input by the user and validates that the 
 *           numbers are an integer using isNumber
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
    bool        inputIsANum = false;
    
    // Loop to accept input and determine if input is a positive integer
    while(!inputIsANum)
    {
        // Retrieve the string entered
        std::cin >> userInput;
        
        // Pass the string to isPositiveDouble function for validation
        inputIsANum = isNumber(userInput);
        
        // Ask for new input if the previous entry is invalid
        if (!inputIsANum)
        {
            std::cout << "\nPlease enter a number without a fractional part.\n";
        }
    }
    
    // Convert the string entered into a double
    validatedInt = std::stoi(userInput);
    
    return (validatedInt);
}

/*   *   *   *   *   *   *
 * 
 * Function: isNumber
 * 
 *    Entry: Input parameter is a string
 *
 *     Exit: Validates the string input is a number
 *
 *  Purpose: Validates the string input is a number
 *
 *
 *   *   *   *   *   *   */
bool isNumber(std::string input)
{
    // Initialize boolean return variable
    bool isValid = true;
    
    // This loop checks each character in the string
    for (int i = 0; i < input.size(); i++)
    {
        // determine if the characters entered are numbers
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
 * Function: printInstruct
 * 
 *    Entry: none
 *
 *     Exit: Prints instructions for game
 *
 *  Purpose: Prints instructions for game
 *
 *
 *   *   *   *   *   *   */
void printInstruct()
{
    std::cout << "\n" << "*******  DRAG RACE!  ******\n\n";
    std::cout << "In this game you will enter the weight and horsepower for "
              << "your car, then you\nwill be able to drag race against one "
              << "of eight different opponents. The computer\nwill calculate "
              << "the times and declare a winner. You may race as many times as"
              << " you\nlike with your car and your win / loss record will be "
              << "displayed when you quit.\n\n";
}

/*   *   *   *   *   *   *
 * 
 * Function: dragRace
 * 
 *    Entry: The function accepts the player's RaceCar object as a parameter
 *           that is passed by reference along with a vector, also passed by
 *           reference, containing the opponent RaceCar objects for the player
 *           to race
 *
 *     Exit: The function allows the player to race against opponents as many
 *           times as the player wishes. The wins and losses variables for the
 *           player's car are altered by the function
 *
 *  Purpose: Allow the player to race against his choice of several opponents as
 *           many times as the player wishes and keep track of the player's win
 *           loss record
 *
 *
 *   *   *   *   *   *   */
void dragRace(RaceCar &playerCar, std::vector<RaceCar> &opponentCars)
{
    /*
     * In a future version I may try to reduce the size of this function by
     * breaking out some tasks into separate functions
     */
    
    // Declare a variable for whether the user decides to play the game again
    bool repeatPlay = true;
    
    // This loop allows the user to play again if desired
    while (repeatPlay)
    {
        // Declare variable for player opponent choice
        int  choice;
        bool choiceIsValid = false;
        
        // Ask player to choose opponent from list of RaceCar objects in vector
        std::cout << "Choose from these 8 opponents:\n";
        for (int i = 0; i < 7; i++)
        {
            // Future version - format output so that it looks neater
            std::cout << opponentCars[i].getName() << " [" << i << "]\n";
        }
        
        // Ensure opponent choice input is valid
        while (!choiceIsValid)
        {
            // Retrieve the number entered
            choice = getUserInt();
        
            // Ensure input is valid
            if (choice > 7)
            {
                std::cout << "\n" << "Please enter a number from 0 to 7.\n\n";
            }
            else
            {
                choiceIsValid = true;
            }
        }
        
        // Declare variables for times and assign values
        double playerTime   = playerCar.calcRaceTime();
        double opponentTime = opponentCars[choice].calcRaceTime();
        
        // Compare times and print who won the race
        if (playerTime < opponentTime)
        {
            std::cout << "You beat the " << opponentCars[choice].getName() 
                      << "!\n" << "Your time: " << playerTime << "\n"
                      << opponentCars[choice].getName() << "'s time: " 
                      << opponentTime << "\n\n";
            playerCar.incrementWins();
        }
        else if (opponentTime < playerTime)
        {
            std::cout << "You lost to the " << opponentCars[choice].getName() 
                      << "!\n" << "Your time: " << playerTime << "\n"
                      << opponentCars[choice].getName() << "'s time: " 
                      << opponentTime << "\n\n";
            playerCar.incrementLosses();
        }
        else if (opponentTime == playerTime)
        {
            std::cout << "It's a tie!\n"
                      << "Your time: " << playerTime << "\n"
                      << opponentCars[choice].getName() << "'s time: " 
                      << opponentTime << "\n\n";
        }

        // Check if the user would like to play again
        playAgain(repeatPlay);
    }
    
    // Print record before quitting
    std::cout << "Your record is " << playerCar.getWins() << " wins and "
              << playerCar.getLosses() << " losses.\n\n";
}

/*   *   *   *   *   *   *
 * 
 * Function: playAgain
 * 
 *    Entry: The parameter is the boolean variable indicating whether the
 *           game should be repeated and it is altered by the function
 *
 *     Exit: The function asks the user whether he would like to repeat the
 *           game using the getYesOrNo function; The repeatPlay
 *           parameter is altered by the function
 *
 *  Purpose: Asks the user whether he would like to repeat the calculation         
 *
 *
 *   *   *   *   *   *   */
void playAgain(bool &repeatPlay)
{
	// Ask the user if he would like to perform the calculation again
	std::cout << "Would you like to play again? (Y or N) ";
	
	// Get the user's answer and assign true or false to repeatCalc
	repeatPlay = getYesOrNo();
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
