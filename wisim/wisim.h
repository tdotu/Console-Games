#ifndef WISIM_H_INCLUDED
#define WISIM_H_INCLUDED



#define HOLZ_ANFANGSROHSTOFFE 40
#define STEIN_ANFANGSROHSTOFFE 20
#define NAHRUNG_ANFANGSROHSTOFFE 15
#define GOLD_ANFANGSROHSTOFFE 1000
#define ANFANGSBEWOHNER 10

#define GRUND_LAGERPLATZ 100
#define LAGERHAUS_LAGERPLATZ 100

#define NAHRUNG_ZUFRIEDENHEIT_EINFLUSS 28
#define WOHNUNGEN_ZUFRIEDENHEIT_EINFLUSS 27
#define STEUERN_ZUFRIEDENHEIT_EINFLUSS 35
#define STADTZUSTAND_ZUFRIEDENHEIT_EINFLUSS 10

#define HOLZFAELLER_PRODUKTION 5
#define HOLZFAELLER_BETRIEBSKOSTEN 10
#define HOLZFAELLER_ARBEITER 1
#define STEINBRUCH_HOLZVERBRAUCH 3
#define STEINBRUCH_PRODUKTION 4
#define STEINBRUCH_ARBEITER 2
#define STEINBRUCH_BETRIEBSKOSTEN 15
#define JAGDHUETTEN_PRODUKTION 3
#define JAGDHUETTEN_BETRIEBSKOSTEN 5
#define JAGDHUETTEN_ARBEITER 1

#define BEWOHNER_NAHRUNGSVERBRAUCH 1
#define WOHNHAUS_WOHNRAUM 5
#define GEBURTENRATE_IN_KRANKENHAUS 3
#define ZINSEN 1.1

#define ZUFRIEDENHEIT_FRIEDHOF 10
#define ZUFRIEDENHEIT_STATUE 10
#define ZUFRIEDENHEIT_SPRINGBRUNNEN 20
#define ZUFRIEDENHEIT_BADEHAUS 25
#define ZUFRIEDENHEIT_MUSEUM 5
#define ZUFRIEDENHEIT_PRO_ARTEFAKT 10

#define KOSTEN_GOLD_HOLZFAELLERHUETTE 50
#define KOSTEN_HOLZ_HOLZFAELLERHUETTE 10
#define KOSTEN_STEIN_HOLZFAELLERHUETTE 0
#define KOSTEN_GOLD_STEINBRUCH 75
#define KOSTEN_HOLZ_STEINBRUCH 20
#define KOSTEN_STEIN_STEINBRUCH 0
#define KOSTEN_GOLD_JAGDHUETTE 50
#define KOSTEN_HOLZ_JAGDHUETTE 10
#define KOSTEN_STEIN_JAGDHUETTE 5
#define KOSTEN_GOLD_WOHNHAUS 100
#define KOSTEN_HOLZ_WOHNHAUS 10
#define KOSTEN_STEIN_WOHNHAUS 10
#define KOSTEN_GOLD_LAGERHAUS 100
#define KOSTEN_HOLZ_LAGERHAUS 10
#define KOSTEN_STEIN_LAGERHAUS 15

#define BEENDE_PROGRAMM 1



enum besonderes_gebaeude
{
    nicht_gebaut, gebaut
};


struct spielstand
{
    char dateiname[20];

    int holz;
    int stein;
    int nahrung;
    int gold;


    int bewohner;
    int bewohner_arbeitend;
    int bewohner_nicht_arbeitend;


    float zufriedenheit_nahrung;
    float zufriedenheit_wohnungen;
    float zufriedenheit_steuern;
    float zufriedenheit_stadtzustand;
    float zufriedenheit_gesamt;


    int wohnhaeuser;

    int lagerhaeuser;
    int lagerplatz;

    int holzfaeller_gesamt;
    int holzfaeller_aktiv;
    int holzfaeller_inaktiv;

    int steinbrueche_gesamt;
    int steinbrueche_aktiv;
    int steinbrueche_inaktiv;

    int jagdhuetten_gesamt;
    int jagdhuetten_aktiv;
    int jagdhuetten_inaktiv;


    int runde;
    int steuern;
    int artefakte;
    int kartenstuecke;


    enum besonderes_gebaeude rathaus;
    enum besonderes_gebaeude stadtmauer;
    enum besonderes_gebaeude krankenhaus;       // in funktion
    enum besonderes_gebaeude springbrunnen;     // in funktion
    enum besonderes_gebaeude wunschbrunnen;     // in funktion
    enum besonderes_gebaeude statue;            // in funktion
    enum besonderes_gebaeude badehaus;          // in funktion
    enum besonderes_gebaeude museum;            // teilweise in funktion
    enum besonderes_gebaeude tempel;
    enum besonderes_gebaeude friedhof;          // in funktion
    enum besonderes_gebaeude bank;              // in funktion
    enum besonderes_gebaeude wachturm;
};



typedef enum besonderes_gebaeude besonderes_gebaeude;
typedef struct spielstand spielstand;



spielstand neuer_spielstand();
int berechne_gesamt_zufriedenheit(int z_nahrung, int z_wohnungen, int z_steuern, int z_stadt);
void zeige_statistiken_kurz(spielstand statistiken);
spielstand iteriere_spielstand(spielstand vorher);
int spielmenue(spielstand *spiel);
void gebaeude_bau_menue(spielstand *spiel);
void gebaeude_verwaltungs_menue(spielstand *spiel);



#endif
