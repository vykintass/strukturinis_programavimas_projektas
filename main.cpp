// ============================================================
// NBA Žaidėjų Statistikos Sistema
// Autorius: [Studento vardas]
// Aprašymas: Konsolinė programa NBA komandų ir žaidėjų
//            statistikos valdymui. Leidžia atlikti CRUD
//            operacijas su komandomis ir žaidėjais, filtruoti
//            bei rikiuoti duomenis pagal įvairius kriterijus.
// ============================================================

// Standartinės bibliotekos
#include <iostream>   // cin, cout – įvestis ir išvestis
#include <fstream>    // ifstream, ofstream – failų nuskaitymas ir įrašymas
#include <vector>     // vector – dinaminiai masyvai
#include <string>     // string – teksto tipas
#include <algorithm>  // sort, remove_if – rikiavimas ir filtravimas
#include <iomanip>    // setw, setprecision – formatuota išvestis
#include <sstream>    // stringstream – eilutės skaidymas
#include <limits>     // numeric_limits – cin buferio valymas

using namespace std;

// ============================================================
// STRUKTŪROS
// Aprašomos duomenų struktūros, kurios saugo programos duomenis
// ============================================================

// Žaidėjo struktūra – saugo visą informaciją apie vieną žaidėją
struct Player {
    string firstName;   // Žaidėjo vardas
    string lastName;    // Žaidėjo pavardė
    string team;        // Komandos pavadinimas (ryšys su Team)
    string position;    // Pozicija aikštelėje: PG, SG, SF, PF arba C
    int age;            // Žaidėjo amžius (metais)
    double points;      // Vidutinis taškų skaičius per rungtynes
    double rebounds;    // Vidutinis atkovotų kamuolių skaičius per rungtynes
    double assists;     // Vidutinis rezultatyvių perdavimų skaičius per rungtynes
    double blocks;      // Vidutinis blokų skaičius per rungtynes
    double steals;      // Vidutinis perimtų kamuolių skaičius per rungtynes
    int gamesPlayed;    // Iš viso sužaistų rungtynių skaičius sezone
};

// Komandos struktūra – saugo informaciją apie vieną NBA komandą
struct Team {
    string name;        // Komandos pavadinimas (pvz. "Lakers")
    string city;        // Miestas, kuriam atstovauja komanda
    string conference;  // NBA konferencija: "East" arba "West"
};

// ============================================================
// GLOBALŪS DUOMENYS
// Visi žaidėjai ir komandos saugomi šiuose vektoriuose visą
// programos veikimo laiką. Nuskaitomi iš failų paleidžiant,
// išsaugomi keičiant duomenis.
// ============================================================

vector<Player> players; // Visų žaidėjų sąrašas
vector<Team> teams;     // Visų komandų sąrašas

// ============================================================
// PAGALBINĖS FUNKCIJOS
// Mažos, dažnai naudojamos funkcijos
// ============================================================

// Atspausdina tuščią eilutę – naudojama vizualiam atskirtumui
void clearScreen() {
    cout << "\n";
}

// Sustabdo programą kol vartotojas paspaus Enter
// Naudojama po kiekvienos operacijos, kad vartotojas spėtų perskaityti rezultatą
void waitEnter() {
    cout << "\nSpauskite Enter testi...";
    cin.get();
}

// Tikrina ar komanda su nurodytu pavadinimu jau egzistuoja sąraše
// Grąžina true jei komanda rasta, false jei nerasta
bool teamExists(const string& name) {
    for (const auto& t : teams) {
        if (t.name == name) return true; // Komanda rasta
    }
    return false; // Komanda nerasta
}

// Pašalina tarpus eilutės pradžioje ir pabaigoje
// Reikalinga skaitant duomenis iš failo – jei eilutėje yra papildomų tarpų
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n"); // Pirmas ne-tarpo simbolis
    size_t end = s.find_last_not_of(" \t\r\n");    // Paskutinis ne-tarpo simbolis
    if (start == string::npos) return "";           // Eilutė tuščia arba vien tarpai
    return s.substr(start, end - start + 1);        // Grąžina apkarpytą eilutę
}

