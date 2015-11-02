/*
 *             Author: Michael Marven
 *       Date Created: 10/28/15
 * Last Date Modified: 10/28/15
 *          File Name: dpchange.cpp
 *         Assignment: Project 2 - Project Group 17
 *           Overview: A program to calculate the optimal amount of change for
 *                     a given amount using the coin values in the text file.
 *                     The program uses dynamic programming.
 *
 *                     The output file is takes the name of the input file and
 *                     appends "change" to it. So an input file of coins.txt 
 *                     will result in an output file of coinschange.txt
 *
 *                     Format for command line arguments
 *                     dpc <file name>
 *
 *                     The file name is the input file being used
 *
 *                     The program should be compiled with C++11 flags; Example
 *                     for use with the flip servers below
 *
 *                     g++ -std=c++0x dpchange.cpp -o dpc
 *
 *              Input: Text file with bracketed lines of integers
 *             Output: The integers in the arrays, and the sums are output to a 
 *                     text file
 *
 *
 */

#include <iostream>
#include <string>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>

/*
 * The fillVector() function fills the vector and with the ints from the input
 * line
 */
void fillVector(std::string &s, std::vector<int> &a);

/*
 * The dpChange() function calculates the minimum number of coins for change for
 * an amount using dynamic programming
 */
void dpChange(const std::vector<int> &cvals, int chg, 
               std::vector<int> &cans, int &minC);



int main (int argc, char *argv[])
{
    
    // Declare input and ouput stream objects and open input stream
    std::ifstream inStream(argv[1]);
    if (inStream.fail())
    {
        std::cout << "Input file opening failed.\n";
        exit(1);
    }
    std::string tempStr = argv[1];
    tempStr.erase(tempStr.find('.'));
    std::string output = tempStr + "change.txt";
    std::ofstream outStream(output, std::ios::app);
    
    while (!inStream.eof())
    {
        // Declare a string for the line, get next line, declare vector for coin val
        std::string lineOne;
        getline(inStream, lineOne);
        std::vector<int> coinsVect;
        
        // Fill the vector with the ints in the line
        fillVector(lineOne, coinsVect);
        
        // Declare a string for the line, get next line, declare int for change amt
        std::string lineTwo;
        getline(inStream, lineTwo);
        int change = std::stoi(lineTwo);
        
        // Declare int for minimum number of coins and vector for coin amounts
        int minCoins;
        std::vector<int> coinAmts;
        
        // Calculate coin change
        dpChange(coinsVect, change, coinAmts, minCoins);
        
        // Print out results
        // TODO: Add the output code to write results to text file
        
    }
    
    // Close input and output streams
    inStream.close();
    outStream.close();
    
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: fillVector()
 * 
 *    Entry: A string by reference, a vector by reference
 *
 *     Exit: Values of parameters will be written to the vector             
 *
 *  Purpose: Fill the vector with the integers in the input string
 *
 *   *   *   *   *   *   */
void fillVector(std::string &s, std::vector<int> &a)
{
    // Declare stringstream object for line and int for each number
    std::stringstream lineSS(s);
    int num;

    // Check if the first char in the line is the left bracket
    if (lineSS.peek() == '[')
    {
        lineSS.ignore();
        while (lineSS >> num)
        {
            // Add the number to the vector
            a.push_back(num);
            
            // Ignore commas or spaces
            if (lineSS.peek() == ',' || lineSS.peek() == ' ')
            {
                lineSS.ignore();
            } 
        }
    }
    else
    {
        std::cout << "Error: First character in line not a left bracket.\n";
        exit(1);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: dpChange()
 * 
 *    Entry: The coin values vector by reference, the final coin tally vector by
 *           reference, an int for the total amount needing changed by 
 *           reference, and an int fo the minimum number of coins by reference
 *
 *     Exit: Values of parameters will be written to the vectors and ints             
 *
 *  Purpose: Calculate the minimum number of coins for change for an amount 
 *           using dynamic programming
 *
 *   *   *   *   *   *   */
void dpChange(const std::vector<int> &cvals, int chg, 
               std::vector<int> &cans, int &minC)
{
    // Declare 2D array for use in answer and ints for for loops
    int table[cvals.size() + 1][chg + 1];
    int i;
    int j;
    
    // Fill 1st row with integers from 0 to chg and 1st col with zeroes
    for (i = 0; i < chg + 1; i++)
    {
        table[0][i] = i;
    }
    
    for (i = 0; i < cvals.size() + 1; i++)
    {
        table[i][0] = 0;
    }
    
    // Build table 
    for (i = 1; i < cvals.size() + 1; i++)
    {
        for (j = 1; j < chg + 1; j++)
        {
            if (cvals[i - 1] > j || table[i - 1][j] < table[i][j - cvals[i - 1]] + 1)
            {
                table[i][j] = table[i - 1][j];
            }
            else
            {
                table[i][j] = table[i][j - cvals[i - 1]] + 1;
            }
        }
    }
    
    // Diagnostics print
    /*for (i = 0; i < cvals.size() + 1; i++)
    {
        for (j = 0; j < chg + 1; j++)
        {
            std::cout << table[i][j] << ",";
        }
        std::cout << "\n";
    }
    std::cout << "\n";*/
    
    // Assign min coins value
    minC = table[cvals.size()][chg];

    // Build coin vector
    // Begin by filling with zeroes
    for (i = 0; i < cvals.size(); i++)
    {
        cans.push_back(0);
    }
    int count = table[cvals.size() + 1][chg + 1];
    
    // Start counting coins at lower right corner of table
    i = cvals.size();
    j = chg;
    while (count != 0)
    {
        if (table[i][j] == table[i - 1][j] && i > 1)
        {
            i--;
            count = table[i][j];
        }
        else
        {
            cans[i - 1] = cans[i - 1] + 1;
            j = j - cvals[i - 1];
            count = table[i][j];
        }
    }
    
    // Diagnostics
    /*std::cout << "[";
    for (i = 0; i < cans.size(); i++)
    {
        std::cout << cans[i] << ", ";
    }
    std::cout << "]";
    std::cout << "\n" << minC << "\n";*/
}