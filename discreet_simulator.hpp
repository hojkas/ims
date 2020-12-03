#ifndef _DISCREET_SIMULATOR_H
#define _DISCREET_SIMULATOR_H

/* Entry point to the simulator.
 * The only Header file for all the simulator.
 * Contains definition from all the cpp files belonging to it.
*/

// general dependencies
#include <iostream>

//simulator.cpp definitions
void simulator_loaded_test(); //TODO remove, for initial linking tests

//events.cpp definitions
void events_loaded_test(); //TODO remove, for initial linking tests

//sho.cpp definitions
//sho - systém hromadné obsluhy
void sho_loaded_test(); //TODO remove, for initial linking tests

//random_generator.cpp definitions
void random_loaded_test(); //TODO remove, for initial linking tests

//staticstics.cpp definitions
void statistics_loaded_test(); //TODO remove, for initial linking tests

#endif //_DISCREET_SIMULATOR_H
