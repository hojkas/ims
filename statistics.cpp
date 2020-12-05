/* statistics.cpp
* TODO edit later
* for getting outcome we need out of simulation
* perhaps there should be some kind of calling from simulator after getting next event
* and here could be functions set to different values, depending on setting, getting out
* some output or not
*/

#include "discrete_simulator.hpp"
#define output_separator "========================================"

void Log::SimulatorState()
{
    std::cout << output_separator << std::endl
              << "            SIMULATOR STATE" << std::endl
              << output_separator << std::endl
              << "TIME:" << std::endl
              << "simulation time:   " << Simulator::last_effective_time << std::endl
              << "start time     :   " << Simulator::start_time << std::endl
              << "end time       :   " << Simulator::end_time << std::endl
              << output_separator << std::endl
              << "EVENTS:" << std::endl
              << "scheduled in simulator: " << Simulator::event_queue.size() << std::endl;

    int paused = 0;
    for(const auto& f : Simulator::facilities) {
        paused += f.second->queue->queued_events.size();
    }
    for(const auto& f : Simulator::storages) {
        paused += f.second->queue->queued_events.size();
    }

    std::cout << "paused in queues: " << paused << std::endl
              << output_separator << std::endl
              << "FACILITIES: " << Simulator::facilities.size() << std::endl;
    
    for(const auto& f : Simulator::facilities) {
        std::cout << f.second->name << std::endl << "     state: " << (f.second->capacity ? "free" : "taken") << std::endl
            << "     queued events: " << f.second->queue->queued_events.size() <<
            std::endl << "     queue limit: ";
        if(!f.second->queue->limited) std::cout << "none" << std::endl;
        else std::cout << f.second->queue->limit << std::endl;
    }

    std::cout << output_separator << std::endl
              << "STORAGES: " << Simulator::storages.size() << std::endl;

    for(const auto& f : Simulator::storages) {
        std::cout << f.second->name << std::endl << "     capacity left: " << f.second->capacity << std::endl <<
            "     queued events: " << f.second->queue->queued_events.size() <<
            std::endl << "     queue limit: ";
        if(!f.second->queue->limited) std::cout << "none" << std::endl;
        else std::cout << f.second->queue->limit << std::endl;
    }

    std::cout << output_separator << std::endl;

}

void Log::EventState(Event *event, std::string log_msg)
{
    std::cout << "[" << Simulator::last_effective_time << "] \t" << event->get_name() << ": " << log_msg << std::endl; 
}