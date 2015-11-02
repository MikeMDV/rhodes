/*
 *             Author: Michael Marven
 *       Date Created: 03/20/13
 * Last Date Modified: 03/23/13
 *          File Name: rcar.h
 *           Overview: This is the header file for the RaceCar class
 *              Input: None        
 *             Output: None
 *                     
 *
 */
 
#ifndef RCAR_H
#define RCAR_H

#include <string>

namespace RCarMarven
{
    /*
     * Class for a race car object
     */
    class RaceCar
    {
    public:
        RaceCar(std::string nameInput, int weightInput, int hpInput);
        /*
         * Initializes the name, weight, and horsepower to the arguments, wins &
         * losses to 0 each;
         */
        
        RaceCar();
        /*
         * Default constructor - Initializes variables to default average values
         * for name, weight, & hp, wins & losses to 0 each
         */ 
        
        int getHP();
        int getWeight();
        std::string getName();
        int getWins();
        int getLosses();
        void incrementWins();
        void incrementLosses();
        double calcRaceTime();
    private:
        std::string name;
        int weight;
        int horsepower;
        int wins;
        int losses;
    };
} // RCarMarven
#endif // RCAR_H