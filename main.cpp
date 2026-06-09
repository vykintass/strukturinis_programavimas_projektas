#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// ============================================================
// STRUKTŪROS
// ============================================================

struct Player {
    string firstName;
    string lastName;
    string team;
    string position; // PG, SG, SF, PF, C
    int age;
    double points;
    double rebounds;
    double assists;
    double blocks;
    double steals;
    int gamesPlayed;
};

struct Team {
    string name;
    string city;
    string conference; // East / West
};

// ============================================================
// GLOBALŪS DUOMENYS
// ============================================================

vector<Player> players;
vector<Team> teams;

// ============================================================
// PAGALBINĖS FUNKCIJOS
// ============================================================

void clearScreen() {
    cout << "\n";
}

void waitEnter() {
    cout << "\nSpauskite Enter testi...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool teamExists(const string& name) {
    for (const auto& t : teams) {
        if (t.name == name) return true;
    }
    return false;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

bool validPosition(const string& pos) {
    return pos == "PG" || pos == "SG" || pos == "SF" || pos == "PF" || pos == "C";
}

bool validConference(const string& conf) {
    return conf == "East" || conf == "West";
}

void printPlayerHeader() {
    cout << left
         << setw(18) << "Vardas"
         << setw(12) << "Komanda"
         << setw(6)  << "Poz."
         << setw(5)  << "Amz."
         << setw(7)  << "Tsk."
         << setw(7)  << "Atkv."
         << setw(7)  << "Perd."
         << setw(7)  << "Blok."
         << setw(7)  << "Perim."
         << setw(6)  << "Rung."
         << "\n";
    cout << string(82, '-') << "\n";
}

void printPlayer(const Player& p) {
    cout << left
         << setw(18) << (p.firstName + " " + p.lastName)
         << setw(12) << p.team
         << setw(6)  << p.position
         << setw(5)  << p.age
         << fixed << setprecision(1)
         << setw(7)  << p.points
         << setw(7)  << p.rebounds
         << setw(7)  << p.assists
         << setw(7)  << p.blocks
         << setw(7)  << p.steals
         << right << setw(4) << p.gamesPlayed << "\n";
}

// ============================================================
// FAILŲ OPERACIJOS
// ============================================================

void loadTeams() {
    ifstream file("teams.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Team t;
        getline(ss, t.name, ';');
        getline(ss, t.city, ';');
        getline(ss, t.conference, ';');
        t.name = trim(t.name);
        t.city = trim(t.city);
        t.conference = trim(t.conference);
        if (!t.name.empty()) teams.push_back(t);
    }
    file.close();
}

void saveTeams() {
    ofstream file("teams.txt");
    for (const auto& t : teams) {
        file << t.name << ";" << t.city << ";" << t.conference << "\n";
    }
    file.close();
}

void loadPlayers() {
    ifstream file("players.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Player p;
        string tmp;
        getline(ss, p.firstName, ';');
        getline(ss, p.lastName, ';');
        getline(ss, p.team, ';');
        getline(ss, p.position, ';');
        getline(ss, tmp, ';'); p.age = stoi(tmp);
        getline(ss, tmp, ';'); p.points = stod(tmp);
        getline(ss, tmp, ';'); p.rebounds = stod(tmp);
        getline(ss, tmp, ';'); p.assists = stod(tmp);
        getline(ss, tmp, ';'); p.blocks = stod(tmp);
        getline(ss, tmp, ';'); p.steals = stod(tmp);
        getline(ss, tmp, ';'); p.gamesPlayed = stoi(tmp);
        p.firstName = trim(p.firstName);
        p.lastName = trim(p.lastName);
        p.team = trim(p.team);
        p.position = trim(p.position);
        if (!p.firstName.empty()) players.push_back(p);
    }
    file.close();
}

void savePlayers() {
    ofstream file("players.txt");
    for (const auto& p : players) {
        file << p.firstName << ";"
             << p.lastName << ";"
             << p.team << ";"
             << p.position << ";"
             << p.age << ";"
             << fixed << setprecision(1)
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
// ŽAIDĖJO ĮVEDIMAS / REDAGAVIMAS
// ============================================================

Player inputPlayer(const string& defaultTeam = "") {
    Player p;
    cout << "Vardas: "; cin >> p.firstName;
    cout << "Pavarde: "; cin >> p.lastName;

    if (!defaultTeam.empty()) {
        p.team = defaultTeam;
        cout << "Komanda: " << p.team << "\n";
    } else {
        cout << "Komanda: "; cin >> p.team;
    }

    do {
        cout << "Pozicija (PG/SG/SF/PF/C): "; cin >> p.position;
        if (!validPosition(p.position))
            cout << "Neteisinga pozicija! Bandykite dar karta.\n";
    } while (!validPosition(p.position));

    cout << "Amzius: "; cin >> p.age;
    cout << "Taskai per rungtynes: "; cin >> p.points;
    cout << "Atkovoti kamuoliai: "; cin >> p.rebounds;
    cout << "Rezultatyvus perdavimai: "; cin >> p.assists;
    cout << "Blokai: "; cin >> p.blocks;
    cout << "Perimti kamuoliai: "; cin >> p.steals;
    cout << "Zaista rungtynese: "; cin >> p.gamesPlayed;
    cin.ignore();
    return p;
}

// ============================================================
// KOMANDŲ CRUD
// ============================================================

void showAllTeams() {
    if (teams.empty()) {
        cout << "Nera issaugotu komandu.\n";
        return;
    }
    cout << "\n" << left << setw(20) << "Komanda" << setw(20) << "Miestas" << setw(10) << "Konf." << "\n";
    cout << string(50, '-') << "\n";
    for (const auto& t : teams) {
        cout << setw(20) << t.name << setw(20) << t.city << setw(10) << t.conference << "\n";
    }
}

void addTeam() {
    Team t;
    cout << "Komandos pavadinimas: "; cin >> t.name; cin.ignore();
    if (teamExists(t.name)) {
        cout << "Tokia komanda jau egzistuoja!\n";
        return;
    }
    cout << "Miestas: "; getline(cin, t.city);
    do {
        cout << "Konferencija (East/West): "; cin >> t.conference; cin.ignore();
        if (!validConference(t.conference))
            cout << "Turi buti East arba West!\n";
    } while (!validConference(t.conference));
    teams.push_back(t);
    saveTeams();
    cout << "Komanda prideta!\n";
}

void editTeam() {
    showAllTeams();
    if (teams.empty()) return;
    cout << "\nIveskite komandos pavadinima redaguoti: ";
    string name; cin >> name; cin.ignore();
    for (auto& t : teams) {
        if (t.name == name) {
            cout << "Naujas pavadinimas (Enter - palikti '" << t.name << "'): ";
            string tmp; getline(cin, tmp);
            if (!tmp.empty()) {
                // Atnaujinti žaidėjų komandos pavadinimą
                for (auto& p : players) if (p.team == t.name) p.team = tmp;
                t.name = tmp;
            }
            cout << "Naujas miestas (Enter - palikti '" << t.city << "'): ";
            getline(cin, tmp);
            if (!tmp.empty()) t.city = tmp;
            cout << "Nauja konferencija (Enter - palikti '" << t.conference << "'): ";
            getline(cin, tmp);
            if (!tmp.empty() && validConference(tmp)) t.conference = tmp;
            saveTeams();
            savePlayers();
            cout << "Komanda atnaujinta!\n";
            return;
        }
    }
    cout << "Komanda nerasta.\n";
}

void deleteTeam() {
    showAllTeams();
    if (teams.empty()) return;
    cout << "\nIveskite komandos pavadinima istrinti: ";
    string name; cin >> name; cin.ignore();
    for (auto it = teams.begin(); it != teams.end(); ++it) {
        if (it->name == name) {
            // Ištrinti ir komandos žaidėjus
            players.erase(remove_if(players.begin(), players.end(),
                [&name](const Player& p){ return p.team == name; }), players.end());
            teams.erase(it);
            saveTeams();
            savePlayers();
            cout << "Komanda ir jos zaidejai istrinti!\n";
            return;
        }
    }
    cout << "Komanda nerasta.\n";
}

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
        switch (choice) {
            case 1: showAllTeams(); waitEnter(); break;
            case 2: addTeam(); waitEnter(); break;
            case 3: editTeam(); waitEnter(); break;
            case 4: deleteTeam(); waitEnter(); break;
        }
    } while (choice != 0);
}

// ============================================================
// ŽAIDĖJŲ CRUD (komandos kontekste)
// ============================================================

void showTeamPlayers(const string& teamName) {
    bool found = false;
    cout << "\n--- " << teamName << " zaidejai ---\n";
    printPlayerHeader();
    for (const auto& p : players) {
        if (p.team == teamName) {
            printPlayer(p);
            found = true;
        }
    }
    if (!found) cout << "Sios komandos zaideju nerasta.\n";
}

void addPlayerToTeam(const string& teamName) {
    cout << "\n--- Naujas zaidejas: " << teamName << " ---\n";
    Player p = inputPlayer(teamName);
    players.push_back(p);
    savePlayers();
    cout << "Zaidejas pridetas!\n";
}

void editPlayerInTeam(const string& teamName) {
    showTeamPlayers(teamName);
    cout << "\nIveskite zaidejo pavarde redaguoti: ";
    string lastName; cin >> lastName; cin.ignore();
    for (auto& p : players) {
        if (p.team == teamName && p.lastName == lastName) {
            cout << "Redaguojamas: " << p.firstName << " " << p.lastName << "\n";
            cout << "(Iveskite naują reikšmę arba 0 palikti esamą)\n";

            cout << "Vardas (" << p.firstName << "): ";
            string tmp; getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.firstName = tmp;

            cout << "Pozicija (" << p.position << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty() && validPosition(tmp)) p.position = tmp;

            cout << "Amzius (" << p.age << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.age = stoi(tmp);

            cout << "Taskai (" << p.points << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.points = stod(tmp);

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

            cout << "Rungtynės (" << p.gamesPlayed << "): ";
            getline(cin, tmp);
            if (tmp != "0" && !tmp.empty()) p.gamesPlayed = stoi(tmp);

            savePlayers();
            cout << "Zaidejas atnaujintas!\n";
            return;
        }
    }
    cout << "Zaidejas nerastas.\n";
}

