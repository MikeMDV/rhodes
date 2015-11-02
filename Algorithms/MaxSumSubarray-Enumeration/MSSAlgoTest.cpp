/*
 *             Author: Michael Marven
 *       Date Created: 10/12/15
 * Last Date Modified: 10/15/15
 *          File Name: MSSAlgoTest.cpp
 *         Assignment: Project 1 - Project Group 17
 *           Overview: A program to test the running times of the first 2 
 *                     algorithms by generating arrays filled with random 
 *                     numbers; The program runs ten tests on each algorithm 
 *                     each with a different array of numbers; The arrays are a
 *                     fixed size that is entered as a command line option; The 
 *                     program prints the times to the screen
 *
 *                     Format for command line arguments
 *                     algo <array size>
 *
 *                     The program should be compiled with C++11 flags; Example
 *                     for use with the flip servers below
 *
 *                     g++ -std=c++0x MSSAlgoTest.cpp -o algo
 *
 *              Input: Command line option for the size of the arrays
 *             Output: The running times and the averages are printed to the
 *                     screen
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
#include <ctime>

const int NUMBER_OF_TESTS = 10;
const int RANGE_MIN       = -99;
const int RANGE_MAX       = 99;

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
 * The randRange() function returns an random integer in the range, inclusive of
 * the stated limits
 */
int randRange(int min, int max);

int main(int argc, char *argv[])
{
    // Seed the random number generator for the rand_range function
    srand(std::time(NULL));
    
    // Convert array size argument to int and declare an int for the numbers
    int arrayLength = std::atoi(argv[1]);
    int num;
    
    // Loop through tests for enumeration algorithm
    for (int i = 0; i < NUMBER_OF_TESTS; i++)
    {
        // Declare a vector for the random numbers and for the subarray
        std::vector<int> numArray;
        std::vector<int> subArray1;
        int sum;
        
        // Loop for adding numbers to vector
        for (int j = 0; j < arrayLength; j++)
        {
            numArray.push_back(randRange(RANGE_MIN, RANGE_MAX));
        }
        
        // Set first time point
        std::chrono::high_resolution_clock::time_point t1 = 
            std::chrono::high_resolution_clock::now();
        
        // Call enumeration function on the array
        enumMSS(numArray, subArray1, sum);
        
        // Set second time point
        std::chrono::high_resolution_clock::time_point t2 = 
            std::chrono::high_resolution_clock::now();
            
        // Compute duration of function
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        
        // Print results
        std::cout << "The enumeration function took " << duration 
            << "\nmicroseconds to run\n";
    }
    
    // Loop through tests for better enumeration algorithm
    for (int i = 0; i < NUMBER_OF_TESTS; i++)
    {
        // Declare a vector for the random numbers and for the subarray
        std::vector<int> numArray;
        std::vector<int> subArray1;
        int sum;
        
        // Loop for adding numbers to vector
        for (int j = 0; j < arrayLength; j++)
        {
            numArray.push_back(randRange(RANGE_MIN, RANGE_MAX));
        }
        
        // Set first time point
        std::chrono::high_resolution_clock::time_point t1 = 
            std::chrono::high_resolution_clock::now();
        
        // Call enumeration function on the array
        betterEnumMSS(numArray, subArray1, sum);
        
        // Set second time point
        std::chrono::high_resolution_clock::time_point t2 = 
            std::chrono::high_resolution_clock::now();
            
        // Compute duration of function
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        
        // Print results
        std::cout << "The better enumeration function took " << duration 
            << "\nmicroseconds to run\n";
    }
    
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
 * Function: randRange()
 * 
 *    Entry: The parameters are ints for the minimum and maximum range values
 *
 *     Exit: The function returns an int in the range, inclusive of the stated
 *           limits
 *
 *  Purpose: The function generates a number within a range without some 
 *           numbers generating more than others and output skewing toward those
 *           values; More information available at the link below;
 *           Adapted from the Stack Overflow answer located at:
 *           http://stackoverflow.com/questions/2999075/generate-a-random-
 *           number-within-range/2999130#2999130
 *
 *
 *   *   *   *   *   *   */
int randRange(int min, int max)
{
    // Generate a divisor for the rand() function
    int divisor = RAND_MAX / ((max - min) + 1);
    int output;

    do
    {
        output = rand() / divisor;
    }while (output > (max - min));

    return (output + min);
}