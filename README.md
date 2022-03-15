# ClasciOS-AVR
ClasciOS to system operacyjny dla najmniejszych mikrokontrolerów AVR

## Zalozenia
 * ClasciOS da sie uruchomic na mikrokontrolerach AVR z 1kb flash
 * ClasciOS opiera sie na modelu dzialania, gdzie aplikacja (funkcja main) jest przerywana raz na jakis czas aby wykonac proces. Procesow moze byc dowolnie duzo, kazdy posiada wlasne dane, piszac proces nalezy pamietac iz ten nie zostanie przerwany, wiec musi zakonczyc sie mozliwie szybko
 * Procesow moze byc dowolnie duzo, a ich ilosc maksymalna definiowana jest na etapie kompilacji
 * Taki model dzialania (gdzie tylko jeden proces moze zostac wywlaszczony) zapewnia mozliwie najnizsze zuzycie zasobow (nie trzeba przechowywac kontekstu wielu procesow a tylko jednego, pozostale traca kontekst w momencie zakonczenia)

## Najnowsze zmiany
 * Dodanie przestrzeni nazw Drivers, sluzacej do przechowywania klas sterownikow urzadzen
 * Dodanie sterownika podstawowego do obslugi pinow (Pin), oraz sterownika rejestru przesuwajacego (ShiftRegister)
 * Poprawienie przywracania zapisywania i przywracania kontekstu, od teraz rejestry procesora sa zapisywane na stosie przez wykonaniem procesu

### Uwaga: Nie jest jeszcze skończony, więc nie zaleca się używania go, poza środowiskiem testowym
