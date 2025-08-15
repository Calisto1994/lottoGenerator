# LottoGenerator

Der _Lottogenerator_ kann Zahlenreihen generieren, mit welchen man dann auf einem Lottoschein (z.B. Lotto 6aus49, EuroJackpot) tippen kann.  
Dabei werden die _Hauptzahlen_ und die _Superzahlen_ (bei EuroJackpot _Eurozahlen_ genannt) als separate Zahlenreihe generiert. Nach der Generierung der Zahlenreihen wird über Schleifen so lange durch diese durch-iteriert, bis es keine doppelten Zahlen mehr gibt (dies widerspräche den Lotto-Regeln und wäre zudem auch physikalisch bei einer Ziehung gar nicht möglich.)  
Der _Lottogenerator_ ist in C geschrieben und kann folgendermaßen (unter Linux/GCC) kompiliert werden:

```bash
gcc -o ./lottoGenerator ./lottoGenerator.c
```

Der Quellcode enthält bereits die notwendigen Anpassungen, sodass der Code auch auf Win32 kompiliert werden kann. Je nach Konfiguration des Systems und des Compilers kann der Befehl jedoch abweichen.  
Wenn die Kompilierung der Windows-Binärdatei unter Linux gewünscht ist, ginge das (Linux Mint/Ubuntu/Debian) folgendermaßen:

```
x86_64-w64-mingw32-gcc -o ./lottoGenerator.exe ./lottoGenerator.c
```

Für das Programm sind **keine** zusätzlichen Programmbibliotheken erforderlich, da keine Funktionen eingesetzt werden, die nicht bereits im C-Standard `stdio.h`, `stdlib.h`, `time.h`, `string.h` sowie systemspezifischen (`Win32`: `windows.h` oder `Linux`: `locale.h`, `unistd.h`)) Standardbibliotheken enthalten sind.

### Handhabung der Binärdatei(en)

Ist das Programm erst einmal kompiliert, kann es folgendermaßen verwendet werden:

```bash
./lottoGenerator [lottoTyp] [anzahlZahlenreihen]
```

Die beiden in eckigen Klammern gefassten Argumente sind dabei völlig optional. Wird nichts angegeben, wird der Lottotyp abgefragt, dabei sind zwei im Code enthalten:

```ini
1="Lotto 6aus49"
2="EuroJackpot"
```

Das zweite Argument `anzahlZahlenreihen` bestimmt, wie viele Zahlenreihen generiert werden. Ist dies nicht angegeben, wird der _Lottogenerator_ standardmäßig 10 Zahlenreihen generieren.