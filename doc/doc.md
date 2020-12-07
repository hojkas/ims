<div style="text-align:center; margin-top:38.2%">
    <hspace></hspace>
    <h1>
        Dokumentace
    </h1>
    <h3>
        Implementace diskrétního simulátoru s podporou SHO
    </h3>
    <p style="margin-top:15%"></p>
    <h3>
        IMS - Modelování a simulace
    </h3>
    <p style="margin-top:15%"></p>
    Iveta Strnadová<br>
    xstrna14<br>
    Denis Lebó<br>
    xlebod00
</div>
<p style="text-align:center; margin-top:60%">7. prosinec 2020</p>




<div style="page-break-after: always; break-after: page;"></div>

<h1>Obsah</h1>

[toc]

<div style="page-break-after: always; break-after: page;"></div>

# 1. Úvod

Práce se zabývá vývojem simulátoru založeného na událostech s podporou systému hromadné obsluhy. Tento systém popisuje, vysvětluje pro použití a na zvolené simulaci ho implementuje.

## Zadání

**Téma č. 4: Implementace diskrétního simulátoru s podporou SHO**

Implementujte vlastní diskrétní simulátor založený na procesech nebo událostech. Implementujte podporu pro SHO (fronty, linky) a generování pseudonáhodných čísel (pro různá rozložení). Demonstrujte na hypotetickém modelu SHO s několika linkami, různými procesy příchodů apod.

## Pojmy

Kvůli implementaci simulátoru v anglickém jazyce (pro snazší porovnání s ostatními simulátory, např. SIMLIB, kterým byly názvy tříd a funkcí částečně inspirovány) jsou v této práci používány pro popis české a u konkrétní implementace anglické výrazy. Pro snazší zorientování čtenáře jsou zde vypsány ty nejpoužívanější a jejich ekvivalenty v druhém jazyce.

event = událost
seize = zabrat
release = uvolnit
storage = sklad
zařízení = facility

<div style="page-break-after: always; break-after: page;"></div>

# 2. Simulátor

## Stavba

Jeden hlavičkový soubor `discrete_simulator.hpp` obsahuje deklarace všech tříd a funkcí.  Definice jsou umístěny v několika souborech podle funkce.

### Simulátor (`simulator.cpp`)

Statická implementace třídy `Simulator` funguje jako hlavní rozhraní mezi uživatelem a simulátorem. Zajišťuje inicializaci simulátoru a jeho spuštění, tj. procházení naplánovaných eventů a jejich vykonávání a obsahuje funkci pro naplánování eventů.

Procházení eventů, akce po spuštění simulace příkazem `Run()`, probíhá vybíráním prvního eventu z jejich seznamu a spuštěním jeho funkce `Behaviour()`. Při vkládání eventu je zařazen podle času a shodují-li se časy, ten s vyšší prioritou má přednost.

Simulátor také obsahuje mapy všech vytvořených zařízení a skladů, které uživateli umožňuje vytvářet a které se mohou eventy pokusit zabrat a následně opustit. Při pokusu o zabrání zařízení či skladu navíc umožňuje nastavit timeout pro čekání ve frontě při nedostatku kapacity.

### Events (`events.cpp`)

Existují zde tři druhy tříd eventů.

Třída `Event` je základem všech eventů. Pro vytvoření vlastního eventu uživateli stačí vytvořit novou třídu dědící z Event a definovat její `Behaviour()`, tedy funkci, která se provede v momentě, kdy je event vybrán z naplánovaných a spuštěn.

Třída `EventGenerator` existuje stejně jako Event jako rodičovská třída, z níž si uživatel vytvoří vlastní. Oproti Event obsahuje navíc proměnnou, která zajistí, že se po provedení jejího `Behaviour()` v simulátoru neodstraní.

Třída `TimeoutWatch` není přístupná pro uživatele. V případě, že uživatel bude chtít zabrat sklad/zařízení s timeoutem, naplánuje se po daném čase a pokud najde sledovaný event stále čekající ve frontě, vyjme ho a spustí event předaný uživatelem pro případ timeoutu.

