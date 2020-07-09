#include <iostream>
#include <string>
using namespace std;

const char* crt = "\n----------------------------------------------------\n";
int brRacuna = 1;
int GenerisiSljedeciBrojRacuna()
{
    return brRacuna++;
}

template<class T1, class T2>
class Kolekcija
{
    friend ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& obj);
private:
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
    Kolekcija(const Kolekcija<T1, T2>& org) :
        _elementi1(new T1[org.getTrenutno()]), _elementi2(new T2[org.getTrenutno()]),
        _trenutno(new int(org.getTrenutno()))
    {
        for (int i = 0; i < org.getTrenutno(); i++)
        {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
    }
    //1. potrebne funkcije	
    ~Kolekcija()
    {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }
    Kolekcija& operator=(const Kolekcija<T1, T2>& obj)
    {
        if (this != &obj)
        {
            delete[]_elementi1; _elementi1 = NULL;
            delete[]_elementi2; _elementi2 = NULL;
            delete _trenutno; _trenutno = NULL;
            _elementi1 = new T1[obj.getTrenutno()];
            _elementi2 = new T2[obj.getTrenutno()];
            _trenutno = new int(*obj._trenutno);
            for (int i = 0; i < obj.getTrenutno(); i++)
            {
                _elementi1[i] = obj._elementi1[i];
                _elementi2[i] = obj._elementi2[i];
            }
        }
        return *this;
    }
    bool AddElement(T1 elm1, T2 elm2)
    {
        T1* temp1 = new T1[getTrenutno() + 1];
        T2* temp2 = new T2[getTrenutno() + 1];
        for (int i = 0; i < getTrenutno(); i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        temp1[getTrenutno()] = elm1;
        temp2[getTrenutno()] = elm2;
        (*_trenutno)++;
        delete[]_elementi1; _elementi1 = NULL;
        delete[]_elementi2; _elementi2 = NULL;
        _elementi1 = temp1;
        _elementi2 = temp2;
        temp1 = NULL;
        temp2 = NULL;
        return true;
    }
    T1& getElement1(int index)const
    {
        if (index >= 0 && index < *(_trenutno))
        {
            return _elementi1[index];
        }
        return _elementi1[0];
    }
    T2& getElement2(int index)
    {
        if (index >= 0 && index < *(_trenutno))
        {
            return _elementi2[index];
        }
        return _elementi2[0];
    }
    int& getTrenutno()const { return *(_trenutno); }
    void setElement2(T2 elm)
    {
        *(_elementi2) += elm;
    }

};
template<class T1,class T2>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& obj)
{
    for (int i = 0; i < obj.getTrenutno(); i++)
    {
        COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
    }
    return COUT;
}
class Datum
{
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum& org):_dan(new int(*org._dan)),_mjesec(new int(*org._mjesec)),
        _godina(new int(*org._godina))
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
            delete _dan; _dan = NULL;
            delete _mjesec; _mjesec = NULL;
            delete _godina; _godina = NULL;
            _dan = new int(*obj._dan);
            _mjesec = new int(*obj._mjesec);
            _godina = new int(*obj._godina);
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj)
    {
        COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
        return COUT;
    }
    //1. potrebne funkcije
};
class Osoba
{
protected:
    char* _imePrezime;
    Datum* _datumRodjenja;
public:
    Osoba(const char* imePrezime, Datum datum)
    {
        _imePrezime = new char[strlen(imePrezime) + 1];
        strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
        _datumRodjenja = new Datum(datum);
    }
    Osoba(const Osoba& obj)
    {
        _imePrezime = new char[strlen(obj._imePrezime) + 1];
        strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
        _datumRodjenja = new Datum(*obj._datumRodjenja);
    }
    ~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }
    Osoba& operator=(const Osoba& obj)
    {
        if (this != &obj)
        {
            delete[]_imePrezime; _imePrezime = NULL;
            delete _datumRodjenja; _datumRodjenja = NULL;
            _imePrezime = new char[strlen(obj._imePrezime) + 1];
            strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
            _datumRodjenja = new Datum(*(obj._datumRodjenja));
        }
        return *this;
    }
    //1. potrebne funkcije
};


