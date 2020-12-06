#include "discrete_simulator.hpp"

#define msgOK "[\033[0;32mOK\033[0m] "

// Forward definice eventů, které budu později používat, abych je mohla definovat
// v pořadí jakém půjdou
// potřebuji to jen u eventů, které se řetězí (tudíž vytváří v sobě instanci následujího
// eventu), v tomto případě Person, Person2-4
class Person : public Event
{
    public:
        void Behaviour(); //funkce behavior musí být vždy přepsaná
                          //určuje, co event udělá až bude volaný
};

class Person2 : public Event
{
    public:
        Person2(Event* parent); //konstruktor Person2, defaultně se použije konstruktor Eventu
                                //(který nastaví prioritu na 0 a event_id na nové volné a proměnnou repeat_itself,
                                //která určuje generátor, na false)
                                //když se přidá vlastní konstruktor, může se při vytváření změnit vlastnosti
                                //zde chceme event co ho vytváří, abychom si z něj vzali id a jméno (kvůli kontrolním
                                //výpisům, jsou to samostatné eventy ale pro účely simulace aby se zdálo, že to je
                                //jeden event)
        void Behaviour();
};

class Person3 : public Event
{
    public:
        Person3(Event* parent);
        void Behaviour();
};

class Person4 : public Event
{
    public:
        Person4(Event* parent);
        void Behaviour();
};
//konec forward definicí

//co bude event Person dělat až se zavolá
inline void Person::Behaviour() {
    //zavolá se log, který vytvoří na stdout zprávu o čase simulace, jménu eventu a touto zprávou
    Log::EventState(this, "enters system and attempts to get unit from storage");
    //pokusí se vzít jednotku ze storage
    //protože storage je sklad s omezenou kapacitou queue (viz main), musíme ošetřit případ, 
    //kdy by byla queue plná (funkce SeizeStorage by vrátila false; není-li omezený, vždy vrátí true)
    //SeizeStorage volám s parametrem jména skladu a eventem, který bude pokračovat v momentě, kdy
    //se dostane jednotka ze skladu
    //[!!!] musí to být nový event jiného druhu, a tento event Person zde končí, po SeizeStorage
    //nesmí být jiné akce, protože odtud už to přebírá druhý event
    //Person2 se předá this, aby si vzal jméno a id, to tu chceme aby se jmenoval stejně a
    //simulace dávala smysl (nemusí to být vždy tho)
    if(!Simulator::SeizeStorage("storage", new Person2(this))) {
        //zde se dostane když byla queue plná, může zde třeba počkat, nebo cokoliv
        //v tomto případě jen vypíše, že byl znechucen, a opustí systém
        //NOTE správně by new Person2(this) mělo být ještě před ifem a tady by se mělo
        //nad nově vytvořeným eventem zavolat "delete Event", aby nenastaly memory leaks
        //ale došlo mi to až po tom, co jsem napsala komenty, a už se mi to nechce měnit
        Log::EventState(this, "didn't get storage unit and queue was full so event leaft system");
    }
}

inline Person2::Person2(Event* parent) {
    //konstruktor Person2, který převezme od eventu co ho vytvořil vlastnosti, protože to chceme
    //aby navenek vypadaly jako jeden
    //NOTE pořád se volá i konstruktor Event(), nevím v jakém pořadí a asi to není potřeba zkoumat,
    //just so you know že nemusíš ošetřovat chybějící vlastnosti
    event_name = parent->event_name;
    event_id = parent->event_id;
    priority = parent->priority;
}
inline void Person2::Behaviour() {
    Log::EventState(this, "got unit from storage and start to wait 5s");
    //Wait funguje podobně jako Storage nebo Facility Seize
    //Po Wait už nesmí být nic jiného v Behaviour aby simulace dávala smysl
    //Wait se předá čas čekání a nově vytvořený event, který "pokračuje" v naší řadě chování
    //NOTE Wait se dá použít i při generátoru jako alternativa k přístupu o kus níže
    Simulator::Wait(5.0, new Person3(this));
}

