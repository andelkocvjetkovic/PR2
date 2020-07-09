#include <iostream>
using namespace std;


/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
6. ZA POTREBE TESTIRANJA, UNUTAR MAIN FUNKCIJE MOZETE DODAVATI NOVE TESTNE PODATKE
****************************************************************************/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char* crt = "\n----------------------------------------------------\n";
class Datum
{
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int d=1, int m=1, int g=2000)
    {
        _dan = new int(d);
        _mjesec = new int(m);
        _godina = new int(g);
    }
    Datum(const Datum& org):
        _dan(new int(*(org._dan))),
        _mjesec(new int(*(org._mjesec))),
        _godina(new int(*(org._godina)))
    {}
    ~Datum()
    {
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
    friend ostream& operator<<(ostream& COUT, Datum obj)
    {
        COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina << endl;
        return COUT;
    }
};

template<class T, int velicina>
class FITArray
{
    friend ostream& operator<< <>(ostream& COUT,const FITArray<T, velicina>& obj);
private:
    T _niz[velicina];
    int _trenutno;
    bool _dozvoliDupliciranje;
public:
    //OSTALE FUNKCIJE CLANICE DEFINISATI VAN TEMPLATE KLASE
    FITArray(bool dozvoliDupliciranje = true) { _dozvoliDupliciranje = dozvoliDupliciranje; _trenutno = 0; }
    FITArray(const FITArray<T, velicina>& org) :
        _trenutno(org._trenutno),
        _dozvoliDupliciranje(org._dozvoliDupliciranje)
    {
        for (int i = 0; i < _trenutno; i++)
            _niz[i] = org._niz[i];
    }

    FITArray& operator=(const FITArray<T, velicina>& obj)
    {
        if (this != &obj)
        {
            _trenutno = obj._trenutno;
            _dozvoliDupliciranje = obj._dozvoliDupliciranje;
            for (int i = 0; i < _trenutno; i++)
                _niz[i] = obj._niz[i];
        }
        return *this;
    }
    int GetTrenutno() { return _trenutno; }
    T* GetNiz() { return _niz; }
    bool operator +=(T element)
    {
        if (_dozvoliDupliciranje)
        {
            _niz[_trenutno] = element;
            _trenutno++;
            return true;
        }
        int brojac = 0;
        for (int i = 0; i < _trenutno; i++)
        {
            if (_niz[i] == element)
                brojac++;
        }
        if (brojac == 0)
        {
            _niz[_trenutno] = element;
            _trenutno++;
            return true;
        }
        return false;
    }
    bool operator -=(T vrijednost)
    {
        int indeks = -1;
        for (int i = 0; i < _trenutno; i++)
        {
            if (_niz[i] == vrijednost)
            {
                indeks = i;
                break;
            }  
        }
        if (indeks != -1)
        {
            FITArray<T, velicina> tempNiz(this->_dozvoliDupliciranje);
            for (int i = 0; i < indeks; i++)
                tempNiz._niz[i] = _niz[i];
            for (int i = indeks; i < _trenutno - 1; i++)
                tempNiz._niz[i] = _niz[i + 1];
            _trenutno--;
            for (int i = 0; i < _trenutno; i++)
                _niz[i] = tempNiz._niz[i];
            return true;
        }
        return false;

    }
    FITArray<T, velicina> operator()(int OD, int DO)
    {
    // parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u 
    // slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije 
    // OD pa do posljednje dostupnog elementa 


        FITArray<T, velicina> tempNiz(this->_dozvoliDupliciranje);
        if (OD < _trenutno)
        {
            if (OD < 0)
                OD = 0;
            if (DO >= 0)
            {
                if (DO >= _trenutno)
                    DO = _trenutno;
                for (int i = OD; i < DO; i++)
                {
                    tempNiz += _niz[i];
                }
            }
        }
      
        return tempNiz;
    }
};
template<class T,int velicina>
ostream& operator<< <>(ostream& COUT,const FITArray<T, velicina>& obj)
{
    for (int i = 0; i < obj._trenutno; i++)
        COUT << obj._niz[i];
    return COUT;
}
class PolozeniPredmet
{
    char* _naziv;
    int _ocjena;
    Datum _datumPolaganja;
public:
    PolozeniPredmet():
        _naziv(NULL),
        _ocjena(1),
        _datumPolaganja()
    {}
    PolozeniPredmet(Datum datumPolaganja, const char* naziv = "not-set", int ocjena = 1) :
        _datumPolaganja(datumPolaganja),
        _naziv(new char[strlen(naziv) + 1]),
        _ocjena(ocjena)
    {
        strcpy_s(_naziv, strlen(naziv) + 1, naziv);
    }
    PolozeniPredmet(const PolozeniPredmet& org) :
        _datumPolaganja(org._datumPolaganja),
        _naziv(new char[strlen(org._naziv) + 1]),
        _ocjena(org._ocjena)
    {
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
    }
    ~PolozeniPredmet() { delete[] _naziv; _naziv = nullptr; }
    PolozeniPredmet& operator=(const PolozeniPredmet& obj)
    {
        if (this != &obj)
        {
            delete[] _naziv; _naziv = nullptr;
            _datumPolaganja = obj._datumPolaganja;
            _ocjena = obj._ocjena;
            _naziv = new char[strlen(obj._naziv) + 1];
            strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
        }
        return *this;
    }
    bool operator==(const PolozeniPredmet& obj)
    {
        return (strcmp(this->_naziv, obj._naziv) == 0);
    }
    int getOcijena()const
    {
        return _ocjena;
    }
    friend ostream& operator<<(ostream& COUT,const PolozeniPredmet& obj)
    {
        COUT << obj._naziv << "(" << obj._ocjena << ")" << obj._datumPolaganja << endl;
        return COUT;
    }
   
};

