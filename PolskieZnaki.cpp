// Wersja arduino IDE: 1.8.10

// zakłada że ostani adres nadpisuje wartość pierwszą (\x00)
// ponieważ nie można jej użyć w stringu bo jest to znak końca ciągu
// jeśli z nie nadpisuje pierwszego adresu ustawić wartość pamięci mniejszą o 1

// #define DEBUG

#include <Arduino.h>
#include "PolskieZnaki.h"

#ifdef DEBUG
void PolskieZnaki::showAddr()
{
  for(byte i = 0; i < sizeof(znak)/sizeof(znak[0]) ;i++)
  {
      Serial.print("index: ");
      Serial.print(i);
      Serial.print(" - Adres: ");
      Serial.println((long int) znak[i],HEX);
  }
}
#endif

int PolskieZnaki::count()
{
  // zwraca ile znaków zostało ustawionych
  int licz = 0;
  for(byte i=0; i < sizeof(znak)/sizeof(znak[0]); i++)
  {
    if(znak[i] != 0)
      licz++;
  }
  return licz;
}

void PolskieZnaki::clear()
{
  // czysczenie informacji o przechowywanych znakach
  for(byte i=0; i < sizeof(znak)/sizeof(znak[0]); i++)
  {
    znak[i] = 0;
  }
}

String PolskieZnaki::create(String incomingString)
{
  // rozpoznaje ile rodzaji polskich znaków użyto i zapisuje dane do wewnętrzenej struktury
  // zwraca podmieniony tekst dla wyświetlacza
  clear();

  int ilcd = 1;
  String outString = incomingString;

#ifdef DEBUG
  String znaki;
  Serial.println("Prepared Size:" + String(incomingString.length()) + " \"" + incomingString + "\"" );
#endif

  for(int iiS = 0; iiS < incomingString.length(); iiS++)
  {      
    if(incomingString[iiS] > "\xFF")
    {
      
#ifdef DEBUG
      Serial.print(incomingString.substring(iiS,iiS+2));
      Serial.print(F(" = \"\\x"));
      Serial.print(incomingString[iiS], HEX);
      Serial.print(F("\\x"));
      Serial.print(incomingString[iiS+1], HEX);
      if(ilcd <= pamiec_znakow)
      {
        Serial.print(F("\" \\\\ lcd Char["));
        Serial.print(ilcd);
        Serial.println(F("]"));
      }
      else
      {
        Serial.println(F("\"     "));
      }

      // dodanie do stringu polskich
      if(znaki.indexOf(incomingString.substring(iiS,iiS+2)) == -1)
      {
        znaki += incomingString.substring(iiS,iiS+2);
      }
#endif

      // dodanie 8 polskich znaków do pamięci LCD, \x08 nadpisze znak \x00 w pamięci LCD
      // znak \x00 jest też znakiem końca stringu
      for(byte ipz = 0;  ipz < slownik_size; ipz++)
      {
        if(String(polskie_znaki[ipz].key) == incomingString.substring(iiS,iiS+2))
        {
          if(ilcd <= pamiec_znakow)
          {
            // zapisujemy w tabliczy nr. pod jakim został zapisany w LCD
            znak[ilcd] = polskie_znaki[ipz].value;
            outString.replace( String(polskie_znaki[ipz].key), String(char(ilcd)));
#ifdef DEBUG
            Serial.print("+ ");
            Serial.print(ilcd);
            Serial.println(outString);
#endif
          }
          else
          {
            outString.replace( String(polskie_znaki[ipz].key), String(polskie_znaki[ipz].ascii));
#ifdef DEBUG
            Serial.print("- ");
            Serial.println(outString);
#endif
          }
        }
      }
      ilcd++;
      iiS++;
    }
  }

#ifdef DEBUG
  Serial.print(F("Polskie znaki "));
  Serial.print(znaki.length() / 2);
  Serial.print(F(" = "));
  Serial.println(znaki);
#endif

  return outString;
}

PolskieZnaki::PolskieZnaki()
: PolskieZnaki(8)
{ 
}

