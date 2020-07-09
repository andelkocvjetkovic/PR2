#include <iostream>
#include <string>
#include<vector>
#include<sstream>
#include<iomanip>
#include<iterator>
#include<regex>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum stringFormat { DDMMGG, DDMMGGGG, DDMMGGSSMM, DDMMGGGGSSMM };
enum prikazPoruka { SVE_PORUKE, SAKRIJ_POBRISANE, PRIKAZI_SAMO_POBRISANE };
vector<string> nedozvoljeneRijeci;
/*
DDMMGG - 29.8.17
DDMMGGGG - 29.8.2017
DDMMGGGGSSMM - 29.8.2017 10:15
*/

template<class T1, class T2>
class Kolekcija {
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
        _trenutno(new int(*(org._trenutno)))
    {
        for (int i = 0; i < *org._trenutno; i++)
        {
            _elementi1[i] = org._elementi1[i];
            _elementi2[i] = org._elementi2[i];
        }
    }
    Kolekcija& operator=(const Kolekcija<T1, T2>& orig)
    {
        if (this != &orig)
        {
            delete[]_elementi1;
            delete[]_elementi2;

            _elementi1 = new T1[*orig._trenutno];
            _elementi2 = new T2[*orig._trenutno];
            *_trenutno = *orig._trenutno;

            for (int i = 0; i < *_trenutno; i++)
            {
                _elementi1[i] = orig._elementi1[i];
                _elementi2[i] = orig._elementi2[i];
            }
        }
        return *this;
    }

    bool AddElement(T1 elem1, T2 elem2)
    {
        T1* temp1 = new T1[*_trenutno + 1];
        T2* temp2 = new T2[*_trenutno + 1];

        for (int i = 0; i < *_trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }

        delete[] _elementi1;
        delete[] _elementi2;

        _elementi1 = temp1;
        _elementi2 = temp2;

        _elementi1[*_trenutno] = elem1;
        _elementi2[*_trenutno] = elem2;
        (*_trenutno)++;
        return true;
    }
    ~Kolekcija()
    {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
        delete _trenutno; _trenutno = nullptr;
    }

