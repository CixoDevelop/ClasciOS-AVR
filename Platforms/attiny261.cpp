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
		criticalStop();
		
		TIMSK |= (1<<TOIE0) | (1<<TOV0);
		
		sei();
	}
	
	void criticalStop(){
		/* Uruchamia odliczanie TIMER0 */

		TCCR0B = SCHEDULER_INT;
	}
	
	void criticalStart(){
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

	/* Zapisuje kontekst */
	asm("push R31");
	asm("in R31, __SREG__");
	asm("push R31");
	asm("push R0");
	asm("push R1");
	asm("push R2");
	asm("push R3");
	asm("push R4");
	asm("push R5");
	asm("push R6");
	asm("push R7");
	asm("push R8");
	asm("push R9");
	asm("push R10");
	asm("push R11");
	asm("push R12");
	asm("push R13");
	asm("push R14");
	asm("push R15");
	asm("push R16");
	asm("push R17");
	asm("push R18");
	asm("push R19");
	asm("push R20");
	asm("push R21");
	asm("push R22");
	asm("push R23");
	asm("push R24");
	asm("push R25");
	asm("push R26");
	asm("push R27");
	asm("push R28");
	asm("push R29");
	asm("push R30");

	/* Uruchamia scheduler systemu */
	Clasci::Scheduler::switchContext();

	/* Przywraca kontekst */
	asm("pop R30");
	asm("pop R29");
	asm("pop R28");
	asm("pop R27");
	asm("pop R26");
	asm("pop R25");
	asm("pop R24");
	asm("pop R23");
	asm("pop R22");
	asm("pop R21");
	asm("pop R20");
	asm("pop R19");
	asm("pop R18");
	asm("pop R17");
	asm("pop R16");
	asm("pop R15");
	asm("pop R14");
	asm("pop R13");
	asm("pop R12");
	asm("pop R11");
	asm("pop R10");
	asm("pop R9");
	asm("pop R8");
	asm("pop R7");
	asm("pop R6");
	asm("pop R5");	
	asm("pop R4");
	asm("pop R3");
	asm("pop R2");
	asm("pop R1");
	asm("pop R0");
	asm("pop R31");
	asm("out __SREG__, R31");
	asm("pop R31");
}
