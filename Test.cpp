/*
 * Plik jest przykladem dzialania systemu ClasciOS na mikrokontrolerze ATtiny261
 * Mruga on 3 diodami (Procesy) oraz miga diodami podlaczonymi do rejestru 
 * przesuwnego (Aplikacja)
 *
 * Autor: Cixo
 */

/* Dolacza naglowki systemu operacyjnego */
#include "Clasci/Process.hpp"
#include "Clasci/Scheduler.hpp"
#include "Clasci/Platform.hpp"

/* Dolacza naglowki sterownikow */
#include "Drivers/Pin.hpp"
#include "Drivers/ShiftRegister.hpp"

/* Dolacza naglowki dla mikrokontrolera */
#include <avr/io.h>

/* Funkcja procesu, ktora zamienia wartosc pinu */
Clasci::ProcessStatus PinInvert(void *process_data){
	/* Konwertuje na pin */
	Drivers::Pin *pin = (Drivers::Pin*)(process_data);
	/* Ustawia pin */
	pin->setState((Drivers::PinState)(!pin->getState()));
	/* Zwraca chec dalszej pracy */
	return Clasci::RUNNING;
}

int main(){
	/* Czysci stos procesow */
	Clasci::Scheduler::clearProcessesStack();

	/* Ustawia kolejne piny oraz tworzy procesy */

	Drivers::Pin psOne(&PINA, 1);
	Drivers::Pin psTwo(&PINA, 2);
	Drivers::Pin psThree(&PINA, 3);

	psOne = Drivers::OUT;
	psOne = Drivers::LOW;
	Clasci::Scheduler::createProcess(PinInvert, &psOne);

	psTwo = Drivers::OUT;
	psTwo = Drivers::LOW;
	Clasci::Scheduler::createProcess(PinInvert, &psTwo);

	psThree = Drivers::OUT;
	psThree = Drivers::LOW;
	Clasci::Scheduler::createProcess(PinInvert, &psThree);
	
	/* Uruchamia system operacyjny */
	Platform::setupSchedulerINT();

	/* Tworzy rejestr do ktorego podlaczone sa diody */
	Drivers::ShiftRegister LEDS(
		Drivers::Pin(&PINA, 4),
		Drivers::Pin(&PINA, 6),
		Drivers::Pin(&PINA, 7)	
	);

	/* W petli zamienia kolejne piny rejestru */
	while(true){
		for(int x = 8; x > 0; x--){
			for(long y = 32000; y > 0; y--)
				asm("nop");
			LEDS.setState(x - 1, (Drivers::PinState)(!LEDS.getState(x - 1)));
		}
	}
}
