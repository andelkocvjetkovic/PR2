#include <iostream>
#include <sstream>
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOCNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
using namespace std;

const char* crt = "\n------------------------------------\n";

char* AlocirajNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)
        return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}
class Datum {
    int* _dan, * _mjesec, * _godina;
    int getBrojPrestupnihGodina()const
    {
        int godina = *_godina;
        if (*_mjesec <= 2)
            godina--;
        return godina / 4 - godina / 100 + godina / 400;
    }
    long int GetUkupnoDana()const
    {
        const int danaUmjesecu[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        long int broj = (*_godina) * 365 + (*_dan);
        for (int i = 0; i < (*_mjesec); i++)
        {
            broj += danaUmjesecu[i];
        }
        broj += getBrojPrestupnihGodina();
        return broj;
    }
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& org):
        _dan(new int(*(org._dan))),
        _mjesec(new int(*(org._mjesec))),
        _godina(new int(*(org._godina)))
    {   }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    Datum& operator=(const Datum& obj)
    {
        if (this != &obj)
        {
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;
            _dan = new int(*(obj._dan));
            _mjesec = new int(*(obj._mjesec));
            _godina = new int(*(obj._godina));
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
        return COUT;
    }
    int operator-(const Datum& obj)
    {
        return this->GetUkupnoDana() - obj.GetUkupnoDana();
    }
};

template<class T>
class Kolekcija {
    //friend funkcije
    friend ostream& operator<< <>(ostream& COUT, const Kolekcija<T>& obj);
private:
    T* _niz;
    int _trenutno;
public:
    Kolekcija() : _trenutno(0), _niz(nullptr) {}
    Kolekcija(const Kolekcija<T>& org) :
        _niz(new T[org._trenutno]),
        _trenutno(org._trenutno)
    {
        for (int i = 0; i < _trenutno; i++)
            _niz[i] = org._niz[i];
    }
    ~Kolekcija() { delete[] _niz; _niz = nullptr; _trenutno = 0; }
    Kolekcija& operator=(const Kolekcija<T>& obj)
    {
        if (this != &obj)
        {
            delete[] _niz; _niz = nullptr; _trenutno = 0;
            _niz = new T[obj._trenutno];
            for (int i = 0; i < obj._trenutno; i++)
                _niz[i] = obj._niz[i];
            _trenutno = obj._trenutno;
        }
        return *this;
    }
    T* GetNiz() { return _niz; }
    int GetTrenutno() { return _trenutno; }
    //implementirati potrebne funkcije
    void Add(T elem)
    {
        T* temp = new T[GetTrenutno() + 1];
        for (int i = 0; i < GetTrenutno(); i++)
        {
            temp[i] = _niz[i];
        }
        temp[GetTrenutno()] = elem;
        delete[]_niz;
        _niz = temp;
        temp = NULL;
        _trenutno++;
    }
    T& operator[](int index)const
    {
        if (index >= 0 && index < _trenutno)
            return _niz[index];
    }
};
template<class T>
ostream& operator<<(ostream& COUT, const Kolekcija<T>& obj)
{
    COUT << crt;
    for (int i = 0; i < obj._trenutno; i++)
        COUT << obj._niz[i] << endl;
    COUT << crt << endl;
    return COUT;
}
class Usluga {
    char* _naziv;
    double _cijena;//cijena usluge je izrazena po danu koristenja
public:
    Usluga() : _naziv(nullptr), _cijena(0) {}
    Usluga(const char* naziv, double cijena) : _cijena(cijena) {
        _naziv = AlocirajNizKaraktera(naziv);
    }
    Usluga(const Usluga& org) :
        _naziv(new char[strlen(org._naziv) + 1]),
        _cijena(org._cijena)
    {
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
    }
    Usluga( Usluga&& org) :
        _naziv(new char[strlen(org._naziv)+1]),
        _cijena(org._cijena)
    {   
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
        org._naziv = NULL;
        cout << "MOVE CONSTRUKTOR" << endl;
    }
    ~Usluga() { delete[] _naziv; _naziv = nullptr; }
    Usluga& operator=(const Usluga& obj)
    {
        if (this != &obj)
        {
            delete[] _naziv; _naziv = nullptr;
            _naziv = new char[strlen(obj._naziv) + 1];
            strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
            _cijena = obj._cijena;
        }
        return *this;
    }
    double GetCijena() { return _cijena; }
    const char* GetNaziv() { return _naziv; }
    friend ostream& operator<< (ostream& COUT, const Usluga& obj);
    bool operator==(const Usluga& usluge)
    {
        return (strcmp(this->_naziv,usluge._naziv)==0);
    }
};
ostream& operator<< (ostream& COUT, const Usluga& obj) {
    COUT << obj._naziv << " -> \t\t" << obj._cijena << " KM" << endl;
    return COUT;
}

class Rezervacija {
    static int _rezervacijeIDBrojac;
    const int _rezervacijaID; ///broj rezervacije krece od 160000
    Datum _rezervacijaOD;
    Datum _rezervacijaDO;
    double _cijenaPoDanu; //cijena osnovnih usluga koja ne ukljucuje dodatne usluge
    Kolekcija<Usluga> _dodatneUsluge; //dodatne usluge mogu biti: sauna, sportski tereni, kuglanje, masaza i sl. 
public:
    //implementirati potrebne funkcije
    Rezervacija(Datum rezrvacijaOd, Datum rezervacijaDo,double cijenaPoDanu):
        _rezervacijaID(_rezervacijeIDBrojac++),_cijenaPoDanu(cijenaPoDanu),
        _rezervacijaOD(rezrvacijaOd),_rezervacijaDO(rezervacijaDo)
    {    }
    bool operator+=(Usluga& usluga)
    {
        //dodaje novu uslugu u rezervaciju. onemoguciti ponavljanje usluga
        for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
        {
            if (_dodatneUsluge.GetNiz()[i] == usluga)
                return false;
        }
        _dodatneUsluge.Add(usluga);
        return true;
    }
    Usluga* operator[](string naziv)
    {
        for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
        {
            string temp(_dodatneUsluge.GetNiz()[i].GetNaziv());
            if (temp == naziv)
            {
                return &_dodatneUsluge.GetNiz()[i];
            }
        }
        return NULL;
    }
    Kolekcija<Usluga> operator()(double odC, double doC)
    {
        //vraca sve dodatne usluge koje imaju cijenu u opsegu vrijednosti parametara
        Kolekcija<Usluga>temp;
        for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
        {
            if ((odC <= _dodatneUsluge.GetNiz()[i].GetCijena()) &&
                (_dodatneUsluge.GetNiz()[i].GetCijena() <= doC))
            {
                temp.Add(_dodatneUsluge.GetNiz()[i]);
            }
        }
        return temp;
    }
    void PrintRacun()
    {
    /*prikazuje racun formatiran na nacin prikazan na slici.
    prilikom izracunavanja iznosa koristiti cijene osnovnih i dodatnih usluga,
    kao i broj dana za koje je napravljena rezervacija
    (napomena: duzina trajanja rezervacije je maksimalno 30 dana)*/
    /*
------------------------------------
R.ID: 160000
OD: 25 10 2018
DO: 30 10 2018
------------------------------------
DODATNE USLUGE:
------------------------------------
Parking -> 5 KM
Dorucak -> 10 KM
Rucak -> 15 KM
Veseraj -> 10 KM
------------------------------------
Osnovne usluge-> 299.5 KM
------------------------------------
Dodatne usluge-> 200 KM
------------------------------------
Ukupno -> 499.5 KM
------------------------------------
*/
        int trajanjeRezervacije = _rezervacijaDO - _rezervacijaOD;
        if (trajanjeRezervacije > 30)
            cout << "Trajanje rezervacije je duze on maximuma" << endl;
        else
        {
            double cijenaDodatneUsluge = 0,
                cijenaOsnovneUsluge = _cijenaPoDanu * trajanjeRezervacije,cijenaUkupno;
            cout << crt;
            cout << "R.ID: " << _rezervacijaID << endl;
            cout << "OD: " << _rezervacijaOD<<endl;
            cout << "DO: " << _rezervacijaDO << endl;
            cout << crt;
            cout << "DODATNE USLUGE: " << endl;
            for (int i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
            {
                cout << _dodatneUsluge.GetNiz()[i];
                cijenaDodatneUsluge += _dodatneUsluge.GetNiz()[i].GetCijena();
            }
            cout << crt;
            cout << "Osnovne usluge->" << cijenaOsnovneUsluge << " KM" << crt;
            cout << "Dodatne usluge->" << cijenaDodatneUsluge << " KM" << crt;
            cijenaUkupno = cijenaOsnovneUsluge + cijenaDodatneUsluge;
            cout << "Ukupno->" << cijenaUkupno << " KM" << crt;
        }
    }
};
int Rezervacija::_rezervacijeIDBrojac = 160000;
void main() {
    cout << crt << "TRAJANJE ISPITA -> 90 minuta " << crt;
    Kolekcija<int> brojevi;
    for (int i = 0; i < 15; i++)
        brojevi.Add(i);
    cout << brojevi << crt;

    //u okviru klase Usluga implementirati MOVE konstruktor
    Usluga Parking("Parking", 5), Dorucak("Dorucak", 10), Rucak("Rucak", 15), Veseraj("Veseraj", 10);

    //rezervacijaOD, rezervacijaDO, cijenaPoDanu
    Rezervacija rezervacija(Datum(25, 10, 2018), Datum(30, 10, 2018), 59.9);

    //dodaje novu uslugu u rezervaciju. onemoguciti ponavljanje usluga
    if (rezervacija += Parking)
        cout << "Usluga uspjesno dodana!" << endl;
    if (rezervacija += Dorucak)
        cout << "Usluga uspjesno dodana!" << endl;
    if (rezervacija += Rucak)
        cout << "Usluga uspjesno dodana!" << endl;
    if (rezervacija += Veseraj)
        cout << "Usluga uspjesno dodana!" << endl;
    if (!(rezervacija += Dorucak))
        cout << "Usluga vec postoji!" << endl;

    /*prikazuje racun formatiran na nacin prikazan na slici. 
    prilikom izracunavanja iznosa koristiti cijene osnovnih i dodatnih usluga, 
    kao i broj dana za koje je napravljena rezervacija 
    (napomena: duzina trajanja rezervacije je maksimalno 30 dana)*/
    rezervacija.PrintRacun();

    //vraca pokazivac na uslugu sa proslijedjenim nazivom
    Usluga* p = rezervacija["Dorucak"];
    if (p != nullptr)
        cout << *p << crt;

    //vraca sve dodatne usluge koje imaju cijenu u opsegu vrijednosti parametara
    Kolekcija<Usluga> uslugePoCijeni = rezervacija(8, 12);
    for (size_t i = 0; i < uslugePoCijeni.GetTrenutno(); i++)
        cout << uslugePoCijeni[i] << crt;


    cin.get();
}

/*
------------------------------------
R.ID: 160000
OD: 25 10 2018
DO: 30 10 2018
------------------------------------
DODATNE USLUGE:
------------------------------------
Parking -> 5 KM
Dorucak -> 10 KM
Rucak -> 15 KM
Veseraj -> 10 KM
------------------------------------
Osnovne usluge-> 299.5 KM
------------------------------------
Dodatne usluge-> 200 KM
------------------------------------
Ukupno -> 499.5 KM
------------------------------------
*/