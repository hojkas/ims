/*
* simulator.cpp
* main component and main interface for user
* handles creating simulation time, scheduling events and working them out one by one.
* also helps user to create and use Storages and Facilites
*/

#include "discrete_simulator.hpp"

//declaring for static Simulator
std::list<Event*> Simulator::event_queue;
double Simulator::start_time;
double Simulator::end_time;
double Simulator::last_effective_time;
uint32_t Simulator::free_event_id;
std::map<std::string, Facility*> Simulator::facilities;
std::map<std::string, Storage*> Simulator::storages;

void Simulator::Init(double start_t, double end_t)
{
    start_time = start_t;
    last_effective_time = start_time;
    end_time = end_t;
    event_queue.clear();
    free_event_id = 0;
    facilities.clear();
    storages.clear();
    RandomGenerator::Init();
}

void Simulator::deconstruct()
{
    Event* curr_event = pop_event();
    while(curr_event != NULL) {
        delete curr_event;
        curr_event = pop_event();
    }

    for(const auto& kv : facilities) {
        delete kv.second;
    }
    facilities.clear();

    for(const auto& kv : storages) {
        delete kv.second;
    }
    storages.clear();
    RandomGenerator::deconstruct();
}

/* Runs the simulaton from data previously set up in Simulator. Will simulate events from start time to end time.
 * Beware, if there are any events before start time, they will be ignored in full - including generators which will render
 * then efectless.
*/
void Simulator::Run()
{
    Event* curr_event = pop_event();
    while(curr_event != NULL) {
        //if time is before simulation time
        if(curr_event->time < start_time) {
            std::cerr << "Event " << curr_event->get_name() << " starts before simulation start time, " <<
                "possibly throwing off the whole siulation - all events before time are fully ignored, " <<
                "including repeating generators. Please check the events and their start times." << std::endl;
            delete curr_event;
            curr_event = pop_event();
            continue;
        }
        //if time is exceeded
        if(curr_event->time > end_time) {
            last_effective_time = end_time;
            delete curr_event;
            break;
        }

        last_effective_time = curr_event->time;
        //work out current event and log to stats what's neccessary
        curr_event->Behaviour();

        //if not marked as generator, delete curr_event
        if(!curr_event->repeat_itself) delete curr_event;
        //gets new event
        curr_event = pop_event();
    }

    deconstruct();
}


/* Inserts Event e in simulator's event_queue based on its time value
 * @param Event* Event to insert
 */
void Simulator::ScheduleEvent(Event* e)
{
    std::list<Event*>::iterator it;
    for (it = event_queue.begin(); it != event_queue.end(); ++it)
    {
        if(((*it)->time == e->time && e->priority > (*it)->priority) || (*it)->time > e->time) {
            event_queue.insert(it, e);
            return;
        } 
    }
    event_queue.push_back(e);
}

/* Inserts Event e in simulator's event_queue based on its time value
 * @param Event* Event to insert
 * @param double Time to schedule event to
 */
void Simulator::ScheduleEvent(Event* e, double time)
{
    e->time = time;
    std::list<Event*>::iterator it;
    for (it = event_queue.begin(); it != event_queue.end(); ++it)
    {
        if(((*it)->time == e->time && e->priority > (*it)->priority) || (*it)->time > e->time) {
            event_queue.insert(it, e);
            return;
        } 
    }
    event_queue.push_back(e);
}

/* Schedules event after wait. 
 * @param wait_time Time to wait. In other words, time after which the event will be scheduled. Cannot be less than 0.
 * @param event Event to be run after wait_time.
 */
void Simulator::Wait(double wait_time, Event *event)
{
    if(wait_time < 0.0) {
        std::cerr << "Wait time given (" << wait_time << ") cannot be less than 0." << std::endl;
        exit(1);
    }
    double time = last_effective_time + wait_time;
    ScheduleEvent(event, time);
}


/* Inserts event to be done immediatelly after current one. Shouldn't be called from outside,
 * is usefull for removing items from queue after Facility or Storage clears up.
 * Skips all time requirements but still honors priority. Time is set to current simulation time.
 * @param e Event to put at beginning of events scheduled.
*/
void Simulator::fastforward_event(Event* e)
{
    e->time = last_effective_time;
    ScheduleEvent(e);
}

/* Removes first Event* from simulator's event_queue and returns it
 * @return Event* poped from front, the one with lowest time value
 */
Event* Simulator::pop_event()
{
    if(event_queue.size() == 0) return NULL;
    Event* front_event = event_queue.front();
    event_queue.pop_front();
    return front_event;
}

//======================================================================
//                      SHO MANIPULATION
//======================================================================

/* Creates Storage and appends to Simulator list of storages.
 * @param storage_name Name of storage for future referring. Must be unique.
 * @param storage_capacity Size of storage. Must be bigger than 0.
 * @param queue_limit Limits the size of queue of waiting events. If limit is reached, Seize will return false.
**/
void Simulator::CreateStorage(std::string storage_name, size_t storage_capacity, size_t storage_queue_limit)
{
    if(storages.find(storage_name) != storages.end()) {
        std::cerr << "Storage with name " << storage_name << " was defined twice. Please make sure each storage has unique name."
            << std::endl;
        deconstruct();
        exit(1);
    }

    Storage* new_storage = new Storage(storage_name, storage_capacity, storage_queue_limit);

    storages.insert(std::pair<std::string, Storage*>(storage_name, new_storage));
}   

