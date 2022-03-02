/*
 * Ten plik przechowuje definicje funkcji schedulera, ich deklaracje znajduja
 * sie w pliku Scheduler.hpp i tamten plik naglowkowy powinien byc domyslnie
 * dolaczany, a ten tylko w formie pliku .o
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-08
 * Data ostatniej modyfikajci: 2022-02-24
 */
 
#include "Scheduler.hpp"
#include "Platform.hpp"

namespace Clasci{

	/* 
	 * Przypomnienie kompilatorowi o zmiennych statycznych, poniewaz GCC < 5.4
	 * nie wspiera zmiennych inline co rodzi problemy na wielu systemach
	 */
	int Scheduler::actual_pid;
	volatile WorkProcess Scheduler::processes_stack[];

	void Scheduler::createProcess(NewProcess new_process){
		/*
		 * Funckja odpowiada za utworzenie nowego procesu w systemie, jako
		 * parametr dostaje strukture tego procesu, nastepnie ustawia wskaznik
		 * na najwyzszy proces i przeszukuje tablice procesow w poszukiwaniu
		 * miejsca na jego utworzenie, gdy juz znajdzie to ustawia status na
		 * ten jaki zwrocila funkcja init, ustawia funkcje loop i konczy
		 */ 
		
		for(
			volatile WorkProcess *test_process = &processes_stack[MAX_PROCESS];
			test_process >= processes_stack;
			test_process --
		  ){
			if(test_process->status == EMPTY){
				test_process->status = new_process.init(test_process - processes_stack);
				test_process->loop = new_process.loop;
				break;
			}
		}
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
		for(int process_counter = MAX_PROCESS; process_counter > 0; process_counter --){
			if(--actual_pid < 0)
				actual_pid = MAX_PROCESS;

			if(processes_stack[actual_pid].status == RUNNING){
				processes_stack[actual_pid].status = processes_stack[actual_pid].loop();
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
}
