#include "discrete_simulator.hpp"


class Skier : public Event
{
	public:
		void Behaviour();
};

class Racer : public Event
{
	public:
		void Behaviour();
};

class PersonInQueue : public Event
{
	public:
		PersonInQueue(Event *parent);
		void Behaviour();
};

class PersonOnLift : public Event
{
	public:
		PersonOnLift(Event *parent);
		void Behaviour();
};

class PersonLeavesLift : public Event
{
	public:
		PersonLeavesLift(Event *parent);
		void Behaviour();
};

class EmptyLiftTrip : public Event
{
	public:
		void Behaviour();
};


class LiftTripBack : public Event
{
	public:
		void Behaviour();
};

class LiftReturns : public Event
{
	public:
		void Behaviour();
};



inline void Skier::Behaviour()
{
	Log::EventState(this, "postavi se do rady");
	Event *new_person = new PersonInQueue(this);
	Simulator::SeizeFacility("stanoviste", new_person);
}


inline void Racer::Behaviour()
{
	Log::EventState(this, "postavi se do rady");
	Event *new_person = new PersonInQueue(this);
	Simulator::SeizeFacility("stanoviste", new_person);
}

inline PersonInQueue::PersonInQueue(Event *parent)
{
	event_name = parent->event_name;
	event_id = parent->event_id;
	priority = parent->priority;
}

inline void PersonInQueue::Behaviour()
{
	Log::EventState(this, "ceka na kotvu");
	Event *new_person = new PersonOnLift(this);
	Simulator::SeizeStorage("kotva", new_person);
}

inline PersonOnLift::PersonOnLift(Event *parent)
{
	event_name = parent->event_name;
	event_id = parent->event_id;
	priority = parent->priority;
}

inline void PersonOnLift::Behaviour()
{
	if (RandomGenerator::Random() > 0.1){
		Log::EventState(this, "je na kotve");
		Simulator::ReleaseFacility("stanoviste");
		Event *new_person = new PersonLeavesLift(this);
		Simulator::Wait(4.0, new_person);
	} else {
		Log::EventState(this, "nepodari se naskocit na kotvu");
		Event *new_event = new EmptyLiftTrip();
		Simulator::ScheduleEvent(new_event, Simulator::last_effective_time);
		Event *new_person = new PersonInQueue(this);
		Simulator::ScheduleEvent(new_person, Simulator::last_effective_time);
	}
}

inline PersonLeavesLift::PersonLeavesLift(Event *parent)
{
	event_name = parent->event_name;
	event_id = parent->event_id;
	priority = parent->priority;
}

inline void PersonLeavesLift::Behaviour()
{
	Log::EventState(this, "zisel z kotvy");
	Event *new_event = new LiftTripBack();
	new_event->event_id = event_id;
	Simulator::Wait(4.0, new_event);
}

inline void EmptyLiftTrip::Behaviour()
{
	Event *new_event = new LiftTripBack();
	new_event->event_id = event_id;
	Simulator::Wait(4.0, new_event);
}

inline void LiftTripBack::Behaviour()
{
	Event *new_event = new LiftReturns();
	new_event->event_id = event_id;
	Simulator::Wait(4.0, new_event);
}

inline void LiftReturns::Behaviour()
{
	Simulator::ReleaseStorage("kotva");
	std::cout << "[" << Simulator::last_effective_time << "]\t  Kotva se vratila" << std::endl;
}


class SkierGenerator : public EventGenerator
{
	private:
		int person_id;

	public:

		SkierGenerator() {
			person_id = 0;
		}

		void Behaviour() 
		{
			time += RandomGenerator::Exponential(1);
			Event *new_skier = new Skier();
			new_skier->event_name = "Lyzar" + std::to_string(person_id);
			person_id += 1;
			Simulator::ScheduleEvent(new_skier, time);
			Simulator::ScheduleEvent(this, time);
		}
};

class RacerGenerator : public EventGenerator
{
	private:
		int person_id;

	public:

		RacerGenerator(){
			person_id = 0;
		}

		void Behaviour()
		{
			time += RandomGenerator::Exponential(10);
			Event *new_racer = new Racer();
			new_racer->event_name = "Zavodnik" + std::to_string(person_id);
			new_racer->priority = 1;
			person_id += 1;
			Simulator::ScheduleEvent(new_racer, time);
			Simulator::ScheduleEvent(this, time);
		}
};


int main()
{
	Simulator::Init(0.0, 100.0);
    RandomGenerator::SetSeed(1607298254);
    std::cout << "Seed of simulation: " <<RandomGenerator::randomNumber << std::endl;
    std::cout << "To repeat simulation use RandomGenerator::SetSeed(" << RandomGenerator::randomNumber << "); " << 
    "before the beginning of the simulation" << std::endl;
	Simulator::ScheduleEvent(new SkierGenerator(), 0.0);
	Simulator::ScheduleEvent(new RacerGenerator(), 0.0);
	Simulator::CreateFacility("stanoviste");
	Simulator::CreateStorage("kotva", 1);
	Log::SimulatorState();
	Simulator::Run();

    return 0;
}