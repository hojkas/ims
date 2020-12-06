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
class Queue;
class Log;
class EventGenerator;

// =========================================================================
//                          simulator.cpp definitions
// =========================================================================
Simulator* init_simulator(double startTime,double endTime);
Simulator* init_simulator(double endTime);

class Simulator
{
    private:
        //functions
        static void deconstruct();
        static void fastforward_event(Event *);
        static Event* pop_event();

        //properties
        static double start_time;
        static double end_time;
        static double last_effective_time;
        static uint32_t free_event_id;
        static std::list<Event*> event_queue;

        //maps of Facilities and storages
        static std::map<std::string, Facility*> facilities;
        static std::map<std::string, Storage*> storages;

        //friend classes from our simulator to be allowed to use private properties
        friend class Event;
        friend class EventGenerator;
        friend class Storage;
        friend class Facility;
        friend class Queue;
        friend class Log;

    public:

        static void Init(double, double);
        static void Run();
        static void ScheduleEvent(Event *, double);
        static void ScheduleEvent(Event *);
        static void Wait(double, Event *);

        static void CreateStorage(std::string, size_t);
        static void CreateStorage(std::string, size_t, size_t);
        static void CreateFacility(std::string);
        static void CreateFacility(std::string, size_t);

        static bool SeizeStorage(std::string, Event *);
        static bool SeizeFacility(std::string, Event *);
        static void ReleaseStorage(std::string);
        static void ReleaseFacility(std::string);
};

// =========================================================================
//                          events.cpp definitions
// =========================================================================

//Class for creating Events. Each new Event should have it as parent when defining it.
class Event
{
    private:

    public:
        Event();                        //default constructor (sets priority to 0, assings event_id)
        ~Event();                       //default deconstructor
        
        virtual void Behaviour() = 0;   //to be overriden in child class, place to define what will Event do
        std::string get_name();         //help to get event name, returns event#event_id if it doesn't have name
        std::string event_name;         //optional name of event to make simulation more organised
        double time;                    //inner variable for scheduling, should only be accessed by generator, not
                                        //regular events to avoid inconsistency
        int priority;                   //priority of event, should it be in queue or at same time as another event, it skipps
                                        //lesser priority
        uint32_t event_id;              //at the time of construct unique id of event, may be overriden to make new event
                                        //look like another one continuing (e.g. after Wait), only affects output name 
        bool repeat_itself;             //if true, event is not deconstruct after being run (desired for generators)
};

//Class for Generator of events. Each Generator class should have this class as parent.
class EventGenerator : public Event
{
    public:
        EventGenerator();               //default generator, does what Event() but sets repeat_itself to true
};

// =========================================================================
//                          sho.cpp definitions
// =========================================================================
//sho - systém hromadné obsluhy

//private, only for Facility and Storage use
class Queue
{
    private:
        Queue();
        Queue(size_t);
        ~Queue();

        Event* pop_front();
        bool push_back(Event*);
        bool insert_event(Event *);

        bool limited;
        size_t limit;
        std::list<Event*> queued_events;

        friend class Simulator;
        friend class Log;
        friend class Facility;
        friend class Storage;
};

class Facility
{
    private:
        Facility(std::string);
        Facility(std::string, size_t);
        ~Facility();
        bool Seize(Event* );
        void Release();

        std::string name;
        size_t capacity;
        Queue* queue;

        friend class Simulator;
        friend class Log;
};

class Storage
{
    private:
        Storage(std::string, size_t);
        Storage(std::string, size_t, size_t);
        ~Storage();
        bool Seize(Event *);
        void Release();

        std::string name;
        size_t capacity;
        Queue* queue;

        friend class Simulator;
        friend class Log;
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

    static void Init();
    static void deconstruct();

    friend class Simulator;
    friend class Log;

public:

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
