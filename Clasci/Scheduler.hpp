/*
 * Plik z deklaracjami funkcji schedulera systemu ClasciOS, definiuje funkcje
 * jakich mozna uayzwac w systemie, ich definicje mozna znalesc oczywiscie w
 * pliku Scheduler.cpp
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-08
 * Data ostatniej modyfikajci: 2022-02-24
 */

#pragma once

/* Definiuje domyslna maksymalna liczbe procesow */
#ifndef PROCESSES_STACK_SIZE
	#define PROCESSES_STACK_SIZE 4
#endif

/* Oblicza PID ostatniego procesu */
#define MAX_PROCESS PROCESSES_STACK_SIZE - 1

#include "Process.hpp"

namespace Clasci{
	
	class Scheduler{
		/*
		 * Statyczna klasa schedulera systemu, statyczna klasa, a nie namespace
		 * poniewaz posiada rowniez pola ukryte, co nie bylo by mozliwe z uzywajac
		 * namespace a nie statycznej klasy
		 */
		 
		public:
			/* Funkcja tworzy nowy proces */
			static void createProcess(NewProcess);
			
			/* Ta za wykonanie odpowiedniego w nowym kontekscie */
			static void switchContext();
			
			/* Ta funkcja czysci tablice procesow, oraz inicjuje scheduler */
			static void clearProcessesStack();
			
			/* Przechowuje PID aktualnie dzialajacego procesu */
			static int actual_pid;
			
			/* Przechowuje pracujace w tle procesy */
			volatile static WorkProcess processes_stack[PROCESSES_STACK_SIZE];
	};
	
}