    T1* getElementi1Pok() { return _elementi1; }
    T2* getElementi2Pok() { return _elementi2; }
    T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
    int getTrenutno() { return *_trenutno; }
    int size()const { return *_trenutno; }
    friend ostream& operator<<(ostream& COUT, Kolekcija<T1, T2>& obj)
    {
        for (size_t i = 0; i < *obj._trenutno; i++)
        {
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        }
        return COUT;
    }
    T1& operator[](int index)
    {
        return _elementi1[index];
    }
};
class DatumVrijeme
{
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
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
        _dan(new int(*(org._dan))), _mjesec(new int(*(org._mjesec))), _godina(new int(*(org._godina))),
        _sati(new int(*(org._sati))), _minuti(new int(*(org._minuti)))
    {   }
    ~DatumVrijeme()
    {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        delete _sati; _sati = nullptr;
        delete _minuti; _minuti = nullptr;
    }
    DatumVrijeme& operator=(const DatumVrijeme& obj)
    {
        if (this != &obj)
        {
            delete _dan; _dan = nullptr;
            delete _mjesec; _mjesec = nullptr;
            delete _godina; _godina = nullptr;
            delete _sati; _sati = nullptr;
            delete _minuti; _minuti = nullptr;
            _dan = new int(*(obj._dan));
            _mjesec = new int(*(obj._mjesec));
            _godina = new int(*(obj._godina));
            _sati = new int(*(obj._sati));
            _minuti = new int(*(obj._minuti));
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj)
    {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
        return COUT;
    }
    string ToString(stringFormat format)
    {
        //enum stringFormat { DDMMGG, DDMMGGGG, DDMMGGSSMM, DDMMGGGGSSMM };
        stringstream obj;
        if (format == 0)
        {
            obj << setw(2) << setfill('0') << *(this->_dan) << "." << setw(2) << setfill('0') << *(this->_mjesec) << ".";
            int zadnjaDva = *(this->_godina) % 100;
            obj << setw(2) << setfill('0') << zadnjaDva;
            return obj.str();
        }
        else if (format == 1)
        {
            obj << *(this->_dan) << "." << *(this->_mjesec) << "." << *(this->_godina);
            return obj.str();
        }
        else if (format == 2)
        {
            obj << *(this->_dan) << "." << *(this->_mjesec) << ".";
            int zadnjaDva = *(this->_godina) % 100;
            obj << zadnjaDva << " " << *(this->_sati) << ":" << *(this->_minuti);
            return obj.str();
        }
        else if (format == 3)
        {
            obj << *(this->_dan) << "." << *(this->_mjesec) << "." << *(this->_godina)
                << " " << *(this->_sati) << ":" << *(this->_minuti);
            return obj.str();
        }
        else
            throw exception("Pogresan unos!");
    }
    long int operator-(const DatumVrijeme& obj)
    {
        return (((*_dan + *_mjesec * 30 + *_godina * 365) * 1440 + *_sati * 60 + *_minuti) -
            ((*obj._dan + *obj._mjesec * 30 + *obj._godina * 365) * 1440 + *obj._sati * 60 + *obj._minuti));
    }
};
class Poruka
{
    string _posiljalac;
    string _primalac;
    string _sadrzaj;
    DatumVrijeme _vrijeme;
public:
    Poruka()
    {
        _posiljalac = "Nije postavljeno.";
        _primalac = "Nije postavljeno.";
        _sadrzaj = "Nije postavljeno.";
    }
    Poruka(string posiljalac, string primalac, string sadrzaj, DatumVrijeme vrijeme) :_vrijeme(vrijeme)
    {
        _posiljalac = posiljalac;
        _primalac = primalac;
        _sadrzaj = sadrzaj;
    }
    friend ostream& operator<< (ostream& COUT, Poruka& obj)
    {
        COUT << "(" << obj._vrijeme << ") " << obj._posiljalac << " -> ";
        COUT << obj._sadrzaj;
        return COUT;
    }
    string getPrimalac() { return _primalac; }
    string getPosiljalac() { return _posiljalac; }
    string getSadrzaj() { return _sadrzaj; }
    DatumVrijeme getDatumVrijem()const { return _vrijeme; }

};
class Komunikacija
{
    string _naslov;
    int _ocjena; //ocjena komunikacije 1 - 5 se automatski izracunava
    Kolekcija<Poruka, bool> _poruke;
    //bool se odnosi na podatak da li je poruka pobrisana tj. nece biti vidljiva u prikazu ukoliko se drugacije ne navede
public:
    Komunikacija()
    {
        _naslov = "Nepoznat";
        _ocjena = 1;
    }
    Komunikacija(string naslov)
    {
        _naslov = naslov;
        _ocjena = -1;
    }
    friend ostream& operator<< (ostream& COUT, Komunikacija& obj)
    {
        COUT << "(" << obj._ocjena << ") " << obj._naslov << " -> ";
        for (size_t i = 0; i < obj._poruke.size(); i++)
        {
            if (!obj._poruke.getElement2(i))
            {
                cout << obj._poruke.getElement1(i) << endl;
            }
            
        }
        return COUT;
    }
    Kolekcija<Poruka, bool>* getPoruke() { return &_poruke; }
    int getOcjena() { return _ocjena; }
    string getNaslov() { return _naslov; }
    void setOcjena(int ocijena) { _ocjena = ocijena; }
};

class Korisnik
{
    string _korisnickoIme;
    char* _imePrezime;
    //char * - se odnosi na naziv predmeta ili tema gdje æe se èuvati kompletna komunikacija sa 
    //nastavnim osobljem ili korisnikom na odreðenom predmetu/temi
    Kolekcija<char*, Komunikacija> _komunikacija;
public:
    Korisnik(const char* korisnickoIme,const char* imePrezime)
    {
        int vel = strlen(imePrezime) + 1;
        _imePrezime = new char[vel];
        strcpy_s(_imePrezime, vel, imePrezime);
        _korisnickoIme = korisnickoIme;
    }
    Korisnik(Korisnik& original) :_komunikacija(original._komunikacija)
    {
        int vel = strlen(original._imePrezime) + 1;
        _imePrezime = new char[vel];
        strcpy_s(_imePrezime, vel, original._imePrezime);
        _korisnickoIme = original._korisnickoIme;
    }
    