PolskieZnaki::PolskieZnaki(byte ile_znakow)
{
  // ile znaków przechowuje wyświetlacz
  pamiec_znakow = ile_znakow;

  Slownik tmp[] = {
    // małe polskie znaki znaki
    {"\xFFFFFFC4\xFFFFFF87", znak_c, 'c'}, // ć
    {"\xFFFFFFC4\xFFFFFF85", znak_a, 'a'}, // ą
    {"\xFFFFFFC5\xFFFFFF82", znak_l, 'l'}, // ł
    {"\xFFFFFFC5\xFFFFFFBC", znak_z, 'z'}, // ż
    {"\xFFFFFFC5\xFFFFFFBA", znak_x, 'z'}, // ź
    {"\xFFFFFFC3\xFFFFFFB3", znak_o, 'o'}, // ó
    {"\xFFFFFFC5\xFFFFFF84", znak_n, 'n'}, // ń
    {"\xFFFFFFC4\xFFFFFF99", znak_e, 'e'}, // ę
    {"\xFFFFFFC5\xFFFFFF9B", znak_s, 's'}, // ś

    // duże polskie znaki znaki
    {"\xFFFFFFC4\xFFFFFF86", znak_C, 'C'}, // Ć
    {"\xFFFFFFC4\xFFFFFF84", znak_A, 'A'}, // Ą
    {"\xFFFFFFC5\xFFFFFF81", znak_L, 'L'}, // Ł
    {"\xFFFFFFC5\xFFFFFFBB", znak_Z, 'Z'}, // Ż
    {"\xFFFFFFC5\xFFFFFFB9", znak_X, 'Z'}, // Ź
    {"\xFFFFFFC3\xFFFFFF93", znak_O, 'O'}, // Ó
    {"\xFFFFFFC5\xFFFFFF83", znak_N, 'N'}, // Ń
    {"\xFFFFFFC4\xFFFFFF98", znak_E, 'E'}, // Ę
    {"\xFFFFFFC5\xFFFFFF9A", znak_S, 'S'}  // Ś
    };

  memcpy(&polskie_znaki, &tmp, sizeof(tmp[0])*slownik_size);
}



//wzory znaków
//https://sites.google.com/site/sebastiankosiarek/home/diy/elektronika/wyswietlacz-od-nokii-5110
//https://sites.google.com/site/sebastiankosiarek/_/rsrc/1449127627642/home/diy/elektronika/wyswietlacz-od-nokii-5110/microlcd_font_tab_5110.png?height=309&width=400

const byte PolskieZnaki::znak_c[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b10000,
  0b10000,
  0b10001,
  0b01110,
  0b00000
};

const byte PolskieZnaki::znak_a[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b00001,
  0b01111,
  0b10001,
  0b01111,
  0b00010
};

const byte PolskieZnaki::znak_l[8] = {
  0b01100,
  0b00100,
  0b00110,
  0b01100,
  0b00100,
  0b00100,
  0b01110,
  0b00000
};

const byte PolskieZnaki::znak_z[8] = {
  0b00100,
  0b00000,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};

const byte PolskieZnaki::znak_x[8] = {
  0b00010,
  0b00100,
  0b11111,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};

const byte PolskieZnaki::znak_o[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};

const byte PolskieZnaki::znak_n[8] = {
  0b00010,
  0b00100,
  0b10110,
  0b11001,
  0b10001,
  0b10001,
  0b10001,
  0b00000
};

const byte PolskieZnaki::znak_e[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b10001,
  0b11111,
  0b10000,
  0b01110,
  0b00010
};

const byte PolskieZnaki::znak_s[8] = {
  0b00010,
  0b00100,
  0b01110,
  0b10000,
  0b01110,
  0b00001,
  0b11110,
  0b00000
};

const byte PolskieZnaki::znak_C[8] = {
  0b00010,
  0b01110,
  0b10101,
  0b10000,
  0b10000,
  0b10001,
  0b01110,
  0b00000
};

const byte PolskieZnaki::znak_A[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b00010
};

const byte PolskieZnaki::znak_L[8] = {
  0b10000,
  0b10000,
  0b10010,
  0b10100,
  0b11000,
  0b10000,
  0b11111,
  0b00000
};

const byte PolskieZnaki::znak_Z[8] = {
  0b00100,
  0b11111,
  0b00001,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};

const byte PolskieZnaki::znak_X[8] = {
  0b00100,
  0b11111,
  0b01001,
  0b00010,
  0b00100,
  0b01000,
  0b11111,
  0b00000
};

const byte PolskieZnaki::znak_O[8] = {
  0b00010,
  0b01110,
  0b10101,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000
};

const byte PolskieZnaki::znak_N[8] = {
  0b00100,
  0b10101,
  0b11001,
  0b10101,
  0b10011,
  0b10001,
  0b10001,
  0b00000
};

const byte PolskieZnaki::znak_E[8] = {
  0b11111,
  0b10000,
  0b10000,
  0b11110,
  0b10000,
  0b10000,
  0b11111,
  0b00010
};

const byte PolskieZnaki::znak_S[8] = {
  0b00010,
  0b01111,
  0b10100,
  0b10000,
  0b01110,
  0b00001,
  0b11110,
  0b00000
};
