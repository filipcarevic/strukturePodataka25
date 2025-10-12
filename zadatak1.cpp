/*
 Napisati program koji prvo pročita koliko redaka ima datoteka, 
 tj. koliko ima studenata zapisanih u datoteci. Nakon toga potrebno 
 je dinamički alocirati prostor za niz struktura studenata (ime, prezime, bodovi) i 
 učitati iz datoteke sve zapise. Na ekran ispisati ime, prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju. 
relatvan_br_bodova = br_bodova/max_br_bodova*100
*/

#include <iostream>
#include <fstream> // za rad s datotekama
#include <string> // za rad s stringovima umjesto sa cahr[]
using namespace std;

// defniranje strukutre i njezinih clanova
struct Student {
    string ime;
    string prezime;
    double bodovi; // tipa double je zbog toga sta cemo morat racunat postotke 
};

int main() {
    string imeDatoteke = "studenti.txt"; // ime datoteke za citanje, na desktopu je
    ifstream datoteka(imeDatoteke); // otvara istu datoteku za citanje

    // provjera je li datoteka otvoran, mozda nepotreban korak
    if (!datoteka.is_open()) {
        cout << "Ne mogu otvoriti datoteku!" << endl;
        return 1;
    }

    // sliuzi smao da prebroji koliko redaka ima datoteka 
    int brojRedaka = 0;
    string linija; // linija je string varijabla koja se koristi samo za čitanje jednog reda iz datoteke
    while (getline(datoteka, linija)) brojRedaka++; // u uvejtu while se koristi funkcija getline koja cita jedan red iz datoteke iz onda ga spremi u var. liiju

    datoteka.clear(); // resetira stanje datoteke 
    datoteka.seekg(0, ios::beg); // cita datoteku ponovo od pocetka

    Student* studenti = new Student[brojRedaka]; // c++ verzija malloc stvaranje liste
    double maxBodova = 0;

    for (int i = 0; i < brojRedaka; i++) {
        datoteka >> studenti[i].ime >> studenti[i].prezime >> studenti[i].bodovi; // podaci se citaju iz datoteke i spremaju se u variable iz strukture, a kako je to niz elemanta, mora se koratiti index se spreme elemtni
        if (studenti[i].bodovi > maxBodova) maxBodova = studenti[i].bodovi;
    }
    datoteka.close(); // zatvara datoteku kad vise nije potrebna

    cout << "Ime\tPrezime\tBodovi\tRelativni %" << endl; // samo ispisuje zagalavlelje "tablice"
    // ispiuje rijesenje/podatke iz datoteke 
    for (int i = 0; i < brojRedaka; i++) {
        double relativni = (studenti[i].bodovi / maxBodova) * 100;
        cout << studenti[i].ime << "\t" 
             << studenti[i].prezime << "\t" 
             << studenti[i].bodovi << "\t" 
             << relativni << "%" << endl;
    }

    delete[] studenti; // ekvivalnet free(studenti);
    return 0;
}

