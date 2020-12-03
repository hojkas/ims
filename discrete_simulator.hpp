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

//general definitions that need to be used before referencing
class Simulator;
class Event;

//simulator.cpp definitions

//double time, Event event
#define t_event_queue std::list<Event*>
void simulator_loaded_test(); //TODO remove, for initial linking tests

class Simulator
{
    private:
        t_event_queue event_queue;

    public:
        Simulator(double endTime);
        Simulator(double startTime, double endTime);
        ~Simulator();

        Event* pop_event();
        void insert_event(Event*);

        //properties
        double current_time;
        double end_time;
};

//events.cpp definitions
void events_loaded_test(); //TODO remove, for initial linking tests
class Event
{
    public:
        Event(Simulator*);
        ~Event();
        
        std::string msg;
        Simulator* simulator;
        double time;
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
