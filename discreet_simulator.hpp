#ifndef _DISCREET_SIMULATOR_H
#define _DISCREET_SIMULATOR_H

/* Entry point to the simulator.
 * The only Header file for all the simulator.
 * Contains definition from all the cpp files belonging to it.
*/

// general dependencies
#include <iostream>
#include <cstdint>

//simulator.cpp definitions
void simulator_loaded_test(); //TODO remove, for initial linking tests

//events.cpp definitions
void events_loaded_test(); //TODO remove, for initial linking tests

//sho.cpp definitions
//sho - systém hromadné obsluhy
void sho_loaded_test(); //TODO remove, for initial linking tests

//random_generator.cpp definitions
void random_loaded_test(); //TODO remove, for initial linking tests
class RandomGenerator
{
private:
    static uint32_t randomNumber;

    // Generates a new random number into randomNumber
    void RandomNumberGenerator();

public:
    RandomGenerator();
    ~RandomGenerator();

    double Random();
    double Uniform(double MIN, double MAX);
    double Exponential(double E);
    double Normal(double MEAN, double SIGMA);
};

RandomGenerator::RandomGenerator()
{
    // Base Seed
    this->randomNumber = 0x626f6f70;

}

RandomGenerator::~RandomGenerator()
{
}

//staticstics.cpp definitions
void statistics_loaded_test(); //TODO remove, for initial linking tests

#endif //_DISCREET_SIMULATOR_H
