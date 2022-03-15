/*
 * Plik z deklaracjami funkcji schedulera systemu ClasciOS, definiuje funkcje
 * jakich mozna uayzwac w systemie, ich definicje mozna znalesc oczywiscie w
 * pliku Scheduler.cpp
 * 
 * Autor: Cixo
 */

#pragma once

/* Definiuje domyslna maksymalna liczbe procesow */
#ifndef PROCESSES_STACK_SIZE
	#define PROCESSES_STACK_SIZE 4
#endif

/* Definiuje ERROR_PROCESSES_LIMIT */
#define ERROR_PROCESSES_LIMIT PROCESSES_STACK_SIZE

/* Oblicza PID ostatniego procesu */
#define MAX_PROCESS PROCESSES_STACK_SIZE - 1

/* Definiuje typ PID */
typedef int PID; 

#include "Process.hpp"

namespace Clasci{
	
	class Scheduler{
		/*
		 * Statyczna klasa schedulera systemu, statyczna klasa, a nie namespace
		 * poniewaz posiada rowniez pola ukryte, co nie bylo by mozliwe z 
		 * uzywajac namespace a nie statycznej klasy
		 */
		 
		public:
			/* 
			 * Funkcja tworzy nowy proces, jako parametry przyjmuje wpierw
			 * adres do funkcji loop procesu, natomiast jako drugi argument
			 * stan jaki przyjmie proces po poprawnym dodaniu. Zwraca PID 
			 * procesu lub ERROR_PROCESSES_LIMIT 
			 */
			static PID createProcess(ProcessStatus(*)(void*),  void *, ProcessStatus = RUNNING);
			
			/* 
			 * Ta za wykonanie odpowiedniego w nowym kontekscie. Powinna być
			 * wykonywana tylko przez ISR oraz przez funkcje systemowe
			 */
			static void switchContext();
			
			/* 
			 * Ta funkcja czysci tablice procesow, oraz inicjuje scheduler. 
			 * Powinna zostac wykonana przed inicjalizacja platformy oraz 
			 * gdy zachodzi potrzeba usuniecia wszystkich procesow
			 */
			static void clearProcessesStack();

			/*
			 * Ta funkcja odpowiada za zmiane stanu procesu podanego w PID
			 * na stan podany w ProcessStatus. Zwraca true jezeli sie udalo
			 * lub false jezeli proces nie istnieje
			 */
			static bool changeProcessStatuss(PID, ProcessStatus);

			/*
			 * Ta funkcja ustawia jaki proces bedzie wykonany jako kolejny,
			 * kiedy scheduler ponownie odczyta tablice, zwraca true jezeli
			 * sie udalo, lub false jezeli proces nie istnieje
			 */
			static bool setNextProcess(PID);
			
		private:	
			/* Przechowuje PID aktualnie dzialajacego procesu */
			static PID actual_pid;
			
			/* Przechowuje pracujace w tle procesy */
			volatile static Process processes_stack[PROCESSES_STACK_SIZE];
	};
	
}
