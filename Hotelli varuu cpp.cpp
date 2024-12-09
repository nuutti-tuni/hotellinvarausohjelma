// C++ Kurssi lopputyö hotellivaraus ohjelma
// Nuutti Ruuskanen
// Arvosana 3?

#include <iostream> // Tarvitaan cout ja cin tarkoitukseen.
#include <string> // Tarvitaan kirjainten tallenmiseen.
#include <vector>
#include <iomanip> // Tarvitaan tulostuksen desimaalilukejen tulostukseen (setprecision).
#include <limits>
#include <algorithm> // Tarvitaan algoritmifunktioihin.

using namespace std;

// Luodaan struct varaajan tietojen tallentamiseen
struct Varaaja {
    string nimi;
    string katuOsoite;
    string puhelinNumero;
    int varausNumero;
    vector<string> kanssamatkustajat; // Tehädään kanssamatkustajasta globaali muuttuja vectorilla.
};

// Luodaan struct huonetietojen tallentamiseen
struct Huone {
    int numero;
    int opiskelijaAle;
    bool onYhdenhuone;
    bool onVarattu = false; 
    Varaaja varaaja = {}; // Tuodaan huonetietoihin varaajan tiedot
};

const int YHDEN_HENGEN_HINTA = 100; // Määritetään hinta yhden hengen huoneelle.
const int KAHDEN_HENGEN_HINTA = 150; // Määritetään hinta kahden hengen huoneelle.


vector<Huone> hotellinHuoneet; // Lisätään globaali muuttuja huonelista vectorilla structiin "Huone".

void arvoHuonenumerot() { // Tämä aliohjlelma arpoo huonenumeron ja alennuksen.

    for (int i = 1; i <= 17; ++i) { // Määritetään hotelliin 17 huonetta.
        Huone uusiHuone; // Tuodaan huonestructiin huone
        uusiHuone.numero = rand() % 150 + 1; // Arvotaan huonenumerot välille 1-150.
        uusiHuone.onYhdenhuone = (i <= 7); // Asetattaan 7 ensimmäistä huonetta yhden hengen huoneiksi.
        uusiHuone.opiskelijaAle = 7 + rand() % 11; // Arvotaan alennus välille 7-17%.
        hotellinHuoneet.push_back(uusiHuone); // Tuodaan huone huonelistaukseen
    }
}

// Tulostaa hotellin huonelistauksen
void tulostaHuoneet() {

    cout << "\nTässä ovat saatavilla olevat huoneemme:\n";
    cout << "Huoneen numero  -  Huonetyyppi  -  Onko varattu  -  Opiskelija-alennus\n";
    cout << "-------------------------------------------------------------\n";

    // Rakennetaan kenttä saataville hotellihuoneille
    for (const auto& huone : hotellinHuoneet) {
        cout << setw(5) << huone.numero << "         | " // Huoneen numero.
            << setw(17) << (huone.onYhdenhuone ? "Yhden hengen" : "Kahden hengen") << "  | " // Huoneen tyyppi.
            << (huone.onVarattu ? "Kyllä" : "Ei") << "       | " // Tulostaa varauksen statuksen.
            << huone.opiskelijaAle << "%\n"; // Tulostaa arvotun alennus prosentin.
    }
}

// 1 Vaihtoehto: Huoneen varaus Void aliohjelma suorittaa varauksen