class Kupac: public Osoba
{
    friend ostream& operator<<(ostream& COUT, const Kupac& obj);
    //svaki kupac ima popust koji mu se zaracunava prilikom svake kupovine
    //pocetni popust je 0.10%
    //svaka kupovina u iznosu 100 - 500KM povecava iznos popusta za 0.15%
    //svaka kupovina u iznosu 500 - 1000KM povecava iznos popusta za 0.25%
    //svaka kupovina u iznosu vecem od 1000KM povecava iznos popusta za 0.30%
private:
    float _odobreniPopust;
    
public:
    //1. potrebne funkcije
    Kupac(const char* imePrezime,Datum datumRodenja):
        Osoba(imePrezime,datumRodenja),_odobreniPopust(0.10)
    {   }
    Kupac(const Kupac& org) :
        Osoba(org),_odobreniPopust(org._odobreniPopust)
    {   }
    Kupac& operator=(const Kupac& obj)
    {
        if (this != &obj)
        {
            Osoba::operator=(obj);
            _odobreniPopust = obj._odobreniPopust;
        }
        return *this;
    }
    void setPopust(float noviPopust)
    {
        _odobreniPopust += noviPopust;
    }
    float getPopust()const
    {
        return _odobreniPopust;
    }
};
ostream& operator<<(ostream& COUT, const Kupac& obj)
{
    COUT << "Ime prezime: " << obj._imePrezime << "\tDatum rodnjea: " << *(obj._datumRodjenja) << endl;
    COUT << "Odobreni popust: " << obj._odobreniPopust << endl;
    return COUT;
}
class Artikal
{
    string _barCode;
    char* _naziv;
    float _cijena;
public:
    Artikal(const char* naziv, float cijena, string barCode)
    {
        int vel = strlen(naziv) + 1;
        _naziv = new char[vel];
        strcpy_s(_naziv, vel, naziv);
        _cijena = cijena;
        _barCode = barCode;
    }

    Artikal(const Artikal& org):
        _naziv(new char[strlen(org._naziv)+1]),_cijena(org._cijena),_barCode(org._barCode)
    { 
        strcpy_s(_naziv, strlen(org._naziv) + 1, org._naziv);
    }
    ~Artikal() { delete[]_naziv; _naziv = nullptr; }
    Artikal& operator=(const Artikal& obj)
    {
        if (this != &obj)
        {
            delete[]_naziv; _naziv = NULL;
            _naziv = new char[strlen(obj._naziv)+1];
            strcpy_s(_naziv, strlen(obj._naziv) + 1, obj._naziv);
            _barCode = obj._barCode;
            _cijena = obj._cijena;
        }
        return *this;
    }
    bool operator==(const Artikal& obj)
    {
        if ((_cijena == obj._cijena) && (_barCode == obj._barCode))
        {
            if (strcmp(_naziv, obj._naziv) == 0)
                return true;
        }
        return false;
    }
    friend ostream& operator<< (ostream& COUT, const Artikal& obj)
    {
        COUT << obj._naziv << " " << obj._cijena << " KM" << endl;
        return COUT;
    }
    float getCijena()const { return _cijena; }
    char* getNaziv()const { return _naziv; }
    //1. potrebne funkcije
};
class Racun
{
    friend ostream& operator<<(ostream& COUT, const Racun& obj);
private:
    //format broja racuna: RN6005491
    //osigurati automatsko generisanje broja racuna pocevsi od RN0000001
    char _brojRacuna[10];
    Datum _datumKreiranja;
    //prvi argument je Artikal, a drugi kolicina/broj artikala koju je kupac uzeo
    Kolekcija<Artikal*, int>* _stavkeRacuna;
    Kupac* _kupac;
    int ZadnjaZnamenka(int broj)
    {
        return broj % 10;
    }
    
public:
    // 1. potrebne (de)konstruktor i druge funkcije
    // 2. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. Onemoguciti ponavljanje 
    //    artikala na nacin da se uvecava samo kolicina ukoliko korisnik vise puta pokusa dodati isti artikal.
    // 3. operator<< - ispisuje racun u formatu prikazanom na slici(nije obavezno da bude identican, ali je svakako pozeljno).
    //    Nakon ispisa svakog racuna, ukoliko su ispunjeni definisani uslovi, kupcu se uvecava popust.
    // 4. Sortiraj - sortira artikle (rastuci) po kolicini novca kojom ucestvuju u ukupnom iznosu racuna
    Racun(int brRacuna, Datum datum, Kupac* kupac)
        :_datumKreiranja(datum),_kupac(new Kupac(*(kupac)))
    {
        _stavkeRacuna = new Kolekcija<Artikal*, int>();
        _brojRacuna[0] = 'R';
        _brojRacuna[1] = 'N';
        for (int i = 8; i >= 2; i--)
        {
            _brojRacuna[i] = ZadnjaZnamenka(brRacuna)+'0';
            brRacuna /= 10;
        }
        _brojRacuna[9] = '\0';

    }
    Racun(const Racun& org) :
        _datumKreiranja(org._datumKreiranja), _stavkeRacuna(new Kolekcija<Artikal*, int>(*(org._stavkeRacuna))), _kupac(new Kupac(*(org._kupac)))
    {
        strcpy_s(_brojRacuna, 10, org._brojRacuna);
    }

