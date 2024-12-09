// C++ Kurssi lopputy� hotellivaraus ohjelma
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
    vector<string> kanssamatkustajat; // Teh�d��n kanssamatkustajasta globaali muuttuja vectorilla.
};

// Luodaan struct huonetietojen tallentamiseen
struct Huone {
    int numero;
    int opiskelijaAle;
    bool onYhdenhuone;
    bool onVarattu = false; 
    Varaaja varaaja = {}; // Tuodaan huonetietoihin varaajan tiedot
};

const int YHDEN_HENGEN_HINTA = 100; // M��ritet��n hinta yhden hengen huoneelle.
const int KAHDEN_HENGEN_HINTA = 150; // M��ritet��n hinta kahden hengen huoneelle.


vector<Huone> hotellinHuoneet; // Lis�t��n globaali muuttuja huonelista vectorilla structiin "Huone".

void arvoHuonenumerot() { // T�m� aliohjlelma arpoo huonenumeron ja alennuksen.

    for (int i = 1; i <= 17; ++i) { // M��ritet��n hotelliin 17 huonetta.
        Huone uusiHuone; // Tuodaan huonestructiin huone
        uusiHuone.numero = rand() % 150 + 1; // Arvotaan huonenumerot v�lille 1-150.
        uusiHuone.onYhdenhuone = (i <= 7); // Asetattaan 7 ensimm�ist� huonetta yhden hengen huoneiksi.
        uusiHuone.opiskelijaAle = 7 + rand() % 11; // Arvotaan alennus v�lille 7-17%.
        hotellinHuoneet.push_back(uusiHuone); // Tuodaan huone huonelistaukseen
    }
}

// Tulostaa hotellin huonelistauksen
void tulostaHuoneet() {

    cout << "\nT�ss� ovat saatavilla olevat huoneemme:\n";
    cout << "Huoneen numero  -  Huonetyyppi  -  Onko varattu  -  Opiskelija-alennus\n";
    cout << "-------------------------------------------------------------\n";

    // Rakennetaan kentt� saataville hotellihuoneille
    for (const auto& huone : hotellinHuoneet) {
        cout << setw(5) << huone.numero << "         | " // Huoneen numero.
            << setw(17) << (huone.onYhdenhuone ? "Yhden hengen" : "Kahden hengen") << "  | " // Huoneen tyyppi.
            << (huone.onVarattu ? "Kyll�" : "Ei") << "       | " // Tulostaa varauksen statuksen.
            << huone.opiskelijaAle << "%\n"; // Tulostaa arvotun alennus prosentin.
    }
}

// 1 Vaihtoehto: Huoneen varaus Void aliohjelma suorittaa varauksen

