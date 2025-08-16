#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define ERROR_LOCALE "Warnung: Locale konnte nicht gesetzt werden.\n"

#ifndef WIN32
    #include <locale.h>
    #include <unistd.h>
    void Sleep (int milliseconds) {
        usleep(milliseconds*1000);
    }
#else
    #include <windows.h>
#endif

int main(int argc, char* argv[]);
int zufallsZahl(int min, int max);
int compare (const void *a, const void *b);

int compare (const void *a, const void *b) { // Vergleichsfunktion für qsort-Algorithmus zur Sortierung der Zahlen
    return (*(int*)a - *(int*)b);
}

int main (int argc, char* argv[]) {
    #ifdef WIN32
        if (!SetConsoleCP(65001) || !SetConsoleOutputCP(65001)) fprintf(stderr, ERROR_LOCALE);
    #else
        if (!setlocale(LC_ALL, "de_DE.UTF-8")) fprintf(stderr, ERROR_LOCALE);
    #endif

    int lottoType, anzahlZahlenreihen, countNum, lottoMin, lottoMax, countSupNum, lottoSupMin, lottoSupMax;
    if (argc > 1) { // Kommandozeilenargument abfragen. Auch hier kann dann direkt 1 (6aus49) oder 2 (EuroJackpot) gewählt werden.
        if (strcmp(argv[1], "--help") < 1) {
            char* outputString =    "--- Hilfe zum Lotto-Generator ---\n"
                                    "./%s [lottoTyp(1 oder 2)] [lottoReihen]\n"
                                    "\n"
                                    "lottoTyp:\n"
                                    "  1 = Lotto 6aus49\n"
                                    "  2 = EuroJackpot\n"
                                    "\n"
                                    "lottoReihen\n"
                                    "  Anzahl an Zahlenreihen, die generiert werden sollen\n"
                                    "\n"
                                    "Ohne Kommandozeilenargument wird der Lottotyp erfragt und 10 Zahlenreihen standardmäßig generiert.\n";

            printf(outputString, argv[0]);
            return 0;
        }
        lottoType = (int)strtol(argv[1], NULL, 10);
    } 
    else {
        printf("Welcher Lottotyp ist gewünscht? (1=6aus49, 2=EuroJackpot): ");
        scanf(" %d", &lottoType);
    }

    switch (lottoType) {
        case 1:                 // Lotto 6aus49
            lottoMin = 1;       // Die einzelnen Lottozahlen sind immer >= 1
            lottoMax = 49;      // Die einzelnen Lottozahlen sind immer <= 49
            countNum = 6;       // Es werden 6 Lottozahlen generiert
            lottoSupMin = 0;    // Die einzelnen Superzahlen sind immer >= 0
            lottoSupMax = 9;    // Die einzelnen Superzahlen sind immer <= 9
            countSupNum = 1;    // Es gibt nur eine Superzahl
            break;
        case 2:                 // EuroJackpot
            lottoMin = 1;       // Die einzelnen Lottozahlen sind immer >= 1
            lottoMax = 50;      // Die einzelnen Lottozahlen sind immer <= 50
            countNum = 5;       // Es werden 5 Lottozahlen generiert
            lottoSupMin = 1;    // Die einzelnen Eurozahlen sind immer >= 1
            lottoSupMax = 12;   // Die einzelnen Eurozahlen sind immer <= 12
            countSupNum = 2;    // Es werden zwei Eurozahlen generiert
            break;
        default:
            printf("Du kannst nur 1 oder 2 wählen!\n");
            exit(EXIT_FAILURE); // Programm beenden, Nutzerauswahl war ungültig.
            break;
    }

    if (argc > 2) { // Kommandozeilenargument 2, falls gegeben. Ansonsten 10 Reihen.
        anzahlZahlenreihen = (int)strtol(argv[2], NULL, 10);
    } else anzahlZahlenreihen = 10;

    srand((unsigned int)time(NULL)); // Zahlengenerator wird nur einmalig initialisiert vor der Schleife
    rand(); // Erste Zufallszahl verwerfen; Ermöglicht Portabilität des Codes auf ältere Systeme/Compiler, bei welchen der Zahlengenerator in den ersten zwei Durchläufen
    // gleiche oder zumindest sehr ähnliche/nahe Zahlen lieferte.

    // Zwei Arrays zur Prüfung nach Dubletten (unabhängige Prüfung für Hauptzahlen sowie Euro-/Superzahlen)
    int *doubleNums = calloc(countNum,sizeof(int)); if (doubleNums == NULL) fprintf(stderr, "Fehler bei Speicherzuordnung\n"), exit(EXIT_FAILURE);
    int *doubleSupNums = calloc(countSupNum,sizeof(int)); if (doubleSupNums == NULL) fprintf(stderr, "Fehler bei Speicherzuordnung\n"), exit(EXIT_FAILURE);
    int doubleCounter = 0, intDoubleCounter = 0, countGenNums = 0; // externer Dublettenzähler und interner Dublettenzähler
    // countGenNums zählt JEDE Zufallsgenerierte Zahl, um festzustellen, wie viele Zahlen insgesamt generiert wurden.

    for (int x=0;x<anzahlZahlenreihen;x++) { // Generiere 10 Zahlenreihen
        printf("%d: ", x); // Zeige, wie vielte Zahlenreihe
        for (int i=0;i < countNum; i++) { // Hauptzahlen
            doubleNums[i] = zufallsZahl(lottoMin, lottoMax);
            countGenNums++;
        }
        for (int i=0;i < countSupNum; i++) { // Super- bzw. Eurozahlen
            doubleSupNums[i] = zufallsZahl(lottoSupMin, lottoSupMax);
            countGenNums++;
        }

        for (;;) { // Endlosschleife. Ermöglicht wiederholte Prüfung, so lange notwendig
            for (int index=0;index<countNum;index++) { // Dublettenprüfung Hauptzahlen
                for (int inIndex=0;inIndex<countNum;inIndex++) { // innere Schleife. Prüfe innere gegen äußere Schleife.
                    if (index != inIndex && doubleNums[index] == doubleNums[inIndex]) { // Ausschließen, dass eine Zahl sich selbst (gleicher Index) als Dublette erkennt
                        doubleCounter++;
                        intDoubleCounter++;
                        doubleNums[inIndex] = zufallsZahl(lottoMin, lottoMax);
                        countGenNums++;
                    }
                }
            }

            if (intDoubleCounter == 0) {
                break; // Keine Dubletten mehr zu finden. Ende.
            }

            intDoubleCounter=0; // zurücksetzen
            continue; // und nochmal auf Dubletten prüfen, bis der Counter auf 0 steht.
        }
        for (;;) { // Endlosschleife. Ermöglicht wiederholte Prüfung, so lange notwendig
            for (int index=0;index<countSupNum && countSupNum>1;index++) { // Dublettenprüfung Super- bzw. Eurozahlen (nur bei >1)
                for (int inIndex=0;inIndex<countSupNum;inIndex++) { // innere Schleife. Prüfe innere gegen äußere Schleife.
                    if (index != inIndex && doubleSupNums[index] == doubleSupNums[inIndex]) { // Ausschließen, dass eine Zahl sich selbst (gleicher Index) als Dublette erkennt
                        doubleCounter++;
                        intDoubleCounter++;
                        doubleSupNums[inIndex] = zufallsZahl(lottoSupMin, lottoSupMax);
                        countGenNums++;
                    }
                }
            }

            if (intDoubleCounter == 0) {
                break; // Keine Dubletten mehr zu finden. Ende.
            }

            intDoubleCounter=0; // zurücksetzen
            continue; // und nochmal auf Dubletten prüfen, bis der Counter auf 0 steht.
        }

        if (anzahlZahlenreihen <= 10) Sleep(100); // Nur, wenn 10 oder weniger Zahlenreihen generiert werden, 100ms pro Zeile "schlafen".
        qsort(doubleNums, countNum, sizeof(int), compare); // Sortiere die Zahlen aufsteigend.
        for (int i=0;i<countNum;i++) i==0?printf("%02d",doubleNums[i]):printf(" %02d", doubleNums[i]);
        qsort(doubleSupNums, countSupNum, sizeof(int), compare); // Sortiere die Superzahlen aufsteigend.
        for (int i=0;i<countSupNum;i++) printf(" (%02d)", doubleSupNums[i]);
        printf("\n");
    }
        printf("\n---------------"
               "\n%d Zahlen generiert."
               "\n%d Zahlen regulär"
               "\n%d Zahlen aufgrund von Dubletten"
               "\n---------------"
               "\n", countGenNums, (countGenNums - doubleCounter), doubleCounter);
    free(doubleNums), free(doubleSupNums);
}

int zufallsZahl (int min, int max) {    // Generiere Zufallszahl in einem bestimmten Bereich (von min bis max)
    return (rand() % (max - min + 1) + min);
}