class Student
{
    static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000
    int _indeks;
    char* _imePrezime;
    FITArray<PolozeniPredmet, 40> _polozeniPredmeti;
public:
    Student(const char* imePrezime) :
        _indeks(_indeksCounter++),
        _imePrezime(new char[strlen(imePrezime) + 1])
    {
        strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
    }
    Student(const Student& org) :
        _indeks(org._indeks),
        _imePrezime(new char[strlen(org._imePrezime) + 1])
    {
        strcpy_s(_imePrezime, strlen(org._imePrezime) + 1, org._imePrezime);
        _polozeniPredmeti = org._polozeniPredmeti;
    }
    ~Student()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    Student& operator=(const Student& obj)
    {
        if (this != &obj)
        {
            delete[] _imePrezime; _imePrezime = nullptr;
            _imePrezime = new char[strlen(obj._imePrezime) + 1];
            strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
            _indeks = obj._indeks;
            _polozeniPredmeti = obj._polozeniPredmeti;
        }
        return *this;
    }
    void DodajPredmet(PolozeniPredmet predmet)
    {
        _polozeniPredmeti += predmet;
    }
    double GetProsjek()
    {
        double sum = 0;
        for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
        {
            sum += _polozeniPredmeti.GetNiz()[i].getOcijena();
        }
        return sum / static_cast<double>(_polozeniPredmeti.GetTrenutno());
    }
    friend ostream& operator<<(ostream& COUT,const Student& obj)
    {
        COUT << obj._indeks << " " << obj._imePrezime << endl << obj._polozeniPredmeti;
        return COUT;
    }
};
int Student::_indeksCounter = 160000;
void main()
{
    const int max = 20;

    Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);
    cout << jucer << danas << sutra << endl;
    jucer = danas;
    cout << jucer << danas << sutra << endl;

    //parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata
    FITArray<int, max> nizIntegera(false);

    for (size_t i = 0; i < max - 1; i++)
        nizIntegera += i;//dodaje novog clana niza

    if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza
        cout << "Element nije dodan u niz" << endl;

    cout << crt << nizIntegera << crt; //ispisuje sve clanove niza

    nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9
    nizIntegera -= 17;

    FITArray<int, max> noviNizIntegera(nizIntegera);

    cout << crt << noviNizIntegera << crt;

    // parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti. u 
    // slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element od lokacije 
    // OD pa do posljednje dostupnog elementa 
    cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(-2, 40) << crt;

    Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

    PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

    adel.DodajPredmet(prII);
    adel.DodajPredmet(prIII);
    cout << adel << endl;
    //vraca prosjecnu ocjenu studenta
    cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

    jasmin = adel;
    cout << jasmin << endl;
    cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
}