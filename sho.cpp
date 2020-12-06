/* sho.cpp
 * SHO - system hromadne obsluhy
 * covers Queues, Storages, Facitilies
*/

#include "discrete_simulator.hpp"

// =========================================================================
//                              QUEUE
// =========================================================================
Queue::Queue()
{
    limited = false;
}

Queue::Queue(size_t queue_limit)
{
    limit = queue_limit;
    limited = true;
}

Queue::~Queue()
{
    for(const auto & event : queued_events) {
        delete event;
    }
    queued_events.clear();
}

bool Queue::push_back(Event *e)
{
    if(limited && queued_events.size() >= limit) return false;
    queued_events.push_back(e);
    return true;
}

bool Queue::insert_event(Event *e)
{
    if(limited && queued_events.size() >= limit) return false;
    std::list<Event*>::iterator it;
    for (it = queued_events.begin(); it != queued_events.end(); ++it)
    {
        if(e->priority > (*it)->priority) {
            queued_events.insert(it, e);
            return true;
        } 
    }
    queued_events.push_back(e);
    return true;
}

Event* Queue::pop_front()
{
    if(queued_events.size() == 0) return NULL;
    Event *poped = queued_events.front();
    queued_events.pop_front();
    return poped;
}

bool Queue::remove_event(Event *event)
{
    std::list<Event*>::iterator it;
    for (it = queued_events.begin(); it != queued_events.end(); ++it)
    {
        Event* selected = (*it);
        if(selected == event) {
            queued_events.remove(*it);
            return true;
        }
    }
}

// =========================================================================
//                              FACILITY
// =========================================================================
/* Constructor for Facility.
 * @param facility_name Name of facility, must be unique for any facility.
 */
Facility::Facility(std::string facility_name)
{
    name = facility_name;
    queue = new Queue();
    capacity = 1;
}

/* Constructor for Facility.
 * @param facility_name Name of facility, must be unique for any facility.
 * @param queue_limit Limits size of queue. If it is full, Seize facility will return false
 */
Facility::Facility(std::string facility_name, size_t queue_limit)
{
    name = facility_name;
    queue = new Queue(queue_limit);
    capacity = 1;
}

Facility::~Facility()
{
    delete queue;
}

bool Facility::Seize(Event *event)
{
    if(capacity > 0) {
        capacity -= 1;
        event->Behaviour();
        return true;
    }

    if(queue->limited) {
        //if the queue is full or zero sized
        if(queue->queued_events.size() >= queue->limit) return false;
    }

    queue->insert_event(event);
    return true;
}

void Facility::Release()
{
    if(queue->queued_events.empty()) capacity += 1;
    else {
        //queue not empty, take one event from it instead
        Event* e = queue->pop_front();
        Simulator::fastforward_event(e);
    }
}

// =========================================================================
//                              STORAGE
// =========================================================================
/* Constructor for Storage
 * @param storage_name Name of storage for future referring. Must be unique.
 * @param storage_capacity Size of storage. Must be bigger than 0.
**/
Storage::Storage(std::string storage_name, size_t storage_capacity)
{
    name = storage_name;
    capacity = storage_capacity;
    queue = new Queue();
}

/* Constructor for Storage
 * @param storage_name Name of storage for future referring. Must be unique.
 * @param storage_capacity Size of storage. Must be bigger than 0.
 * @param queue_limit Limits the size of queue of waiting events. If limit is reached, Seize will return false.
**/
Storage::Storage(std::string storage_name, size_t storage_capacity, size_t queue_limit)
{
    name = storage_name;
    capacity = storage_capacity;
    queue = new Queue(queue_limit);
}

Storage::~Storage()
{
    delete queue;
}

bool Storage::Seize(Event *event)
{
    if(capacity > 0) {
        capacity -= 1;
        event->Behaviour();
        return true;
    }

    if(queue->limited) {
        //if the queue is full or zero sized
        if(queue->queued_events.size() >= queue->limit) return false;
    }

    queue->insert_event(event);
    return true;
}

void Storage::Release()
{
    if(queue->queued_events.empty()) capacity += 1;
    else {
        //queue not empty, take one event from it instead
        Event* e = queue->pop_front();
        Simulator::fastforward_event(e);
    }
}


