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
    current_time = startTime;
}

Simulator::~Simulator()
{

}

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

Event* Simulator::pop_event()
{
    if(event_queue.size() == 0) return NULL;
    Event* front_event = event_queue.front();
    event_queue.pop_front();
    return front_event;
}