/*
 * Plik przechowuje struktury i enumy odpowiedzialne za procesy. Nie posiada
 * swojego odpowiednika .cpp, poniewaz nie deklaruje zadnych funkcji, calosc
 * znajduje sie w przestrzeni nazw Clasci
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-08
 * Data ostatniej modyfikajci: 2022-02-24
 */

#pragma once

namespace Clasci{
	
	enum ProcessStatus{
		/*
		 * Typ wyliczeniowy uzywany do przechowywania stanu w jakim aktialnie
		 * znajduje sie proces. Jest uzywany przez scheduler by szybko moc 
		 * sie orientowac czy proces istnieje i moze zostac wykonany
		 */
		 
		/* Proces nie istnieje, mozna w tym miejscu umiescic inny proces */
		EMPTY = 0x00,
		
		/* Proces istnieje i bedzie wykonany gdy  */
		RUNNING = 0x01,
		
		/* Oznacza ze proces zostanie wykonany przy jakims przerwaniu */
		INT_SLEEP = 0x02,

		/* Oznacza ze proces jest zawieszony, ale nie moze zostac nadpisany */
		NONINT_SLEEP = 0x03,
	};
	
	
	typedef struct{
		/*
		 * Kontener przechowujacy dzialajacy proces. Jest tworzony przez 
		 * scheduler w momencie jego inicjalizacji, a nastepnie tylko zmieniany
		 * przez funkcje tworzaca proces. 
		 */

		/* 
		 * Ta zmienna przechowuje aktualny stan procesu i to ona jest brana pod
		 * uwage przez scheduler kiedy ten sprawdza czy proces moze zostac
		 * wykonany lub czy w ogole istnieje. Jezeli jest ustawiona na EMPTY to
		 * scheduler w zadnych okolicznosciach nie wykona procesu (czyli loop)
		 * poniewaz sa tam smieci. Kolejne stany opisane sa w ProcessStatus
		 */
		ProcessStatus status;
		
		/* 
		 * Funkcja loop procesu, nie dostaje parametrow, wartosc jaka zwroci to
		 * stan procesu po jej zakonczeniu, tak wiec aby zakonczyc proces i dac
		 * zwolnic miejsce kolejnym nalezy zwrocic EMPTY, aby proces dalej 
		 * dzialal RUNNING, przy dalszym oczekiwaniu na przerwanie INT_SLEEP, a
		 * przy oczekiwaniu na reakcje innego procesu lub aplikacji 
		 * NONINT_SLEEP. JEZELI TA FUNKCJA NIC NIE ZWROCI PROCES PRAWDOPODOBNIE 
		 * PRZESTANIE SIE WYKONYWAC PONIEWAZ DO status TRAFIA SMIECI!!!
		 */
		ProcessStatus (*loop)();
	} WorkProcess;
	

	typedef struct{
		/*
		 * Kontener przechowuje dane potrzebne do utworzenia nowego procesu,
		 * podczas tworzenia przez scheduler jest przeksztaucany w kontener
		 * WorkProcess, na tym etapie dodawane jest pole przechowujace stan
		 * procesu, za to funkcja init nie jest przechowywana, poniewaz
		 * spelnila juz swoje zadanie
		 */

		/* 
		 * Wskazuje na funkcje inicjujaca proces, PID to numer PID jaki proces
		 * otrzymal na etapie tworzenia, a funkcja ta zwraca stan w jakim jest
		 * proces po utworzeni, zwykle RUNNING, ale jezeli proces bedzie 
		 * odpowiadal na przerwanie to moze zwrocic chocby INT_SLEEP, a jezeli
		 * wyniknie jakis blad to zwrocenie EMPTY spowoduje ze proces nie 
		 * zostanie utworzony
		 */
		ProcessStatus (*init)(int PID);
		
		/* 
		 * Funkcja loop procesu, nie dostaje parametrow, wartosc jaka zwroci to
		 * stan procesu po jej zakonczeniu, tak wiec aby zakonczyc proces i dac
		 * zwolnic miejsce kolejnym nalezy zwrocic EMPTY, aby proces dalej 
		 * dzialal RUNNING, przy dalszym oczekiwaniu na przerwanie INT_SLEEP, a
		 * przy oczekiwaniu na reakcje innego procesu lub aplikacji 
		 * NONINT_SLEEP. JEZELI TA FUNKCJA NIC NIE ZWROCI PROCES PRAWDOPODOBNIE 
		 * PRZESTANIE SIE WYKONYWAC PONIEWAZ DO status TRAFIA SMIECI!!!
		 */
		ProcessStatus (*loop)();
	} NewProcess;
	
}
