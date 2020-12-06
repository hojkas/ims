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
    repeat_itself = false;
}

Event::~Event()
{

}

std::string Event::get_name()
{
    if(event_name.empty()) {
        return std::string("Event#" + std::to_string(event_id));
    }
    else {
        return event_name;
    }
}

//generator
EventGenerator::EventGenerator()
{
    repeat_itself = true;
    time = Simulator::start_time;
}