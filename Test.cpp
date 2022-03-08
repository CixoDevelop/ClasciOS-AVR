#include "Clasci/Process.hpp"
#include "Clasci/Scheduler.hpp"
#include "Clasci/Platform.hpp"

#include <avr/io.h>

Clasci::ProcessStatus ProcessOne(){
	if(PORTA & _BV(2))
		PORTA &= ~_BV(2);
	else
		PORTA |= _BV(2);
	return Clasci::RUNNING;
}

Clasci::ProcessStatus ProcessTwo(){
	if(PORTA & _BV(1))
		PORTA &= ~_BV(1);
	else
		PORTA |= _BV(1);
	return Clasci::RUNNING;
}

int main(){
	Clasci::Scheduler::clearProcessesStack();
	
	DDRA |= _BV(2);
	Clasci::Scheduler::createProcess(ProcessOne);

	DDRA |= _BV(1);
	PORTA |= _BV(1);
	Clasci::Scheduler::createProcess(ProcessTwo);
	
	Platform::setupSchedulerINT();
	
	DDRA |= _BV(3);
	DDRA |= _BV(7);
	
	DDRB = 0b01111111;
	unsigned int value = 0;

	while(true){
		
		PORTB = value & 0b01111111;
		PORTA = (PORTA & 0b00001110) | (value & 0b10000000);

		if(PORTA & _BV(3))
			PORTA &= ~_BV(3);
		else
			PORTA |= _BV(3);
		
		for(long y = 32200; y > 0; y--)
			asm("nop");
			
		value = value + 1;
		if(value == 255)
			value = 0;
	}
}
