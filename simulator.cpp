/*
* simulator.cpp
* TODO edit later
* main component, for init simulation, seting time, controlling events by calling next from
* event queue etc.
*/

#include "discrete_simulator.hpp"

//declaring for static Simulator
std::list<Event*> Simulator::event_queue;
double Simulator::start_time;
double Simulator::end_time;
uint32_t Simulator::free_event_id;
std::map<std::string, Facility*> Simulator::facilities;
std::map<std::string, Storage*> Simulator::storages;

void Simulator::Init(double start_t, double end_t)
{
    start_time = start_t;
    end_time = end_t;
    event_queue.clear();
    free_event_id = 0;
    facilities.clear();
    storages.clear();
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
            delete curr_event;
            break;
        }

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
void Simulator::schedule_event(Event* e)
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


/* Inserts event to be done immediatelly after current one. Shouldn't be called from outside,
 * is usefull for removing items from queue after Facility or Storage clears up.
 * Skips all time requirements but still honors priority.
*/
void Simulator::fastforward_event(Event* e)
{
    //in case of empty event_queue, time is left as it was as it shouldn't matter unless it's generator
    if(event_queue.empty()) {
        event_queue.push_back(e);
        return;
    }

    //gets time of first value in queue and schedules it
    e->time = event_queue.front()->time;
    schedule_event(e);
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
