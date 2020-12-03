/*
* simulator.cpp
* TODO edit later
* main component, for init simulation, seting time, controlling events by calling next from
* event queue etc.
*/

#include "discrete_simulator.hpp"

void simulator_loaded_test()
{
    std::cout << "simulator.cpp loaded" << std::endl;
}

Simulator::Simulator(double endTime)
{
    end_time = endTime;
    current_time = 0.0;
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

}

Event* Simulator::pop_event()
{
    
}