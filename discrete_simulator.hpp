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
#include <ctime>

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
        static void ScheduleEvent(Event *);

        //properties
        static double start_time;
        static double end_time;
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
        /* Inicializes simulator. Must be called before any other simulator function is used.
         * @param start_time Time of beggining of simulation. All events before are ignored.
         * @param end_time Time of end of simulation. All events after are ignored.
         */
        static void Init(double start_time, double end_time);
        /* Executes the simulation and deconstructs all simulator parts. Unless it is
         * followed by another Init, no other simulator function should be used after Run was called.
         */
        static void Run();
        /* Schedules Event into Simulator to the time given. If there are more events
         * with the same time, they are sorted according to their priority.
         * @param event Event to schedule.
         * @param time Time to schedule the event.
         */
        static void ScheduleEvent(Event *event, double time);
        /* Waits for given time and then executes given event. For the true purpose of
         * pausing the event and waiting some time, Event needs to be "split" in two parts.
         * First part ends with Wait and the second part, separate Event, is given to the
         * Wait as parameter. Once the wait is over, new event is executed.
         * @param time waiting time
         * @param event Event which Behaviour will be executed once the wait is over.
         */
        static void Wait(double time, Event *event);

        /* Creates Storage of given name and capacity with unlimited queue in Simulator.
         * @param name Name of storage.
         * @param capacity Capacity of storage.
         */
        static void CreateStorage(std::string name, size_t capacity);
        /* Creates Storage of given name and capacity with limited queue in Simulator.
         * @param name Name of storage.
         * @param capacity Capacity of storage.
         * @param queue_limit Limit of events waiting in the queue.
         */
        static void CreateStorage(std::string name, size_t capacity, size_t queue_limit);
        /* Creates Facility of given name with ulimited queue in Simulator.
         * @param name Name of Facility.
         */
        static void CreateFacility(std::string name);
        /* Creates Facility of given name with limited queue in Simulator.
         * @param name Name of Facility.
         * @param queue_limit Limit of events waiting in the queue.
         */
        static void CreateFacility(std::string name, size_t queue_limit);

        /* Tries to seize Storage. As in Wait, after Seize shouldn't be any other actions
         * in event. Event passed to this function will be executed after accessing Storage.
         * @param storage_name Name of Storage to Seize.
         * @param event Event to execute after successfully Seizing Storage.
         * @return Bool true if event Seized Storage or was taken to its Queue. False if
         * the queue was limited and already full. (Gives the event trying to Seize
         * opportunity to do something else.)
         */
        static bool SeizeStorage(std::string storage_name, Event *event);
        /* Tries to seize Storage. As in Wait, after Seize shouldn't be any other actions
         * in event. Event passed to this function will be executed after accessing Storage.
         * @param storage_name Name of Storage to Seize.
         * @param timeout_time Time after which the success_event will be removed from queue and timeout
         * event will be run instead
         * @param on_success_event Event to execute after successfully Seizing Storage.
         * @param on_timeout_event Event that will be executed if timeout occurs.
         * @return Bool true if event Seized Storage or was taken to its Queue. False if
         * the queue was limited and already full. (Gives the event trying to Seize
         * opportunity to do something else.)
         */
        static bool SeizeStorageWithTimeout(std::string storage_name, double timeout_time, Event *on_success_event, Event *on_timeout_event);
        /* Tries to seize Facility. As in Wait, after Seize shouldn't be any other actions
         * in event. Event passed to this function will be executed after accessing Facility.
         * @param timeout_time Time after which the success_event will be removed from queue and timeout
         * event will be run instead
         * @param on_success_event Event to execute after successfully Seizing Storage.
         * @param on_timeout_event Event that will be executed if timeout occurs.
         * @param event Event to execute after successfully Seizing Facility.
         * @return Bool true if event Seized Facility or was taken to its Queue. False if
         * the queue was limited and already full. (Gives the event trying to Seize
         * opportunity to do something else.)
         */
        static bool SeizeFacility(std::string facility_name, Event *event);
        /* Tries to seize Facility. As in Wait, after Seize shouldn't be any other actions
         * in event. Event passed to this function will be executed after accessing Facility.
         * @param facility_name Name of Facility to Seize.
         * @param event Event to execute after successfully Seizing Facility.
         * @return Bool true if event Seized Facility or was taken to its Queue. False if
         * the queue was limited and already full. (Gives the event trying to Seize
         * opportunity to do something else.)
         */
        static bool SeizeFacilityWithTimeout(std::string facility_name, double timeout_time, Event *on_success_event, Event *on_timeout_event);
        /* Releases Storage. Should be used only once in event that is successor of event
         * that Seized the Storage in the first place. Unlike Seize and Wait, this action
         * doesn't need to be at the end of Behaviour and doesn't require event split in two.
         * @param storage_name Name of Storage to Release
         */
        static void ReleaseStorage(std::string storage_name);
        /* Releases Facility. Should be used only once in event that is successor of event
         * that Seized the Facility in the first place. Unlike Seize and Wait, this action
         * doesn't need to be at the end of Behaviour and doesn't require event split in two.
         * @param facility_name Name of Facility to Release
         */
        static void ReleaseFacility(std::string facility_name);

        //time of event that was triggered most recently
        static double last_effective_time;
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

class TimeoutWatch : public Event
{
    private:
        Event* on_timeout_event;
        Event* queued_event;
        Queue* queue_with_event;
        TimeoutWatch(Event* on_success_event, Event* on_timeout_event, Queue* queue_with_on_success_event);
        void Behaviour();

        friend Simulator;
        friend Storage;
        friend Facility;
};

// =========================================================================
//                          sho.cpp definitions
// =========================================================================
//sho - systém hromadné obsluhy

//Queue, Facility and Storage are not usable from outside, they can be interacted with only through Simulator methods
class Queue
{
    private:
        Queue();
        Queue(size_t);
        ~Queue();

        Event* pop_front();
        bool push_back(Event*);
        bool insert_event(Event *);
        bool remove_event(Event *);

        bool limited;
        size_t limit;
        std::list<Event*> queued_events;

        friend class Simulator;
        friend class Log;
        friend class Facility;
        friend class Storage;
        friend class TimeoutWatch;
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
    static void SetSeed(uint32_t seed);
    static double Random();
    static double Uniform(double MIN, double MAX);
    static double Exponential(double E);
    static double Normal(double MEAN, double SIGMA);
};

// =========================================================================
//                         staticstics.cpp definitions
// =========================================================================
//Class Log helps user get information about state of Simulator and Event on stdout.
class Log {
public:
    //Writes state of simulator on stdout. Includes such information as simulation time, number of
    //queued events and details about defined Storages and Facilites.
    static void SimulatorState();
    /* Writes simulation time, event name or identifier and given message on stdout.
     * @param Event* Event which name will be shown in output.
     * @param std::string Message to include with event to make simulation output
     * more descriptive. (e.g. "seized facility XY")
    */
    static void EventState(Event *, std::string);
};


#endif //_DISCREET_SIMULATOR_H
