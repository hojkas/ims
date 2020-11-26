# IMS

## Téma č. 4: Implementace diskrétního simulátoru s podporou SHO

Implementujte vlastní diskrétní simulátor založený na procesech nebo událostech. Implementujte podporu pro SHO (fronty, linky) a generování pseudonáhodných čísel (pro různá rozložení). Demonstrujte na hypotetickém modelu SHO s několika linkami, různými procesy příchodů apod.

(yup, that's the whole assignment)

## Possible components

* Simulátor - založený na procesech nebo událostech, procesy zní víc ez but what do I know
  * Process - (??) posloupnost akcí jako zaber linku, počkej x, opusť linku...
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