// Tikrina ar įvesta pozicija yra viena iš leistinų NBA pozicijų
// PG = Point Guard, SG = Shooting Guard, SF = Small Forward,
// PF = Power Forward, C = Center
bool validPosition(const string& pos) {
    return pos == "PG" || pos == "SG" || pos == "SF" || pos == "PF" || pos == "C";
}

// Tikrina ar įvesta konferencija yra leistina (East arba West)
bool validConference(const string& conf) {
    return conf == "East" || conf == "West";
}

// Atspausdina žaidėjų lentelės antraštę su stulpelių pavadinimais
// Naudojama prieš spausdinant žaidėjų sąrašą
void printPlayerHeader() {
    cout << left
         << setw(18) << "Vardas"    // Vardas ir pavardė kartu
         << setw(12) << "Komanda"   // Komandos pavadinimas
         << setw(6)  << "Poz."      // Pozicija (PG/SG/SF/PF/C)
         << setw(5)  << "Amz."      // Amžius
         << setw(7)  << "Tsk."      // Taškai per rungtynes
         << setw(7)  << "Atkv."     // Atkovoti kamuoliai
         << setw(7)  << "Perd."     // Rezultatyvūs perdavimai
         << setw(7)  << "Blok."     // Blokai
         << setw(7)  << "Perim."    // Perimti kamuoliai
         << setw(6)  << "Rung."     // Sužaista rungtynių
         << "\n";
    cout << string(82, '-') << "\n"; // Atskyrimo linija
}

// Atspausdina vieno žaidėjo duomenis lentelės formatu
// Naudoja fixed ir setprecision(1) – rodo vieną skaičių po kablelio
void printPlayer(const Player& p) {
    cout << left
         << setw(18) << (p.firstName + " " + p.lastName) // Pilnas vardas
         << setw(12) << p.team
         << setw(6)  << p.position
         << setw(5)  << p.age
         << fixed << setprecision(1) // Visi skaičiai su vienu skaitmeniu po kablelio
         << setw(7)  << p.points
         << setw(7)  << p.rebounds
         << setw(7)  << p.assists
         << setw(7)  << p.blocks
         << setw(7)  << p.steals
         << right << setw(4) << p.gamesPlayed << "\n";
}

// ============================================================
// FAILŲ OPERACIJOS
// Nuskaitymas iš tekstinių failų ir įrašymas į juos.
// Failų formatas: laukai atskirti kabliataškiu (;)
// ============================================================

// Nuskaito komandas iš failo "teams.txt"
// Kiekviena eilutė: Pavadinimas;Miestas;Konferencija
void loadTeams() {
    ifstream file("teams.txt"); // Atidaro failą skaitymui
    if (!file.is_open()) return; // Jei failas neegzistuoja – nieko nedaro

    string line;
    // Skaitome eilutė po eilutės kol pasiekiame failo pabaigą
    while (getline(file, line)) {
        if (line.empty()) continue; // Tuščias eilutes praleidžiame

        stringstream ss(line); // Eilutę paverčiame srautu skaidymui
        Team t;
        // Skaidom eilutę pagal skyriklį ';'
        getline(ss, t.name, ';');
        getline(ss, t.city, ';');
        getline(ss, t.conference, ';');

        // Pašaliname tarpus (jei yra)
        t.name = trim(t.name);
        t.city = trim(t.city);
        t.conference = trim(t.conference);

        // Įtraukiame tik jei pavadinimas netuščias
        if (!t.name.empty()) teams.push_back(t);
    }
    file.close(); // Uždarome failą
}