/* Creates Storage and appends to Simulator list of storages.
 * @param storage_name Name of storage for future referring. Must be unique.
 * @param storage_capacity Size of storage. Must be bigger than 0.
**/
void Simulator::CreateStorage(std::string storage_name, size_t storage_capacity)
{
    if(storages.find(storage_name) != storages.end()) {
        std::cerr << "Storage with name " << storage_name << " was defined twice. Please make sure each storage has unique name."
            << std::endl;
        deconstruct();
        exit(1);
    }

    Storage* new_storage = new Storage(storage_name, storage_capacity);

    storages.insert(std::pair<std::string, Storage*>(storage_name, new_storage));
}

/* Creates Facility and ads it to Simulator list of Facilities.
 * @param facility_name Name of facility, must be unique among facilites.
 * @param facility_queue_limit Limits events waiting for facility to be free. When limit is reached,
 *  Seize facility will return false and can be handled in Behaviour of event.
 */
void Simulator::CreateFacility(std::string facility_name, size_t facility_queue_limit)
{
    if(facilities.find(facility_name) != facilities.end()) {
        std::cerr << "Facility with name " << facility_name << " was defined twice. Please make sure each facility has unique name."
            << std::endl;
        deconstruct();
        exit(1);
    }

    Facility* new_facility;
    if(facility_queue_limit == 0) new_facility = new Facility(facility_name);
    else new_facility = new Facility(facility_name);

    facilities.insert(std::pair<std::string, Facility*>(facility_name, new_facility));
}   

/* Creates Facility and ads it to Simulator list of Facilities.
 * @param facility_name Name of facility, must be unique among facilites.
 */
void Simulator::CreateFacility(std::string facility_name)
{
    CreateFacility(facility_name, 0);
}

/* Tries to Seize Storage of given name. If the storage is free, immediately does Event *event.
 * If storage doesn't have any free space, event is stored in storage queue according to its priority, until there is
 * free space. If the queue is full (the Storage was created with limited queue), Event* event is NOT handled at all and
 * function returns false.
 * @param storage_name Name of storage to seize.
 * @param event Event that will be run (its Behaviour will be simulated), when it gets one unit from Storage.
 * @return True if event either got unit from storage or was put inside its queue, false if queue was full and event remains
 * unhandled.
 */
bool Simulator::SeizeStorage(std::string storage_name, Event* event)
{
    std::map<std::string, Storage*>::iterator it;
    it = storages.find(storage_name);
    if(it == storages.end()) {
        std::cerr << "Storage with name \"" << storage_name << "\" not found. Halting simulation. Please make sure you " <<
            "create all the Storages needed with CreateStorage before running simulation." << std::endl;
        deconstruct();
        exit(1);
    }

    return it->second->Seize(event);
}

/* Tries to Seize Facility of given name. If the Facility is free, immediately does Event *event.
 * If facility isn't free, event is stored in facility queue according to its priority, until there is
 * free space. If the queue is full (the Facility was created with limited queue), Event* event is NOT handled at all and
 * function returns false.
 * @param facility_name Name of facility to seize.
 * @param event Event that will be run (its Behaviour will be simulated), when it gets access Facility.
 * @return True if event either got access to facility or was put inside its queue, false if queue was full and event remains
 * unhandled.
 */
bool Simulator::SeizeFacility(std::string facility_name, Event* event)
{
    std::map<std::string, Facility*>::iterator it;
    it = facilities.find(facility_name);
    if(it == facilities.end()) {
        std::cerr << "Facility with name \"" << facility_name << "\" not found. Halting simulation. Please make sure you " <<
            "create all the Facilites needed with CreateFacility before running simulation." << std::endl;
        deconstruct();
        exit(1);
    }

    return it->second->Seize(event);
}

/* Releases storage unit. Should only ever be used once by Event that was created by Event seizing Storage.
 * If there are any events in storage queue, the first one is scheduled at the first place in event calendar.
 * @param storage_name Name of storage that is to be released.
 */
void Simulator::ReleaseStorage(std::string storage_name)
{
    std::map<std::string, Storage*>::iterator it;
    it = storages.find(storage_name);
    if(it == storages.end()) {
        std::cerr << "Storage with name \"" << storage_name << "\" not found. Halting simulation. Please make sure you " <<
            "create all the Storages needed with CreateStorage before running simulation." << std::endl;
        deconstruct();
        exit(1);
    }

    it->second->Release();
}

/* Releases facility. Should only ever be used once by Event that was created by Event seizing Facility.
 * If there are any events in facility queue, the first one is scheduled at the first place in event calendar.
 * @param storage_name Name of facility that is to be released.
 */
void Simulator::ReleaseFacility(std::string facility_name)
{
    std::map<std::string, Facility*>::iterator it;
    it = facilities.find(facility_name);
    if(it == facilities.end()) {
        std::cerr << "Facility with name \"" << facility_name << "\" not found. Halting simulation. Please make sure you " <<
            "create all the Facilites needed with CreateFacility before running simulation." << std::endl;
        deconstruct();
        exit(1);
    }

    it->second->Release();
}