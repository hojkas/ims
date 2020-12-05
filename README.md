# IMS

## HOWTO: TEST

> make test

pokud máš test s mainem v `test.cpp`, jinak

> make test testfile=new_test.cpp

Prvně vytvoříme samotný simulátor, takže ho potřebujeme přes něco testovat. Vytvoři si nějaký cpp soubor, doporučuji `test.cpp` protože to má Makefile hardwired a je to v gitignore.

Pro začátek může vypadat takto:

```cpp
#include "discreet_simulator.hpp"

int main()
{
 simulator_loaded_test();
 events_loaded_test();
 sho_loaded_test();
 random_loaded_test();
 statistics_loaded_test();
 return 0;
}
```

To by ti nemělo vyhodit žádný error a mělo by to vypsat, že ty komponenty jsou loaded (aka že byly ty funkce součástí make a dobře se nalinkovaly).

Cokoliv bude deklarované v souboru `discreet_simulator.hpp`, půjde použít zvnějšku.

## TODOs

### První vlna - Basic event based simulátor

[&#10003;] eventy

[&#10003;] basic funkce simulátoru, aka práce s časem a next event handling

[&#10003;] generátor eventů

[X] random number generator (viz přednášky slide 167, tyto implementovat)

### Druhá vlna - SHO

[&#10003;] Zapracovat do simulátoru podporu sho, aka věci jako že po eventu co sahá na facility se checkne queue té facility atd

[&#10003;] Queue

[&#10003;] Storage, Facility

[X] Otestovat

### Třetí vlna - Simulace

[X] Najít ukázkovou netriviální simulaci

[X] Naprogramovat ji pomocí našeho simulátoru

### Čtvrtá vlna - Dokumentace

[X] Dokumentace každé části simulátoru, jak se chovají a jak je zvnějšku používat

[X] U random number generator udělat výstupy z mnohonásobného generování různých rozložení a udělat grafy, aby se ukázalo, že fakt fungují

[X] Dokumentace k simulaci

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

## Téma č. 4: Implementace diskrétního simulátoru s podporou SHO

Implementujte vlastní diskrétní simulátor založený na procesech nebo událostech. Implementujte podporu pro SHO (fronty, linky) a generování pseudonáhodných čísel (pro různá rozložení). Demonstrujte na hypotetickém modelu SHO s několika linkami, různými procesy příchodů apod.

(yup, that's the whole assignment)

## Possible components

* Simulátor - založený na procesech nebo událostech - události jsou prý o dost snazší
  * Proces nebo událost - máme mít buď na tom nebo na tom... prý v demu říkal, že události jsou snazší na implementaci a nemusíme řešit přepínání kontextu and stuff
  * Fronta
  * Linka, sklad (myslím, že to je to samé, jen se to liší v počtu zařízení)
  * Pseudo-náhodné generování čísel pro základní rozložení
    * Exponenciální
    * Gaussovo/normální
    * ...?
  * Funkce na zjištění stavu simulace/vytvoření výstupů nebo tak něco
* Ukázková simulace
* Dokumentace

## Simulátor

Jedno rozhraní s možnostmi inicializovat simulátor, nastavit různý shit, vytvořit takový a onen proces...

Chtělo by to mrknout se na syntaxi SIMLIB a inspirovat se.

## Ukázková simulace

Jen jeden file co použije simulátor a udělá simulaci. Nebo dva, jeden simple af jeden těžší, I dunno, myslím, že to bude ez anyway a bude to fajn na ověření funkčnosti.

Na cviku myslím zmínil něco ve smyslu, že stačí nějaká z komplikovanějších z cvika na komplikovanější petriho sítě, aka basically vzít odtud něco vyřešeného a plácnout z grafu do simulace: https://web.microsoftstream.com/video/1d9cc486-2919-46d7-aef2-bd5ed41da862?list=user&userId=ccfcd434-db90-4d69-8afe-b02d153d6776.

## Dokumentace

Welp this will be fun.

Jestli se s tím nechceme srát, jakože ne, udělala bych to prostě v Markdownu, já to pak vezmu, v Typora tomu hodím html flare (jako přední stranu, page breaks, table of contets automaticky z úrovní nadpisů) a vyexportuji. Dělala jsem to tak v IPK a ISA a nestěžovali si a nevypadalo to tak hnusně + close to zero effort :)