// Išsaugo visas komandas į failą "teams.txt"
// Perrašo visą failą iš naujo su esamais duomenimis
void saveTeams() {
    ofstream file("teams.txt"); // Atidaro failą rašymui (perrašo)
    for (const auto& t : teams) {
        // Kiekviena komanda – viena eilutė, laukai atskirti ';'
        file << t.name << ";" << t.city << ";" << t.conference << "\n";
    }
    file.close();
}

// Nuskaito žaidėjus iš failo "players.txt"
// Kiekviena eilutė: Vardas;Pavardė;Komanda;Pozicija;Amžius;Taškai;...
void loadPlayers() {
    ifstream file("players.txt"); // Atidaro failą skaitymui
    if (!file.is_open()) return;  // Jei failas neegzistuoja – nieko nedaro

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Tuščias eilutes praleidžiame

        stringstream ss(line);
        Player p;
        string tmp; // Laikinas kintamasis skaitinių reikšmių konvertavimui

        // Skaitome tekstinius laukus tiesiogiai į struktūrą
        getline(ss, p.firstName, ';');
        getline(ss, p.lastName, ';');
        getline(ss, p.team, ';');
        getline(ss, p.position, ';');

        // Skaitiniai laukai: pirmiausia nuskaitome į string, tada konvertuojame
        getline(ss, tmp, ';'); p.age = stoi(tmp);         // string → int
        getline(ss, tmp, ';'); p.points = stod(tmp);      // string → double
        getline(ss, tmp, ';'); p.rebounds = stod(tmp);
        getline(ss, tmp, ';'); p.assists = stod(tmp);
        getline(ss, tmp, ';'); p.blocks = stod(tmp);
        getline(ss, tmp, ';'); p.steals = stod(tmp);
        getline(ss, tmp, ';'); p.gamesPlayed = (int)stod(tmp); // stod toleruoja "76.0" formato skaičius

        // Pašaliname tarpus iš tekstinių laukų
        p.firstName = trim(p.firstName);
        p.lastName = trim(p.lastName);
        p.team = trim(p.team);
        p.position = trim(p.position);

        if (!p.firstName.empty()) players.push_back(p);
    }
    file.close();
}

// Išsaugo visus žaidėjus į failą "players.txt"
// Perrašo visą failą iš naujo su esamais duomenimis
void savePlayers() {
    ofstream file("players.txt"); // Atidaro failą rašymui (perrašo)
    for (const auto& p : players) {
        // Kiekvienas žaidėjas – viena eilutė, laukai atskirti ';'
        file << p.firstName << ";"
             << p.lastName << ";"
             << p.team << ";"
             << p.position << ";"
             << p.age << ";"
             << fixed << setprecision(1) // Statistika su 1 skaitmeniu po kablelio
             << p.points << ";"
             << p.rebounds << ";"
             << p.assists << ";"
             << p.blocks << ";"
             << p.steals << ";"
             << p.gamesPlayed << "\n";
    }
    file.close();
}

// ============================================================
// ŽAIDĖJO ĮVEDIMAS
// Universali funkcija naujam žaidėjui įvesti per konsolę.
// Parametras defaultTeam: jei nurodyta – komandos neklausoma
// (naudojama kai pridedame žaidėją iš komandos meniu)
// ============================================================

Player inputPlayer(const string& defaultTeam = "") {
    Player p;

    // Vardas ir pavardė
    cout << "Vardas: "; cin >> p.firstName;
    cout << "Pavarde: "; cin >> p.lastName;

    // Komanda: arba iš parametro, arba klausiame vartotojo
    if (!defaultTeam.empty()) {
        p.team = defaultTeam; // Komanda jau žinoma iš konteksto
        cout << "Komanda: " << p.team << "\n";
    } else {
        cout << "Komanda: "; cin >> p.team;
    }

    // Pozicija su validacija – kartojame kol įvesta teisinga reikšmė
    do {
        cout << "Pozicija (PG/SG/SF/PF/C): "; cin >> p.position;
        if (!validPosition(p.position))
            cout << "Neteisinga pozicija! Bandykite dar karta.\n";
    } while (!validPosition(p.position));

    // Statistiniai duomenys
    cout << "Amzius: ";                    cin >> p.age;
    cout << "Taskai per rungtynes: ";      cin >> p.points;
    cout << "Atkovoti kamuoliai: ";        cin >> p.rebounds;
    cout << "Rezultatyvus perdavimai: ";   cin >> p.assists;
    cout << "Blokai: ";                    cin >> p.blocks;
    cout << "Perimti kamuoliai: ";         cin >> p.steals;
    cout << "Zaista rungtynese: ";         cin >> p.gamesPlayed;

    cin.ignore(); // Išvalo '\n' iš buferio po paskutinio cin >>
    return p;     // Grąžina užpildytą žaidėjo struktūrą
}

