#include <stdio.h>
#include <string.h>
#include <math.h>

#include "wisim.h"



int main(int argc, char **argv)
{
    spielstand testspielstand = neuer_spielstand();
    strncpy(testspielstand.dateiname, "test.wss", 8);
    spielstand temporaerer_spielstand;
    int beenden = 0;
    int fehlerfrei = 1;

    do
    {
        do
        {
            zeige_statistiken_kurz(testspielstand);

            beenden = spielmenue(&testspielstand);

            if(beenden == BEENDE_PROGRAMM)
                return 0;

            temporaerer_spielstand = iteriere_spielstand(testspielstand);

            fehlerfrei = 1;

            if(temporaerer_spielstand.holz < 0)
            {
                printf("Du hast nicht genug Holz um alle gewuenschten Aktionen durchzufuehren.\n");
                printf("Es fehlt %d Holz. Aendere deine Einstellungen um genug Holz zu erhalten.\n", abs(temporaerer_spielstand.holz));
                fehlerfrei = 0;
                getchar();
            }

            if(temporaerer_spielstand.gold < 0 && temporaerer_spielstand.bank == nicht_gebaut)
            {
                printf("Du hast nicht genug Gold um alle gewuenschten Aktionen durchzufuehren.\n");
                printf("Es fehlt %d Gold. Aendere deine Einstellungen um genug Gold zu erhalten.\n", abs(temporaerer_spielstand.gold));
                fehlerfrei = 0;
                getchar();
            }

            if(temporaerer_spielstand.bewohner_nicht_arbeitend < 0)
            {
                printf("Du hast nicht genug Bewohner um alle gewuenschten Aktionen durchzufuehren.\n");
                printf("Es fehlen %d Bewohner. Aendere deine Einstellungen um genug Bewohner zu erhalten.\n", abs(temporaerer_spielstand.bewohner_nicht_arbeitend));
                fehlerfrei = 0;
                getchar();
            }

            if(temporaerer_spielstand.bewohner <= 0)
            {
                zeige_statistiken_kurz(temporaerer_spielstand);

                printf("\nAlle deine Bewohner sind gestorben oder haben die Stadt verlassen.\n");
                printf("Du hast in deinem Auftrag, sie zu beschuetzen\nund in ein besseres Leben zu fuehren, versagt.\n");
                printf("Du hast verloren.\n");

                fehlerfrei = 1;
                beenden = BEENDE_PROGRAMM;
            }

            printf("\n\n");
        }while(!fehlerfrei);

        testspielstand = temporaerer_spielstand;
    }while(beenden != BEENDE_PROGRAMM);

    return 0;
}


spielstand neuer_spielstand()
{
    spielstand neu;

    memset(neu.dateiname, '\0', sizeof(neu.dateiname));

    neu.holz = HOLZ_ANFANGSROHSTOFFE;
    neu.stein = STEIN_ANFANGSROHSTOFFE;
    neu.nahrung = NAHRUNG_ANFANGSROHSTOFFE;
    neu.gold = GOLD_ANFANGSROHSTOFFE;


    neu.bewohner = ANFANGSBEWOHNER;
    neu.bewohner_arbeitend = 0;
    neu.bewohner_nicht_arbeitend = neu.bewohner;

    neu.zufriedenheit_nahrung = 100.0;
    neu.zufriedenheit_wohnungen = 0.0;
    neu.zufriedenheit_steuern = 90.0;
    neu.zufriedenheit_stadtzustand = 0.0;
    neu.zufriedenheit_gesamt = berechne_gesamt_zufriedenheit(neu.zufriedenheit_nahrung, neu.zufriedenheit_wohnungen, neu.zufriedenheit_steuern, neu.zufriedenheit_stadtzustand);


    neu.wohnhaeuser = 0;

    neu.lagerhaeuser = 0;
    neu.lagerplatz = GRUND_LAGERPLATZ;

    neu.holzfaeller_gesamt = 0;
    neu.holzfaeller_aktiv = 0;
    neu.holzfaeller_inaktiv = 0;

    neu.steinbrueche_gesamt = 0;
    neu.steinbrueche_aktiv = 0;
    neu.steinbrueche_inaktiv = 0;

    neu.jagdhuetten_gesamt = 0;
    neu.jagdhuetten_aktiv = 0;
    neu.jagdhuetten_inaktiv = 0;


    neu.runde = 1;
    neu.steuern = 1;
    neu.artefakte = 0;
    neu.kartenstuecke = 0;


    neu.rathaus = nicht_gebaut;
    neu.stadtmauer = nicht_gebaut;
    neu.krankenhaus = nicht_gebaut;
    neu.springbrunnen = nicht_gebaut;
    neu.wunschbrunnen = nicht_gebaut;
    neu.statue = nicht_gebaut;
    neu.badehaus = nicht_gebaut;
    neu.museum = nicht_gebaut;
    neu.tempel = nicht_gebaut;
    neu.friedhof = nicht_gebaut;
    neu.bank = nicht_gebaut;
    neu.wachturm = nicht_gebaut;

    return neu;
}


