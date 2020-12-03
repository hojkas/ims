/*
* events.cpp
* for creating and handling Events
* Event - main component of simulator
*/

#include "discrete_simulator.hpp"

void events_loaded_test()
{
    std::cout << "events.cpp loaded" << std::endl;
}

Event::Event(Simulator* sim)
{
    simulator = sim;
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