/*
* simulator.cpp
* TODO edit later
* main component, for init simulation, seting time, controlling events by calling next from
* event queue etc.
*/

#include "discrete_simulator.hpp"

Simulator* init_simulator(double startTime, double endTime)
{
    Simulator* new_s = new Simulator(startTime, endTime);
    return new_s;
}

Simulator* init_simulator(double endTime)
{
    Simulator* new_s = new Simulator(0.0, endTime);
    return new_s;
}

Simulator::Simulator(double startTime, double endTime)
{
    end_time = endTime;
    start_time = startTime;
}

Simulator::~Simulator()
{

}

void Simulator::Run()
{
    Event* curr_event = pop_event();
    while(curr_event != NULL) {
        //work out current event and log to stats what's neccessary
        curr_event->Output();

        //gets new event
        curr_event = pop_event();
    }
}


/* Inserts Event e in simulator's event_queue based on its time value
 * @param Event* Event to insert
 */
void Simulator::insert_event(Event* e)
{
    std::list<Event*>::iterator it;
    for (it = event_queue.begin(); it != event_queue.end(); ++it)
    {
        if((*it)->time > e->time) {
            event_queue.insert(it, e);
            return;
        } 
    }
    event_queue.push_back(e);
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