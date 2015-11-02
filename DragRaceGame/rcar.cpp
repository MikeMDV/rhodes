/*
 *             Author: Michael Marven
 *       Date Created: 03/20/13
 * Last Date Modified: 03/23/13
 *          File Name: rcar.cpp
 *           Overview: This is the implementation file for the RaceCar class
 *              Input: None        
 *             Output: None
 *                     
 *
 */
 
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include "rcar.h"

namespace RCarMarven
{
    RaceCar::RaceCar(std::string nameInput, int weightInput, int hpInput)
        : name(nameInput), weight(weightInput), horsepower(hpInput), wins(0),
          losses(0)
    {/*Body intentionally empty*/}

    RaceCar::RaceCar() : name("Average Car"), weight(3551),
                         horsepower(160), wins(0), losses(0)
    {/*Body intentionally empty*/}

    /*   *   *   *   *   *   *
     * 
     * Function: getHP
     * 
     *    Entry: none
     *
     *     Exit: Returns an int with the value of the horsepower variable for a 
     *           RaceCar object
     *
     *  Purpose: Return an int with the value of the horsepower variable for a 
     *           RaceCar object
     *
     *
     *   *   *   *   *   *   */
    int RaceCar::getHP()
    {
        return (horsepower);
    }

    /*   *   *   *   *   *   *
     * 
     * Function: getWeight
     * 
     *    Entry: none
     *
     *     Exit: Returns an int with the value of the weight variable for a 
     *           RaceCar object
     *
     *  Purpose: Return an int with the value of the weight variable for a 
     *           RaceCar object
     *
     *
     *   *   *   *   *   *   */
    int RaceCar::getWeight()
    {
        return (weight);
    }

    /*   *   *   *   *   *   *
     * 
     * Function: getName
     * 
     *    Entry: none
     *
     *     Exit: Returns a string with the value of the name variable for a 
     *           RaceCar object
     *
     *  Purpose: Return a string with the value of the name variable for a 
     *           RaceCar object
     *
     *
     *   *   *   *   *   *   */
    std::string RaceCar::getName()
    {
        return (name);
    }

    /*   *   *   *   *   *   *
     * 
     * Function: getWins
     * 
     *    Entry: none
     *
     *     Exit: Returns an int with the value of the wins variable for a 
     *           RaceCar object
     *
     *  Purpose: Return an string with the value of the wins variable for a 
     *           RaceCar object
     *
     *
     *   *   *   *   *   *   */
    int RaceCar::getWins()
    {
        return (wins);
    }

    /*   *   *   *   *   *   *
     * 
     * Function: getLosses
     * 
     *    Entry: none
     *
     *     Exit: Returns an int with the value of the losses variable for a 
     *           RaceCar object
     *
     *  Purpose: Return an string with the value of the losses variable for a 
     *           RaceCar object
     *
     *
     *   *   *   *   *   *   */
    int RaceCar::getLosses()
    {
        return (losses);
    }

    /*   *   *   *   *   *   *
     * 
     * Function: incrementWins
     * 
     *    Entry: none
     *
     *     Exit: Increments the wins variable by 1
     *
     *  Purpose: Increment the wins variable by 1
     *
     *
     *   *   *   *   *   *   */
    void RaceCar::incrementWins()
    {
        wins = wins++;
    }

    /*   *   *   *   *   *   *
     * 
     * Function: incrementLosses
     * 
     *    Entry: none
     *
     *     Exit: Increments the losses variable by 1
     *
     *  Purpose: Increment the losses variable by 1
     *
     *
     *   *   *   *   *   *   */
    void RaceCar::incrementLosses()
    {
        losses = losses++;
    }

    /*   *   *   *   *   *   *
     * 
     * Function: calcRaceTime
     * 
     *    Entry: none
     *
     *     Exit: Calculates the 1/4 mile drag time in seconds using a formula 
     *           that uses the weight and hp numbers; 
     *           The formula is (cube-root of (Weight / HP)) X 5.825
     *           The function returns a double with the result of the formula
     *
     *  Purpose: Return the 1/4 mile drag time in seconds
     *
     *
     *   *   *   *   *   *   */
    double RaceCar::calcRaceTime()
    {
        /*
         * I found this formula on this web page:
         * http:://http://www.schwarzmann.com/garage/tech/carmath.htm
         * It works surprisingly well. I checked it with some auto magazine 
         * reviews in which the hp, weight, and 1/4 mile times were listed. This
         * formula usually is within 3 - 4 tenths of a second of the published 
         * times.
         */
        double time = (std::cbrt(static_cast<double>(weight)/horsepower)) 
                       * 5.825;
        
        return (time);
    }
} //RCarMarven