void varaaHuone() {
    int huoneNumero, matkustajienMaara; // Määritetään muuttujat huoneille ja matkustajamäärälle.
    string nimi, katuOsoite, puhelinNumero; // Määrietään muuttujat varaajan tiedoille.

    tulostaHuoneet(); // Tulosttaan menuun arvotut hotellihuoneet

    cout << "Minkä huoneen haluaisitte varata? "; // Kysytään huonetta.
    cin >> huoneNumero; // Kirjoitetaan huoneen numero muistiin.

    bool huoneLoytyi = false; // Trakastetaan löytyykö huonetta
    for (auto& huone : hotellinHuoneet) { // Käydään kaikki huoneet läpi listasta auto komennolla.
        if (huone.numero == huoneNumero) { // Tarkistetaan huonenumero.
            if (!huone.onVarattu) { // Tarkistetaan huoneen varaustilanne.
                cin.ignore(); // Tyhjennetään syöttökenttä, jos huone varattu.

                // Kysytään, millä nimellä huone varataan.
                cout << "Millä nimellä huone varataan?: ";
                getline(cin, nimi); // Luetaan annettu nimi.

                // Kysytään käyttäjän katuosoite.
                cout << "Mikä on katuosoitteenne?: ";
                getline(cin, katuOsoite); // Luetaan annettu katuosoite.

                // Kysytään käyttäjän puhelin numero.
                cout << "Saisinko vielä puhelinnumeronne? ";
                while (true) {
                    getline(cin, puhelinNumero); // Luetaan annettu puhelin numero.

                    // Tarkistetaan, että puhelin numero on numeraalinen tai kenttä on täytetty.
                    if (!puhelinNumero.empty() && all_of(puhelinNumero.begin(), puhelinNumero.end(), ::isdigit)) {
                        break;

                        // Tulostetaan virheilmoitus
                    }
                    cout << "Puhelinnumeron tulee olla numeerinen, yritä uudelleen: ";
                }

                cout << "Onko teillä kanssamatkustajia? (0 jos ei ole): "; // Kysytään onko kanssamatkustajaa 
                cin >> matkustajienMaara;
                cin.ignore(); // Tyhjennetään kenttä lukemisen helpoittamiseksi.

                vector<string> kanssamatkustajat; // Luodaan globaali muutuja kanssamatkustajalle
                for (int i = 0; i < matkustajienMaara; ++i) {
                    string matkustajanNimi;
                    cout << "Anna kanssamatkustajan " << i + 1 << " nimi: "; // Pyydetään kanssamatkustajan nimi.
                    getline(cin, matkustajanNimi); // Luetaan matkustajan ninmi.
                    kanssamatkustajat.push_back(matkustajanNimi);
                }

                // Tulostetaan varausnumero
                static int seuraavaVarausNumero = 1;
                huone.varaaja = { nimi, katuOsoite, puhelinNumero, seuraavaVarausNumero++, kanssamatkustajat };
                huone.onVarattu = true;

                // Lasketaan hotellihuoneen hinta
                int perushinta = huone.onYhdenhuone ? YHDEN_HENGEN_HINTA : KAHDEN_HENGEN_HINTA;
                double alennettuHinta = perushinta * (1 - huone.opiskelijaAle / 100.0);

                // Tulostetaan varaustiedot
                cout << "Huone " << huoneNumero << " on nyt varattu nimellä " << nimi
                    << ". Varausnumero on " << huone.varaaja.varausNumero << ".\n"

                    // Asetetaan käyttöön 2 desimaalin tarkkuus.
                    << "Hinta: " << fixed << setprecision(2) << alennettuHinta << " euroa ("
                    << huone.opiskelijaAle << "% opiskelija-alennus).\n";

                if (!kanssamatkustajat.empty()) { // Tarkastellaan onko kanssamatkustajia if/else menetelmällä.
                    cout << "Kanssamatkustajat: ";
                    for (const auto& matkustaja : kanssamatkustajat) {
                        cout << matkustaja << ", ";
                    }
                    cout << "\n";
                }
                huoneLoytyi = true;
                break; // Lopetetaan looppi.
            }

            // Jos huone on varattu tulostetaan ilmoitus.
            else {
                cout << "Huone " << huoneNumero << " on jo varattu.\n";
                huoneLoytyi = true;
                break; // Lopetetaan looppi.
            }
        }
    }

    // Jos huonetta ei löytynyt tulostetaan ilmoitus.
    if (!huoneLoytyi) {
        cout << "Huonetta numero " << huoneNumero << " ei löytynyt.\n";
    }
}

// 2 Vaihtoehto: Varauksen tarkistus Void aliohjelmalla

void tarkistaVaraus() {
    int varausNumero; // Varausnumeron tallennus
    cout << "Anna varausnumero: "; // Pyydetään varausnumeroa
    cin >> varausNumero; // Luetaan varausnumero

    bool varausLoytyi = false; // Alustetaan muuttuja tarkistamaan, löytyykö varaus

    for (const auto& huone : hotellinHuoneet) {

        // Tarkistetaan, onko huoneessa oleva varausnumero sama kuin käyttäjän antama

        if (huone.varaaja.varausNumero == varausNumero) {
            // Varaus löytyi tulostetaan tiedot
            cout << "Varaus löytyi! Varaajan nimi: " << huone.varaaja.nimi

                << ", Huone: " << huone.numero << ".\n" // Varaajan nimi ja huonenumero
                << "Osoite: " << huone.varaaja.katuOsoite // Varaajan osoite
                << ", Puhelinnumero: " << huone.varaaja.puhelinNumero << ".\n"; // Varaajan puhelinnumero

            // Tarkistetaan, onko kanssamatkustajia
            if (!huone.varaaja.kanssamatkustajat.empty()) {
                cout << "Kanssamatkustajat: "; // Tulostuksen aloitus kanssamatkustajille
                for (const auto& matkustaja : huone.varaaja.kanssamatkustajat) {
                    cout << matkustaja << ", "; // Tulostetaan kanssamatkustajien nimi/nimet
                }
                cout << "\n"; // Rivinvaihto lukemisen selkeyttämiseksi
            }
            varausLoytyi = true; // Merkitään, että varaus löytyi
            break; // Lopetetaan looppi, kun varaus on löytynyt
        }
    }

    // Virheilmoituksen tulostus, jos varausta ei löydy
    if (!varausLoytyi) {
        cout << "Varausta ei löytynyt annetulla numerolla.\n"; // Virheilmoitus
    }
}