void deletePlayerFromTeam(const string& teamName) {
    showTeamPlayers(teamName);
    cout << "\nIveskite zaidejo pavarde istrinti: ";
    string lastName; cin >> lastName; cin.ignore();
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->team == teamName && it->lastName == lastName) {
            cout << "Istrinti " << it->firstName << " " << it->lastName << "? (t/n): ";
            char c; cin >> c; cin.ignore();
            if (c == 't' || c == 'T') {
                players.erase(it);
                savePlayers();
                cout << "Zaidejas istrinti!\n";
            }
            return;
        }
    }
    cout << "Zaidejas nerastas.\n";
}

// ============================================================
// RIKIAVIMAS (komandos kontekste)
// ============================================================

void sortTeamPlayers(const string& teamName) {
    vector<Player*> teamPlayers;
    for (auto& p : players) {
        if (p.team == teamName) teamPlayers.push_back(&p);
    }
    if (teamPlayers.empty()) { cout << "Zaideju nerasta.\n"; return; }

    cout << "\nRikiuoti pagal:\n";
    cout << "1. Taskus\n2. Atkovotus kamuolius\n3. Perdavimus\n4. Blokus\n5. Perimtus\n";
    cout << "Pasirinkimas: "; int c; cin >> c; cin.ignore();

    auto cmp = [&](const Player* a, const Player* b) {
        switch(c) {
            case 1: return a->points > b->points;
            case 2: return a->rebounds > b->rebounds;
            case 3: return a->assists > b->assists;
            case 4: return a->blocks > b->blocks;
            case 5: return a->steals > b->steals;
            default: return a->points > b->points;
        }
    };
    sort(teamPlayers.begin(), teamPlayers.end(), cmp);

    cout << "\n--- " << teamName << " (surikiuota) ---\n";
    printPlayerHeader();
    for (const auto& p : teamPlayers) printPlayer(*p);
}

