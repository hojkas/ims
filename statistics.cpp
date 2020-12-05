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
              << "FACILITIES:   " << Simulator::facilities.size() << std::endl;
    
    for(const auto& f : Simulator::facilities) {
        std::cout << f.second->name << std::endl << "     queued events:" << f.second->queue->queued_events.size() <<
            std::endl << "     queue limit: ";
        if(!f.second->queue->limited) std::cout << "none" << std::endl;
        else std::cout << f.second->queue->limit << std::endl;
    }

    std::cout << output_separator << std::endl
              << "STORAGES:   " << Simulator::storages.size() << std::endl;

    for(const auto& f : Simulator::storages) {
        std::cout << f.second->name << std::endl << "     capacity: " << f.second->capacity << std::endl <<
            "     queued events: " << f.second->queue->queued_events.size() <<
            std::endl << "     queue limit: ";
        if(!f.second->queue->limited) std::cout << "none" << std::endl;
        else std::cout << f.second->queue->limit << std::endl;
    }

    std::cout << output_separator << std::endl;

}

void Log::EventState(Event *event, std::string log_msg)
{
    std::cout << "[" << Simulator::last_effective_time << "] " << event->get_name() << ": " << log_msg << std::endl; 
}