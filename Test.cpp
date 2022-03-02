#include "Clasci/Process.hpp"
#include "Clasci/Scheduler.hpp"
#include "Clasci/Platform.hpp"

#include <avr/io.h>

namespace ProcessOne{
	Clasci::ProcessStatus init(int pid){
		DDRA |= _BV(2);
		return Clasci::RUNNING;
	}
	
	Clasci::ProcessStatus loop(){
		if(PORTA & _BV(2))
			PORTA &= ~_BV(2);
		else
			PORTA |= _BV(2);
		return Clasci::RUNNING;
	}
}

namespace ProcessTwo{
	Clasci::ProcessStatus init(int pid){
		DDRA |= _BV(1);
		PORTA |= _BV(1);
		return Clasci::RUNNING;
	}
	
	Clasci::ProcessStatus loop(){
		if(PORTA & _BV(1))
			PORTA &= ~_BV(1);
		else
			PORTA |= _BV(1);
		return Clasci::RUNNING;
	}
}

int main(){
	Clasci::Scheduler::clearProcessesStack();
	
	Clasci::Scheduler::createProcess({ProcessOne::init, ProcessOne::loop});
	Clasci::Scheduler::createProcess({ProcessTwo::init, ProcessTwo::loop});
	
	Platform::setupSchedulerINT();
	
	DDRA |= _BV(3);
	
	DDRB = 0xff;
	PORTB = 0;

	while(true){
		
		if(PORTA & _BV(3))
			PORTA &= ~_BV(3);
		else
			PORTA |= _BV(3);
		
		for(long y = 32200; y > 0; y--)
			asm("nop");
		for(long y = 32200; y > 0; y--)
			asm("nop");
			
		PORTB = PORTB + 1;
		if(PORTB == 32)
			PORTB = 0;
	}
}
