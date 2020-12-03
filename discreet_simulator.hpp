#ifndef _DISCREET_SIMULATOR_H
#define _DISCREET_SIMULATOR_H

/* Entry point to the simulator.
 * The only Header file for all the simulator.
 * Contains definition from all the cpp files belonging to it.
*/

// general dependencies
#include <iostream>
#include <list>
#include <tuple>

//simulator.cpp definitions

//double time, Event event
#define t_event_queue std::list<std::tuple<double, Event>>
void simulator_loaded_test(); //TODO remove, for initial linking tests

class Simulator
{
    private:
        t_event_queue event_queue;

    public:
        Simulator(int time);
        ~Simulator();

        Event pop_event();
        void insert_event(Event);

};

//events.cpp definitions
void events_loaded_test(); //TODO remove, for initial linking tests
class Event
{
    public:
        Event();
        ~Event();
        
        std::string msg;
};

//sho.cpp definitions
//sho - systém hromadné obsluhy
void sho_loaded_test(); //TODO remove, for initial linking tests

//random_generator.cpp definitions
void random_loaded_test(); //TODO remove, for initial linking tests

double Random();
double Uniform();
double Exponential(double E);
double Normal(double M, double S);

//staticstics.cpp definitions
void statistics_loaded_test(); //TODO remove, for initial linking tests

#endif //_DISCREET_SIMULATOR_H