// ============================================================
// KOMANDŲ CRUD OPERACIJOS
// Create, Read, Update, Delete funkcijos komandų valdymui
// ============================================================

// READ – parodo visų komandų sąrašą lentelės formatu
void showAllTeams() {
    if (teams.empty()) {
        cout << "Nera issaugotu komandu.\n";
        return;
    }
    // Antraštė
    cout << "\n" << left << setw(20) << "Komanda" << setw(20) << "Miestas" << setw(10) << "Konf." << "\n";
    cout << string(50, '-') << "\n";
    // Spausdinamas kiekvienos komandos įrašas
    for (const auto& t : teams) {
        cout << setw(20) << t.name << setw(20) << t.city << setw(10) << t.conference << "\n";
    }
}

// CREATE – prideda naują komandą į sąrašą ir išsaugo į failą
void addTeam() {
    Team t;
    cout << "Komandos pavadinimas: "; cin >> t.name; cin.ignore();

    // Tikrinamas dublikatų vengimas
    if (teamExists(t.name)) {
        cout << "Tokia komanda jau egzistuoja!\n";
        return;
    }

    cout << "Miestas: "; getline(cin, t.city);

    // Konferencija su validacija
    do {
        cout << "Konferencija (East/West): "; cin >> t.conference; cin.ignore();
        if (!validConference(t.conference))
            cout << "Turi buti East arba West!\n";
    } while (!validConference(t.conference));

    teams.push_back(t); // Pridedame į vektorių
    saveTeams();         // Iškart išsaugome į failą
    cout << "Komanda prideta!\n";
}

// UPDATE – redaguoja esamos komandos duomenis
// Jei pavadinimas pakeistas – atnaujinamas ir visuose žaidėjuose
void editTeam() {
    showAllTeams();
    if (teams.empty()) return;

    cout << "\nIveskite komandos pavadinima redaguoti: ";
    string name; cin >> name; cin.ignore();

    // Ieškome komandos vektoriuje
    for (auto& t : teams) {
        if (t.name == name) {
            // Kiekvienam laukui: rodome esamą reikšmę, Enter – palikti nepakeistą
            cout << "Naujas pavadinimas (Enter - palikti '" << t.name << "'): ";
            string tmp; getline(cin, tmp);
            if (!tmp.empty()) {
                // Svarbu: atnaujinti komandos pavadinimą ir visuose žaidėjuose
                for (auto& p : players) if (p.team == t.name) p.team = tmp;
                t.name = tmp;
            }

            cout << "Naujas miestas (Enter - palikti '" << t.city << "'): ";
            getline(cin, tmp);
            if (!tmp.empty()) t.city = tmp;

            cout << "Nauja konferencija (Enter - palikti '" << t.conference << "'): ";
            getline(cin, tmp);
            if (!tmp.empty() && validConference(tmp)) t.conference = tmp;

            // Išsaugome abu failus, nes žaidėjų komandos pavadinimas galėjo pasikeisti
            saveTeams();
            savePlayers();
            cout << "Komanda atnaujinta!\n";
            return;
        }
    }
    cout << "Komanda nerasta.\n";
}