    ~Racun()
    {
        delete _stavkeRacuna; _stavkeRacuna = NULL;
        delete _kupac; _kupac = NULL;
    }
    bool DodajArtikal(Artikal artikl,int kolicina)
    {
        for(int i=0;i<_stavkeRacuna->getTrenutno();i++)
            if (*(_stavkeRacuna->getElement1(i)) == artikl)
            {
                _stavkeRacuna->setElement2(kolicina);
                return true;
            }
        if(_stavkeRacuna->AddElement(new Artikal(artikl),kolicina));
            return true;
    }
    void Sortiraj()
    {
        //Sortiraj - sortira artikle(rastuci) po kolicini novca kojom ucestvuju u ukupnom iznosu racuna
        int ok;
        for (int i = _stavkeRacuna->getTrenutno() - 1; i > 0; i--)
        {
            ok = 1;
            for (int j = 0; j < i; j++)
            {
                float cijenaJ1 = _stavkeRacuna->getElement1(j + 1)->getCijena() * _stavkeRacuna->getElement2(j + 1);
                float cijenaJ = _stavkeRacuna->getElement1(j)->getCijena() * _stavkeRacuna->getElement2(j);
                if (cijenaJ1 < cijenaJ)
                {
                    int temp = _stavkeRacuna->getElement2(j);
                    Artikal* tempA = new Artikal(*_stavkeRacuna->getElement1(j));
                    _stavkeRacuna->getElement1(j)->~Artikal();
                    _stavkeRacuna->getElement1(j) = new Artikal(*_stavkeRacuna->getElement1(j+1));
                    _stavkeRacuna->getElement2(j) = _stavkeRacuna->getElement2(j + 1);

                    _stavkeRacuna->getElement1(j + 1)->~Artikal();
                    _stavkeRacuna->getElement1(j + 1) = new Artikal(*tempA);
                    _stavkeRacuna->getElement2(j + 1) = temp;
                    tempA = NULL;
                    ok = 0;
                    
                }
                
            }
            if (ok) break;
        }
    }
};
ostream& operator<<(ostream& COUT, const Racun& obj)
{
    COUT << "FITStore::Racun::" << obj._brojRacuna << endl;
    COUT << "| RB Artikal Cijena Kolicina Ukupno |" << endl;
    float suma = 0;
    for (int i = 0; i < obj._stavkeRacuna->getTrenutno(); i++)
    {
        cout << "  " << i + 1 << ". " << obj._stavkeRacuna->getElement1(i)->getNaziv() << "  " << obj._stavkeRacuna->getElement1(i)->getCijena() << "  " <<
            obj._stavkeRacuna->getElement2(i) << "  " << obj._stavkeRacuna->getElement1(i)->getCijena() * float(obj._stavkeRacuna->getElement2(i)) << endl;
        suma += (obj._stavkeRacuna->getElement1(i)->getCijena() * float(obj._stavkeRacuna->getElement2(i)));
    }
    COUT << "SUMA : "<< suma << endl;
    COUT << "Popust: " << obj._kupac->getPopust() << endl;
    suma -= suma * (obj._kupac->getPopust() / 100.);
    COUT << "Za platit : " << suma << endl;
    if (suma >= 100 && suma <= 500)
    {
        obj._kupac->setPopust(0.15);
    }
    else if (suma > 500 && suma <= 1000)
    {
        obj._kupac->setPopust(0.25);
    }
    else if (suma > 1000)
    {
        obj._kupac->setPopust(0.30);
    }
    //svaki kupac ima popust koji mu se zaracunava prilikom svake kupovine
    //pocetni popust je 0.10%
    //svaka kupovina u iznosu 100 - 500KM povecava iznos popusta za 0.15%
    //svaka kupovina u iznosu 500 - 1000KM povecava iznos popusta za 0.25%
    //svaka kupovina u iznosu vecem od 1000KM povecava iznos popusta za 0.30%
   /* FITStore::Racun::RN000001
        | RB Artikal Cijena Kolicina Ukupno |
        | 1. Milka Milkins 2.55 15 38.25 |
        | 2. Cappy narandza 2.1 20 42 |
        | 3. Meggle jogurt 2.55 20 51 |
        | Suma:131.25KM
        | Popust : 0.1KM
        | Za platiti : 131.15KM*/
    return COUT;
}

