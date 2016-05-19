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

const int a0 = 16000, a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0; //Amplituden
const double pl0 = 0, pl1 = 0, pl2 = 0, pl3 = 0, pl4 = 0, pl5 = 0;	//Phasenverschiebung linker Kanal
const double pr0 = 0, pr1 = 0, pr2 = 0, pr3 = 0, pr4 = 0, pr5 = 0;	//Phasenverschiebung rechter Kanal
const int signaldauer = 10;		//Dauer des Signals in Sekunden

char dateiname[] = "D:\\test.wav";	//Startwerte für Dateiname, Frequenz, Signalform
int frequenz = 440;	//Frequenz in Hertz
int signalform = 2; //0=Sinus, 1=Rechteck, 2=Dreieck

int parameterauflistung()
{
    cout << "Dateiname: "<< dateiname << "\n Frequenz(Grundtonfrequenz): "<< frequenz << " \n Signalform(0=Sinus, 1=Rechteck, 2=Dreieck): " << signalform << endl;
    cout << "Wollen sie diese Parameter beibehalten? (1 = ja; sonst nein)" << endl;
    int i;
    cin >> i;
    return i;
}

void parametereingabe() //Abfragen der zu ändernden Werte für Dateiname, Frequenz, Signalform
{
    cout << "Name der Datei? (Die Endung .wav wird empfohlen)" << endl;
    cin >> dateiname;

    cout << "Frequenz? (bei Multiton Grundtonfrequenz)" << endl;
    cin >> frequenz;

    cout << "Signalform? (nur bei Einzelton wichtig; 0=Sinus, 1=Rechteck, 2=Dreieck)" << endl; //nur Einzelton
    cin >> signalform;
}

double ergebnis(double d) //errechnet Werte für verschiedene Signalformen ohne Amplitude
{
//	double d = sin(winkel);	//Sinus

    if(signalform == 1) //Rechteck
    {
        if(d < 0)
        {
            d = -1;
        }
        if(d > 0)
        {
            d = 1;
        }
        else
        {
            d = 0;
        }
    }
    if(signalform == 2) //Dreieck
    {
        d = asin(d);
    }

    return d;
}

void dateierzeugung() //Schreibt die Datei
{
    short kanallinks, kanalrechts;
    struct header	//erzeugen des Headers
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
    auto h =
            {
                    {'R','I','F','F'},
                    signaldauer * 174600 + 36, //dateigroesse = signaldauer * bytes_s + headergroesse - 8
                    {'W','A','V','E'},
                    {'f','m','t',' '},
                    16,
                    1,
                    2,
                    44100,
                    17640,
                    4,
                    16,
                    {'d','a','t','a'},
                    signaldauer * 176400
            };

    ofstream datei(dateiname);

    datei.write((char*)&h, sizeof(h)); //Schreibt header in Datei

    for(double s = 0; s < frequenz * signaldauer * 6.28318530; s+= frequenz * 6.28318530 / 44100)	//schreibt alle Werte in Datei
    {
        kanallinks = a0*ergebnis(sin(s + pl0)) +
                     a1*ergebnis(sin(s + pl1) * 2) +
                     a2*ergebnis(sin(s + pl2) * 3) +
                     a3*ergebnis(sin(s + pl3) * 4) +
                     a4*ergebnis(sin(s + pl4) * 5) +
                     a5*ergebnis(sin(s + pl5) * 6);

        kanalrechts = a0*ergebnis(sin(s + pr0)) +
                      a1*ergebnis(sin(s + pr1) * 2) +
                      a2*ergebnis(sin(s + pr2) * 3) +
                      a3*ergebnis(sin(s + pr3) * 4) +
                      a4*ergebnis(sin(s + pr4) * 5) +
                      a5*ergebnis(sin(s + pr5) * 6);

        datei.write((char*)&kanallinks, sizeof(kanallinks));
        datei.write((char*)&kanalrechts, sizeof(kanalrechts));
    }

    datei.close();

}

int main()
{
    while(1)
    {
        int i = parameterauflistung();
        if (i == 1) break;

        parametereingabe();
    }

    dateierzeugung();
    return 0;
}