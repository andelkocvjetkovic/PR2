#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include<iterator>
#include<mutex>
using namespace std;

#pragma warning(disable:4996)
const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const float minimalan_prosjek = 4.5;


char* AlocirajNizKaraktera(const char* sadrzaj)
{
    if (sadrzaj == nullptr)
        return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

template<class T1, class T2>
class Kolekcija
{
    T1* _elementi1;
    T2* _elementi2;
    int* _trenutno;
public:
    Kolekcija()
    {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = new int(0);
    }
    Kolekcija(const Kolekcija<T1, T2>& org):
    _elementi1(new T1[*org._trenutno]),
    _elementi2(new T2[*org._trenutno]),
    _trenutno(new int(*org._trenutno))  {
        for (int i = 0; i < *_trenutno; i++) {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
    }
    ~Kolekcija()
    {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }
    Kolekcija& operator=(const Kolekcija<T1, T2>& obj) {
        if (this != &obj) {
            delete[]_elementi1; _elementi1 = nullptr;
            delete[]_elementi2; _elementi2 = nullptr;
            delete _trenutno; _trenutno = nullptr;
            _elementi1 = new T1[*obj._trenutno];
            _elementi2 = new T2[*obj._trenutno];
            _trenutno = new int(*obj._trenutno);
            for (int i = 0; i < *_trenutno; i++) {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        return *this;
    }
    T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
    int getTrenutno() { return *_trenutno; }
    bool AddElement(const T1& elem1, const T2& elem2) {
        T1* temp1 = new T1[getTrenutno() + 1];
        T2* temp2 = new T2[getTrenutno() + 1];
        for (int i = 0; i < getTrenutno(); i++) {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[*_trenutno] = elem1;
        temp2[*_trenutno] = elem2;
        (*_trenutno)++;
        delete[]_elementi1;
        delete[]_elementi2;
        _elementi1 = temp1;
        _elementi2 = temp2;
        temp1 = NULL; temp2 = NULL;
        return true;
    }
    void SortOpadajuciByT2() {
        //od najvece prema najmanjoj vrijednosti

        for (int i = 0; i < *_trenutno - 1; i++) {
            for (int j = 0; j < *_trenutno - i - 1; j++) {
                if (_elementi2[j] < _elementi2[j + 1]) {
                    T1 temp1 = _elementi1[j];
                    T2 temp2 = _elementi2[j];

                    _elementi1[j] = _elementi1[j + 1];
                    _elementi2[j] = _elementi2[j + 1];

                    _elementi1[j + 1] = temp1;
                    _elementi2[j + 1] = temp2;
                }
            }
        }


    }
    friend ostream& operator<< (ostream& COUT, Kolekcija& obj)
    {
        for (size_t i = 0; i < *obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
};

class DatumVrijeme
{
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
    char* _datumVrijemeArray = NULL;
public:
    DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
        _sati = new int(sati);
        _minuti = new int(minuti);
    }
    DatumVrijeme(const DatumVrijeme& org) :
        _dan(new int(*org._dan)),
        _mjesec(new int(*org._mjesec)),
        _godina(new int(*org._godina)),
        _sati(new int(*org._sati)),
        _minuti(new int(*org._minuti)) {
        if (org._datumVrijemeArray != NULL) {
            _datumVrijemeArray = AlocirajNizKaraktera(org._datumVrijemeArray);
        }
        else
            _datumVrijemeArray = NULL;
    }
    ~DatumVrijeme()
    {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        delete _sati; _sati = nullptr;
        delete _minuti; _minuti = nullptr;
    }
    DatumVrijeme& operator=(const DatumVrijeme& obj) {
        if (this != &obj) {
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;
            delete _sati; _sati = nullptr;
            delete _minuti; _minuti = nullptr;
            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
            _sati = new int(*obj._sati);
            _minuti = new int(*obj._minuti);
            _datumVrijemeArray = AlocirajNizKaraktera(obj._datumVrijemeArray);
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj)
    {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
        return COUT;
    }
    char* ToCharArray() {
        //treba ispisati: 19.6.2018 10:15
        stringstream obj;
        obj << *_dan;
        obj << ".";
        obj << *_mjesec;
        obj << ".";
        obj << *_godina;
        obj << " ";
        obj << *_sati;
        obj << ":";
        obj << *_minuti;
        _datumVrijemeArray = AlocirajNizKaraktera(obj.str().c_str());
        return _datumVrijemeArray;
    }
    long int getMinuteUkupno()const {
        long int rez;
        int godUmin = ((*_godina * 365) * 1440);
        int mjeUmin = ((*_mjesec * 30) * 1440);
        int danUmin = ((*_dan) * 1440);
        int satiUmin = (*_sati * 60);
        int min = *_minuti;
        rez = godUmin + mjeUmin + danUmin + satiUmin + min;
        return rez;
    }
};

class Predmet
{
    char* _naziv;
    int _ocjena;
    string _napomena;
public:
    Predmet(const char* naziv, int ocjena, string napomena = "") :_ocjena(ocjena), _napomena(napomena)
    {
        _naziv = AlocirajNizKaraktera(naziv);
    }
    Predmet(const Predmet& org) :
        _naziv(new char[strlen(org._naziv) + 1]),
        _ocjena(org._ocjena),
        _napomena(org._napomena) {
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
    }
    ~Predmet()
    {
        delete[] _naziv; _naziv = nullptr;
    }
    Predmet& operator=(const Predmet& obj) {
        if (this != &obj) {
            delete[] _naziv; _naziv = nullptr;
            _ocjena = obj._ocjena;
            _napomena = obj._napomena;
            _naziv = AlocirajNizKaraktera(obj._naziv);
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, Predmet& obj)
    {
        COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
        return COUT;
    }
    char* GetNaziv() { return _naziv; }
    string GetNapomena() { return _napomena; }
    void DodajNapomenu(string napomena)
    {
        _napomena += " " + napomena;
    }
    bool operator==(const Predmet& desni) {
        if (strcmp(this->_naziv, desni._naziv) == 0)
            return true;
        return false;
    }
    int getOcijena()const { return _ocjena; }
};
struct Uspjeh
{
    eRazred _razred;
    //datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
    Kolekcija<Predmet*, DatumVrijeme>* _predmeti;
    DatumVrijeme _zadnjeDodavanDatum;
    int _brojac;
public:
    Uspjeh(eRazred razred) :_razred(razred), _predmeti(new Kolekcija<Predmet*,DatumVrijeme>)
    {
        _zadnjeDodavanDatum = DatumVrijeme(0, 0, 0,0, 0);
        _brojac = 0;
    }
    Uspjeh(const Uspjeh& org) :
        _razred(org._razred),
        _zadnjeDodavanDatum(org._zadnjeDodavanDatum),
        _brojac(org._brojac)
    {
        _predmeti = new Kolekcija<Predmet*, DatumVrijeme>(*org._predmeti);
    }
    ~Uspjeh()
    {
        delete _predmeti; _predmeti = nullptr;
    }
    bool dodajPredmet(Predmet predmet, DatumVrijeme datumVrijme) {
        if (_predmeti->getTrenutno() > 6)
            return false;
        for (int i = 0; i < _predmeti->getTrenutno(); i++) {
            if (predmet == *_predmeti->getElement1(i)) {
                return false;
            }
        }
        long int minuteZadnjegDodavanja = _zadnjeDodavanDatum.getMinuteUkupno();
        long int minuteTrenutnoDodavanje = datumVrijme.getMinuteUkupno();
        long int rezultatMinute = minuteTrenutnoDodavanje - minuteZadnjegDodavanja;
        if (rezultatMinute < 0)
            rezultatMinute = abs(rezultatMinute);
        if (rezultatMinute < 1)
            return false;
        _predmeti->AddElement(new Predmet(predmet),datumVrijme);
        _brojac++;
        _zadnjeDodavanDatum = datumVrijme;
        return true;
        
    }
    eRazred getRazred()const {
        return _razred;
    }
    Kolekcija<Predmet*, DatumVrijeme>* GetPredmeti() { return _predmeti; }
    double prosjek()const {
        double suma = 0;
        for (int i = 0; i < _predmeti->getTrenutno(); i++) {
            suma += _predmeti->getElement1(i)->getOcijena();
        }
        return suma / static_cast<double>(_predmeti->getTrenutno());
    }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj)
    {
        COUT << "Razred: "<< obj._razred << endl;
        for (int i = 0; i < obj._predmeti->getTrenutno(); i++) {
            COUT << *obj._predmeti->getElement1(i) << obj._predmeti->getElement2(i);
        }
        return COUT;
    }
};
mutex cuvar;
class Kandidat
{
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh*> _uspjeh;
    void slanjePoruke(string poruka) {
        cuvar.lock();
        cout << poruka << endl;
        cuvar.unlock();
    }
    void slanjeEMail(string email) {
        cuvar.lock();
        cout << email << endl;
        cuvar.unlock();
    }
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona)
        : _emailAdresa(emailAdresa), _brojTelefona(brojTelefona)
    {
        _imePrezime = AlocirajNizKaraktera(imePrezime);
    }
    Kandidat(const Kandidat& org) :
        _emailAdresa(org._emailAdresa),
        _brojTelefona(org._brojTelefona) {
        for (int i = 0; i < _uspjeh.size(); i++) {
            _uspjeh[i] = new Uspjeh(*org._uspjeh[i]);
        }
        _imePrezime = AlocirajNizKaraktera(org._imePrezime);
    }
    ~Kandidat()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
        for (size_t i = 0; i < _uspjeh.size(); i++)
        {
            delete _uspjeh[i];
            _uspjeh[i] = nullptr;
        }
    }
    Kandidat& operator=(const Kandidat& obj) {
        if (this != &obj) {
            delete[] _imePrezime; _imePrezime = nullptr;
            for (size_t i = 0; i < _uspjeh.size(); i++)
            {
                delete _uspjeh[i];
                _uspjeh[i] = nullptr;
            }
            _imePrezime = AlocirajNizKaraktera(obj._imePrezime);
            _brojTelefona = obj._brojTelefona;
            _emailAdresa = obj._emailAdresa;
            for (int i = 0; i < _uspjeh.size(); i++) {
                _uspjeh[i] = new Uspjeh(*obj._uspjeh[i]);
            }
        }
        return *this;
    }
    bool AddPredmet(eRazred razred, Predmet predmet, DatumVrijeme datumVrijeme) {

       /*  uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
         tom prilikom onemoguciti:
         - dodavanje vise od 6 predmeta za jedan razred
         - dodavanje istoimenih predmeta na nivou jednog razreda,
         - dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja 
         pojedinih predmeta mora biti najmanje minut).
         razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom 
         (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
         Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja*/
        vector<Uspjeh*>::iterator itr = _uspjeh.begin();
        bool imaRazred = false;
        bool indeks = -1;
        for (int i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i]->getRazred() == razred) {
                imaRazred = true;
                indeks = i;
            }
        }
        if (imaRazred) {
            bool uvijet = _uspjeh[indeks]->dodajPredmet(predmet, datumVrijeme);
            if (_uspjeh[indeks]->_brojac == 6 && uvijet) {
                double prosjek = _uspjeh[indeks]->prosjek();
                int razred = _uspjeh[indeks]->getRazred();
                thread threadSMS(&Kandidat::slanjePoruke, this, "Evidentirali ste uspjeh za " +
                    to_string(razred) + "razred");
                threadSMS.join();
                this_thread::sleep_for(chrono::seconds(2));
                if (prosjek >= minimalan_prosjek) {
                    thread threadEmail(&Kandidat::slanjeEMail, this, "Svaka cast za uspjeh " +
                        to_string(prosjek));
                    threadEmail.join();
                    this_thread::sleep_for(chrono::seconds(2));
                }
                
                
            }
            return uvijet;
        }
        else if (!imaRazred) {
            Uspjeh noviRazred(razred);
            noviRazred.dodajPredmet(predmet, datumVrijeme);
            _uspjeh.push_back(new Uspjeh(noviRazred));
        }
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj)
    {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << *obj._uspjeh[i]<< endl;
        return COUT;
    }
    vector<Uspjeh*> GetUspjeh() { return _uspjeh; }

};

void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
    // �E BITI OZNACENO KAO "RE"
    // 3. SPA�AVAJTE PROJEKAT KAKO BI SE SPRIJE�ILO GUBLJENJE URA�ENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKO�ER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTI�NI ONIMA KOJI SU KORI�TENI U TESTNOM
    // CODE-U, OSIM U SLU�AJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMO�NE FUNKCIJE
    // MO�ETE IMENOVATI I DODAVATI PO �ELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNA�ENO.
    ****************************************************************************/
    DatumVrijeme temp,
        datum19062018_1015(19, 6, 2018, 10, 15),
        datum20062018_1115(20, 6, 2018, 11, 15),
        datum30062018_1215(30, 6, 2018, 12, 15),
        datum05072018_1231(5, 7, 2018, 12, 31),
        datum20062018_1115_Copy(datum20062018_1115);
    //funkcija ToCharArray vraca datum i vrijeme kao char *
    cout << datum19062018_1015.ToCharArray() << endl;//treba ispisati: 19.6.2018 10:15
    temp = datum05072018_1231;
    cout << temp.ToCharArray() << endl;//treba ispisati: 5.7.2018 12:31
    cout << datum20062018_1115_Copy.ToCharArray() << endl;//treba ispisati: 20.6.2018 11:15

    const int kolekcijaTestSize = 10;
    Kolekcija<int, int> kolekcija1;
    for (size_t i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i + 1, i * i);
    cout << kolekcija1 << endl;

    Kolekcija<int, int> kolekcija2 = kolekcija1;
    cout << kolekcija2 << crt;
    Kolekcija<int, int> kolekcija3;
    kolekcija3 = kolekcija1;
    kolekcija3.SortOpadajuciByT2(); //od najvece prema najmanjoj vrijednosti
    cout << kolekcija3 << crt;

    //napomena se moze dodati i prilikom kreiranja objekta
    Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
        Fizika("Fizika", 5),
        Hemija("Hemija", 2),
        Engleski("Engleski", 5),
        Tjelesno("Tjelesno",5),
        Likovno("Likovno",5);
    Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh"); // dodaje novu napomenu zadrzavajuci ranije dodane
    cout << Matematika << endl;


    Kandidat jasmin("Jasmin Azemovic", "jasmin@gmail.com", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");

    // uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
    // tom prilikom onemoguciti:
    // - dodavanje vise od 6 predmeta za jedan razred
    // - dodavanje istoimenih predmeta na nivou jednog razreda,
    // - dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja 
    // pojedinih predmeta mora biti najmanje minut).
    // razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
    // razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    //if (jasmin.AddPredmet(DRUGI, Fizika, datum20062018_1115))
    //    cout << "Predmet uspjesno dodan!" << crt;
    //if (jasmin.AddPredmet(DRUGI, Hemija, datum30062018_1215))
    //    cout << "Predmet uspjesno dodan!" << crt;
    //
    if (jasmin.AddPredmet(PRVI, Matematika, datum19062018_1015))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Fizika, datum30062018_1215))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Hemija, datum05072018_1231))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, datum19062018_1015))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Tjelesno, datum30062018_1215))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Likovno, datum05072018_1231))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (!jasmin.AddPredmet(PRVI, Matematika, datum05072018_1231))
        cout << "Predmet NIJE DODAN uspjesno dodan!" << crt;
    //ne treba dodati Fiziku jer nije prosao minut od dodavanja posljednjeg predmeta
    if (!jasmin.AddPredmet(PRVI, Fizika, datum20062018_1115))
        cout << "Predmet NIJE DODAN  uspjesno dodan!" << crt;
    cout << jasmin << endl;
    
    //// pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
    //// upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
    //// sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci od 
    //// minimalan_prosjek kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova 
    //// implemenitrati koristeci zasebne thread-ove.


    //Uspjeh* u = jasmin(PRVI);//vraca uspjeh kandidata ostvaren u prvom razredu
    //if (u != nullptr) cout << *u << endl;

    system("pause>0");
}

