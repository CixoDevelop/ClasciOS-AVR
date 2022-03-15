/*
 * Ten plik przechowuje definicje funkcji schedulera, ich deklaracje znajduja
 * sie w pliku Scheduler.hpp i tamten plik naglowkowy powinien byc domyslnie
 * dolaczany, a ten tylko w formie pliku .o
 * 
 * Autor: Cixo
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
		ProcessStatus (*new_loop)(void*), void *process_data, ProcessStatus new_status
	  ){
		/*
		 * Funckja odpowiada za utworzenie nowego procesu w systemie, jako
		 * parametr dostaje strukture tego procesu, nastepnie ustawia wskaznik
		 * na najwyzszy proces i przeszukuje tablice procesow w poszukiwaniu
		 * miejsca na jego utworzenie, gdy juz znajdzie to ustawia status na
		 * ten jaki podano w parametrze, przepisuje funkcje loop, adres danych
		 * oraz status i konczy
		 */ 
		
		for(
			volatile Process *test_process = &processes_stack[MAX_PROCESS];
			test_process >= processes_stack;
			test_process --
		  ){
			if(test_process->status == EMPTY){
				test_process->status = new_status;
				test_process->loop = new_loop;
				test_process->process_data = process_data;
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
		Platform::criticalStart();
		
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
			if(actual_pid-- < 0)
				actual_pid = MAX_PROCESS;

			if(processes_stack[actual_pid].status == RUNNING){
				processes_stack[actual_pid].status = 
					processes_stack[actual_pid].loop(processes_stack[actual_pid].process_data);
				break;
			}
		}
		
		/* Resetuje odliczanie i wznow je */
		Platform::resetSchedulerINT();
		Platform::criticalStop();
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

		if(processes_stack[process_pid].status != EMPTY){
			processes_stack[process_pid].status = new_status;
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

		if(processes_stack[process_pid].status == RUNNING){
			actual_pid = process_pid + 1;
			return true;
		}
		
		return false;
	}
}