int berechne_gesamt_zufriedenheit(int z_nahrung, int z_wohnungen, int z_steuern, int z_stadt)
{
    return ((z_nahrung / 100.0) * NAHRUNG_ZUFRIEDENHEIT_EINFLUSS) + ((z_wohnungen / 100.0) * WOHNUNGEN_ZUFRIEDENHEIT_EINFLUSS) + ((z_steuern / 100.0) * STEUERN_ZUFRIEDENHEIT_EINFLUSS) + ((z_stadt / 100.0) * STADTZUSTAND_ZUFRIEDENHEIT_EINFLUSS);
}


void zeige_statistiken_kurz(spielstand statistiken)
{
    printf("Runde:        %d\n\n", statistiken.runde);
    printf("Holz:          %d\n", statistiken.holz);
    printf("Stein:         %d\n", statistiken.stein);
    printf("Nahrung:       %d\n", statistiken.nahrung);
    printf("Gold:          %d\n", statistiken.gold);
    printf("Bewohner:      %d\n", statistiken.bewohner);
    printf("Zufriedenheit: %g%c\n\n\n", statistiken.zufriedenheit_gesamt, '%');
}


spielstand iteriere_spielstand(spielstand vorher)
{
    vorher.holz += vorher.holzfaeller_aktiv * HOLZFAELLER_PRODUKTION;
    vorher.holz -= vorher.steinbrueche_aktiv * STEINBRUCH_HOLZVERBRAUCH;
    vorher.stein += vorher.steinbrueche_aktiv * STEINBRUCH_PRODUKTION;
    vorher.nahrung += vorher.jagdhuetten_aktiv * JAGDHUETTEN_PRODUKTION;
    vorher.nahrung -= vorher.bewohner * BEWOHNER_NAHRUNGSVERBRAUCH;
    vorher.zufriedenheit_nahrung = vorher.nahrung >= 0 ? 100 : 100 + ((vorher.nahrung / BEWOHNER_NAHRUNGSVERBRAUCH) / ((float)vorher.bewohner) * 100.0);
    vorher.nahrung = vorher.nahrung < 0 ? 0 : vorher.nahrung;
    vorher.gold += vorher.bewohner * vorher.steuern;
    vorher.gold += (vorher.wunschbrunnen == gebaut) * (vorher.bewohner / 10);
    vorher.zufriedenheit_steuern = (10 - vorher.steuern) * 10;
    vorher.gold -= vorher.holzfaeller_aktiv * HOLZFAELLER_BETRIEBSKOSTEN;
    vorher.gold -= vorher.steinbrueche_aktiv * STEINBRUCH_BETRIEBSKOSTEN;
    vorher.gold -= vorher.jagdhuetten_aktiv * JAGDHUETTEN_BETRIEBSKOSTEN;
    vorher.gold = vorher.gold < 0 ? vorher.gold * ZINSEN : vorher.gold;
    vorher.zufriedenheit_wohnungen = vorher.wohnhaeuser * WOHNHAUS_WOHNRAUM >= vorher.bewohner ? 100 : 100 - (100 * ((vorher.bewohner - vorher.wohnhaeuser * WOHNHAUS_WOHNRAUM) / (float)vorher.bewohner));
    vorher.zufriedenheit_stadtzustand = ((vorher.friedhof == gebaut) * ZUFRIEDENHEIT_FRIEDHOF) + ((vorher.statue == gebaut) * ZUFRIEDENHEIT_STATUE) + ((vorher.springbrunnen == gebaut) * ZUFRIEDENHEIT_SPRINGBRUNNEN) + ((vorher.badehaus == gebaut) * ZUFRIEDENHEIT_BADEHAUS) + ((vorher.museum == gebaut) * ZUFRIEDENHEIT_MUSEUM) + (vorher.artefakte == gebaut * ZUFRIEDENHEIT_PRO_ARTEFAKT);
    vorher.zufriedenheit_gesamt = berechne_gesamt_zufriedenheit(vorher.zufriedenheit_nahrung, vorher.zufriedenheit_wohnungen, vorher.zufriedenheit_steuern, vorher.zufriedenheit_stadtzustand);
    vorher.bewohner += round((vorher.bewohner / 5.0) * (((vorher.zufriedenheit_gesamt / 100.0) - 0.5) * 1.3));
    vorher.bewohner += (vorher.krankenhaus == gebaut) * GEBURTENRATE_IN_KRANKENHAUS;
    vorher.bewohner_arbeitend = (vorher.holzfaeller_aktiv * HOLZFAELLER_ARBEITER) + (vorher.steinbrueche_aktiv * STEINBRUCH_ARBEITER) + (vorher.jagdhuetten_aktiv * JAGDHUETTEN_ARBEITER);
    vorher.bewohner_nicht_arbeitend = vorher.bewohner - vorher.bewohner_arbeitend;
    vorher.lagerplatz = GRUND_LAGERPLATZ + (vorher.lagerhaeuser * LAGERHAUS_LAGERPLATZ);
    vorher.holz = vorher.holz > vorher.lagerplatz ? vorher.lagerplatz : vorher.holz;
    vorher.stein = vorher.stein > vorher.lagerplatz ? vorher.lagerplatz : vorher.stein;
    vorher.nahrung = vorher.nahrung > vorher.lagerplatz ? vorher.lagerplatz : vorher.nahrung;
    vorher.runde++;

    return vorher;
}


