#ifndef POLSKIE_ZNAKI
#define POLSKIE_ZNAKI

class PolskieZnaki {
  private:
    static const int slownik_size = 18;

  public:
    byte *znak[slownik_size];

    int count();
    void clear();
    String create(String incomingString);
    
    PolskieZnaki();
    PolskieZnaki(byte ile_znakow);

    //void showAddr();

  private:
    typedef struct {
      const char *key;
      const char *value;
      char ascii;
    } Slownik;

    byte pamiec_znakow;
    Slownik polskie_znaki[slownik_size];
    
    bool debug_serial;
    static const byte znak_c[8];
    static const byte znak_a[8];
    static const byte znak_l[8];
    static const byte znak_z[8];
    static const byte znak_x[8];
    static const byte znak_o[8];
    static const byte znak_n[8];
    static const byte znak_e[8];
    static const byte znak_s[8];
    static const byte znak_C[8];
    static const byte znak_A[8];
    static const byte znak_L[8];
    static const byte znak_Z[8];
    static const byte znak_X[8];
    static const byte znak_O[8];
    static const byte znak_N[8];
    static const byte znak_E[8];
    static const byte znak_S[8];

};



#endif
