/*
 * Ten plik przechowuje klase sterownika rejestru przesuwnego, pozwalajaca na
 * reprezentacje tego urzadzenia w pamieci, zapisywanie danych do niego oraz
 * uzywanie w sposob podobny do rejestrow mikrokontrolera
 *
 * Autor: Cixo
 */

/* Potrzebna do obslugi synchronizacji krytycznej czasowo */
#include "../Clasci/Platform.hpp"

/* Potrzebna do ustawienia pinow rejestru */
#include "Pin.hpp"

#pragma once

namespace Drivers{
    class ShiftRegister{
        /*
         * Klasa reprezentujaca rejestr przesuwajacy w pamieci mikrokontrolera
         * pozwalajacy na przesylanie danych do niego jak rowniez uzywanie
         * go jak pinow mikrokontrolera
         */

        public:
            ShiftRegister(Pin DS, Pin ST_CP, Pin SH_CP):
                DS(DS), ST_CP(ST_CP), SH_CP(SH_CP)
            {
                /*
                 * Ta funkcja tworzy obiekt reprezentujacy w pamieci rejestr 
                 * przesuwajacy oraz umozliwia zapisywanie i odczytywanie danych
                 * jakie sie w nim znajduja. DS, ST_CP, SH_CP to odpowiadajace
                 * im piny ukladu jako obiekty Pin. Ta funkcja od razu ustawia
                 * wartosc w rejestrze na 0b0000000, piny jak nalezy oraz 
                 * przesyla stan do rejestru
                 */

                buffer = 0b00000000;

                DS.setMode(OUT);
                ST_CP.setMode(OUT);
                SH_CP.setMode(OUT);

                DS.setState(LOW);
                ST_CP.setState(LOW);
                SH_CP.setState(LOW);
            
                sync();
            }

            void sync(){
                /*
                 * Ta funkcja odpowiada za synchronizacje wewnetrznego bufora
                 * z rzeczywistym ukladem. Ustawia numer bitu jaki aktualnie
                 * jest przepisywany, po czym w petli ustawia bit i przesuwa
                 * rejestr. Na koniec przepisuje rejest przesuwajacy do 
                 * zatrzaskow. Funkcja jest krytyczna czasowo wiec pauzuje
                 * przerwanie schedulera systemu
                 */ 

                Platform::criticalStart();

                uint8_t actual_bit = 7;

                do{
                    DS.setState((PinState)(buffer & (1 << actual_bit)));

                    SH_CP.setState(HIGH);
                    SH_CP.setState(LOW);
                }while(actual_bit-- != 0);

                ST_CP.setState(HIGH);
                ST_CP.setState(LOW);

                Platform::criticalStop();
            }

            void setState(uint8_t pin_number, PinState state, bool now = true){
                /*
                 * Ustawia pin ukladu numer pin_numer na stan state. Pozwala to
                 * uzywac rejestru przesuwnego w sposob podobny do pinow 
                 * mikrokontrolera. Nadto jezeli now = true, to bufor od razu
                 * zostanie zsynchronizowany. Jezeli false, to przestawiona 
                 * zostanie tylko zmienna
                 */

                if(state == HIGH)
                    buffer |= (1 << pin_number);
                else   
                    buffer &= ~(1 << pin_number);
            
                if(now)
                    sync();
            }

            PinState getState(uint8_t pin_number){
                /*
                 * Zwraca stan bitu o numerze podanym w parametrze pin_number
                 * przy czym nalezy pamietac, iz wartosc jest zwracana ze 
                 * zmiennej, jezeli nie jest zsynchronizowana, to nie ma
                 * gwarancji iz w rejestrze jest ta sama wartosc
                 */

                return (PinState)(buffer & (1 << pin_number));
            }
            
            inline void write(uint8_t new_buffer){
                /*
                 * Przepisuje new_buffer do buffer, przez co pozwala na uzywac
                 * rejestru do rownoleglej transmisji danych
                 */

                buffer = new_buffer;
            }

            inline uint8_t read(){
                /*
                 * Zwraca aktualna wartosc zmiennej buffer, pozwalajac na 
                 * weryfikacje czy znajduje sie to co nalezy
                 */
                
                return buffer;
            }

        private:
            Pin DS, ST_CP, SH_CP;   /* Przechowuja wskazniki pinow komunikacji */
            uint8_t buffer;         /* Bufor przechowujacy zawartosc rejestru */
    };
}