### Generátor náhodných rozložení (`random_generator.cpp`)

Pseudo generátor náhodných rozložení pro svou funkci využívá náhodného seed, které se vygeneruje při inicializaci simulátoru a vypíše na standartní výstup. Uživatel může tuto hodnotu změnit na vlastní.

Generátor obsahuje funkci pro generování následujících náhodných čísel:

#### Random - náhodné od 0 do 1

TODO Denis - popsat jak a graf jak to vychází

#### Uniformní

TODO Denis - popsat jak a graf jak to vychází

#### Exponenciální

TODO Denis - popsat jak a graf jak to vychází

#### Normální

TODO Denis - popsat jak a graf jak to vychází

### Systém hromadné obsluhy (`sho.cpp`)

Obsahuje třídy pro podporu SHO. Všechny jsou uživateli nepřístupné, vytváří a používá je pouze pomocí funkcí v simulátoru k tomu určené.

`Queue` implementuje frontu čekajících eventů v případě plné kapacity zařízení/skladu Může mít omezený limit nebo nemusí (popř. může být nulová). Je-li při novém požadavku na zabrání plná, uživateli se o tom vrací informace. Příchozí eventy řadí podle priority, vyšší priorita má přednost.

`Storage`/`Facility` jsou podobné třídy. Jediný rozdíl je v kapacitě, zatímco sklad může mít libovolnou, zařízení má vždy jen jednu. Obě třídy sledují volnou kapacitu. Spouští eventy, které se je pokusí zabrat pokud je kapacita volná, nebo je ukládají do fronty když kapacita volná není. Při uvolnění jejich kapacity spouští event, který je vpředu fronty.

Jsou rozpoznávány pomocí jejich jména. Musí být definovány před spuštěním simulace a pokud dojde k redefinici nebo použití nedefinovaného skladu/zařízení, program se ukončí.

### Informační výpisy (`statistics.cpp`)

Obsahuje statickou třídu `Log` obstarávající výpisy o stavu simulátoru a o aktuálně spouštěném eventu.

## Návod k použití

### Připojení simulátoru a nezbytné funkce

```cpp
#include "discrete_simulator.hpp"
int main()
{
    double start_time = 0.0;
    double end_time = 10.0;
    Simulator::Init(start_time, end_time);
    Simulator::Run();
}
```

`Simulator::Init(start_time, end_time)` - inicializuje simulátor a nastaví čas simulace. `Simulator::Run()` - spustí simulaci podle toho, co bylo definováno mezi `Init` a `Run`. Je důležité, aby případné další funkce byly volané mezi těmito funkcemi.

### Vytváření a plánování eventů

```cpp
class Person : public Event {
    public:
        void Behaviour() {
            Log::EventState(this, "Person appeared in the system.")
        }
};
class PersonGenerator : public EventGenerator {
    public:
    	void Behaviour() {
            double next_time = Simulator::last_effective_time + 1;
            Simulator::ScheduleEvent(new Person(), next_time);
            Simulator::ScheduleEvent(this, next_time);
        }
}
int main() {
    Simulator::Init(0.0, 2.0);
    Simulator::ScheduleEvent(new PersonGenerator(), 0.0);
    Simulator::Run();
}
//výstup:
//[1]	Event#1: Person appeared in the system.
//[2]	Event#2: Person appeared in the system.
```

Třída pro vlastní event i generátor se vytvoří děděním z `Event`/`EventGenerator` a napsáním vlastního `Behaviour`. V tomto případě generátor vytvoří event Person a naplánuje sebe i jeho do kalendáře událostí za 1 (např. 1 sekundu, záleží jak se na čas díváme).

Proměnná `Simulator::last_effective_time` v sobě uchovává čas posledního spuštěného eventu.

`Simulator::ScheduleEvent(*event, time)` - naplánuje událost event na čas simulace time

### Další funkce simulátoru

`Simulator::Wait(time_to_wait, *event)` - počká daný čas a poté spustí přiložený event

