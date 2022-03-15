/*
 * Ten plik naglowkowy przechowuje deklaracje funkcji platformy uzywanych przez
 * system oraz aplikacje i procesy. Funkcje te swoje ciala maja w pliku .o, 
 * ktory wybierany dla danej platformy i dolaczany na etapie kompilacji
 * 
 * Autor: Cixo
 */
 
#pragma once

namespace Platform{
	
	/* Funkcja konfiguruje przerwanie, oraz je uruchamia */
	void setupSchedulerINT();
	
	/* Funkcja wlacza timer przerwania */
	void criticalStop();
	
	/* Funkcja pauzuje timer przerwania */
	void criticalStart();
	
	/* Funkcja zeruje timer przerwania */
	void resetSchedulerINT();
}
