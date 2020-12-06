/*
* events.cpp
* for creating and handling Events
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

TimeoutWatch::TimeoutWatch(Event* watched, Event* on_timeout, Queue* watched_queue)
{
    on_timeout_event = on_timeout;
    queued_event = watched;
    queue_with_event = watched_queue;
}

void TimeoutWatch::Behaviour()
{
    if(queue_with_event->remove_event(queued_event)) {
        delete queued_event;
        on_timeout_event->Behaviour();
    }
}