// ============================================================
// KOMANDOS MENIU
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
            case 1: showTeamPlayers(teamName); waitEnter(); break;
            case 2: addPlayerToTeam(teamName); waitEnter(); break;
            case 3: editPlayerInTeam(teamName); waitEnter(); break;
            case 4: deletePlayerFromTeam(teamName); waitEnter(); break;
            case 5: sortTeamPlayers(teamName); waitEnter(); break;
        }
    } while (choice != 0);
}

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
    teamMenu(name);
}

// ============================================================
// VISI ŽAIDĖJAI — FILTRAVIMAS IR RIKIAVIMAS
// ============================================================

void sortAllPlayers() {
    if (players.empty()) { cout << "Zaideju nerasta.\n"; return; }

    vector<Player*> filtered;

    cout << "\nFiltruoti pagal:\n";
    cout << "1. Pozicija\n2. Amziaus grupe\n3. Visi zaidejai (be filtro)\n";
    cout << "Pasirinkimas: "; int filterChoice; cin >> filterChoice; cin.ignore();

    if (filterChoice == 1) {
        cout << "Pozicija (PG/SG/SF/PF/C): "; string pos; cin >> pos; cin.ignore();
        for (auto& p : players) if (p.position == pos) filtered.push_back(&p);
        if (filtered.empty()) { cout << "Zaideju su sia pozicija nerasta.\n"; return; }
    } else if (filterChoice == 2) {
        cout << "Amziaus grupe:\n1. Iki 25 metu\n2. 25-30 metu\n3. Virs 30 metu\n";
        cout << "Pasirinkimas: "; int ag; cin >> ag; cin.ignore();
        for (auto& p : players) {
            if      (ag == 1 && p.age < 25)  filtered.push_back(&p);
            else if (ag == 2 && p.age >= 25 && p.age <= 30) filtered.push_back(&p);
            else if (ag == 3 && p.age > 30)  filtered.push_back(&p);
        }
        if (filtered.empty()) { cout << "Zaideju siam amziaus filtrue nerasta.\n"; return; }
    } else {
        for (auto& p : players) filtered.push_back(&p);
    }

    cout << "\nRikiuoti pagal:\n";
    cout << "1. Taskus\n2. Atkovotus\n3. Perdavimus\n4. Blokus\n5. Perimtus\n";
    cout << "Pasirinkimas: "; int sortChoice; cin >> sortChoice; cin.ignore();

    auto cmp = [&](const Player* a, const Player* b) {
        switch(sortChoice) {
            case 1: return a->points > b->points;
            case 2: return a->rebounds > b->rebounds;
            case 3: return a->assists > b->assists;
            case 4: return a->blocks > b->blocks;
            case 5: return a->steals > b->steals;
            default: return a->points > b->points;
        }
    };
    sort(filtered.begin(), filtered.end(), cmp);

    cout << "\n--- Rezultatai (" << filtered.size() << " zaideju) ---\n";
    printPlayerHeader();
    for (const auto& p : filtered) printPlayer(*p);
}

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
                printPlayerHeader();
                for (const auto& p : players) printPlayer(p);
                waitEnter();
                break;
            case 2: sortAllPlayers(); waitEnter(); break;
        }
    } while (choice != 0);
}

// ============================================================
// PAGRINDINIS MENIU
// ============================================================

int main() {
    loadTeams();
    loadPlayers();

    cout << "====================================\n";
    cout << "   NBA ZAIDEJU STATISTIKOS SISTEMA  \n";
    cout << "====================================\n";

    int choice;
    do {
        cout << "\n===== PAGRINDINIS MENIU =====\n";
        cout << "1. Komandu valdymas\n";
        cout << "2. Pasirinkti komanda\n";
        cout << "3. Visi zaidejai\n";
        cout << "0. Iseiti\n";
        cout << "Pasirinkimas: "; cin >> choice; cin.ignore();
        switch (choice) {
            case 1: teamsMenu(); break;
            case 2: selectTeamMenu(); break;
            case 3: allPlayersMenu(); break;
            case 0:
                saveTeams();
                savePlayers();
                cout << "Duomenys issaugoti. Viso gero!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}