// DELETE – pašalina komandą ir visus jos žaidėjus iš sąrašų ir failų
void deleteTeam() {
    showAllTeams();
    if (teams.empty()) return;

    cout << "\nIveskite komandos pavadinima istrinti: ";
    string name; cin >> name; cin.ignore();

    // Ieškome komandos iteratoriumi (reikalingas erase operacijai)
    for (auto it = teams.begin(); it != teams.end(); ++it) {
        if (it->name == name) {
            // Pirmiausia triname visus šios komandos žaidėjus
            // remove_if perkelia ištrintus į galą, erase juos pašalina
            players.erase(
                remove_if(players.begin(), players.end(),
                    [&name](const Player& p){ return p.team == name; }),
                players.end()
            );

            teams.erase(it); // Triname pačią komandą
            saveTeams();
            savePlayers();
            cout << "Komanda ir jos zaidejai istrinti!\n";
            return;
        }
    }
    cout << "Komanda nerasta.\n";
}

// Komandų valdymo submeniu – do-while ciklas kol pasirenkama "0. Atgal"
void teamsMenu() {
    int choice;
    do {
        cout << "\n===== KOMANDU VALDYMAS =====\n";
        cout << "1. Rodyti visas komandas\n";
        cout << "2. Prideti komanda\n";
        cout << "3. Redaguoti komanda\n";
        cout << "4. Istrinti komanda\n";
        cout << "0. Atgal\n";
        cout << "Pasirinkimas: "; cin >> choice; cin.ignore();
        // switch/case pagal vartotojo pasirinkimą iškviečia atitinkamą funkciją
        switch (choice) {
            case 1: showAllTeams(); waitEnter(); break;
            case 2: addTeam();      waitEnter(); break;
            case 3: editTeam();     waitEnter(); break;
            case 4: deleteTeam();   waitEnter(); break;
        }
    } while (choice != 0); // Grįžtame į meniu kol vartotojas pasirenka 0
}

// ============================================================
// ŽAIDĖJŲ CRUD (komandos kontekste)
// Šios funkcijos veikia su konkrečios komandos žaidėjais.
// teamName parametras nusako, kurios komandos žaidėjai rodomi.
// ============================================================

// READ – parodo vienos komandos žaidėjų sąrašą
void showTeamPlayers(const string& teamName) {
    bool found = false;
    cout << "\n--- " << teamName << " zaidejai ---\n";
    printPlayerHeader();

    // Filtruojame globalų žaidėjų vektorių pagal komandą
    for (const auto& p : players) {
        if (p.team == teamName) {
            printPlayer(p);
            found = true;
        }
    }
    if (!found) cout << "Sios komandos zaideju nerasta.\n";
}

// CREATE – prideda naują žaidėją į konkrečią komandą
void addPlayerToTeam(const string& teamName) {
    cout << "\n--- Naujas zaidejas: " << teamName << " ---\n";
    // inputPlayer su defaultTeam – komandos lauko nereikia įvesti rankiniu būdu
    Player p = inputPlayer(teamName);
    players.push_back(p); // Pridedame į globalų vektorių
    savePlayers();
    cout << "Zaidejas pridetas!\n";
}

