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
#include <cmath>
#include <limits>
#include <utility>

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
        //for inner use by other classes or for keen user wanting to edit more

        static void deconstruct();
        static void fastforward_event(Event *);
        static Event* pop_event();

        //functions for user for easiest creation of simulation

        static void Init(double, double);
        static void Run();
        static void ScheduleEvent(Event *, double);
        static void ScheduleEvent(Event *);
        static void Wait(double, Event *);

        //sho

        static void CreateStorage(std::string, size_t);
        static void CreateStorage(std::string, size_t, size_t);
        static void CreateFacility(std::string);
        static void CreateFacility(std::string, size_t);

        static bool SeizeStorage(std::string, Event *);
        static bool SeizeFacility(std::string, Event *);
        static void ReleaseStorage(std::string);
        static void ReleaseFacility(std::string);

        //properties
        static double start_time;
        static double end_time;
        static double last_effective_time;
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
        bool insert_event(Event *);

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
        bool Seize(Event* );
        void Release();

        std::string name;
        size_t capacity;
        Queue* queue;
};

class Storage
{
    public:
        Storage(std::string, size_t);
        Storage(std::string, size_t, size_t);
        ~Storage();
        bool Seize(Event *);
        void Release();

        std::string name;
        size_t capacity;
        Queue* queue;
};

// =========================================================================
//                     random_generator.cpp definitions
// =========================================================================
void random_loaded_test(); //TODO remove, for initial linking tests
class RandomGenerator
{
private:
    static double second_BM_variable;
    static constexpr double fraction { 1.0 / ((double)UINT32_MAX + 1.0) };
    // Generates a new random number into randomNumber
    static void RandomNumberGenerator();

public:
    static void Init();
    static void deconstruct();
    static uint32_t randomNumber;

    static double Random();
    static double Uniform(double MIN, double MAX);
    static double Exponential(double E);
    static double Normal(double MEAN, double SIGMA);
};

// =========================================================================
//                         staticstics.cpp definitions
// =========================================================================
class Log {
public:
    static void SimulatorState();
    static void EventState(Event *, std::string);
};


#endif //_DISCREET_SIMULATOR_H
