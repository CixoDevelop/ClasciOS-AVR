/*
 * Ten plik naglowkowy przechowuje deklaracje funkcji platformy uzywanych przez
 * system oraz aplikacje i procesy. Funkcje te swoje ciala maja w pliku .o, 
 * ktory wybierany dla danej platformy i dolaczany na etapie kompilacji
 * 
 * Autor: Cixo
 * Data utworzenia: 2022-02-19
 * Data ostatniej modyfikajci: 2022-02-24
 */
 
#pragma once

namespace Platform{
	
	/* Funkcja konfiguruje przerwanie, oraz je uruchamia */
	void setupSchedulerINT();
	
	/* Funkcja wlacza timer przerwania */
	void playSchedulerINT();
	
	/* Funkcja pauzuje timer przerwania */
	void pauseSchedulerINT();
	
	/* Funkcja zeruje timer przerwania */
	void resetSchedulerINT();
}