// UPDATE – redaguoja konkretaus žaidėjo duomenis komandos viduje
// Žaidėjas ieškomas pagal pavardę ir komandą
void editPlayerInTeam(const string& teamName) {
    showTeamPlayers(teamName); // Pirmiausia parodome sąrašą orientacijai

    cout << "\nIveskite zaidejo pavarde redaguoti: ";
    string lastName; cin >> lastName; cin.ignore();

    // Ieškome žaidėjo pagal komandą ir pavardę
    for (auto& p : players) {
        if (p.team == teamName && p.lastName == lastName) {
            cout << "Redaguojamas: " << p.firstName << " " << p.lastName << "\n";
            cout << "(Iveskite naują reikšmę arba 0 palikti esamą)\n";

            // Kiekvienam laukui: rodome esamą reikšmę, Enter – palikti
            string tmp;

            cout << "Vardas (" << p.firstName << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.firstName = tmp;

            cout << "Pozicija (" << p.position << "): ";
            getline(cin, tmp);
            // Pozicijai papildoma validacija
            if (tmp != "0" && !tmp.empty() && validPosition(tmp)) p.position = tmp;

            cout << "Amzius (" << p.age << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.age = stoi(tmp); // Konvertuojame į int

            cout << "Taskai (" << p.points << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.points = stod(tmp); // Konvertuojame į double

            cout << "Atkovoti (" << p.rebounds << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.rebounds = stod(tmp);

            cout << "Perdavimai (" << p.assists << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.assists = stod(tmp);

            cout << "Blokai (" << p.blocks << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.blocks = stod(tmp);

            cout << "Perimti (" << p.steals << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.steals = stod(tmp);

            cout << "Rungtynes (" << p.gamesPlayed << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.gamesPlayed = stoi(tmp);

            savePlayers(); // Išsaugome pakeitimus į failą
            cout << "Zaidejas atnaujintas!\n";
            return;
        }
    }
    cout << "Zaidejas nerastas.\n";
}

// DELETE – pašalina žaidėją iš komandos
// Prieš trindamas klausia patvirtinimo (t/n)
void deletePlayerFromTeam(const string& teamName) {
    showTeamPlayers(teamName);

    cout << "\nIveskite zaidejo pavarde istrinti: ";
    string lastName; cin >> lastName; cin.ignore();

    // Naudojamas iteratorius, nes reikia erase operacijos
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->team == teamName && it->lastName == lastName) {
            // Patvirtinimas prieš trynimą
            cout << "Istrinti " << it->firstName << " " << it->lastName << "? (t/n): ";
            char c; cin >> c; cin.ignore();
            if (c == 't' || c == 'T') {
                players.erase(it); // Pašaliname žaidėją iš vektoriaus
                savePlayers();
                cout << "Zaidejas istrinti!\n";
            }
            return;
        }
    }
    cout << "Zaidejas nerastas.\n";
}

// ============================================================
// PAPILDOMA FUNKCIJA #1 – RIKIAVIMAS KOMANDOS VIDUJE
// Surikiuoja pasirinktos komandos žaidėjus pagal statistikos
// kategoriją ir parodo rezultatą ekrane (nekeičia duomenų)
// ============================================================

void sortTeamPlayers(const string& teamName) {
    // Surenkame nuorodas (pointerius) į šios komandos žaidėjus
    // Naudojame pointerius, kad nerašytume kopijų ir operuotume su originalais
    vector<Player*> teamPlayers;
    for (auto& p : players) {
        if (p.team == teamName) teamPlayers.push_back(&p);
    }

    if (teamPlayers.empty()) { cout << "Zaideju nerasta.\n"; return; }

    // Vartotojas pasirenka rikiavimo kriterijų
    cout << "\nRikiuoti pagal:\n";
    cout << "1. Taskus\n2. Atkovotus kamuolius\n3. Perdavimus\n4. Blokus\n5. Perimtus\n";
    cout << "Pasirinkimas: "; int c; cin >> c; cin.ignore();

    // Lambda funkcija kaip palyginimo kriterijus sort algoritmui
    // Rikiuojame mažėjančia tvarka (didžiausias pirmiausia) – a > b
    auto cmp = [&](const Player* a, const Player* b) {
        switch(c) {
            case 1: return a->points   > b->points;    // Pagal taškus
            case 2: return a->rebounds > b->rebounds;  // Pagal atkovotus
            case 3: return a->assists  > b->assists;   // Pagal perdavimus
            case 4: return a->blocks   > b->blocks;    // Pagal blokus
            case 5: return a->steals   > b->steals;    // Pagal perimtus
            default: return a->points  > b->points;    // Numatytasis – pagal taškus
        }
    };

    sort(teamPlayers.begin(), teamPlayers.end(), cmp); // Standartinis rikiavimo algoritmas

    // Rodome surikiuotus rezultatus
    cout << "\n--- " << teamName << " (surikiuota) ---\n";
    printPlayerHeader();
    for (const auto& p : teamPlayers) printPlayer(*p); // Dereferencuojame pointerį
}

