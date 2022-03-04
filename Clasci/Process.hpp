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
		/* Kontener przechowujacy dzialajacy proces. */

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
	} Process;
}
