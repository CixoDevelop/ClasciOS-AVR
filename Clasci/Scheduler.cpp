/*
 * Ten plik przechowuje definicje funkcji schedulera, ich deklaracje znajduja
 * sie w pliku Scheduler.hpp i tamten plik naglowkowy powinien byc domyslnie
 * dolaczany, a ten tylko w formie pliku .o
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-08
 * Data ostatniej modyfikajci: 2022-03-08
 */
 
#include "Scheduler.hpp"
#include "Platform.hpp"

namespace Clasci{

	/* 
	 * Przypomnienie kompilatorowi o zmiennych statycznych, poniewaz GCC < 5.4
	 * nie wspiera zmiennych inline co rodzi problemy na wielu systemach
	 */
	PID Scheduler::actual_pid;
	volatile Process Scheduler::processes_stack[];

	PID Scheduler::createProcess(
		ProcessStatus (*new_loop)(), ProcessStatus new_status
	  ){
		/*
		 * Funckja odpowiada za utworzenie nowego procesu w systemie, jako
		 * parametr dostaje strukture tego procesu, nastepnie ustawia wskaznik
		 * na najwyzszy proces i przeszukuje tablice procesow w poszukiwaniu
		 * miejsca na jego utworzenie, gdy juz znajdzie to ustawia status na
		 * ten jaki podano w parametrze, przepisuje funkcje loop i konczy
		 */ 
		
		for(
			volatile Process *test_process = &processes_stack[MAX_PROCESS];
			test_process >= processes_stack;
			test_process --
		  ){
			if(test_process->status == EMPTY){
				test_process->status = new_status;
				test_process->loop = new_loop;
				return test_process - processes_stack;
			}
		}

		return ERROR_PROCESSES_LIMIT;
	}
	
	void Scheduler::switchContext(){
		/*
		 * Funckja odpowiada za przerwanie kontekstu aplikacji, znalezienie
		 * procesu jaki powinien zostac wykonany (ma status RUNNING) a potem
		 * jego wykonanie i powrot do aplikacji
		 */
		
		/* Zatrzymaj odliczanie do kolejnego przerwania */
		Platform::pauseSchedulerINT();
		
		/* 
		 * Przeszukaj tablice procesow, zaczynajac od aktualnego, i wykonaj
		 * pierwszy jaki ma status RUNNING, jezeli po przeszukaniu calej
		 * tablicy nie ma ani jednego procesu RUNNING to przerwij i wroc do
		 * aplikajci
		 */
		 
		/* Odlicza aby przerwac gdyby zadnego RUNNING nie bylo */
		for(
			PID process_counter = MAX_PROCESS; 
			process_counter > 0; 
			process_counter --
		  ){
			if(--actual_pid < 0)
				actual_pid = MAX_PROCESS;

			if(processes_stack[actual_pid].status == RUNNING){
				processes_stack[actual_pid].status = 
					processes_stack[actual_pid].loop();
				break;
			}
		}
		
		/* Resetuje odliczanie i wznow je */
		Platform::resetSchedulerINT();
		Platform::playSchedulerINT();
	}
	
	void Scheduler::clearProcessesStack(){
		/*
		 * Funkcja czysci tablice procesow schedulera, ustawiajac kazdy proces
		 * jako nieistniejacy
		 */
				 
		for(actual_pid = MAX_PROCESS; actual_pid >= 0; actual_pid --)
			processes_stack[actual_pid].status = EMPTY;
	}

	bool Scheduler::changeProcessStatuss(
		PID process_pid, ProcessStatus new_status
	  ){
		/*
		 * Funckja zmienia status procesu process_pid na new_status, zwraca
		 * true jezeli sie uda lub false jezeli proces nie istnieje
		 */

		if(processes_stack[process_pid] != EMPTY){
			processes_stack[process_pid] = new_status;
			return true;
	  	}

		return false;
	}

	bool Scheduler::setNextProcess(PID process_pid){
		/*
		 * Jezeli proces istnieje, to ustawia scheduler by przy kolejnym 
		 * wywlaszczeniu wykonal wlasnie ten proces oraz zwraca true, w 
		 * przeciwnym wypadku zwraca false
		 */

		if(processes_stack[process_pid] == RUNNING){
			actual_pid = process_pid + 1;
			return true;
		}
		
		return false;
	}
}