// 3 Vaihtoehto: Varauksen peruutus Void aliohjelmalla

void peruVaraus() {

    int varausNumero; // Etsitään käyttäjän huone, jotta varaus voidaan perua
    cout << "Anna peruutettavan varauksen numero: "; // Pyydetään käyttäjää syöttämään varausnumero
    cin >> varausNumero; // Luetaan käyttäjän antama varausnumero

    bool varausLoytyi = false; // Tarkastetaan köytyykö varausta

    // Etsitään löytyykö varausnumeroa
    for (auto& huone : hotellinHuoneet) {
        if (huone.varaaja.varausNumero == varausNumero) { // Tarkistetaan täsmääkö varausnumerot

            // Jos tiedot ovat oikein tulostetaan peruutus ilmoitus
            cout << "Varaus " << huone.numero << " nimellä " << huone.varaaja.nimi << " on nyt peruttu.\n";

            // Tyhjennetään varaajan tiedot ja asetetaan huone vapaaksi

            huone.varaaja = { "", "", "", 0, {} }; // Poistetaan varaaja listalta
            huone.onVarattu = false; // Muutetaan huone vapaaksi

            varausLoytyi = true; // Merkitään varaus peruutetuksi
            break; // Lopetetaan looppi.
        }
    }

    // Jos silmukan jälkeen varausta ei löytynyt, ilmoitetaan käyttäjälle
    if (!varausLoytyi) {
        cout << "Varausta ei löytynyt annetulla numerolla.\n"; // Virheilmoitus
    }
}


// 4 Vaihtoehto: Valikosta poistuminen Void aliohjelmalla

void poistuValikosta() { // Tätä ei olisi välttämättä tarvinnut tehdä voidilla, mutta olin tehnyt jo casen:)
    cout << "Kiitos käynnistä ja tervetuloa uudelleen!\n";
    exit(0); // Return komento ei toimi Void aliohjelmissa, siksi exit.
}


// Pääohjlemana toimii käyttäjäpaneeli johoin tuodaan tietoa esimerksiksi void aliohjelmistani.

int main() {

    setlocale(LC_ALL, "fi_FI"); // Asetetaan ääkköset näkyviin
    arvoHuonenumerot(); // Void aliohjelma näyttää saatavilla olevat huoneet ja aleprosentit

    // Rakennetaan yksinkertainen käyttäjäpaneeli

    while (true) {
        int valinta;
        cout << "\nTervetuloa Scandic Laajavuoreen! Valitse palvelun numero.\n";
        cout << "-----------------------------------------------------------\n";
        cout << "1. Varaa huone\n";
        cout << "2. Tarkista varaus\n";
        cout << "3. Peru varaus\n";
        cout << "4. Poistu\n";
        cout << "Valintasi: ";

        cin >> valinta; // Käyttäjä syöttää numeron, josta tulee valinta
 
// Rakennetaan "failsafe", eli jos käyttäjä syöttää jotain muuta, kuin numerot 1-4 ohjelma aloittaa alusta.

     if (cin.fail() || valinta < 1 || valinta > 4) {
     cout << "Meinasi mennä bitti solmuun, yrittäisittekö uudelleen?\n"; // Tulostaa hauskan virheilmoituksen.
     cin.clear(); // Tyhjennetään virhe ilmoitus
     cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennetään tekstikenttä
     continue; // Ohjelma jatkuu
     }

        // Loin case rankenteen selkeyttämään ohljelman hahmottelua.
        // Case rankentteella oli helppo määrittää esimerkiksi mitä valintoja menuun halutaan.
        // Caseissa on aliohjelmat, joilla tuodaan haluttua dataa pääohjelmaan.

        switch (valinta) {

        case 1:
            varaaHuone();
            break; // Lopetetaan looppi.

        case 2:
            tarkistaVaraus();
            break; // Lopetetaan looppi.

        case 3:
            peruVaraus();
            break; // Lopetetaan looppi.

        case 4:
            poistuValikosta();
            break; // Lopetetaan looppi.
        }
    }

    return 0;
}
