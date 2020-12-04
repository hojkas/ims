/*
* events.cpp
* for creating and handling Events
* Event - main component of simulator
*/

#include "discrete_simulator.hpp"

Event::Event()
{
    priority = 0;
    event_id = Simulator::free_event_id;
    Simulator::free_event_id += 1;
    event_name = std::string();
}

Event::~Event()
{

}

//operators override
bool Event::operator<(Event& e)
{
    return (time < e.time);
}

bool Event::operator>(Event& e)
{
    return (time > e.time);
}