void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU
    //    RUNTIME ERROR CE BITI OZNACENO KAO "RE"
    // 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
    // 5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
    // 6. NIJE POTREBNO BACATI IZUZETKE.
    ****************************************************************************/
#pragma region TestiranjeDatuma
    Datum danas(26, 11, 2015);
    Datum sutra(danas);
    Datum prekosutra;
    prekosutra = danas;
    cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeKolekcije
    Kolekcija<int, int> kolekcija1;
    int brojElemenata = 15;
    for (int i = 0; i < brojElemenata; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1.getElement1(0) << " " << kolekcija1.getElement2(0) << endl;
    cout << kolekcija1 << endl;
    cout << kolekcija1.getTrenutno() << endl;

    Kolekcija<int, int> kolekcija2;
    kolekcija2 = kolekcija1;

    cout << kolekcija2 << crt;
    if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
        cout << "ISTI BROJ ELEMENATA" << endl;

    Kolekcija<int, int> kolekcija3(kolekcija2);
    cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeKupca
    Kupac jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
    Kupac adel("Adel Handzic", Datum(15, 10, 1981));

    Kupac jasmin2 = jasmin;
    cout << jasmin << endl << jasmin2 << endl;
    jasmin = jasmin2;
    cout << jasmin << endl << jasmin2 << endl;
#pragma endregion

#pragma region TestiranjeArtikla
    Artikal sokCppyNarandza("Cappy narandza", 2.10, "699511236545");
    Artikal cokoladaMilkaMilkins("Milka Milkins", 2.55, "622445875463");
    Artikal jogurtMeggle("Meggle jogurt", 2.55, "155338774458");

    cout << sokCppyNarandza << cokoladaMilkaMilkins << jogurtMeggle << endl;
    /*sokCppyNarandza = cokoladaMilkaMilkins;*/
    cout << sokCppyNarandza << endl;

    if (sokCppyNarandza == cokoladaMilkaMilkins)
        cout << "Artikli su isti" << endl;
#pragma endregion

#pragma region TestiranjeRacuna
    /*GenerisiSljedeciBrojRacuna - globalna funkcija koja generise i vraca naredni broj racuna u
    prethodno definisanom formatu*/
    Racun racun1(GenerisiSljedeciBrojRacuna(), danas, &jasmin);

    racun1.DodajArtikal(cokoladaMilkaMilkins, 15);
    racun1.DodajArtikal(sokCppyNarandza, 20);
    racun1.DodajArtikal(jogurtMeggle, 20);

    cout << racun1 << endl;
    GenerisiSljedeciBrojRacuna();
    Racun racun2(GenerisiSljedeciBrojRacuna(), sutra, &jasmin);
    racun2.DodajArtikal(cokoladaMilkaMilkins, 15);
  
    racun2.DodajArtikal(jogurtMeggle, 20);
    racun2.DodajArtikal(sokCppyNarandza, 20);
    cout << racun2 << endl;

    Racun racun3(racun2);
    cout << racun3 << endl;
    racun3.Sortiraj();
    cout << racun3 << endl;

#pragma endregion
    
}
//popust mozete izracunati kao procenat od ukupne sume racuna (pozeljna opcija) 
//ili kao vrijednost u KM-ovima (kao u nastavku)
/*
FITStore::Racun::RN000001
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.1KM
| Za platiti : 131.15KM


FITStore::Racun::RN000002
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.25KM
| Za platiti : 131KM


FITStore::Racun::RN000002
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.4KM
| Za platiti : 130.85KM
*/