#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

struct header {
    char riff[4];
    long dateigroesse;
    char wave[4];
    char fmt[4];
    long chdatgroesse;
    short wavetyp;
    short kanal;
    long samples_s;
    long bytes_s;
    short blockalign;
    short bits_sample;
    char data[4];
    long groesse_daten;
};

enum typ {
    SINUS, RECHTECK, DREIECK
};

const double PI = 3.14159265358979323846;
const int abtastfrequenz = 44100;
const double y_norm = 30000; // Normierte Amplitude
const int n = 2; // Anzahl Oberschwingungen
const int a[] = {1, 1, 0}; //Amplituden
const double pl[] = {0, 0, 0}; //Phasenverschiebungen linker Kanal in Grad
const double pr[] = {0, 0, 0}; //Phasenverschiebungen rechter Kanal in Grad
const double signaldauer = 10; //Dauer des Signals in Sekunden

char dateiname[] = "test.wav"; //Startwerte für Dateiname, Frequenz, Signalform
double grundfrequenz = 440; //Frequenz in Hertz
typ signalform = RECHTECK;

double signalwert(double t, double a, double f, double phi) {
    double y = sin(2.0 * PI * f * t + phi * PI / 180);

    if (signalform == RECHTECK)
        y = (y > 0) ? 1 : ((y < 0) ? -1 : 0);

    if (signalform == DREIECK)
        y = asin(y) * 2.0 / PI;

    return y * a;
}

void dateierzeugung() //Schreibt die Datei
{
    header h =
            {
                    {'R', 'I', 'F', 'F'},
                    signaldauer * 176400 + 36, //dateigroesse = signaldauer * bytes_s + headergroesse - 8
                    {'W', 'A', 'V', 'E'},
                    {'f', 'm', 't', ' '},
                    16,
                    1,
                    2,
                    abtastfrequenz,
                    176400,
                    4,
                    16,
                    {'d', 'a', 't', 'a'},
                    signaldauer * 176400
            };

    ofstream datei(dateiname, std::ios::binary);

    datei.write((char *) &h, sizeof(h)); //Schreibt header in Datei

    double y_max = 0;
    for (int i = 0; i <= n; i++) {
        y_max += a[i];
    }

    for (int k = 0; k < abtastfrequenz * signaldauer; k++) //Schreibe Samples
    {
        double t = (double) k / (double) abtastfrequenz;

        short kanallinks = 0;
        short kanalrechts = 0;

        for (int i = 0; i <= n; i++) {
            kanallinks += (short) (signalwert(t, a[i], grundfrequenz * (i + 1), pl[i]) * y_norm / y_max);
            kanalrechts += (short) (signalwert(t, a[i], grundfrequenz * (i + 1), pr[i]) * y_norm / y_max);
        }

        datei.write((char *) &kanallinks, sizeof(kanallinks));
        datei.write((char *) &kanalrechts, sizeof(kanalrechts));
    }

    datei.close();
}

int main() {
    dateierzeugung();
    return 0;
}