int spielmenue(spielstand *spiel)
{
    int befehl = 0;
    int helper = 0;
    FILE *f = NULL;

    do
    {
        printf("1. Gebaeude bauen\n");
        printf("2. Gebaeude verwalten\n");
        printf("3. Besondere Gebaeude verwalten\n");
        printf("4. Statistiken wiederholen\n");
        printf("5. Steuern festlegen\n");
        printf("6. Meinung der Bevölkerung anhören\n");
        printf("7. Spiel speichern\n");
        printf("8. Naechste Runde\n");
        printf("9. Spiel beenden\n");
        printf("Ihre Befehle? ");
        scanf("%d", &befehl);
        printf("\n");

        switch(befehl)
        {
            case 1:
                gebaeude_bau_menue(spiel);
            break;

            case 2:
                gebaeude_verwaltungs_menue(spiel);
            break;

            case 3:
                printf("Diese Option steht leider momentan noch nicht zur Verfügung.\n\n");
            break;

            case 4:
                zeige_statistiken_kurz(*spiel);
            break;

            case 5:
                printf("Aktueller Steuersatz: %d Goldmuenzen pro Person und Runde\n", spiel->steuern);
                printf("Zufriedenheit der Bevoelkerung mit den Steuern: %g%c\n", spiel->zufriedenheit_steuern, '%');
                printf("Waehlen sie einen neuen Steuersatz zwischen 0 und 10:\n");
                scanf("%d", &helper);

                if(helper < 0 || helper > 10)
                {
                    printf("Die gewaehlte Zahl liegt nicht im gültigen Bereich.\n\n");
                }
                else
                {
                    spiel->steuern = helper;
                    spiel->zufriedenheit_steuern = (10 - spiel->steuern) * 10;
                    spiel->zufriedenheit_gesamt = berechne_gesamt_zufriedenheit(spiel->zufriedenheit_nahrung, spiel->zufriedenheit_wohnungen, spiel->zufriedenheit_steuern, spiel->zufriedenheit_stadtzustand);
                    printf("Neuer Steuersatz festgelegt.\n\n");
                }
            break;

            case 6:
                printf("Zufriedenheit der Bevoelkerung mit der Nahrungsversorgung: %g%c\n", spiel->zufriedenheit_nahrung, '%');
                printf("Zufriedenheit der Bevoelkerung mit den Wohnungen:          %g%c\n", spiel->zufriedenheit_wohnungen, '%');
                printf("Zufriedenheit der Bevoelkerung mit den Steuern:            %g%c\n", spiel->zufriedenheit_steuern, '%');
                printf("Zufriedenheit der Bevoelkerung mit dem Ausbau der Stadt:   %g%c\n", spiel->zufriedenheit_stadtzustand, '%');
                printf("Zufriedenheit der Bevoelkerung mit der Gesamtsituation:    %g%c\n\n", spiel->zufriedenheit_gesamt, '%');
            break;

            case 7:
                f = fopen(spiel->dateiname, "w");
                fwrite(spiel, sizeof(spielstand), 1, f);
                fclose(f);
                f = NULL;
                printf("Spiel gespeichert.\n\n");
            break;

            case 8:
                printf("Wie sie wünschen.\n\n");
            break;

            case 9:
                printf("Spiel wird beendet...\n");
                return BEENDE_PROGRAMM;
            break;

            default:
                printf("Was genau wollen sie?\n\n");
            break;
        }
    }while(befehl != 8);

    return 0;
}


