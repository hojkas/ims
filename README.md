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

[X] eventy

[&#10003;] basic funkce simulátoru, aka práce s časem a next event handling

[X] random number generator (viz přednášky slide 167, tyto implementovat)

### Druhá vlna - SHO

[X] Zapracovat do simulátoru podporu sho, aka věci jako že po eventu co sahá na facility se checkne queue té facility atd

[X] Queue

[X] Storage, Facility

### Třetí vlna - Simulace

[X] Najít ukázkovou netriviální simulaci

[X] Naprogramovat ji pomocí našeho simulátoru

### Čtvrtá vlna - Dokumentace

[X] Dokumentace každé části simulátoru, jak se chovají a jak je zvnějšku používat

[X] U random number generator udělat výstupy z mnohonásobného generování různých rozložení a udělat grafy, aby se ukázalo, že fakt fungují

[X] Dokumentace k simulaci

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
