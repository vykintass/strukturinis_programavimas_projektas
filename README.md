# NBA Žaidėjų Statistikos Sistema

## Projekto tikslas

Sukurti konsolinę C++ programą, skirtą NBA komandų ir žaidėjų statistikos valdymui. Programa leidžia nuskaityti duomenis iš tekstinių failų, juos redaguoti, išsaugoti bei atlikti statistikos analizę naudojant filtravimo ir rikiavimo funkcijas.

## Pasirinktos temos aprašymas

Tema – NBA komandų ir žaidėjų statistikos valdymo sistema. Programa leidžia saugoti informaciją apie NBA komandas ir jų žaidėjus, peržiūrėti statistiką, atlikti CRUD operacijas bei analizuoti duomenis pagal pasirinktus kriterijus.

## Programos funkcionalumas

### Komandų valdymas (CRUD)

* Peržiūrėti visas komandas
* Pridėti naują komandą
* Redaguoti komandos duomenis
* Ištrinti komandą
* Ištrinant komandą automatiškai pašalinami ir visi jos žaidėjai

### Žaidėjų valdymas (CRUD)

Pasirinkus komandą galima:

* Peržiūrėti komandos žaidėjų sąrašą
* Pridėti naują žaidėją
* Redaguoti žaidėjo duomenis
* Ištrinti žaidėją
* Rikiuoti komandos žaidėjus pagal statistiką

### Visų žaidėjų peržiūra

* Peržiūrėti visų komandų žaidėjus
* Filtruoti pagal poziciją
* Filtruoti pagal amžiaus grupę
* Rikiuoti pagal pasirinktą statistikos kategoriją

## Naudojami failai

| Failas        | Aprašymas                                   |
| ------------- | ------------------------------------------- |
| `main.cpp`    | Pagrindinis programos kodas                 |
| `teams.txt`   | NBA komandų duomenys                        |
| `players.txt` | NBA žaidėjų statistikos duomenys (30 įrašų) |

### teams.txt formatas

```text
Komanda;Miestas;Konferencija
```

Pavyzdys:

```text
Lakers;Los Angeles;West
Celtics;Boston;East
```

### players.txt formatas

```text
Vardas;Pavardė;Komanda;Pozicija;Amžius;Taškai;Atkovoti;Perdavimai;Blokai;Perimti;Rungtynės
```

Pavyzdys:

```text
Luka;Doncic;Lakers;PG;27;31.8;8.5;9.1;0.6;1.5;72
```

## Programos paleidimo instrukcija

1. Atidaryti projektą CLion aplinkoje.
2. Įsitikinti, kad failai `players.txt` ir `teams.txt` yra projekto kataloge.
3. Paspausti **Run** mygtuką arba naudoti **Shift + F10**.
4. Programa bus automatiškai sukompiliuota ir paleista.

## Papildomos funkcijos

### 1. Žaidėjų filtravimas

Galima filtruoti žaidėjus pagal:

* Poziciją (PG, SG, SF, PF, C)
* Amžiaus grupę:

  * iki 25 metų
  * 25–30 metų
  * virš 30 metų
* Arba rodyti visus žaidėjus be filtro

### 2. Žaidėjų rikiavimas

Galima rikiuoti pagal:

* Taškus per rungtynes
* Atkovotus kamuolius
* Rezultatyvius perdavimus
* Blokus
* Perimtus kamuolius

Rikiavimas galimas tiek konkrečios komandos viduje, tiek tarp visų žaidėjų.

## Techniniai sprendimai

Programoje panaudota:

* `struct Player`
* `struct Team`
* `vector<Player>`
* `vector<Team>`
* Funkcijos
* Sąlyginiai sakiniai (`if`, `else`, `switch`)
* Ciklai (`for`, `while`, `do-while`)
* Failų nuskaitymas naudojant `ifstream`
* Failų įrašymas naudojant `ofstream`
* Duomenų rikiavimas naudojant `std::sort`
* Lambda funkcijos rikiavimo kriterijams

## Duomenys

Projekte naudojama:

* 10 NBA komandų
* 30 NBA žaidėjų

Visi duomenys saugomi tekstiniuose failuose ir automatiškai išsaugomi atlikus pakeitimus.

## Naudotos technologijos

* C++
* CLion IDE
* STL bibliotekos (`vector`, `string`, `algorithm`, `fstream`, `sstream`, `iomanip`)

## Autorius

Vykintas Daugintis
