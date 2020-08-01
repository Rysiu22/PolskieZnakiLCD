#include <LiquidCrystal.h> // biblioteka stadardowa dostarczona ze środowiskiem

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//Konkretnie sygnały zostały podpięte kolejno do: 8 (RS), 9 (Enable), 4 (D4), 5 (D5), 5 (D6) i 7 (D7).

#include "PolskieZnaki.h"

PolskieZnaki polskie(8); // 8 to rozmiar pamięci lcd. Domyślnie 8

String tekst = "ćąłżźóńęś";
String pamiec_lcd = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"; // \x00 robi psikusa

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  tekst = polskie.create(tekst); // obliczamy i zmieniamy tekst

  // Tu wysyłamy nasze znaki specjalne do wyświetlacza
  for(byte i = 0; i < sizeof(polskie.znak)/sizeof(polskie.znak[0]); i++) // sprawdzamy liste wskaźników
  {
    if(polskie.znak[i] != 0) // pomijamy nie ustawione
    {
      lcd.createChar(i, polskie.znak[i]); // 'i' to nasz index dla ustawienie lcd
    }
  }

  lcd.setCursor(0, 0);
  // lcd.createChar(..., ...); // nie może się tu znaleźć te polecenie bo nie będzie wyświetlać tekstu
  lcd.print(tekst);

  lcd.setCursor(0, 1);
  lcd.print(pamiec_lcd);
}

void loop() {

}