    ~Korisnik()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    Korisnik& operator=(const Korisnik& obj)
    {
        if (this != &obj)
        {
            _komunikacija.operator=(obj._komunikacija);
            delete[] _imePrezime;
            _korisnickoIme = obj._korisnickoIme;
            _imePrezime = new char[(strlen(obj._imePrezime) + 1)];
            strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
        }
        return *this;
    }
    friend ostream& operator<< (ostream& COUT, Korisnik& obj)
    {
        COUT << obj._imePrezime << endl << obj._komunikacija << endl;
        return COUT;
    }
    Kolekcija<char*, Komunikacija>* GetKomunikacija() { return &_komunikacija; }
    string getKorisnickoIme() { return _korisnickoIme; }
    void PrikaziKomunikacijuPoNaslovu(string naslov,prikazPoruka prikazP)
    {
        /*enum prikazPoruka { SVE_PORUKE, SAKRIJ_POBRISANE, PRIKAZI_SAMO_POBRISANE };*/
        if (prikazP == 0)//prikazP == SVE_PORUKE
        {
            for (int i = 0; i < _komunikacija.getTrenutno(); i++)
            {
                if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
                {
                    for (int j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
                    {
                        if (!_komunikacija.getElement2(i).getPoruke()->getElement2(j))
                        {
                            cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << endl;
                        }
                        else
                            cout << "PORUKA IZBRISANA" << endl;
                    }
                }
            }
        }
        else if (prikazP == 1) // SAKRIJ_POBRISANE  
        {
            for (int i = 0; i < _komunikacija.getTrenutno(); i++)
            {
                if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
                {
                    for (int j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
                    {
                        if (!_komunikacija.getElement2(i).getPoruke()->getElement2(j))
                        {
                            cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << endl;
                        }
                    }
                }
            }
        }
        else if (prikazP == 3) // PRIKAZI_SAMO_POBRISANE
        {
            for (int i = 0; i < _komunikacija.getTrenutno(); i++)
            {
                if (strcmp(_komunikacija.getElement1(i), naslov.c_str()) == 0)
                {
                    for (int j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
                    {
                        if (_komunikacija.getElement2(i).getPoruke()->getElement2(j))
                        {
                            cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j) << endl;
                        }
                    }
                }
            }
        }
    }
    int PrikaziKomunikacijuSaKorisnikom(string korisnickoime)
    {
        int brojac = 0;
        for (int i = 0; i < _komunikacija.getTrenutno(); i++)
        {
            cout << crt << "Poruke korisnika: " << korisnickoime << crt << endl;
            for (int j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno(); j++)
            {
                if (_komunikacija.getElement2(i).getPoruke()->getElement1(j).getPosiljalac() == korisnickoime ||
                    _komunikacija.getElement2(i).getPoruke()->getElement1(j).getPrimalac() == korisnickoime)
                {
                    cout << _komunikacija.getElement2(i).getPoruke()->getElement1(j)<<endl;
                    brojac++;
                }
            }
            cout << crt;
        }
        return brojac;
    }
    void OcijeniKomunikaciju(string naslov)
    {
        for (int i = 0; i < _komunikacija.getTrenutno(); i++)
        {
            float prosjek = 0;
            int brojac = 0;
            for (int j = 0; j < _komunikacija.getElement2(i).getPoruke()->getTrenutno() -1; j++)
            {
                prosjek += abs(_komunikacija.getElement2(i).getPoruke()->getElement1(j).getDatumVrijem() -
                    _komunikacija.getElement2(i).getPoruke()->getElement1(j + 1).getDatumVrijem());
                brojac++;
            }
            prosjek = prosjek / static_cast<float>(brojac);
            if (prosjek >= 20)
            {
                _komunikacija.getElement2(i).setOcjena(1);
            }
            else if (prosjek >= 15)
            {
                _komunikacija.getElement2(i).setOcjena(2);
            }
            else if (prosjek >= 10)
            {
                _komunikacija.getElement2(i).setOcjena(3);
            }
            else if (prosjek >= 5)
            {
                _komunikacija.getElement2(i).setOcjena(4);
            }
            else
            {
                _komunikacija.getElement2(i).setOcjena(5);
            }
            return;
        }

    }
};
void PosaljiPoruku(Korisnik** korisnici, int max, string naslov, string korisnickoPosiljatelj, string korisnikoPrimatelj, string sadrzaj, DatumVrijeme datumVrijeme)
{
    string pravilo = "(";
    bool primatelj = false,posiljatelj = false;
    int indPrimatelj = 0, indPosiljatelj = 0;
    vector<string>::iterator it = nedozvoljeneRijeci.begin();
    for (it; it != nedozvoljeneRijeci.end(); it++)
    {
        pravilo += *it;
        if (it != nedozvoljeneRijeci.end() - 1)
            pravilo += "|";
    }
    pravilo += ")";
    if (regex_search(sadrzaj, regex(pravilo)))
        throw exception("U textu postoje nedozvoljene rijeci");
    for (int i = 0; i < max; i++)
    {
        if (korisnici[i]->getKorisnickoIme() == korisnickoPosiljatelj)
        { 
            posiljatelj = true;
            indPosiljatelj = i;
        }
        if (korisnici[i]->getKorisnickoIme() == korisnikoPrimatelj)
        {
            primatelj = true;
            indPrimatelj = i;
        }
    }
    if (!posiljatelj || !primatelj)
        throw exception("Nepostojeci korisnici");
    if (indPosiljatelj == indPrimatelj)
        throw exception("Pogresan unos primatelja i posiljatelja");
    for (int i = 0; i < max; i++)
    {
        if (korisnici[i]->getKorisnickoIme() == korisnickoPosiljatelj ||
            korisnici[i]->getKorisnickoIme() == korisnikoPrimatelj)
        {
            bool postojiKumunikacija = false;
            for (int j = 0; j < korisnici[i]->GetKomunikacija()->getTrenutno(); j++)
            {
                if (strcmp(naslov.c_str(), korisnici[i]->GetKomunikacija()->getElement1(j)) == 0)
                {
                    korisnici[i]->GetKomunikacija()->getElement2(j).getPoruke()->AddElement(Poruka(korisnickoPosiljatelj, korisnikoPrimatelj, sadrzaj, datumVrijeme),false);
                    postojiKumunikacija = true;
                }

            }
            if (!postojiKumunikacija)
            {
                char* naziv = new char[strlen(naslov.c_str()) + 1];
                strcpy_s(naziv, strlen(naslov.c_str()) + 1, naslov.c_str());
                korisnici[i]->GetKomunikacija()->AddElement(naziv, Komunikacija(naslov));
                korisnici[i]->GetKomunikacija()->getElement2(korisnici[i]->GetKomunikacija()->getTrenutno() - 1).getPoruke()->AddElement(Poruka(korisnickoPosiljatelj, korisnikoPrimatelj, sadrzaj, datumVrijeme), false);
            }
        }
    }

}

void main()
{

    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU
    // RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
    // 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI
    // U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE,
    // POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/


    /*rijeèi koje su zabranjene u komunikaciji*/
    nedozvoljeneRijeci.push_back("ubiti");
    nedozvoljeneRijeci.push_back("mrzim");
    nedozvoljeneRijeci.push_back("rat");

    DatumVrijeme temp,
        datum19062017_1015(19, 6, 2017, 10, 15),
        datum19062017_1020(20, 6, 2017, 10, 20),
        datum19062017_1021(21, 6, 2017, 10, 21),
        datum19062017_1030(22, 6, 2017, 10, 30);

    cout << datum19062017_1015.ToString(DDMMGG) << endl;//vraca string: 19.6.2017
    cout << datum19062017_1015.ToString(DDMMGGGG) << endl;//vraca string: 19.6.2017
    cout << datum19062017_1015.ToString(DDMMGGSSMM) << endl;//vraca string: 19.6.2017 10:15
    cout << datum19062017_1015.ToString(DDMMGGGGSSMM) << endl;//vraca string: 19.6.2017
    


    const int max = 3;
    Korisnik* korisnici[max];
    korisnici[0] = new Korisnik("jasmin", "Jasmin Azemovic");
    korisnici[1] = new Korisnik("adel", "Adel Handzic");
    korisnici[2] = new Korisnik("denis", "Denis Music");
   

    // sve poruke se cuvaju u komunikaciji posiljaoca i primaoca tako da u svakom momentu oba ucesnika u 
    // komunikaciji posjeduju kompletnu historiju poruka. poruke se grupisu na osnovu naslova, pa ce poruke 
    // pod naslovom "Sastanak" izmedju dva korisnika sadrzavati sve poruke koje su razmijenjene na tu temu
    // u slucaju da posiljalac ili primalac ne postoje funkcija treba baciti izuzetak sa odgovarajucom porukom. 
    // takodjer, koristeci regex sve poruke koje sadrze neku od zabranjenih rijeci trebaju biti oznacene kao
    PosaljiPoruku(korisnici, max, "Sastanak", korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(), "Danas imamo sastanak u 10h!", datum19062017_1015);

    PosaljiPoruku(korisnici, max, "Sastanak", korisnici[1]->getKorisnickoIme(), korisnici[0]->getKorisnickoIme(), "Naravno, vidimo se u sali za sastanke!", datum19062017_1020);

    PosaljiPoruku(korisnici, max, "Sastanak", korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(), "Sa sobom ponesi radni materijal!", datum19062017_1021);


    // ispisuje sve poruke koje je korisnik razmijenio pod naslovom definisanim parametrom. ispis komunikacije
    // treba biti organizovan po datumu na nacin da se prvo prikaze datum, a nakon toga sve poruke koje su razmijenjene 
    // tog datuma. Na mjestima pobrisanih poruka prikazati tekst "PORUKA IZBRISANA"

    korisnici[0]->PrikaziKomunikacijuPoNaslovu("Sastanak", SVE_PORUKE);

    // funkcija prikazuje sve poruke sa korisnikom cije korisnicko ime je definisano parametrom, te vraca ukupan 
    // broj (razmijenjenih poruka sa tim korisnikom)
    int ukupnoPoruka = korisnici[0]->PrikaziKomunikacijuSaKorisnikom("adel");

    ///*formira ocjenu na nivou komunikacije na nacin da izracunava prosjecno vrijeme odgovora drugog ucesnika izrazeno u minutama i to prema sljedecoj skali
    //prosjek > 20 minuta = ocjena 1
    //prosjek > 15 minuta = ocjena 2
    //prosjek > 10 minuta = ocjena 3
    //prosjek > 5 minuta = ocjena 4
    //prosjek <=5 minuta = ocjena 5
    //*/
    korisnici[0]->OcijeniKomunikaciju("Sastanak");

    for (size_t i = 0; i < max; i++)
    {
        delete korisnici[i];
        korisnici[i] = nullptr;
    }

    system("PAUSE");
}