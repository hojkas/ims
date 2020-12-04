/* sho.cpp
 * SHO - system hromadne obsluhy
 * TODO edit this later
 * should cover Queues, Storages, Facitilies
*/

#include "discrete_simulator.hpp"

// =========================================================================
//                              QUEUE
// =========================================================================
Queue::Queue()
{
    
}

Queue::~Queue()
{

}

// =========================================================================
//                              FACILITY
// =========================================================================
Facility::Facility(std::string facility_name)
{
    name = facility_name;
}

Facility::~Facility()
{

}

// =========================================================================
//                              STORAGE
// =========================================================================
/* Constructor for Storage
 * @param storage_name Name of storage for future referring. Must be unique.
 * @param storage_capacity Size of storage. Must be bigger than 0.
**/
Storage::Storage(std::string storage_name, uint32_t storage_capacity)
{
    name = storage_name;
    capacity = storage_capacity;
}

Storage::~Storage()
{

}