void varaaHuone() {
    int huoneNumero, matkustajienMaara; // M��ritet��n muuttujat huoneille ja matkustajam��r�lle.
    string nimi, katuOsoite, puhelinNumero; // M��riet��n muuttujat varaajan tiedoille.

    tulostaHuoneet(); // Tulosttaan menuun arvotut hotellihuoneet

    cout << "Mink� huoneen haluaisitte varata? "; // Kysyt��n huonetta.
    cin >> huoneNumero; // Kirjoitetaan huoneen numero muistiin.

    bool huoneLoytyi = false; // Trakastetaan l�ytyyk� huonetta
    for (auto& huone : hotellinHuoneet) { // K�yd��n kaikki huoneet l�pi listasta auto komennolla.
        if (huone.numero == huoneNumero) { // Tarkistetaan huonenumero.
            if (!huone.onVarattu) { // Tarkistetaan huoneen varaustilanne.
                cin.ignore(); // Tyhjennet��n sy�tt�kentt�, jos huone varattu.

                // Kysyt��n, mill� nimell� huone varataan.
                cout << "Mill� nimell� huone varataan?: ";
                getline(cin, nimi); // Luetaan annettu nimi.

                // Kysyt��n k�ytt�j�n katuosoite.
                cout << "Mik� on katuosoitteenne?: ";
                getline(cin, katuOsoite); // Luetaan annettu katuosoite.

                // Kysyt��n k�ytt�j�n puhelin numero.
                cout << "Saisinko viel� puhelinnumeronne? ";
                while (true) {
                    getline(cin, puhelinNumero); // Luetaan annettu puhelin numero.

                    // Tarkistetaan, ett� puhelin numero on numeraalinen tai kentt� on t�ytetty.
                    if (!puhelinNumero.empty() && all_of(puhelinNumero.begin(), puhelinNumero.end(), ::isdigit)) {
                        break;

                        // Tulostetaan virheilmoitus
                    }
                    cout << "Puhelinnumeron tulee olla numeerinen, yrit� uudelleen: ";
                }

                cout << "Onko teill� kanssamatkustajia? (0 jos ei ole): "; // Kysyt��n onko kanssamatkustajaa 
                cin >> matkustajienMaara;
                cin.ignore(); // Tyhjennet��n kentt� lukemisen helpoittamiseksi.

                vector<string> kanssamatkustajat; // Luodaan globaali muutuja kanssamatkustajalle
                for (int i = 0; i < matkustajienMaara; ++i) {
                    string matkustajanNimi;
                    cout << "Anna kanssamatkustajan " << i + 1 << " nimi: "; // Pyydet��n kanssamatkustajan nimi.
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
                cout << "Huone " << huoneNumero << " on nyt varattu nimell� " << nimi
                    << ". Varausnumero on " << huone.varaaja.varausNumero << ".\n"

                    // Asetetaan k�ytt��n 2 desimaalin tarkkuus.
                    << "Hinta: " << fixed << setprecision(2) << alennettuHinta << " euroa ("
                    << huone.opiskelijaAle << "% opiskelija-alennus).\n";

                if (!kanssamatkustajat.empty()) { // Tarkastellaan onko kanssamatkustajia if/else menetelm�ll�.
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

    // Jos huonetta ei l�ytynyt tulostetaan ilmoitus.
    if (!huoneLoytyi) {
        cout << "Huonetta numero " << huoneNumero << " ei l�ytynyt.\n";
    }
}

// 2 Vaihtoehto: Varauksen tarkistus Void aliohjelmalla

void tarkistaVaraus() {
    int varausNumero; // Varausnumeron tallennus
    cout << "Anna varausnumero: "; // Pyydet��n varausnumeroa
    cin >> varausNumero; // Luetaan varausnumero

    bool varausLoytyi = false; // Alustetaan muuttuja tarkistamaan, l�ytyyk� varaus

    for (const auto& huone : hotellinHuoneet) {

        // Tarkistetaan, onko huoneessa oleva varausnumero sama kuin k�ytt�j�n antama

        if (huone.varaaja.varausNumero == varausNumero) {
            // Varaus l�ytyi tulostetaan tiedot
            cout << "Varaus l�ytyi! Varaajan nimi: " << huone.varaaja.nimi

                << ", Huone: " << huone.numero << ".\n" // Varaajan nimi ja huonenumero
                << "Osoite: " << huone.varaaja.katuOsoite // Varaajan osoite
                << ", Puhelinnumero: " << huone.varaaja.puhelinNumero << ".\n"; // Varaajan puhelinnumero

            // Tarkistetaan, onko kanssamatkustajia
            if (!huone.varaaja.kanssamatkustajat.empty()) {
                cout << "Kanssamatkustajat: "; // Tulostuksen aloitus kanssamatkustajille
                for (const auto& matkustaja : huone.varaaja.kanssamatkustajat) {
                    cout << matkustaja << ", "; // Tulostetaan kanssamatkustajien nimi/nimet
                }
                cout << "\n"; // Rivinvaihto lukemisen selkeytt�miseksi
            }
            varausLoytyi = true; // Merkit��n, ett� varaus l�ytyi
            break; // Lopetetaan looppi, kun varaus on l�ytynyt
        }
    }

    // Virheilmoituksen tulostus, jos varausta ei l�ydy
    if (!varausLoytyi) {
        cout << "Varausta ei l�ytynyt annetulla numerolla.\n"; // Virheilmoitus
    }
}


// 3 Vaihtoehto: Varauksen peruutus Void aliohjelmalla

void peruVaraus() {

    int varausNumero; // Etsit��n k�ytt�j�n huone, jotta varaus voidaan perua
    cout << "Anna peruutettavan varauksen numero: "; // Pyydet��n k�ytt�j�� sy�tt�m��n varausnumero
    cin >> varausNumero; // Luetaan k�ytt�j�n antama varausnumero

    bool varausLoytyi = false; // Tarkastetaan k�ytyyk� varausta

    // Etsit��n l�ytyyk� varausnumeroa
    for (auto& huone : hotellinHuoneet) {
        if (huone.varaaja.varausNumero == varausNumero) { // Tarkistetaan t�sm��k� varausnumerot

            // Jos tiedot ovat oikein tulostetaan peruutus ilmoitus
            cout << "Varaus " << huone.numero << " nimell� " << huone.varaaja.nimi << " on nyt peruttu.\n";

            // Tyhjennet��n varaajan tiedot ja asetetaan huone vapaaksi

            huone.varaaja = { "", "", "", 0, {} }; // Poistetaan varaaja listalta
            huone.onVarattu = false; // Muutetaan huone vapaaksi

            varausLoytyi = true; // Merkit��n varaus peruutetuksi
            break; // Lopetetaan looppi.
        }
    }

    // Jos silmukan j�lkeen varausta ei l�ytynyt, ilmoitetaan k�ytt�j�lle
    if (!varausLoytyi) {
        cout << "Varausta ei l�ytynyt annetulla numerolla.\n"; // Virheilmoitus
    }
}


// 4 Vaihtoehto: Valikosta poistuminen Void aliohjelmalla

void poistuValikosta() { // T�t� ei olisi v�ltt�m�tt� tarvinnut tehd� voidilla, mutta olin tehnyt jo casen:)
    cout << "Kiitos k�ynnist� ja tervetuloa uudelleen!\n";
    exit(0); // Return komento ei toimi Void aliohjelmissa, siksi exit.
}


// P��ohjlemana toimii k�ytt�j�paneeli johoin tuodaan tietoa esimerksiksi void aliohjelmistani.

int main() {

    setlocale(LC_ALL, "fi_FI"); // Asetetaan ��kk�set n�kyviin
    arvoHuonenumerot(); // Void aliohjelma n�ytt�� saatavilla olevat huoneet ja aleprosentit

    // Rakennetaan yksinkertainen k�ytt�j�paneeli

    while (true) {
        int valinta;
        cout << "\nTervetuloa Scandic Laajavuoreen! Valitse palvelun numero.\n";
        cout << "-----------------------------------------------------------\n";
        cout << "1. Varaa huone\n";
        cout << "2. Tarkista varaus\n";
        cout << "3. Peru varaus\n";
        cout << "4. Poistu\n";
        cout << "Valintasi: ";

        cin >> valinta; // K�ytt�j� sy�tt�� numeron, josta tulee valinta
 
// Rakennetaan "failsafe", eli jos k�ytt�j� sy�tt�� jotain muuta, kuin numerot 1-4 ohjelma aloittaa alusta.

     if (cin.fail() || valinta < 1 || valinta > 4) {
     cout << "Meinasi menn� bitti solmuun, yritt�isittek� uudelleen?\n"; // Tulostaa hauskan virheilmoituksen.
     cin.clear(); // Tyhjennet��n virhe ilmoitus
     cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Tyhjennet��n tekstikentt�
     continue; // Ohjelma jatkuu
     }

        // Loin case rankenteen selkeytt�m��n ohljelman hahmottelua.
        // Case rankentteella oli helppo m��ritt�� esimerkiksi mit� valintoja menuun halutaan.
        // Caseissa on aliohjelmat, joilla tuodaan haluttua dataa p��ohjelmaan.

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
