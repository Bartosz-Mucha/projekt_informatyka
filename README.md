Do projektu wymagana jest instalacja SFML (wersja 2.6.2 polecana)

Pobierz SFML ze strony: https://www.sfml-dev.org/download.php

Wypakuj SFML do wybranego folderu

KONFIGURACJA VISUAL STUDIO
Utwórz nowy projekt C++ (Empty Project)

Skopiuj wszystkie pliki .cpp i .h i dodaj je do folderu projektu

Ustawienia projektu dla tryby Release(w³aœciwoœci):

C/C++ ? Ogólne ? Dodatkowe katalogi plików nag³ówkowych:
Dodaj œcie¿kê prowadz¹c¹ do pliku z SFML np. "SFML-2.6.2\include" 

C/C++ ? Preprocesor ? definicje preprocesora:
Dopisz SFML_STATIC

Konsolidator ? Ogólne ? Dodatkowe katalogi biblioteki:
Dodaj œcie¿kê prowadz¹c¹ do pliku z SFML np. "SFML-2.6.2\lib" 

Konsolidator ? Dane wejœciowe ? Dodatkowe zale¿noœci:
Dodaj: sfml-window-s.lib; sfml-system-s.lib; sfml-graphics-s.lib; opengl32.lib; freetype.lib; winmm.lib; gdi32.lib; 


Skopiuj plik arial.ttf do folderu z plikiem

KOMPILACJA I URUCHOMIENIE

Wybierz konfiguracjê "Release"

STEROWANIE W GRZE

Strza³ki lewo/prawo: Poruszanie paletk¹

Z: Zapisz grê

ESC: Powrót do menu

PLIKI ZAPISU

Gra automatycznie tworzy pliki:

wyniki.txt - 5 najlepszych wyników

zapis_gry_*.txt - zapisy stanu gry

 

