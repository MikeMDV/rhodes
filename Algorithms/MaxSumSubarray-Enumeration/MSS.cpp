/*
 *             Author: Michael Marven
 *       Date Created: 10/12/15
 * Last Date Modified: 10/15/15
 *          File Name: MSS.cpp
 *         Assignment: Project 1 - Project Group 17
 *           Overview: A program to calculate the maximum subarray sum of arrays
 *                     using different algorithms; The program reads input from 
 *                     a text file and output to a text file; The text file 
 *                     contains lines of integers separated by commas with one
 *                     set of square brackets around each line; The integers on 
 *                     each line are placed in an array; The maximum sum of a 
 *                     subarray of the arrays on each line are calculated; The
 *                     original array, subarray and the sum is output to a text 
 *                     file for each array; The results of each algorithm for a 
 *                     single array are output to the file before moving to the
 *                     next array to be calculated
 *
 *                     The output file is always named MSS_Results.txt and all 
 *                     results are appended to the end of the file. If the 
 *                     program is run more than once, the results for each
 *                     successive program run will be appended to 
 *                     MSS_Results.txt if it is not deleted
 *
 *                     Format for command line arguments
 *                     mss <file name>
 *
 *                     The file name is the input file being used
 *
 *                     The program should be compiled with C++11 flags; Example
 *                     for use with the flip servers below
 *
 *                     g++ -std=c++0x MSS.cpp -o mss
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
 * The enumMSS() function calculates the maximum sum of a subarray of an array
 * through enumeration
 */
void enumMSS(const std::vector<int> &a, std::vector<int> &sa, int &max);

/*
 * The betterEnumMSS() function calculates the maximum sum of a subarray of an array
 * through better enumeration
 */
void betterEnumMSS(const std::vector<int> &a, std::vector<int> &sa, int &max);

/*
 * The outputToFile() function writes the vectors and sum to the output file
 */
void outputToFile(std::ofstream &out, const std::vector<int> &a, 
                  const std::vector<int> &sa, const int &sum);
                  
/*
 * The fillVector() function fills the vector and with the ints from the input
 * line
 */
void fillVector(std::string &s, std::vector<int> &a);

int main(int argc, char *argv[])
{   
    // Declare input and ouput stream objects and open input stream
    std::ifstream inStream(argv[1]);
    if (inStream.fail())
    {
        std::cout << "Input file opening failed.\n";
        exit(1);
    }
    std::ofstream outStream("MSS_Results.txt", std::ios::app);
    
    while (!inStream.eof())
    {
        // Declare a string for the line, get next line, declare vector for ints
        std::string line;
        getline(inStream, line);
        std::vector<int> intVect;
        
        // Fill the vector with the ints in the line
        fillVector(line, intVect);

        // Declare subarray vectors and sum variable
        std::vector<int> subArray1;
        std::vector<int> subArray2;
        int sum;
        
        // Call function to calculate the maximum sum of a subarray
        enumMSS(intVect, subArray1, sum);
        
        // Write vector contents and sum to the output file
        outputToFile(outStream, intVect, subArray1, sum);
        
        // Call the next function to calculate the maximum sum of a subarray
        betterEnumMSS(intVect, subArray2, sum);
        
        // Write vector contents and sum to the output file
        outputToFile(outStream, intVect, subArray2, sum);
        
        
    }
    
    // Close input and output streams
    inStream.close();
    outStream.close();
    
    return 0;
}

/*   *   *   *   *   *   *
 * 
 * Function: enumMSS()
 * 
 *    Entry: A const vector by reference, a vector by reference for the 
 *           subarray, and an int by reference
 *
 *     Exit: Values of parameters sa and sum will be changed by the function             
 *
 *  Purpose: Calculate the maximum sum of a subarray using enumeration
 *
 *   *   *   *   *   *   */
void enumMSS(const std::vector<int> &a, std::vector<int> &sa, int &max)
{
    // Set the sum to 0 and declare a variable for the temp sum
    max = 0;
    int tempSum;
    int startIndex;
    int endIndex;
    
    // Loop over the indices, keeping the best sum found
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = i; j < a.size(); j++)
        {
            tempSum = 0;
            for (int k = i; k <= j; k++)
            {
                tempSum += a[k];
                if (tempSum > max)
                {
                    max = tempSum;
                    startIndex = i;
                    endIndex = j;
                }
            }
        }
    }
    
    // Create the subarray
    for (int i = startIndex; i <= endIndex; i++)
    {
        sa.push_back(a[i]);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: betterEnumMSS()
 * 
 *    Entry: A const vector by reference, a vector by reference for the 
 *           subarray, and an int by reference
 *
 *     Exit: Values of parameters sa and sum will be changed by the function             
 *
 *  Purpose: Calculate the maximum sum of a subarray using better enumeration
 *
 *   *   *   *   *   *   */
void betterEnumMSS(const std::vector<int> &a, std::vector<int> &sa, int &max)
{
    // Set the sum to 0 and declare a variable for the temp sum
    max = 0;
    int tempSum;
    int startIndex;
    int endIndex;
    
    // Loop over the indices, keeping the best sum found
    for (int i = 0; i < a.size(); i++)
    {
        tempSum = 0;
        for (int j = i; j < a.size(); j++)
        {
            tempSum += a[j];
            if (tempSum > max)
            {
                max = tempSum;
                startIndex = i;
                endIndex = j;
            }
        }
    }
    
    // Create the subarray
    for (int i = startIndex; i <= endIndex; i++)
    {
        sa.push_back(a[i]);
    }
}

/*   *   *   *   *   *   *
 * 
 * Function: outputToFile()
 * 
 *    Entry: An ofstream object by reference, a const vector by reference, a  
 *           const vector by reference for the subarray, and an int by reference
 *
 *     Exit: Values of parameters will be written to the output file             
 *
 *  Purpose: Write vectors and sums to the output file
 *
 *   *   *   *   *   *   */
void outputToFile(std::ofstream &out, const std::vector<int> &a, 
                  const std::vector<int> &sa, const int &sum)
{
    // Write vector contents and sum to the output stream
        out << "[";
        for (int i = 0; i < a.size(); i++)
        {
            if (i < a.size() - 1)
            {
                out << a[i] << ", ";
            }
            else
            {
                out << a[i] << "]\n";
            }
        }
        out << "[";
        for (int i = 0; i < sa.size(); i++)
        {
            if (i < sa.size() - 1)
            {
                out << sa[i] << ", ";
            }
            else
            {
                out << sa[i] << "]\n";
            }
        }
        out << sum << "\n\n";
}


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
