/*
 * Plik platformy dla MCUL attiny261 oraz attiny261a
 * Plik powinien zostac skompilowany do pliku .o a nastepnie dolaczany na
 * etapie kompilacji calego projektu, jezeli ten jest oparty o ta platforme
 * 
 * Uwaga: 
 * 	UZYWA TIMER0 CELEM OKRESOWEGO URUCHAMIANA SCHEDULERA, NALEZY WZIASC TO POD
 *	UWAGE PODCZAS PISANIA SWOJEJ APLIKACJI, ZMIENIANIE TAM WARTOSCI BEDZIE
 * 	SKUTKOWALO NIESTABILNA PRACA SYSTEMU!!!
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-18
 * Data ostatniej modyfikajci: 2022-02-24
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "../Clasci/Platform.hpp"
#include "../Clasci/Scheduler.hpp"

/* Pozwala zmienic czestotliwosc uruchamiania schedulera */
#ifndef SCHEDULER_INT
	#define SCHEDULER_INT (1<<CS02)
#endif

namespace Platform{
	
	void setupSchedulerINT(){
		/* Ta funkcja uruchamia przerwania na TIMER0, oraz resetuje i wlacza go */

		cli();
		
		resetSchedulerINT();
		playSchedulerINT();
		
		TIMSK |= (1<<TOIE0) | (1<<TOV0);
		
		sei();
	}
	
	void playSchedulerINT(){
		/* Uruchamia odliczanie TIMER0 */

		TCCR0B = SCHEDULER_INT;
	}
	
	void pauseSchedulerINT(){
		/* Wylacza odliczanie TIMER0 */

		TCCR0B = 0;
	}
	
	void resetSchedulerINT(){
		/* Resetuje timer TIMER0 */

		TCNT0H = 0;
		TCNT0L = 0;
	}
}

ISR(TIMER0_OVF_vect){
	/* Uruchamia scheduler za kazdym razem, kiedy TIMER0 zostanie przepelniony */

	Clasci::Scheduler::switchContext();
}
