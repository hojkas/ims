# Hodnocení

20/20b

# IMS

Podrobnější informace v dokumentaci: /doc/Dokumentace.pdf

## HOWTO: USE SIMULATOR

### Jak vytvořit vlastní event

```cpp
class Person : public Event
{
    public:
        Person(double t) {
            time = t;
        }
        void Behaviour() {
            std::cout << "Person at time " << time << "." << std::endl;
        }
};

int main()
{
  Simulator::Init(0.0, 10.0);
  Simulator::schedule_event(new Person(7.0));
  Simulator::Run();
}
```

Class Person (nový event) dědí z Event. Může mít vlastní konstruktor (zde s parametrem double, který určí čas události). V `Behaviour()` je kód, který event vykoná v čase time.

V `main` je pak potřeba inicializovat simulátor na počáteční a koncový čas, naplánovat event pomocí `schedule_event`, kterému se předá pointer na novou instanci class Person a spustí se simulace.

### Jak vytvořit vlastní generátor

```cpp
class YourGenerator : public EventGenerator
{
    public:
        void Behaviour() {
            time += Random();
            Simulator::schedule_event(new Person(time));
            Simulator::schedule_event(this);
        }
};

int main()
{
  Simulator::Init(0.0, 10.0);
  Simulator::schedule_event(new PersonGenerator());
  Simulator::Run();
}
```

Nadeklarovat class Generátoru s Behaviour s:

- změnou vnitřní proměnné generátoru time přidáním žádaného náhodného rozložení
- naplánováním do simulátoru Event, který vygeneroval, na čas time
- naplánovat sám sebe znovu do simulátoru na čas time

## HOWTO: Předávání eventů, Seize Facility/Storage and Wait

### WAIT a základ akcí přes více eventů

Deklarace eventů:

```cpp
class Person : public Event
{
    public:
        void Behaviour();
};

class Person2 : public Event
{
    public:
        Person2(Event* parent);
        void Behaviour();
};

inline void Person::Behaviour() {
    std::cout << get_name() << std::endl;
    Simulator::Wait(1.0, new Person2(this));
}

inline Person2::Person2(Event* parent) {
    event_name = parent->event_name;
    event_id = parent->event_id;
    priority = parent->priority;
}
inline void Person2::Behaviour() {
    std::cout << get_name() << std::endl;
}
```

V tomto případě je akce Person rozkouskovaná do dvou Eventů - `Person` a `Person2`. Při každém použití akce Wait nebo Seize je potřeba udělat více eventů. Pokud pro účely výpisů chceme ponechat event id, name a prioritu, je třeba u `Person2` vytvořit konstruktor (viz výše), který vezme event a převezme jeho vlastnosti.

Po WAIT nesmí být žádné jiné funkce v Behaviour, jinak to způsobí nežádané nekonzistence.

Seize Storage/Facility funguje obdobně. V napojeném eventu nebo jedním z jeho následovníků se musí volat Release, aby byla nastolena rovnováha.

```cpp
class Person : public Event
{
    public:
        void Behaviour();
};

class Person2 : public Event
{
    public:
        Person2(Event* parent);
        void Behaviour();
};

inline void Person::Behaviour() {
    std::cout << get_name() << std::endl;
    Simulator::SeizeStorage("storage", new Person2(this));
}

inline Person2::Person2(Event* parent) {
    event_name = parent->event_name;
    event_id = parent->event_id;
    priority = parent->priority;
}
inline void Person2::Behaviour() {
    std::cout << get_name() << std::endl;
    Simulator::ReleaseStorage("storage");
}

int main() {
  Simulator::Init(0.0, 4.0);
  Simulator::ScheduleEvent(new Person());
  Simulator::CreateStorage("storage", 2);
  Simulator::Run();
}
```

Storage je třeba vytvořit funkcí CreateStorage před voláním Run.
