#ifndef _DISCREET_SIMULATOR_H
#define _DISCREET_SIMULATOR_H

/* Entry point to the simulator.
 * The only Header file for all the simulator.
 * Contains definition from all the cpp files belonging to it.
*/

// general dependencies
#include <iostream>
#include <list>

//general definitions that need to be used before referencing
class Simulator;
class Event;

//simulator.cpp definitions
Simulator* init_simulator(double startTime,double endTime);
Simulator* init_simulator(double endTime);

class Simulator
{
    private:
        static std::list<Event*> event_queue;

    public:
        static void Init(double, double);
        static void deconstruct();

        static void Run();
        static Event* pop_event();
        static void insert_event(Event*);

        //properties
        static double start_time;
        static double end_time;
};

//events.cpp definitionsS
class Event
{
    public:
        Event();
        ~Event();
        
        virtual void Behavior() = 0;

        //operations
        bool operator<(Event&);
        bool operator>(Event&);
        std::string name;
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
