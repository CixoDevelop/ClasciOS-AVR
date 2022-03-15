/*
 * Ten plik przechowuje klase Pin ktora to klasa umozliwia niskopoziomowy 
 * dostep do pinu mikrokontrolera. Jest uzywana przez inne sterowniki aby
 * definiowac polaczenia pomiedzy mikrokontrolerem a ukladami zewnetrznymi
 * 
 * Autor: Cixo
 */

#pragma once

/* Definiuje uint8_t ktore jest uzywane do adresowania */
#ifndef uint8_t
	typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
#endif

namespace Drivers{

    /* Enum definiujacy tryb pracy pinu */
    enum PinMode{
        IN = false,
        OUT = true,
    };

    /* Enum definiujacy stan pinu */
    enum PinState{
        LOW = false,
        HIGH = true,
    };

    class Pin{
        /*
         * Klasa odpowiadajaca za obsluge pinu, podczas konstruowania wymaga tylko
         * podania adresu PINx, oraz numeru w tym rejestrze, nie potrzebuje
         * PORTx oraz DDRx, te zostaja wyliczone w locie. Klasa zawiera tylko 
         * funkcje inline poniewaz to powinno dzialac jak najszybciej i byc jak 
         * najlzejsze a one to zapewnia.
         */

        public:
            Pin(volatile uint8_t *pin, uint8_t number):
                pin(pin), mask(1 << number)
            {
                /* 
                 * Ten konstruktor odpowiada za stworzenie klasy, pierwszy parametro to
                 * adres do rejestru PINx pinu, a kolejny to numer bitu w tym rejestrze
                 */
            }

            void setMode(PinMode new_mode){
                /* 
                 * Ta funkcja jak parametr przyjmuje tryb pinu, i po jej wywolaniu w 
                 * parametrze podajac nowy tryb, ten wlasnie tryb zostanie ustawiony
                 */

                if(new_mode)
                    *(pin + 1) |= mask;
                else
                    *(pin + 1) &= ~mask;
            }

            void setState(PinState new_state){
                /* 
                 * Ta funkcja jak parametr przyjmuje stan pinu, i po jej wywolaniu w 
                 * parametrze podajac nowy stan, ten wlasnie stan zostanie ustawiony
                 */

                if(new_state)
                    *(pin + 2) |= mask;
                else   
                    *(pin + 2) &= ~mask;
            }
            
            PinMode getMode(){
                /*
                 * Ta funkcja zwraca aktualny tryb w jakim pracuje pin. Moze byc
                 * przydatna kiedy pin pracuje po kolei w kilku trybach a chcemy
                 * zachowac synchronizacje procesu
                 */

                return (PinMode)(*(pin + 1) & mask);
            }

            PinState getState(){
                /*
                 * Ta funkcja zwraca aktualna wartosc jaka panuje na pinie. Nie 
                 * sprawdza czy pin pracuje jako wejscie, poniewaz moze byc uzytecnza
                 * rowniez podczas pracy z pinem w trybie wyjscia
                 */

                return (PinState)(*pin & mask);
            }

            /* Tutaj nadpisywane sa operatory aby uproscic uzywanie klasy */
            Pin &operator =(PinMode new_mode){
                setMode(new_mode);
                return *this;
            }

            Pin &operator =(PinState new_state){
                setState(new_state);
                return *this;
            }

            PinState operator !(){
                return PinState(!getState());
            }

            bool operator ==(PinState compare){
                return getState() == compare;
            }

            bool operator !=(PinState compare){
                return getState() != compare;
            }

            bool operator ==(PinMode compare){
                return getMode() == compare;
            }

            bool operator !=(PinMode compare){
                return getMode() != compare;
            }

        private:
            /* Ta zmienna przechowuje adres PINx pinu */
            volatile uint8_t *pin; 
            
            /* Ta zmienna przechowuje bit w tym rejestrze */
            uint8_t mask; 
    };

}