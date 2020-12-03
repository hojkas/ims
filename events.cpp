/*
* events.cpp
* for creating and handling Events
* Event - main component of simulator
*/

#include "discrete_simulator.hpp"

Event::Event()
{
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