// ============================================================
// KOMANDOS MENIU
// Hierarchinis meniu konkrečiai komandai – visos CRUD operacijos
// ir rikiavimas veikia tik su šios komandos žaidėjais
// ============================================================

void teamMenu(const string& teamName) {
    int choice;
    do {
        cout << "\n===== " << teamName << " =====\n";
        cout << "1. Rodyti zaideju sarasa\n";
        cout << "2. Prideti zaideja\n";
        cout << "3. Redaguoti zaideja\n";
        cout << "4. Istrinti zaideja\n";
        cout << "5. Rikiuoti zaideju statistika\n";
        cout << "0. Atgal\n";
        cout << "Pasirinkimas: "; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: showTeamPlayers(teamName);    waitEnter(); break;
            case 2: addPlayerToTeam(teamName);    waitEnter(); break;
            case 3: editPlayerInTeam(teamName);   waitEnter(); break;
            case 4: deletePlayerFromTeam(teamName); waitEnter(); break;
            case 5: sortTeamPlayers(teamName);    waitEnter(); break;
        }
    } while (choice != 0);
}

// Parodo komandų sąrašą ir leidžia vartotojui pasirinkti komandą
// Pagrindiniam meniui susieti su komandos meniu
void selectTeamMenu() {
    if (teams.empty()) {
        cout << "Nera komandu. Pirmiausia pridekite komanda.\n";
        return;
    }
    showAllTeams();
    cout << "\nIveskite komandos pavadinima: ";
    string name; cin >> name; cin.ignore();

    if (!teamExists(name)) {
        cout << "Tokia komanda nerasta.\n";
        return;
    }
    teamMenu(name); // Pereiname į pasirinktos komandos meniu
}

// ============================================================
// PAPILDOMA FUNKCIJA #2 – FILTRAVIMAS IR RIKIAVIMAS
// Leidžia filtruoti VISUS žaidėjus pagal poziciją arba amžių,
// tada rikiuoti pagal pasirinktą statistikos kategoriją.
// Tai yra dviejų papildomų funkcijų kombinacija.
// ============================================================

void sortAllPlayers() {
    if (players.empty()) { cout << "Zaideju nerasta.\n"; return; }

    // filtered – vektorius su nuorodomis į atrinktus žaidėjus
    vector<Player*> filtered;

    // 1 ŽINGSNIS: Filtravimas
    cout << "\nFiltruoti pagal:\n";
    cout << "1. Pozicija\n2. Amziaus grupe\n3. Visi zaidejai (be filtro)\n";
    cout << "Pasirinkimas: "; int filterChoice; cin >> filterChoice; cin.ignore();

    if (filterChoice == 1) {
        // Filtravimas pagal poziciją – atrenkame tik nurodytą poziciją
        cout << "Pozicija (PG/SG/SF/PF/C): "; string pos; cin >> pos; cin.ignore();
        for (auto& p : players)
            if (p.position == pos) filtered.push_back(&p);
        if (filtered.empty()) { cout << "Zaideju su sia pozicija nerasta.\n"; return; }

    } else if (filterChoice == 2) {
        // Filtravimas pagal amžių – trys amžiaus grupės
        cout << "Amziaus grupe:\n1. Iki 25 metu\n2. 25-30 metu\n3. Virs 30 metu\n";
        cout << "Pasirinkimas: "; int ag; cin >> ag; cin.ignore();
        for (auto& p : players) {
            if      (ag == 1 && p.age < 25)                     filtered.push_back(&p);
            else if (ag == 2 && p.age >= 25 && p.age <= 30)     filtered.push_back(&p);
            else if (ag == 3 && p.age > 30)                     filtered.push_back(&p);
        }
        if (filtered.empty()) { cout << "Zaideju siam amziaus filtrue nerasta.\n"; return; }

    } else {
        // Be filtro – imame visus žaidėjus
        for (auto& p : players) filtered.push_back(&p);
    }

    // 2 ŽINGSNIS: Rikiavimas pagal statistikos kategoriją
    cout << "\nRikiuoti pagal:\n";
    cout << "1. Taskus\n2. Atkovotus\n3. Perdavimus\n4. Blokus\n5. Perimtus\n";
    cout << "Pasirinkimas: "; int sortChoice; cin >> sortChoice; cin.ignore();

    // Lambda funkcija rikiavimui – mažėjančia tvarka
    auto cmp = [&](const Player* a, const Player* b) {
        switch(sortChoice) {
            case 1: return a->points   > b->points;
            case 2: return a->rebounds > b->rebounds;
            case 3: return a->assists  > b->assists;
            case 4: return a->blocks   > b->blocks;
            case 5: return a->steals   > b->steals;
            default: return a->points  > b->points;
        }
    };

    sort(filtered.begin(), filtered.end(), cmp); // Rikiuojame filtruotą sąrašą

    // Rodome rezultatus
    cout << "\n--- Rezultatai (" << filtered.size() << " zaideju) ---\n";
    printPlayerHeader();
    for (const auto& p : filtered) printPlayer(*p);
}