inline Person3::Person3(Event* parent) {
    event_name = parent->event_name;
    event_id = parent->event_id;
    priority = parent->priority;
}
inline void Person3::Behaviour() {
    Log::EventState(this, "after waiting, releases storage unit and leaves system");
    //Release Storage unit
    //Narozdíl od Seize, po Release může následovat další behaviour a není potřeba vytvářet žádné další
    //eventy. Rozkouskovat eventy je potřeba jen při Seize nebo Wait!
    Simulator::ReleaseStorage("storage");
}

//Generátor pro příchod eventů Person
class PersonGenerator : public EventGenerator
{
    private:
        int person_id;          //Privátní proměnná pro pojmenování eventů vycházejících z generátoru
                                //Toto tu vůbec nemusí být
    public:
        PersonGenerator() {     //Konstuktor pro PersonGenerator vůbec nemusí být uveden
            person_id = 0;      //Zde je jen pro umístění počáteční hodnoty pro person_id
                                //NOTE všechny inicializace proměnných pro Eventy i Generátory
                                //musí být ve vlastním konstruktoru, ne v Behaviour, pokud se mají volat
                                //opakovaně a být stejné při každém zavolání
                                //(generátor se vytváří jednou, ale Behaviour se volá opakovaně)
        }
        void Behaviour() {
            //time je vnitřní proměnná generátoru, která udává čas na kdy se naplánuje událost
            //při ScheduleEvent
            //když se dojde do Behaviour, tedy Generátor je spuštěný, její hodnota je aktuální
            //simulační čas Simulator::last_effective_time. Ofc se může použít tato proměnná rovnou
            //NOTE: time je u všech eventů, ale u nich by se neměl přímo používat
            //hodnota time je totiž správná POUZE pokud se event nedostal do žádné fronty,
            //poté může být lehce nestabilní (neměl by, ale prostě time používat jen u generátorů kde je to safe)

            //time tu zvyšuji o jedna na ukázku
            //reálně by tu bylo nějaké roložení aby to bylo náhodné, třeba Exponential(1)
            time += 1;
            //vytvoření nové instance eventu Person
            Event *new_p = new Person();
            //nastavení jména na PersonX. Defaultně by se používalo pro výpis EventXX, ale tam je víc eventů
            //a není to přehledné tolik
            new_p->event_name = "Person" + std::to_string(person_id);
            person_id += 1;
            //ScheduleEvent nový Person na nový vygenerovaný čas
            Simulator::ScheduleEvent(new_p, time);
            //ScheduleEvent: sebe na nový čas, aby vygeneroval dalšího
            //takto generje jednoho po druhém
            Simulator::ScheduleEvent(this, time);
        }
};

int main()
{
    //Init simulátoru - základní příkaz bez kterého to nepojede
    //nastavila jsem tu počáteční čas na 0, koncový na 10
    Simulator::Init(0.0, 10.0);
    //ScheduleEvent zařadí event PersonGenerator na čas 0.0
    //je to jediný event zde, zbytek eventů se generuje v generátoru přímo
    //pozor, kdybys nastavil špatný čas mimo simulaci, odignoroval by se a nic by nedělal
    Simulator::ScheduleEvent(new PersonGenerator(), 0.0);
    //vytvoření Storage se jménem "storage", kapacitou 2 a jeho queue má limit 1
    //parametr limit jde vynechat, pak je nekonečná
    //nebo tam jde dát 0, pak nemá queue (asi neozkoušeno tho)
    //úplně stejně se vytváří Facility, akorát nemá parametr kapacitu, jen jméno nebo jméno + queue_limit
    //s Facility se potom pracuje stejně, SeizeFacility/ReleaseFacility jako se Storage
    Simulator::CreateStorage("storage", 2, 1);
    //Logne stav simulátoru v ento moment na stdout
    //jde volat i v eventech ofc
    Log::SimulatorState();
    //spustí simulaci
    //po konci RUn se všechno hezky zruší a dealokuje, takže bacha
    Simulator::Run();
    return 0;
}