`Simulator::CreateStorage(name, capacity)`/`Simulator::CreateStorage(name, capacity, queue_limit)` - vytvoří sklad daného jména, kapacity a popř. s omezenou frontou

`Simulator::CreateFacility(name)`/`Simulator::CreateFacility(name, queue_limit)` - vytvoří zařízení daného jména, popř. s omezenou linkou

Následující funkce jsou uvedeny pouze pro `Storage`, protože pro `Facility` fungují úplně stejně.

`Simulator::SeizeStorage(name, *event)` - pokusí se zabrat sklad, daný event se vykoná až se dostane na řadu, vrací `false` u plné fronty

`Simulator::SeizeStorageWithTimeout(name, timeout_time, *on_success_event, *on_timeout_event)` - pokusí se zabrat sklad; pokud zůstane ve frontě a bude tam i po uplynutí timeout_time, on_success_event se z ní vyjme, zahodí a provede se místo něj on_timeout_event

`Simulator::ReleaseStorage(name)` - uvolní jednotku ze skladu, pokud byl ve frontě event, umístí se jako příští provedený

**Poznámka**

Simulátor založený na událostech (eventech) je v určitých ohledech omezující. Funkce, které vyžadují v sobě odkaz na event(y) vyžadují speciální zacházení (tj. funkce `Wait`, `SeizeStorage/Facility` a `SeizeStorage/FacilityWithTimeout`).

Protože jejich roli v simulaci “přebírají” přiložené eventy, které se mohou vykonat ihned nebo až po chvíli, event který tyto funkce volá ve svém `Behaviour` již nesmí provést žádnou jinou funkci. Jedinou výjimkou je zpracování hodnoty false, kterou mohou funkce `Seize` vrátit, protože v tom případě se přiložený event nikdy neprovede.

### Funkce z náhodného generátoru čísel

`RandomGenerator::SetSeed(seed)` - nastaví hodnotu seed na požadovanou hodnotu

`RandomGenerator::Random()` - vrátí náhodnou hodnotu z intervalu od 0 do 1

`RandomGenerator::Uniform(low, high)` - vrátí náhodnou hodnotu z intervalu od low do high

`RandomGenerator::Exponential(mean)` - vrátí náhodnou hodnotu z exponenciální rozložení se středem v mean

`RandomGenerator::Normal(mean, std_deviation)` - vrátí náhodnout hodnotu z normálního rozložení se středem v mean a standartní odchylkou std_devition

**Poznámka:** RandomGenerator neošetřuje nevhodné záporné vstupy či výstupy z funkcí generujících náhodná rozložení. Je na uživateli, aby zvážil co generuje a ošetřil případné mezní případy.

### Funkce z `Log`

`Log::SimulatorState` - vypíše na stdout základní informace o simulátoru: čas, počet naplánovaných eventů a eventů pozastavených ve frontách, počet definovaných skladů/zařízení a podrobnější informace o nich

`Log::EventState(*event, msg)` - vypíše na stdout čas simulace, jméno nebo identifikátor předané události a danou zprávu msg

Podrobnější popis zde zmíněných funkcí, jejich parametrů a efektů na simulaci můžete najít v hlavičkovém souboru simulátoru (`discrete_simulator.hpp`). Ukázku použití prvků simulátoru najdete v implementaci simulace (`simulation.cpp`).

<div style="page-break-after: always; break-after: page;"></div>

# 3. Simulace

TODO Denis - zdroj příkladu a slovní zadání

## Abstraktní model

TODO Denis - tu petriho síť, to je abstraktní model

## …

TODO Denis - něco o implementaci simulace

<div style="page-break-after: always; break-after: page;"></div>

# 4. Závěr

Přiložené zdrojové kódy simulátoru se dají použít jako primitivní simulační knihovna založená na událostech. Potenciální uživatel může kromě této dokumentace využít k nalezení více informací hlavičkový zdrojový soubor či vytvořený program simulace obsahující většinu výše zmíněných konstrukcí.

TODO Denis nějaký závěr ze simulace? Z úpravy hodnot nebo z generátorů čísel?