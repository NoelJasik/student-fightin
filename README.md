# Student-fightin' (no bo ballin już nie pasuje)
## Ogólny plan realizacji
Zrobić grę taką w którą da się grać, vibe starych gier na flashu.
Pomysł gry, to taki rts/tower defence gdzie przeciągamy na plansze studentów i bronią agh przed kibolami klubu rks chuwdu :)
![design.png](assets/design.png)
## mechaniki do zaimplementowania
 (osobne kategorie, kolejność priortetów od góry do dołu)
### Gameplay
- Studenci chodzą jak npc, zamiast stać jak słup
- jednostki nie przenikają przez siebie nawzajem, tylko się odbijają
- Animacja rozwalania jednostek (wybuchy, kawałki lecące w powietrze)
- Regeneracja zdrowia jednostek
- Komunikaty o rozpoczęciu i zakończeniu fali
- Efekty podczas walki (dźwięki, cząsteczki itp)

### GUI/UX
- Wyświetlanie HP budynku AGH
- pasek hp nad rannymi jednostkami
- Renderowanie jednostki w szarym kolorze przed jej postawieniem, i na czerwono jak sie nie da jej postawić
- Podstawowy polish (efekty dźwiękowe, muzyka w tle)

### Menusy
- Instrukcje na ekranie (jak grać, jakie klawisze, najlepiej obrazki a nie ściana tekstu, bo tego nikt nigdy nie czyta xd)
- Menu główne z ustawieniami (zmiana rozdzielczości poprzez skalowanie okna z gameplayem, bo fizyka obliczana jest w pixelach, podobno się tak da w SDL2)
- Ekran pauzy
- Ekran końca gry (wygrana/przegrana)

### Ekonomia
- Siano za zabijanie przeciwników
- Budynki dodają nam pasywnie hajs (jednostka 3)
- Hajs na start i rozpoczęcie fali
- Sprzedawanie jednostek
- Ulepszanie bazowych jednostek (że wszystkie jednostki które postawimy, odrazu są ulepszone, była by to droższa opcja)



### lore
kartka w zeszycie znudzonego studenta na 6 godzinie fizyki
### artstyle
Artstyle będzie wyglądał tak jakby gierka była narysowana odręcznie, na kartce w zeszycie.

