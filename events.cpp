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