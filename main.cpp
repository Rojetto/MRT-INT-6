/*
 * main.cpp
 *
 *  Created on: 19.05.2016
 *      Author: Paul
 */

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const double PI = 3.14159265358979323846;
const int abtastfrequenz = 44100;
const short int a = 32000; //Amplitude
const double pl = 0; //Phasenverschiebung linker Kanal
const double pr = 0; //Phasenverschiebung rechter Kanal
const int signaldauer = 10; //Dauer des Signals in Sekunden

char dateiname[] = "test.wav"; //Startwerte für Dateiname, Frequenz, Signalform
int frequenz = 440; //Frequenz in Hertz
int signalform = 2; //0=Sinus, 1=Rechteck, 2=Dreieck

// |y| <= 1.0
double normiertes_signal(double t) {
    double y = sin(2.0 * PI * frequenz * t);

    if (signalform == 1)
        y = (y > 0) ? 1 : ((y < 0) ? -1 : 0);

    if (signalform == 2)
        y = asin(y) * 2.0 / PI;

    return y;
}

void dateierzeugung() //Schreibt die Datei
{
    struct header    //erzeugen des Headers
    {
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

    for (int k = 0; k < abtastfrequenz * signaldauer; k++) //Schreibe Samples
    {
        double t = (double) k / (double) abtastfrequenz;
        short int kanallinks = a * normiertes_signal(t);
        short int kanalrechts = a * normiertes_signal(t);

        datei.write((char*) &kanallinks, sizeof(kanallinks));
        datei.write((char*) &kanalrechts, sizeof(kanalrechts));
    }

    datei.close();
}

int main() {
    dateierzeugung();
    return 0;
}