// Visų žaidėjų meniu – peržiūra ir filtravimas/rikiavimas
void allPlayersMenu() {
    int choice;
    do {
        cout << "\n===== VISI ZAIDEJAI =====\n";
        cout << "1. Rodyti visus zaideju sarasa\n";
        cout << "2. Filtruoti ir rikiuoti\n";
        cout << "0. Atgal\n";
        cout << "Pasirinkimas: "; cin >> choice; cin.ignore();
        switch (choice) {
            case 1:
                // Spausdiname visus žaidėjus be jokio filtro
                printPlayerHeader();
                for (const auto& p : players) printPlayer(p);
                waitEnter();
                break;
            case 2:
                sortAllPlayers(); // Filtravimas + rikiavimas
                waitEnter();
                break;
        }
    } while (choice != 0);
}

// ============================================================
// PAGRINDINIS MENIU – programos įėjimo taškas
// ============================================================

int main() {
    // Paleidžiant programą nuskaitomi duomenys iš failų
    loadTeams();   // Nuskaito komandas iš teams.txt
    loadPlayers(); // Nuskaito žaidėjus iš players.txt

    // Pasisveikinimo pranešimas
    cout << "====================================\n";
    cout << "   NBA ZAIDEJU STATISTIKOS SISTEMA  \n";
    cout << "====================================\n";

    int choice;
    do {
        // Pagrindinis meniu – trijų skyrių navigacija
        cout << "\n===== PAGRINDINIS MENIU =====\n";
        cout << "1. Komandu valdymas\n";  // CRUD operacijos su komandomis
        cout << "2. Pasirinkti komanda\n"; // Žaidėjų valdymas komandos viduje
        cout << "3. Visi zaidejai\n";      // Peržiūra ir filtravimas
        cout << "0. Iseiti\n";
        cout << "Pasirinkimas: "; cin >> choice; cin.ignore();

        switch (choice) {
            case 1: teamsMenu();      break; // Komandų valdymo submeniu
            case 2: selectTeamMenu(); break; // Komandos pasirinkimas ir jos meniu
            case 3: allPlayersMenu(); break; // Visų žaidėjų peržiūra
            case 0:
                // Išeinant išsaugome duomenis į failus
                saveTeams();
                savePlayers();
                cout << "Duomenys issaugoti. Viso gero!\n";
                break;
        }
    } while (choice != 0); // Programa veikia kol vartotojas pasirenka 0

    return 0; // Programa baigta sėkmingai
}