void gebaeude_bau_menue(spielstand *spiel)
{
    int helper;

    do
    {
        printf("Nr Bezeichnung       Bereits gebaut Gold Holz Stein Betriebskosten Arbeiter\n");
        printf("1. Holzfaellerhuette % 14d % 4d % 4d % 5d % 14d % 8d\n", spiel->holzfaeller_gesamt, KOSTEN_GOLD_HOLZFAELLERHUETTE, KOSTEN_HOLZ_HOLZFAELLERHUETTE, KOSTEN_STEIN_HOLZFAELLERHUETTE, HOLZFAELLER_BETRIEBSKOSTEN, HOLZFAELLER_ARBEITER);
        printf("2. Steinbruch        % 14d % 4d % 4d % 5d % 14d % 8d\n", spiel->steinbrueche_gesamt, KOSTEN_GOLD_STEINBRUCH, KOSTEN_HOLZ_STEINBRUCH, KOSTEN_STEIN_STEINBRUCH, STEINBRUCH_BETRIEBSKOSTEN, STEINBRUCH_ARBEITER);
        printf("3. Jagdhuette        % 14d % 4d % 4d % 5d % 14d % 8d\n", spiel->jagdhuetten_gesamt, KOSTEN_GOLD_JAGDHUETTE, KOSTEN_HOLZ_JAGDHUETTE, KOSTEN_STEIN_JAGDHUETTE, JAGDHUETTEN_BETRIEBSKOSTEN, JAGDHUETTEN_ARBEITER);
        printf("4. Wohnhaus          % 14d % 4d % 4d % 5d % 14d % 8d\n", spiel->wohnhaeuser, KOSTEN_GOLD_WOHNHAUS, KOSTEN_HOLZ_WOHNHAUS, KOSTEN_STEIN_WOHNHAUS, 0, 0);
        printf("5. Lagerhaus         % 14d % 4d % 4d % 5d % 14d % 8d\n", spiel->lagerhaeuser, KOSTEN_GOLD_LAGERHAUS, KOSTEN_HOLZ_LAGERHAUS, KOSTEN_STEIN_LAGERHAUS, 0, 0);
        printf("6. Nichts\n");
        printf("Was wollen sie bauen? ");
        scanf("%d", &helper);

        printf("\n");

        switch(helper)
        {
            case 1:
                if(spiel->gold < KOSTEN_GOLD_HOLZFAELLERHUETTE)
                {
                    printf("Du hast nicht genug Gold um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->holz < KOSTEN_HOLZ_HOLZFAELLERHUETTE)
                {
                    printf("Du hast nicht genug Holz um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->stein < KOSTEN_STEIN_HOLZFAELLERHUETTE)
                {
                    printf("Du hast nicht genug Stein um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(helper == 0) break;

                spiel->gold -= KOSTEN_GOLD_HOLZFAELLERHUETTE;
                spiel->holz -= KOSTEN_HOLZ_HOLZFAELLERHUETTE;
                spiel->stein -= KOSTEN_STEIN_HOLZFAELLERHUETTE;

                spiel->holzfaeller_gesamt++;
                spiel->holzfaeller_aktiv++;

                printf("Holzfaellerhuette gebaut.\n");

                helper = 1;
            break;

            case 2:
                if(spiel->gold < KOSTEN_GOLD_STEINBRUCH)
                {
                    printf("Du hast nicht genug Gold um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->holz < KOSTEN_HOLZ_STEINBRUCH)
                {
                    printf("Du hast nicht genug Holz um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->stein < KOSTEN_STEIN_STEINBRUCH)
                {
                    printf("Du hast nicht genug Stein um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(helper == 0) break;

                spiel->gold -= KOSTEN_GOLD_STEINBRUCH;
                spiel->holz -= KOSTEN_HOLZ_STEINBRUCH;
                spiel->stein -= KOSTEN_STEIN_STEINBRUCH;

                spiel->steinbrueche_gesamt++;
                spiel->steinbrueche_aktiv++;

                printf("Steinbruch gebaut.\n");

                helper = 1;
            break;

            case 3:
                if(spiel->gold < KOSTEN_GOLD_JAGDHUETTE)
                {
                    printf("Du hast nicht genug Gold um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->holz < KOSTEN_HOLZ_JAGDHUETTE)
                {
                    printf("Du hast nicht genug Holz um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->stein < KOSTEN_STEIN_JAGDHUETTE)
                {
                    printf("Du hast nicht genug Stein um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(helper == 0) break;

                spiel->gold -= KOSTEN_GOLD_JAGDHUETTE;
                spiel->holz -= KOSTEN_HOLZ_JAGDHUETTE;
                spiel->stein -= KOSTEN_STEIN_JAGDHUETTE;

                spiel->jagdhuetten_gesamt++;
                spiel->jagdhuetten_aktiv++;

                printf("Jagdhuette gebaut.\n");

                helper = 1;
            break;

            case 4:
                if(spiel->gold < KOSTEN_GOLD_WOHNHAUS)
                {
                    printf("Du hast nicht genug Gold um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->holz < KOSTEN_HOLZ_WOHNHAUS)
                {
                    printf("Du hast nicht genug Holz um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->stein < KOSTEN_STEIN_WOHNHAUS)
                {
                    printf("Du hast nicht genug Stein um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(helper == 0) break;

                spiel->gold -= KOSTEN_GOLD_WOHNHAUS;
                spiel->holz -= KOSTEN_HOLZ_WOHNHAUS;
                spiel->stein -= KOSTEN_STEIN_WOHNHAUS;

                spiel->wohnhaeuser++;
                spiel->zufriedenheit_wohnungen = spiel->wohnhaeuser * WOHNHAUS_WOHNRAUM >= spiel->bewohner ? 100 : 100 - (100 * ((spiel->bewohner - (spiel->wohnhaeuser * WOHNHAUS_WOHNRAUM)) / (float)spiel->bewohner));
                spiel->zufriedenheit_gesamt = berechne_gesamt_zufriedenheit(spiel->zufriedenheit_nahrung, spiel->zufriedenheit_wohnungen, spiel->zufriedenheit_steuern, spiel->zufriedenheit_stadtzustand);

                printf("Wohnhaus gebaut.\n");

                helper = 1;
            break;

            case 5:
                if(spiel->gold < KOSTEN_GOLD_LAGERHAUS)
                {
                    printf("Du hast nicht genug Gold um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->holz < KOSTEN_HOLZ_LAGERHAUS)
                {
                    printf("Du hast nicht genug Holz um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(spiel->stein < KOSTEN_STEIN_LAGERHAUS)
                {
                    printf("Du hast nicht genug Stein um dieses Gebaeude zu bauen.\n");
                    helper = 0;
                }

                if(helper == 0) break;

                spiel->gold -= KOSTEN_GOLD_LAGERHAUS;
                spiel->holz -= KOSTEN_HOLZ_LAGERHAUS;
                spiel->stein -= KOSTEN_STEIN_LAGERHAUS;

                spiel->lagerhaeuser++;
                spiel->lagerplatz = GRUND_LAGERPLATZ + (spiel->lagerhaeuser * LAGERHAUS_LAGERPLATZ);

                printf("Lagerhaus gebaut.\n");

                helper = 1;
            break;

            case 6:
                helper = 1;
            break;

            default:
                printf("Dieses Gebaeude ist uns leider nicht bekannt.\n");
                helper = 0;
            break;
        }

        printf("\n");
    }while(helper != 1);
}


void gebaeude_verwaltungs_menue(spielstand *spiel)
{
    int wahl;
    int aktiv;

    do
    {
        printf("Nr Gebaeudename       Gesamt Aktiv Inaktiv Arbeiter Gold Stein Holz Nahrung\n");
        printf("1. Holzfaellerhuetten % 6d % 5d % 7d % 8d % 4d % 5d % 4d % 7d\n", spiel->holzfaeller_gesamt, spiel->holzfaeller_aktiv, spiel->holzfaeller_inaktiv, -spiel->holzfaeller_aktiv * HOLZFAELLER_ARBEITER, -spiel->holzfaeller_aktiv * HOLZFAELLER_BETRIEBSKOSTEN, 0, spiel->holzfaeller_aktiv * HOLZFAELLER_PRODUKTION, 0);
        printf("2. Steinbrueche       % 6d % 5d % 7d % 8d % 4d % 5d % 4d % 7d\n", spiel->steinbrueche_gesamt, spiel->steinbrueche_aktiv, spiel->steinbrueche_inaktiv, -spiel->steinbrueche_aktiv * STEINBRUCH_ARBEITER, -spiel->steinbrueche_aktiv * STEINBRUCH_BETRIEBSKOSTEN, spiel->steinbrueche_aktiv * STEINBRUCH_PRODUKTION, -spiel->steinbrueche_aktiv * STEINBRUCH_HOLZVERBRAUCH, 0);
        printf("3. Jagdhuetten        % 6d % 5d % 7d % 8d % 4d % 5d % 4d % 7d\n", spiel->jagdhuetten_gesamt, spiel->jagdhuetten_aktiv, spiel->jagdhuetten_inaktiv, -spiel->jagdhuetten_aktiv * JAGDHUETTEN_ARBEITER, -spiel->jagdhuetten_aktiv * JAGDHUETTEN_BETRIEBSKOSTEN, 0, 0, spiel->jagdhuetten_aktiv * JAGDHUETTEN_PRODUKTION);
        printf("   Bewohner           % 6d               % 8d % 4d % 5d % 4d % 7d\n", spiel->bewohner, spiel->bewohner, spiel->bewohner * spiel->steuern, 0, 0, -spiel->bewohner * BEWOHNER_NAHRUNGSVERBRAUCH);
        printf("   Gesamt             % 6d % 5d % 7d % 8d % 4d % 5d % 4d % 7d\n", spiel->holzfaeller_gesamt + spiel->steinbrueche_gesamt + spiel->jagdhuetten_gesamt, spiel->holzfaeller_aktiv + spiel->steinbrueche_aktiv + spiel->jagdhuetten_aktiv, spiel->holzfaeller_inaktiv + spiel->steinbrueche_inaktiv + spiel->jagdhuetten_inaktiv, spiel->bewohner - spiel->holzfaeller_aktiv * HOLZFAELLER_ARBEITER - spiel->steinbrueche_aktiv * STEINBRUCH_ARBEITER - spiel->jagdhuetten_aktiv * JAGDHUETTEN_ARBEITER, spiel->bewohner * spiel->steuern - spiel->holzfaeller_aktiv * HOLZFAELLER_BETRIEBSKOSTEN - spiel->steinbrueche_aktiv * STEINBRUCH_BETRIEBSKOSTEN - spiel->jagdhuetten_aktiv * JAGDHUETTEN_BETRIEBSKOSTEN, spiel->steinbrueche_aktiv * STEINBRUCH_PRODUKTION, spiel->holzfaeller_aktiv * HOLZFAELLER_PRODUKTION - spiel->steinbrueche_aktiv * STEINBRUCH_HOLZVERBRAUCH, spiel->jagdhuetten_aktiv * JAGDHUETTEN_PRODUKTION - spiel->bewohner * BEWOHNER_NAHRUNGSVERBRAUCH);
        printf("Was wollen sie aktivieren/deaktivieren? (4: Beenden) ");
        scanf("%d", &wahl);

        switch(wahl)
        {
            case 1:
                printf("Wie viele Gebaeude sollen aktiv sein? ");
                scanf("%d", &aktiv);

                if(aktiv > spiel->holzfaeller_gesamt || aktiv < 0)
                {
                    printf("Illegale Zahl.\n");
                }
                else
                {
                    spiel->holzfaeller_aktiv = aktiv;
                    spiel->holzfaeller_inaktiv = spiel->holzfaeller_gesamt - aktiv;
                    printf("Bestaetigt.\n");
                }
            break;

            case 2:
                printf("Wie viele Gebaeude sollen aktiv sein? ");
                scanf("%d", &aktiv);

                if(aktiv > spiel->steinbrueche_gesamt || aktiv < 0)
                {
                    printf("Illegale Zahl.\n");
                }
                else
                {
                    spiel->steinbrueche_aktiv = aktiv;
                    spiel->steinbrueche_inaktiv = spiel->steinbrueche_gesamt - aktiv;
                    printf("Bestaetigt.\n");
                }
            break;

            case 3:
                printf("Wie viele Gebaeude sollen aktiv sein? ");
                scanf("%d", &aktiv);

                if(aktiv > spiel->jagdhuetten_gesamt || aktiv < 0)
                {
                    printf("Illegale Zahl.\n");
                }
                else
                {
                    spiel->jagdhuetten_aktiv = aktiv;
                    spiel->jagdhuetten_inaktiv = spiel->jagdhuetten_gesamt - aktiv;
                    printf("Bestaetigt.\n");
                }
            break;

            case 4:

            break;

            default:
                printf("Die Zahl muss zwischen 1 und 4 liegen.\n");
            break;
        }

        printf("\n");
    }while(wahl != 4);
}
