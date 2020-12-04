#ifndef _DISCREET_SIMULATOR_H
#define _DISCREET_SIMULATOR_H

/* Entry point to the simulator.
 * The only Header file for all the simulator.
 * Contains definition from all the cpp files belonging to it.
*/

// general dependencies
#include <iostream>
#include <string>
#include <list>
#include <map>

//general definitions that need to be used before referencing
class Simulator;
class Event;
class Storage;
class Facility;

// =========================================================================
//                          simulator.cpp definitions
// =========================================================================
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
        static void schedule_event(Event *);
        static void fastforward_event(Event *);

        //properties
        static double start_time;
        static double end_time;
        static uint32_t free_event_id;

        //maps of Facilities and storages
        static std::map<std::string, Facility*> facilities;
        static std::map<std::string, Storage*> storages;

};

// =========================================================================
//                          events.cpp definitions
// =========================================================================
class Event
{
    public:
        Event();
        ~Event();
        
        virtual void Behaviour() = 0;
        std::string get_name();

        //operations
        bool operator<(Event&);
        bool operator>(Event&);
        std::string event_name;
        double time;
        int priority;
        uint32_t event_id;
        
        bool repeat_itself;
};

class EventGenerator : public Event
{
    public:
        EventGenerator();
};

// =========================================================================
//                          sho.cpp definitions
// =========================================================================
//sho - systém hromadné obsluhy

//private, only for Facility and Storage use
class Queue
{
    public:
        Queue();
        Queue(size_t);
        ~Queue();

        Event* pop_front();
        bool push_back(Event*);

        bool limited;
        size_t limit;
        std::list<Event*> queued_events;
};

class Facility
{
    public:
        Facility(std::string);
        Facility(std::string, size_t);
        ~Facility();

        std::string name;
        Queue* queue;
};

class Storage
{
    public:
        Storage(std::string, size_t);
        Storage(std::string, size_t, size_t);
        ~Storage();

        std::string name;
        size_t capacity;
        Queue* queue;
};

// =========================================================================
//                     random_generator.cpp definitions
// =========================================================================
void random_loaded_test(); //TODO remove, for initial linking tests

double Random();
double Uniform();
double Exponential(double E);
double Normal(double M, double S);

//staticstics.cpp definitions
void statistics_loaded_test(); //TODO remove, for initial linking tests

#endif //_DISCREET_SIMULATOR_H
